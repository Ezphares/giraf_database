/*
 * api_update.cpp
 *
 * TODO This file contains...
 */

#include "api.h"

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

	if (strcmp(request["data"]["type"].asCString(), "profile") == 0) 			call_data;//TODO = update_profile(request["data"], user, errors);
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
