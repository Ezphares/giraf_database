/*
 * api_read.h
 *
 * TODO This file contains...
 */

#include "api.h"

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
			errors.append(Json::Value("\"data\":\"list\" had an invalid value"));
		}
	}

	if (errors.empty()) return 0;
	else return -1;
}

int API::api_read(Json::Value &request, Json::Value &response, Json::Value &errors)
{
	if (validate_read(request["data"], errors) == -1)
	{
		response["status"] = Json::Value("BADREQUEST");
		return -1;
	}

	int user = authenticate(request["auth"]);

	if (user == -1)
	{
		response["status"] = Json::Value("AUTHFAILED");
		errors.append(Json::Value("Authentication failed"));
		return -1;
	}

	if (user == -2)
	{
		response["status"] = Json::Value("BADREQUEST");
		errors.append(Json::Value("Error in authentication keys."));
		return -1;
	}

	// TODO: Read calls here
	if (strcmp(request["data"]["view"].asCString(), "list") == 0)
	{
		if (strcmp(request["data"]["type"].asCString(), "profile") == 0)
		{
			Json::Value data = read_profile_list(request["data"], user, errors);
			// TODO: CHeck for errors
			response["data"] = data;
		}
	}


	return 0;
}

