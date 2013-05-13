/*
 * link.cpp
 *
 *  Created on: May 13, 2013
 *      Author: vixen
 */


#include "api.h"


int API::validate_link(Json::Value &data, Json::Value &errors)
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

int API::api_link(Json::Value &request, Json::Value &response, Json::Value &errors)
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

	if (strcmp(request["data"]["view"].asCString(), "list") == 0)
	{
		if (strcmp(request["data"]["type"].asCString(), "profile") == 0) 			call_data = read_profile_list(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "department") == 0) 	call_data = read_department_list(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "user") == 0) 			call_data = read_user_list(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "pictogram") == 0) 	call_data = read_pictogram_list(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "application") == 0) 	call_data = read_application_list(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "category") == 0) 		call_data = read_category_list(request["data"], user, errors);
		else
		{
			response["status"] = STATUS_STRUCTURE;
			errors.append(Json::Value("Invalid data type requested"));
		}
	}
	else
	{
		if (strcmp(request["data"]["type"].asCString(), "profile") == 0) 			call_data = read_profile_details(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "department") == 0) 	call_data = read_department_details(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "user") == 0) 			call_data = read_user_details(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "pictogram") == 0) 	call_data = read_pictogram_details(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "application") == 0) 	call_data = read_application_details(request["data"], user, errors);
		else if (strcmp(request["data"]["type"].asCString(), "category") == 0) 		call_data = read_category_details(request["data"], user, errors);

		else
		{
			response["status"] = STATUS_STRUCTURE;
			errors.append(Json::Value("Invalid data type requested"));
		}
	}

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
