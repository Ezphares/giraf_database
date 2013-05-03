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
	QueryResult *result = _database->send_query(query);
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
	//TODO sql fun with escaping!

	return call_data;
}
