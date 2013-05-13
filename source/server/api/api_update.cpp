/*
 * api_update.cpp
 *
 * TODO This file contains...
 */

#include "api.h"
#include <iostream>

int API::validate_update(Json::Value &data, Json::Value &errors)
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

	if (data.isMember("values"))
	{
		if (!data["values"].isArray())
		{
			errors.append(Json::Value("\"data\":\"values\" was not an array"));
		}
		else
		{
			for (unsigned int i = 0; i < data["values"].size(); i++)
			{
				Json::Value &object = data["values"][i];
				if (!object.isObject())
				{
					errors.append(Json::Value("Values member was not an object"));
				}
				else
				{
					if (!object.isMember("id"))
					{
						errors.append(Json::Value("Missing update id"));
					}
					else
					{
						if (!object["id"].isInt())
						{
							errors.append(Json::Value("Update id was not an integer"));
						}
					}

					if (!object.isMember("value"))
					{
						errors.append(Json::Value("Missing update value"));
					}
					else
					{
						if (!object["value"].isObject())
						{
							errors.append(Json::Value("Update value was not an object"));
						}
					}
				}
			}
		}
	}
	else
	{
		errors.append(Json::Value("\"data\":\"values\" key missing"));
	}

	if (errors.empty()) return 0;
	else return -1;
}

int API::api_update(Json::Value &request, Json::Value &response, Json::Value &errors)
{
	if (validate_update(request["data"], errors) < 0)
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

	if (strcmp(request["data"]["type"].asCString(), "profile") == 0) 			call_data = update_profile(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "department") == 0) 	call_data = update_department(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "user") == 0) 			call_data = update_user(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "pictogram") == 0) 	call_data = update_pictogram(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "application") == 0) 	call_data = update_application(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "category") == 0) 		call_data = update_category(request["data"], user, errors);
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

Json::Value API::update_profile(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND `update`=1;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department_list` WHERE `user_id`=%d;", user);
	result = _database->send_query(query);
	std::vector<int> departments = build_simple_int_vector_from_query(result, "id");
	delete result;

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND `role`=2;", user);
	result = _database->send_query(query);
	std::vector<int> children = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d;
		if (extract_int(&d, object["value"], "department", false) == 0)
		{
			if (!validate_value_in_vector(d, departments))
			{
				errors.append(Json::Value("Illegal department"));
				return Json::Value(Json::nullValue);
			}
		}

		if(validate_value_in_vector(object["id"].asInt(), accessible) == false)
		{
				errors.append(Json::Value("Invalid ID access"));
				return Json::Value(Json::nullValue);
		}

		int r;
		if (extract_int(&r, object["value"], "role", false) == 0)
		{
			if(object["value"].isMember("guardian_of"))
			{
				if(r != 2)
				{
					if (!validate_array_vector(object["value"]["guardian_of"], children))
					{
						errors.append(Json::Value("Illegal profile(s)"));
						return Json::Value(Json::nullValue);
					}
				}
				else
				{
					if (!object["value"]["guardian_of"].empty())
					{
						errors.append(Json::Value("Child as guardian"));
						return Json::Value(Json::nullValue);
					}
				}
			}
		}
	}

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i]["value"];

		char name[EXTRACT_SIZE];
		char email[EXTRACT_SIZE];
		char address[EXTRACT_SIZE];
		char phone[EXTRACT_SIZE];
		char picture[EXTRACT_SIZE];
		char settings[EXTRACT_SIZE];
		int department;
		int role;

		int err = 0;
		err += extract_string(name, object, "name", true, _database);
		err += extract_string(email, object, "email", true, _database);
		err += extract_string(address, object, "address", true, _database);
		err += extract_string(phone, object, "phone", true, _database);
		err += extract_string(picture, object, "picture", true, _database);
		err += extract_string(settings, object, "settings", true, _database);
		err += extract_int(&department, object, "department", true);
		err += extract_int(&role, object, "role", true);
		if (err != 0)
		{
			errors.append("Type error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		std::stringstream values;
		if(name[0] != 'N') values << "name=" << name;
		if(email[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "email=" << email;
		}

		if(address[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "address=" << address;
		}

		if(phone[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "phone=" << phone;
		}

		if(picture[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "picture=" << picture;
		}

		if(settings[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "settings=" << settings;
		}

		if(department != 0)
		{
			if(values.str().size() != 0) values << ",";
			values << "department=" << department;
		}

		if(role != 0)
		{
			if(values.str().size() != 0) values << ",";
			values << "role=" << role;
		}

		std::string v = values.str();
		char query[API_BUFFER_SIZE];
		QueryResult *result = _database->send_query(query);
		delete result;


		if(object.isMember("guardian_of"))
		{
			int id = data["values"][i]["id"].asInt();
			snprintf(query, API_BUFFER_SIZE, "DELETE FROM `guardian_of` WHERE `guardian_id`=%d;", id);
			QueryResult *result = _database->send_query(query);
			delete result;

			for (unsigned int j = 0; j < data["guardian_of"].size(); j++)
			{
				int child = object["guardian_of"][j].asInt();
				snprintf(query, API_BUFFER_SIZE, "INSERT INTO `guardian_of` (`guardian_id`, `child_id`) VALUES (%d, %d);", id, child);
				QueryResult *result = _database->send_query(query);
				delete result;
			}
		}

	}

	return Json::Value(Json::nullValue);

}

Json::Value API::update_department(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department_list` WHERE `user_id`=%d AND `update`=1;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d = object["id"].asInt();
		if (!validate_value_in_vector(d, accessible))
		{
			errors.append(Json::Value("Illegal department"));
			return Json::Value(Json::nullValue);
		}
	}

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i]["value"];

		char name[EXTRACT_SIZE];
		char email[EXTRACT_SIZE];
		char address[EXTRACT_SIZE];
		char phone[EXTRACT_SIZE];

		int err = 0;
		err += extract_string(name, object, "name", true, _database);
		err += extract_string(email, object, "email", true, _database);
		err += extract_string(address, object, "address", true, _database);
		err += extract_string(phone, object, "phone", true, _database);
		if (err != 0)
		{
			errors.append("Type error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		std::stringstream values;
		if(name[0] != 'N') values << "name=" << name;
		if(email[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "email=" << email;
		}

		if(address[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "address=" << address;
		}

		if(phone[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "phone=" << phone;
		}

		std::string v = values.str();
		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "UPDATE `department` SET %s WHERE `id`=%d;", v.c_str(), data["values"][i]["id"].asInt());
		QueryResult *result = _database->send_query(query);
		delete result;
	}


	return Json::Value(Json::nullValue);
}

Json::Value API::update_user(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `user_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d = object["value"]["id"].asInt();
		if (!validate_value_in_vector(d, accessible))
		{
			errors.append(Json::Value("Illegal user"));
			return Json::Value(Json::nullValue);
		}
	}

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i]["value"];

		char password[EXTRACT_SIZE];
		char certificate[EXTRACT_SIZE];

		int err = 0;
		err += extract_string(password, object, "password", true, _database);
		err += extract_string(certificate, object, "certificate", true, _database);
		if (err != 0)
		{
			errors.append("Type error(s) in user data object");
			return Json::Value(Json::nullValue);
		}

		if(password[0] == 'N' && certificate[0] == 'N')
		{
			errors.append(Json::Value("You must specify either a password or certificate"));
			return Json::Value(Json::nullValue);
		}

		std::stringstream values;
		if(password[0] != 'N') values << "password=" << password;

		if(certificate[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "certificate=" << certificate;
		}

		std::string v = values.str();
		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "UPDATE `user` SET %s WHERE `id`=%d;", v.c_str(), data["values"][i]["id"].asInt());
		QueryResult *result = _database->send_query(query);
		delete result;

	}
	return Json::Value(Json::nullValue);
}

Json::Value API::update_application(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `application_list` WHERE `user_id`=%d AND `author`=1;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d = object["id"].asInt();
		if (!validate_value_in_vector(d, accessible))
		{
			errors.append(Json::Value("Illegal application"));
			return Json::Value(Json::nullValue);
		}
	}

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i]["value"];

		char version[EXTRACT_SIZE];
		char icon[EXTRACT_SIZE];
		char package[EXTRACT_SIZE];
		char activity[EXTRACT_SIZE];
		char description[EXTRACT_SIZE];

		int err = 0;
		err += extract_string(version, object, "version", true, _database);
		err += extract_string(icon, object, "icon", true, _database);
		err += extract_string(package, object, "package", true, _database);
		err += extract_string(activity, object, "activity", true, _database);
		err += extract_string(description, object, "description", true, _database);
		if (err != 0)
		{
			errors.append("Type error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		std::stringstream values;
		if(version[0] != 'N') values << "version=" << version;

		if(icon[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "icon=" << icon;
		}

		if(package[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "package=" << package;
		}

		if(activity[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "activity=" << activity;
		}

		if(description[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "description=" << description;
		}

		std::string v = values.str();
		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "UPDATE `application` SET %s WHERE `id`=%d;", v.c_str(), data["values"][i]["id"].asInt());
		QueryResult *result = _database->send_query(query);
		delete result;
	}

	return Json::Value(Json::nullValue);
}

Json::Value API::update_pictogram(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `category_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> categories = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i]["value"];
		if(object.isMember("categories"))
		{
			if(validate_array_vector(object["categories"], categories) != true)
			{
				errors.append(Json::Value("Invalid ID access"));
				return Json::Value(Json::nullValue);
			}
		}
	}

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `pictogram_list` WHERE `user_id`=%d;", user);
	result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d = object["id"].asInt();
		if (!validate_value_in_vector(d, accessible))
		{
			errors.append(Json::Value("Illegal pictogram"));
			return Json::Value(Json::nullValue);
		}
	}

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		int count = 0;
		int pictogram_id = data["values"][i]["id"].asInt();
		snprintf(query, API_BUFFER_SIZE, "SELECT COUNT(*) AS `count` FROM `profile_pictogram` WHERE `pictogram_id`=%d;", pictogram_id);
		result = _database->send_query(query);
		row_t r = result->next_row();
		delete result;

		count += atoi(r["count"].c_str());

		snprintf(query, API_BUFFER_SIZE, "SELECT COUNT(*) AS `count` FROM `department_pictogram` WHERE `pictogram_id`=%d;", pictogram_id);
		result = _database->send_query(query);
		r = result->next_row();
		delete result;

		count += atoi(r["count"].c_str());

		Json::Value object = data["values"][i]["value"];
		char name[EXTRACT_SIZE];
		char image[EXTRACT_SIZE];
		char sound[EXTRACT_SIZE];
		char text[EXTRACT_SIZE];

		int err = 0;
		err += extract_string(name, object, "name", true, _database);
		err += extract_string(image, object, "image", true, _database);
		err += extract_string(sound, object, "sound", true, _database);
		err += extract_string(text, object, "text", true, _database);
		if (err != 0)
		{
			std::cout << "type errors" << std::endl;
			errors.append("Type error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		std::stringstream values;
		if(name[0] != 'N') values << "name=" << name;

		if(image[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "image_data=" << image;
		}

		if(sound[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "sound_data=" << sound;
		}

		if(text[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "inline_text=" << text;
		}

		std::string v = values.str();
		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "UPDATE `pictogram` SET %s WHERE `id`=%d;", v.c_str(), data["values"][i]["id"].asInt());
		QueryResult *result = _database->send_query(query);
		delete result;

		if(object.isMember("tags"))
		{
			if(!object["tags"].isArray())
			{
				std::cout << "tags not an array" << std::endl;
				errors.append(Json::Value("Tags is not an array"));
				return Json::Value(Json::nullValue);
			}

			snprintf(query, API_BUFFER_SIZE, "DELETE FROM `pictogram_tag` WHERE `pictogram_id`=%d;", pictogram_id);
			result = _database->send_query(query);
			delete result;

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
					snprintf(query, API_BUFFER_SIZE, "INSERT INTO `tag` (`name`) VALUES ('%s');", tag);
					result = _database->send_query(query);
					tag_id = _database->insert_id();
					delete result;
				}

				snprintf(query, API_BUFFER_SIZE, "INSERT INTO `pictogram_tag` (`pictogram_id`, `tag_id`) VALUES (%d, %d);", pictogram_id, tag_id);
				result = _database->send_query(query);
			}
		}

		if(object.isMember("categories"))
		{
			snprintf(query, API_BUFFER_SIZE, "DELETE FROM `pictogram_category` WHERE `pictogram_id`=%d;", pictogram_id);
			result = _database->send_query(query);
			delete result;

			for(i = 0; i < object["categories"].size(); i++)
			{
				int category_id = object["categories"][i].asInt();
				snprintf(query, API_BUFFER_SIZE, "INSERT INTO `pictogram_category` (`pictogram_id`, `category_id`) VALUES (%d, %d);", pictogram_id, category_id);
				result = _database->send_query(query);
				delete result;
			}
		}

	}
	//TODO: create copy of the pictogram if multiple links exist
	return Json::Value(Json::nullValue);
}

Json::Value API::update_category(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `category_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d = object["id"].asInt();
		if (!validate_value_in_vector(d, accessible))
		{
			errors.append(Json::Value("Illegal category"));
			return Json::Value(Json::nullValue);
		}
	}

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i]["value"];

		char name[EXTRACT_SIZE];
		char icon[EXTRACT_SIZE];
		char colour[EXTRACT_SIZE];
		int topcategory;

		int err = 0;
		err += extract_string(name, object, "name", true, _database);
		err += extract_string(icon, object, "icon", true, _database);
		err += extract_string(colour, object, "colour", true, _database);
		err += extract_int(&topcategory, object, "topcategory", true);
		if (err != 0)
		{
			errors.append("Type error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		std::stringstream values;
		if(name[0] != 'N') values << "name=" << name;

		if(icon[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "icon=" << icon;
		}

		if(colour[0] != 'N')
		{
			if(values.str().size() != 0) values << ",";
			values << "colour=" << colour;
		}

		if(topcategory != 0)
		{
			if(values.str().size() != 0) values << ",";
			values << "super_category_id=" << topcategory;
		}

		std::string v = values.str();
		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "UPDATE `category` SET %s WHERE `id`=%d;", v.c_str(), data["values"][i]["id"].asInt());
		QueryResult *result = _database->send_query(query);
		delete result;
	}

	return Json::Value(Json::nullValue);
}
