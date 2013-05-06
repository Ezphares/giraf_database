/*
 * api_write.cpp
 *
 * TODO This file contains...
 */

#include "api.h"

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
	else if (strcmp(request["data"]["type"].asCString(), "pictogram") == 0) 	call_data;//TODO = delete_pictogram(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "application") == 0) 	call_data;//TODO = create_application(request["data"], user, errors);
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
		err += extract_string(name, object, "name", false);
		err += extract_string(email, object, "email", true);
		err += extract_string(address, object, "address", false);
		err += extract_string(phone, object, "phone", true);
		err += extract_string(picture, object, "picture", true);
		err += extract_string(settings, object, "settings", true);
		err += extract_int(&department, object, "department", false);
		err += extract_int(&role, object, "role", false);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `profile` (`name`, `email`, `department_id`, `role`, `address`, `phone`, `picture`, `settings`)"
											"VALUES (%s, %s, %d, %d, %s, %s, %s, %s);", name, email, department, role, address, phone, picture, settings);
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
		char name[EXTRACT_SIZE];
		char email[EXTRACT_SIZE];
		char address[EXTRACT_SIZE];
		char phone[EXTRACT_SIZE];
		int top_department = data["topdepartment"].asInt(); // TODO Validate this

		int err = 0;
		err += extract_string(name, object, "name", false);
		err += extract_string(email, object, "email", false);
		err += extract_string(address, object, "address", false);
		err += extract_string(phone, object, "phone", false);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `department` (`name`, `email`, `address`, `phone`, `super_department_id`)"
													"VALUES (%s, %s, %s, %s, %d);", name, email, address, phone, top_department);
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

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND update=1;", user);
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
		err += extract_string(username, object, "username", false);
		err += extract_string(password, object, "password", true);
		err += extract_string(certificate, object, "certificate", true);
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

		snprintf(query, API_BUFFER_SIZE, "SELECT COUNT (*) AS `createable` FROM `profile` WHERE `id`=%d AND `user_id`=NULL;", profile);
		QueryResult *result = _database->send_query(query);

		row_t r = result->next_row();
		if(atoi(r["createable"].c_str()) != 1)
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

