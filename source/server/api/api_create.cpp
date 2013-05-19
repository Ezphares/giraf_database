/*
 * api_write.cpp
 *
 * TODO This file contains...
 */

#include "api.h"
#include <iostream>

int API::validate_create(Json::Value &data, Json::Value &errors)
{

	if (!data.isMember("type"))
	{
		errors.append(Json::Value("\"data\":\"type\" key missing"));
	}
	else
	{
		if (!data["type"].isString())
		{
			errors.append(Json::Value("\"data\":\"type\" was not a string"));
		}
	}

	if (!data.isMember("values"))
	{
		errors.append(Json::Value("\"data\":\"values\" key missing"));
		if (!data["values"].isArray())
		{
			errors.append(Json::Value("\"data\":\"values\" was not an array"));
		}
	}

	if (errors.empty()) return 0;
	else return -1;
}

int API::api_create (Json::Value &request, Json::Value &response, Json::Value &errors)
{
	if (validate_create(request["data"], errors) < 0)
	{
		response["status"] = Json::Value(STATUS_STRUCTURE);
		return -1;
	}

	int user = authenticate(request["auth"]);

	if (user == -1)
	{
		response["status"] = STATUS_AUTH;
		errors.append(Json::Value("Authentication failed"));
		return -1;
	}
	create_session(response, user);

	Json::Value call_data;

	if (strcmp(request["data"]["type"].asCString(), "profile") == 0) 			call_data = create_profile(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "department") == 0) 	call_data = create_department(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "user") == 0) 			call_data = create_user(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "pictogram") == 0) 	call_data = create_pictogram(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "application") == 0) 	call_data = create_application(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "category") == 0) 		call_data = create_category(request["data"], user, errors);
	else
	{
		response["status"] = STATUS_STRUCTURE;
		errors.append(Json::Value("Invalid data type requested"));
	}

	if (!errors.empty())
	{
		response["status"] = Json::Value(STATUS_ACCESS);
		return -1;
	}

	response["data"] = call_data;

	return 0;
}

Json::Value API::create_profile(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> departments = build_simple_int_vector_from_query(result, "id");
	delete result;

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND `role`=2;", user);
	result = _database->send_query(query);
	std::vector<int> children = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d = object["department"].asInt();
		if (!validate_value_in_vector(d, departments))
		{
			errors.append(Json::Value("Illegal department"));
			return Json::Value(Json::nullValue);
		}

		int r = object["role"].asInt();

		if(r != 2)
		{
			if (!validate_array_vector(object["guardian_of"], children))
			{
				errors.append(Json::Value("Illegal profile(s)"));
				return Json::Value(Json::nullValue);
			}
		}
		else
		{
			if (!object["guardian_of"].empty())
			{
				errors.append(Json::Value("Child as guardian"));
				return Json::Value(Json::nullValue);
			}
		}
	}

	Json::Value call_data(Json::arrayValue);
	std::vector<unsigned int> added_ids;
	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		char name[EXTRACT_SIZE];
		char email[EXTRACT_SIZE];
		char address[EXTRACT_SIZE];
		char phone[EXTRACT_SIZE];
		char picture[EXTRACT_SIZE];
		char settings[EXTRACT_SIZE];
		int department;
		int role;

		int err = 0;
		err += extract_string(name, object, "name", false, _database);
		err += extract_string(email, object, "email", true, _database);
		err += extract_string(address, object, "address", false, _database);
		err += extract_string(phone, object, "phone", true, _database);
		err += extract_string(picture, object, "picture", true, _database);
		err += extract_string(settings, object, "settings", true, _database);
		err += extract_int(&department, object, "department", false);
		err += extract_int(&role, object, "role", false);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `profile` (`name`, `email`, `department_id`, `role`, `address`, `phone`, `picture`, `settings`, `author`)"
											"VALUES (%s, %s, %d, %d, %s, %s, %s, %s, %d);", name, email, department, role, address, phone, picture, settings, user);
		QueryResult *result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;

		if(data.isMember("guardian_of"))
		{
			for (unsigned int i = 0; i < data["guardian_of"].size(); i++)
			{
				int child = data["guardian_of"][i].asInt();
				int guardian = added_ids.back();
				snprintf(query, API_BUFFER_SIZE, "INSERT INTO `guardian_of` (`guardian_id`, `child_id`)"
													"VALUES (%d, %d);", guardian, child);
				QueryResult *result = _database->send_query(query);
				delete result;
			}
		}
	}

	return call_data;
}

Json::Value API::create_department(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> departments = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d = object["topdepartment"].asInt();
		if (!validate_value_in_vector(d, departments))
		{
			errors.append(Json::Value("Illegal topdepartment"));
			return Json::Value(Json::nullValue);
		}

	}

	Json::Value call_data(Json::arrayValue);
	std::vector<unsigned int> added_ids;
	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];


		int top_department;
		char name[EXTRACT_SIZE];
		char email[EXTRACT_SIZE];
		char address[EXTRACT_SIZE];
		char phone[EXTRACT_SIZE];

		int err = 0;
		err += extract_string(name, object, "name", false, _database);
		err += extract_string(email, object, "email", false, _database);
		err += extract_string(address, object, "address", false, _database);
		err += extract_string(phone, object, "phone", false, _database);
		err += extract_int(&top_department, object, "topdepartment", false);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `department` (`name`, `email`, `address`, `phone`, `super_department_id`, `author`)"
													"VALUES (%s, %s, %s, %s, %d, %d);", name, email, address, phone, top_department, user);
		QueryResult *result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;
	}

	return call_data;
}

Json::Value API::create_user(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND `update`=1;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	Json::Value call_data(Json::arrayValue);
	std::vector<unsigned int> added_ids;
	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];
		char username[EXTRACT_SIZE];
		char password[EXTRACT_SIZE];
		char certificate[EXTRACT_SIZE];
		int profile;

		int err = 0;
		err += extract_string(username, object, "username", false, _database);
		err += extract_string(password, object, "password", true, _database);
		err += extract_string(certificate, object, "certificate", true, _database);
		err += extract_int(&profile, object, "profile", false);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		if (password[0]== 'N' && certificate[0] == 'N')
		{
			errors.append(Json::Value("You must specify either a password or certificate"));
			return Json::Value(Json::nullValue);
		}

		if (!validate_value_in_vector(profile, accessible))
		{
			errors.append(Json::Value("Illegal profile(s)"));
			return Json::Value(Json::nullValue);
		}

		snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `user` WHERE `username`=%s;", username);
		QueryResult *result = _database->send_query(query);
		row_t r = result->next_row();
		delete result;

		if(!r.empty())
		{
			errors.append(Json::Value("Duplicate username"));
			return Json::Value(Json::nullValue);
		}

		snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `profile` WHERE `id`=%d AND `user_id` IS NULL;", profile);
		result = _database->send_query(query);
		r = result->next_row();
		delete result;

		if(r.empty())
		{
			errors.append(Json::Value("User already exists"));
			return Json::Value(Json::nullValue);
		}


		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `user` (`username`, `password`, `certificate`)"
											"VALUES (%s, %s, %s);", username, password, certificate);
		result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;

		snprintf(query, API_BUFFER_SIZE, "UPDATE `profile` SET `user_id`=%d WHERE `id`=%d", added_ids.back(), profile);
		result = _database->send_query(query);
		delete result;
	}

	return call_data;
}

Json::Value API::create_pictogram(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `category_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];
		if(object.isMember("categories"))
		{
			if(validate_array_vector(object["categories"], accessible) != true)
			{
				std::cout << "invalid id" << std::endl;
				errors.append(Json::Value("Invalid ID access"));
				return Json::Value(Json::nullValue);
			}
		}
	}

	Json::Value call_data(Json::arrayValue);
	std::vector<unsigned int> added_ids;
	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];
		char name[EXTRACT_SIZE];
		char image[EXTRACT_SIZE];
		char text[EXTRACT_SIZE];
		char sound[EXTRACT_SIZE];
		bool is_public;

		int err = 0;
		err += extract_string(name, object, "name", false, _database);
		err += extract_string(image, object, "image", true, _database);
		err += extract_string(text, object, "text", true, _database);
		err += extract_string(sound, object, "sound", true, _database);
		err += extract_bool(&is_public, object, "public", false);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		int public_int = is_public ? 1 : 0;
		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `pictogram` (`name`, `image_data`, `inline_text`, `sound_data`, `public`, `author`)"
											"VALUES (%s, %s, %s, %s, %d, %d);", name, image, text, sound, public_int, user);
		result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;

		snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `profile` WHERE `user_id`=%d", user);
		result = _database->send_query(query);
		row_t r = result->next_row();
		delete result;

		if(!r.empty())
		{
			snprintf(query, API_BUFFER_SIZE, "INSERT INTO `profile_pictogram` (`profile_id`, `pictogram_id`) VALUES (%d, %d);", atoi(r["id"].c_str()), added_ids.back());
			result = _database->send_query(query);
			delete result;
		}

		if(object.isMember("tags"))
		{
			if(!object["tags"].isArray())
			{
				errors.append(Json::Value("Tags is not an array"));
				return Json::Value(Json::nullValue);
			}


			for(i = 0; i < object["tags"].size(); i++)
			{
				const char *tag = object["tags"][i].asCString();
				int tag_id;
				snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `tag` WHERE `name`=\"%s\";", tag);
				result = _database->send_query(query);
				r = result->next_row();
				delete result;

				if(!r.empty()) tag_id = atoi(r["id"].c_str());
				else
				{
					snprintf(query, API_BUFFER_SIZE, "INSERT INTO `tag` (`name`) VALUES (%s);", tag);
					result = _database->send_query(query);
					tag_id = _database->insert_id();
					delete result;
				}

				snprintf(query, API_BUFFER_SIZE, "INSERT INTO `pictogram_tag` (`pictogram_id`, `tag_id`) VALUES (%d, %d);", added_ids.back(), tag_id);
				result = _database->send_query(query);
			}
		}

		if(object.isMember("categories"))
		{
			for(i = 0; i < object["categories"].size(); i++)
			{
				int category_id = object["categories"][i].asInt();
				snprintf(query, API_BUFFER_SIZE, "INSERT INTO `pictogram_category` (`pictogram_id`, `category_id`) VALUES (%d, %d);", added_ids.back(), category_id);
				result = _database->send_query(query);
				delete result;
			}
		}
	}

	return call_data;
}

Json::Value API::create_application(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	Json::Value call_data(Json::arrayValue);
	std::vector<unsigned int> added_ids;
	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];
		char name[EXTRACT_SIZE];
		char version[EXTRACT_SIZE];
		char icon[EXTRACT_SIZE];
		char package[EXTRACT_SIZE];
		char activity[EXTRACT_SIZE];
		char settings[EXTRACT_SIZE];
		char description[EXTRACT_SIZE];

		int err = 0;
		err += extract_string(name, object, "name", false, _database);
		err += extract_string(version, object, "version", false, _database);
		err += extract_string(icon, object, "icon", false, _database);
		err += extract_string(activity, object, "activity", false, _database);
		err += extract_string(description, object, "description", true, _database);
		err += extract_string(package, object, "package", false, _database);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `application` (`name`, `version`, `icon`, `activity`, `description`, `package`, `author`)"
											"VALUES (%s, %s, %s, %s, %s, %s, %d);", name, version, icon, activity, description, package, user);
		QueryResult *result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;

		snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `profile` WHERE `user_id`=%d", user);
		result = _database->send_query(query);
		row_t r = result->next_row();
		delete result;

		if(!r.empty())
		{
			extract_string(settings, object, "settings", true, _database);
			snprintf(query, API_BUFFER_SIZE, "INSERT INTO `profile_application` (`profile_id`, `application_id`, `settings`) VALUES (%d, %d, %s);", atoi(r["id"].c_str()), added_ids.back(), settings);
			result = _database->send_query(query);
			delete result;
		}

	}

	return call_data;
}

Json::Value API::create_category(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	Json::Value call_data(Json::arrayValue);
	std::vector<unsigned int> added_ids;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];
		char name[EXTRACT_SIZE];
		char colour[EXTRACT_SIZE];
		char icon[EXTRACT_SIZE];
		int topcategory;

		int err = 0;
		err += extract_string(name, object, "name", false, _database);
		err += extract_string(colour, object, "colour", false, _database);
		err += extract_string(icon, object, "icon", true, _database);
		err += extract_int(&topcategory, object, "topcategory", true);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `category_list` WHERE `name`=%s AND `user_id`=%d", name, user);
		QueryResult *result = _database->send_query(query);
		row_t r = result->next_row();
		delete result;

		if(!r.empty())
		{
			errors.append("Category already exists");
			return Json::Value(Json::nullValue);
		}

		if(topcategory != 0)
		{
			snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `category_list` WHERE `id`=%d AND `user_id`=%d", topcategory, user);
			result = _database->send_query(query);
			r = result->next_row();
			delete result;

			if(r.empty())
			{
				errors.append("Invalid topcategory");
				return Json::Value(Json::nullValue);
			}
		}

		char topstr[EXTRACT_SIZE];
		snprintf(topstr, API_BUFFER_SIZE, "%d", topcategory);
		const char *top = topcategory ? topstr : "NULL";

		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `category` (`name`, `colour`, `icon`, `super_category_id`) "
											"VALUES (%s, %s, %s, %s);", name, colour, icon, top);
		result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;

		snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `profile` WHERE `user_id`=%d;", user);
		result = _database->send_query(query);
		r = result->next_row();
		delete result;

		int profile = atoi(r["id"].c_str());
		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `profile_category` (`profile_id`, `category_id`) "
											"VALUES (%d, %d);", profile, added_ids.back());
		result = _database->send_query(query);
		delete result;
	}

	return call_data;
}
