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

	if (!data.isMember("values"))
	{
		errors.append(Json::Value("\"data\":\"values\" key missing"));
		if (!data["values"].isArray())
		{
			errors.append(Json::Value("\"data\":\"values\" was not an array"));
		}
		else
		{
			for (unsigned int i = 0; i < data["values"].size(); i++)
			{
				Json::Value &object = data["values"][i];
				if (!object.isMember("id"))
				{
					errors.append(Json::Value("Missing update id"));
				}
				else
				{
					if (object["id"].isInt())
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
					if (object["value"].isObject())
					{
						errors.append(Json::Value("Update value was not an object"));
					}
				}

			}
		}
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
	else if (strcmp(request["data"]["type"].asCString(), "department") == 0) 	call_data;//TODO = update_department(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "user") == 0) 			call_data;//TODO = update_user(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "pictogram") == 0) 	call_data;//TODO = delete_pictogram(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "application") == 0) 	call_data;//TODO = update_application(request["data"], user, errors);
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

		int d = object["value"]["department"].asInt();
		if (!validate_value_in_vector(d, departments))
		{
			errors.append(Json::Value("Illegal department"));
			return Json::Value(Json::nullValue);
		}

		if(validate_array_vector(object["id"], accessible) == false)
		{
				errors.append(Json::Value("Invalid ID access"));
				return Json::Value(Json::nullValue);
		}

		int r = object["value"]["role"].asInt();
		if(object["value"].isMember("guardian_of"))
		{
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
				if (!object["value"]["guardian_of"].empty())
				{
					errors.append(Json::Value("Child as guardian"));
					return Json::Value(Json::nullValue);
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
		err += extract_string(name, object, "name", true);
		err += extract_string(email, object, "email", true);
		err += extract_string(address, object, "address", true);
		err += extract_string(phone, object, "phone", true);
		err += extract_string(picture, object, "picture", true);
		err += extract_string(settings, object, "settings", true);
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
		snprintf(query, API_BUFFER_SIZE, "UPDATE `profile` SET %s WHERE id=%d;", v.c_str(), data["values"][i]["id"].asInt());
		QueryResult *result = _database->send_query(query);
		delete result;


		if(data.isMember("guardian_of"))
		{
			int id = data["values"][i]["id"].asInt();
			snprintf(query, API_BUFFER_SIZE, "DELETE FROM `guardian_of` WHERE `guardian_id`=%d;", id);
			QueryResult *result = _database->send_query(query);
			delete result;

			for (unsigned int i = 0; i < data["guardian_of"].size(); i++)
			{
				int child = data["guardian_of"][i].asInt();
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

		int d = object["value"]["department"].asInt();
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
		err += extract_string(name, object, "name", true);
		err += extract_string(email, object, "email", true);
		err += extract_string(address, object, "address", true);
		err += extract_string(phone, object, "phone", true);
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
		snprintf(query, API_BUFFER_SIZE, "UPDATE `department` SET %s WHERE id=%d;", v.c_str(), data["values"][i]["id"].asInt());
		QueryResult *result = _database->send_query(query);
		delete result;
	}


	return Json::Value(Json::nullValue);
}
