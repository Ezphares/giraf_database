/*
 * api_delete.cpp
 *
 * TODO This file contains...
 */
#include "api.h"

int API::validate_delete(Json::Value &data, Json::Value &errors)
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

	if (!data.isMember("ids"))
	{
		errors.append(Json::Value("\"data\":\"ids\" key missing"));
		if (!data["ids"].isArray())
		{
			errors.append(Json::Value("\"data\":\"ids\" was not an array"));
		}
	}

	if (errors.empty()) return 0;
	else return -1;
}

int API::api_delete(Json::Value &request, Json::Value &response, Json::Value &errors)
{
	if (validate_delete(request["data"], errors) < 0)
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

	if (strcmp(request["data"]["type"].asCString(), "profile") == 0) 			call_data = delete_profile(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "department") == 0) 	call_data = delete_department(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "user") == 0) 			call_data = delete_user(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "pictogram") == 0) 	call_data = delete_pictogram(request["data"], user, errors);
	else if (strcmp(request["data"]["type"].asCString(), "application") == 0) 	call_data = delete_application(request["data"], user, errors);
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


Json::Value API::delete_profile(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND `delete`=1;", user);
	QueryResult *result = _database->send_query(query);

	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");

	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
		errors.append(Json::Value("Invalid ID access"));
		return Json::Value(Json::nullValue);
	}
	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "DELETE FROM `profile` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);
	delete result;

	return Json::Value(Json::nullValue);
}

Json::Value API::delete_department(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department_list` WHERE `user_id`=%d AND `delete`=1;", user);
	QueryResult *result = _database->send_query(query);

	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");

	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
		errors.append(Json::Value("Invalid ID access"));
		return Json::Value(Json::nullValue);
	}
	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "DELETE FROM `department` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);
	delete result;

	return Json::Value(Json::nullValue);
}

Json::Value API::delete_user(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `user_list` WHERE `user_id`=%d AND `delete`=1;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
		errors.append(Json::Value("Invalid ID access"));
		return Json::Value(Json::nullValue);
	}

	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "DELETE FROM `user` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);
	delete result;

	return Json::Value(Json::nullValue);
}

Json::Value API::delete_pictogram(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `pictogram_list` WHERE `user_id`=%d AND `author`=1;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	if (validate_array_vector(data["ids"], accessible) == false)
	{
		errors.append(Json::Value("Invalid ID access"));
		return Json::Value(Json::nullValue);
	}
	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "UPDATE `pictogram` SET `public`=0, `author`=NULL WHERE `id` IN %s;", st.c_str());

	result = _database->send_query(query);
	delete result;

	return Json::Value(Json::nullValue);
}

Json::Value API::delete_application(Json::Value &data, int user, Json::Value &errors)
{
	errors.append(Json::Value("Applications cannot be deleted. Unlink instead."));
	return Json::Value(Json::nullValue);
}


