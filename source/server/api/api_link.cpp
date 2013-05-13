/*
 * link.cpp
 *
 *  Created on: May 13, 2013
 *      Author: vixen
 */


#include "api.h"


int API::validate_link(Json::Value &data, Json::Value &errors)
{
	int actors = 0;

	if (data.isMember("profile"))
	{
		actors++;
		if (!data["profile"].isInt()) errors.append(Json::Value("\"data\":\"profile\" was not an integer"));
	}

	if (data.isMember("department"))
	{
		actors++;
		if (!data["department"].isInt()) errors.append(Json::Value("\"data\":\"department\" was not an integer"));
	}

	if (actors != 1) errors.append(Json::Value("Exactly one of \"department\" and \"profile\" must be given"));

	if (data.isMember("link"))
	{
		if (data["link"].isArray())
		{
			for (unsigned int i = 0; i < data["link"].size(); i++)
			{
				Json::Value &object = data["link"][i];

				if (!object.isMember("type")) errors.append(Json::Value("Type missing from link"));
				else if (!object["type"].isString()) errors.append(Json::Value("Link type was not a string"));
				else if (strcmp(object["type"].asCString(), "application") != 0 && strcmp(object["type"].asCString(), "pictogram")) errors.append(Json::Value("Invalid link type"));

				if (!object.isMember("id")) errors.append(Json::Value("ID missing from link"));
				else if (!object["id"].isString()) errors.append(Json::Value("Link ID was not an integer"));

				if (object.isMember("settings") && !object["settings"].isString()) errors.append(Json::Value("Link settings was not a string"));
			}

			for (unsigned int i = 0; i < data["unlink"].size(); i++)
			{
				Json::Value &object = data["unlink"][i];

				if (!object.isMember("type")) errors.append(Json::Value("Type missing from unlink"));
				else if (!object["type"].isString()) errors.append(Json::Value("Link type was not a unstring"));
				else if (strcmp(object["type"].asCString(), "application") != 0 && strcmp(object["type"].asCString(), "pictogram")) errors.append(Json::Value("Invalid unlink type"));

				if (!object.isMember("id")) errors.append(Json::Value("ID missing from unlink"));
				else if (!object["id"].isString()) errors.append(Json::Value("Unlink ID was not an integer"));
			}
		}
		else errors.append(Json::Value("\"data\":\"unlink\" was not an array"));
	}


	if (errors.empty()) return 0;
	else return -1;
}

int API::api_link(Json::Value &request, Json::Value &response, Json::Value &errors)
{
	if (validate_link(request["data"], errors) < 0)
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

	Json::Value call_data = execute_link(request["data"], user, errors);

	if (!errors.empty())
	{
		response["status"] = Json::Value(STATUS_ACCESS);
		return -1;
	}

	response["data"] = call_data;

	return 0;
}

Json::Value API::execute_link(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, `role`, MAX(`update`) AS `update`, MAX(`delete`) AS `delete` FROM `profile_list` WHERE `user_id`=%d GROUP BY `id`;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_profile_list);

	delete result;
	return call_data;
}
