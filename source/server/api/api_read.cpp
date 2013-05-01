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

Json::Value API::read_profile_list(Json::Value &data, int user, Json::Value &errors)
{
	QueryResult *result;
	char query[BUFFER_SIZE];
	row_t r;

	char *temp = read_file("api/sql/profile_read_list.sql");
	sprintf(query, temp, user, user, user);
	result = _database->send_query(query);

	r = result->next_row();
	Json::Value call_data(Json::arrayValue);
	std::cout << r["phone"] << std::endl;
	while (!r.empty())
	{
		Json::Value o(Json::objectValue);
		o["id"] = Json::Value(atoi(r["id"].c_str()));
		o["name"] = Json::Value(r["name"].c_str());
		o["role"] = Json::Value(atoi(r["role"].c_str()));

		call_data.append(o);
		r = result->next_row();
	}

	delete result;
	free(temp);

	return call_data;

}

Json::Value API::read_profile_details(Json::Value &data, int user, Json::Value &errors)
{
	QueryResult *result;
	char query[BUFFER_SIZE];
	row_t r;

	char *temp = read_file("api/sql/profile_read_list.sql");
	sprintf(query, temp, user, user, user);
	result = _database->send_query(query);

	r = result->next_row();
	Json::Value call_data(Json::arrayValue);

	std::vector<int> accesible;

	while (!r.empty())
	{
		accesible.push_back(atoi(r["id"].c_str()));
		r = result->next_row();
	}
	std::stringstream l;

	delete result;
	free(temp);

	for (unsigned int i = 0; i < data["ids"].size(); i++)
	{
		bool found = false;
		int id = data["ids"][i].asInt();
		for(std::vector<int>::iterator it = accesible.begin(); it != accesible.end(); it++)
		{
			if(id == *it) found = true;
		}
		if(found == false)
		{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
		}

		if (i != 0) l << ", ";
		l << id;
	}

	sprintf(query, "SELECT * FROM `profile` WHERE `id` IN (%s);", l.str().c_str());
	result = _database->send_query(query);

	r = result->next_row();
	while (!r.empty())
		{
			Json::Value o(Json::objectValue);
			o["id"] = Json::Value(atoi(r["id"].c_str()));
			o["name"] = Json::Value(r["name"].c_str());
			o["email"] = Json::Value(r["email"].c_str());
			o["department"] = Json::Value(atoi(r["department"].c_str()));
			o["role"] = Json::Value(atoi(r["role"].c_str()));
			if(strcmp(r["address"].c_str(), "") != 0) o["address"] = Json::Value(r["address"].c_str());
			if(strcmp(r["phone"].c_str(), "") != 0) o["phone"] = Json::Value(r["phone"].c_str());
			if(strcmp(r["picture"].c_str(), "") != 0) o["picture"] = Json::Value(r["picture"].c_str());
			if(strcmp(r["settings"].c_str(), "") != 0) o["settings"] = Json::Value(r["settings"].c_str());

			call_data.append(o);
			r = result->next_row();
		}

	return call_data;
}
