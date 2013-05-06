/*
 * api_write.cpp
 *
 * TODO This file contains...
 */

#include "api.h"

int API::validate_create(Json::Value &data, Json::Value &errors)
{
	return 0;
}

int API::api_create (Json::Value &request, Json::Value &response, Json::Value &errors)
{
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

		fix_type(object, "department", V_INT);
		int d = object["department"].asInt();
		if (!validate_value_in_vector(d, departments))
		{
			errors.append(Json::Value("Illegal department"));
			return Json::Value(Json::nullValue);
		}

		fix_type(object, "role", V_INT);
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

		char name[strlen(object["name"].asCString())*2 + 3];
		char email[std::max((unsigned int)strlen(object["email"].asCString())*2 + 3, 5u)];
		char address[std::max((unsigned int)strlen(object["address"].asCString())*2 + 3, 5u)];
		char phone[std::max((unsigned int)strlen(object["phone"].asCString())*2 + 3, 5u)];
		char picture[std::max((unsigned int)strlen(object["picture"].asCString())*2 + 3, 5u)];
		char settings[std::max((unsigned int)strlen(object["settings"].asCString())*2 + 3, 5u)];
		int department;
		int role;

		int err = 0;
		err += extract_string(name, object, "name", false);
		err += extract_string(email, object, "email", true);
		err += extract_string(address, object, "address", true);
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
		char name[strlen(object["name"].asCString())*2 + 3];
		char email[strlen(object["email"].asCString())*2 + 3];
		char address[strlen(object["address"].asCString())*2 + 3];
		char phone[strlen(object["phone"].asCString())*2 + 3];
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
		char username[strlen(object["username"].asCString())*2 + 3];
		char password[strlen(object["password"].asCString())*2 + 3];
		char certificate[strlen(object["certificate"].asCString())*2 + 3];
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

