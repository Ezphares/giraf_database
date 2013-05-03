/*
 * api_delete.cpp
 *
 * TODO This file contains...
 */
#include "api.h"

int API::validate_delete(Json::Value &data, Json::Value &errors)
{
	return 0;
}

int API::api_delete(Json::Value &request, Json::Value &response, Json::Value &errors)
{
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

Json::Value API::delete_application(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `application_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
	}



	return Json::Value(Json::nullValue);
}
