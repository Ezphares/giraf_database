/*
 * api_update.cpp
 *
 * TODO This file contains...
 */

#include "api.h"

int API::validate_update(Json::Value &data, Json::Value &errors)
{
	return 0;
}

int API::api_update(Json::Value &data, Json::Value &response, Json::Value &errors)
{
	return 0;
}

//TODO finish this
/*Json::Value API::update_profile(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND update=1;", user);
		QueryResult *result = _database->send_query(query);
		std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
		delete result;

		if(validate_array_vector(data["ids"], accessible) == false)
		{
				errors.append(Json::Value("Invalid ID access"));
				return Json::Value(Json::nullValue);
		}
}*/
