/*
 * api_read.h
 *
 * TODO This file contains...
 */

#include "api.h"
#include <iostream>

int API::validate_read(Json::Value &data, Json::Value &errors)
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

		// TODO: Check if correct type
	}

	if (!data.isMember("ids"))
	{
		errors.append(Json::Value("\"data\":\"ids\" key missing"));
	}

	if (!data.isMember("view"))
	{
		errors.append(Json::Value("\"data\":\"view\" key missing"));
	}
	else if (!data["view"].isString())
	{
		errors.append(Json::Value("\"data\":\"view\" was not a string"));
	}
	else
	{
		if (strcmp(data["view"].asCString(), "list") == 0)
		{
			if (!data["ids"].isNull())
			{
				errors.append(Json::Value("\"data\":\"ids\" was not null for list request"));
			}
		}
		else if (strcmp(data["view"].asCString(), "details") == 0)
		{
			if (!data["ids"].isArray())
			{
				errors.append(Json::Value("\"data\":\"ids\" was not array for details request"));
			}
		}
		else
		{
			errors.append(Json::Value("\"data\":\"view\" had an invalid value"));
		}
	}

	if (errors.empty()) return 0;
	else return -1;
}

int API::api_read(Json::Value &request, Json::Value &response, Json::Value &errors)
{

	if (validate_read(request["data"], errors) < 0)
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

	// TODO: Read calls here
	if (strcmp(request["data"]["view"].asCString(), "list") == 0)
	{
		if (strcmp(request["data"]["type"].asCString(), "profile") == 0) call_data = read_profile_list(request["data"], user, errors);
	}
	else
	{
		if (strcmp(request["data"]["type"].asCString(), "profile") == 0) call_data = read_profile_details(request["data"], user, errors);
	}

	if (!errors.empty())
	{
		response["status"] = Json::Value(STATUS_ACCESS);
		return -1;
	}

	response["data"] = call_data;

	return 0;
}


void fix_profile_list(Json::Value &o)
{
	fix_type(o, "id", V_INT);
	fix_type(o, "role", V_INT);
}

Json::Value API::read_profile_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, `role` FROM `profile_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_profile_list);

	delete result;
	return call_data;
}

void fix_profile_details(Json::Value &o)
{
	fix_rename(o, "department_id", "department");
	fix_remove(o, "user_id");
	fix_type(o, "id", V_INT);
	fix_type(o, "role", V_INT);
	fix_type(o, "department", V_INT);
}

Json::Value API::read_profile_details(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	std::vector<int> accesible = build_simple_int_vector_from_query(result, "id");

	delete result;

	std::stringstream l;
	for (unsigned int i = 0; i < data["ids"].size(); i++)
	{
		bool found = false;
		int id = data["ids"][i].asInt();
		for(std::vector<int>::iterator it = accesible.begin(); it != accesible.end(); it++)	if(id == *it) found = true;

		if(found == false)
		{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
		}

		if (i != 0) l << ", ";
		l << id;
	}
	const std::string &st = l.str();
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT * FROM `profile` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);
	Json::Value call_data = build_array_from_query(result, fix_profile_details);
	delete result;

	for (unsigned int i = 0; i < call_data.size(); i++)
	{
		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `child_id` FROM `guardian_of` WHERE `guardian_id`=%d;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["guardian_of"] = build_simple_array_from_query(result, "child_id", V_INT);
		delete result;
	}

	return call_data;
}

void fix_generic_list(Json::Value &o)
{
	fix_type(o, "id", V_INT);
}

Json::Value API::read_department_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name` FROM `department_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_generic_list);

	delete result;
	return call_data;
}

Json::Value API::read_user_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name` FROM `user_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_generic_list);

	delete result;
	return call_data;
}

Json::Value API::read_application_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name` FROM `application_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_generic_list);

	delete result;
	return call_data;
}

Json::Value API::read_pictogram_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, `categories`, `tags` FROM `pictogram_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_generic_list);

	for (unsigned int i = 0; i < call_data.size(); i++)
	{
		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `extra` FROM `pictogram_extras` WHERE `id`=%d AND `is_category`=1;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["categories"] = build_simple_array_from_query(result, "extra", V_STRING);
		delete result;

		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `extra` FROM `pictogram_extras` WHERE `id`=%d AND `is_category`=0;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["tags"] = build_simple_array_from_query(result, "extra", V_STRING);
		delete result;
	}

	delete result;
	return call_data;
}
