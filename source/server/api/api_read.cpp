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
	char query[API_BUFFER_SIZE];

	//TODO: Add support for department managers to see all profiles in their departments
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, `role` FROM `profile_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	row_t r = result->next_row();
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

	return call_data;

}

Json::Value API::read_profile_details(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	row_t r = result->next_row();
	Json::Value call_data(Json::arrayValue);

	std::vector<int> accesible;

	while (!r.empty())
	{
		accesible.push_back(atoi(r["id"].c_str()));
		r = result->next_row();
	}
	std::stringstream l;

	delete result;

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
	const std::string &st = l.str();
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT * FROM `profile` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);

	r = result->next_row();
	build_object_from_row(r);
	while (!r.empty())
	{
		Json::Value o(Json::objectValue);
		o["id"] = Json::Value(atoi(r["id"].c_str()));
		o["name"] = Json::Value(Json::Value(r["name"].c_str()));
		o["department"] = Json::Value(atoi(r["department_id"].c_str()));
		o["role"] = Json::Value(atoi(r["role"].c_str()));
		o["address"] = Json::Value(r["address"].c_str());
		if(strcmp(r["email"].c_str(), "") != 0) o["email"] = Json::Value(r["email"].c_str());
		if(strcmp(r["phone"].c_str(), "") != 0) o["phone"] = Json::Value(r["phone"].c_str());
		if(strcmp(r["picture"].c_str(), "") != 0) o["picture"] = Json::Value(r["picture"].c_str());
		if(strcmp(r["settings"].c_str(), "") != 0) o["settings"] = Json::Value(r["settings"].c_str());
		Json::Value children(Json::arrayValue);

		snprintf(query, API_BUFFER_SIZE, "SELECT `child_id` FROM `guardian_of` WHERE `guardian_id`=%s;", r["id"].c_str());
		QueryResult *subresult = _database->send_query(query);

		row_t sr = subresult->next_row();
		while (!sr.empty())
		{
			children.append(Json::Value(atoi(sr["child_id"].c_str())));
			sr = subresult->next_row();
		}
		o["guardian_of"] = children;

		call_data.append(o);
		r = result->next_row();
	}

	delete result;
	return call_data;
}

Json::Value API::read_department_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name` FROM `department_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	row_t r = result->next_row();
	Json::Value call_data(Json::arrayValue);

	while (!r.empty())
	{
		Json::Value o(Json::objectValue);
		o["id"] = Json::Value(atoi(r["id"].c_str()));
		o["name"] = Json::Value(r["name"].c_str());

		call_data.append(o);
		r = result->next_row();
	}

	delete result;

	return call_data;

}

Json::Value API::read_user_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name` FROM `user_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	row_t r = result->next_row();
	Json::Value call_data(Json::arrayValue);

	while (!r.empty())
	{
		Json::Value o(Json::objectValue);
		o["id"] = Json::Value(atoi(r["id"].c_str()));
		o["name"] = Json::Value(r["name"].c_str());

		call_data.append(o);
		r = result->next_row();
	}

	delete result;

	return call_data;
}

Json::Value API::read_application_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name` FROM `application_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	row_t r = result->next_row();
	Json::Value call_data(Json::arrayValue);

	while (!r.empty())
	{
		Json::Value o(Json::objectValue);
		o["id"] = Json::Value(atoi(r["id"].c_str()));
		o["name"] = Json::Value(r["name"].c_str());

		call_data.append(o);
		r = result->next_row();
	}

	delete result;

	return call_data;
}

Json::Value API::read_pictogram_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, `categories`, `tags` FROM `pictogram_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	row_t r = result->next_row();
	Json::Value call_data(Json::arrayValue);

	while (!r.empty())
	{
		Json::Value o(Json::objectValue);
		o["id"] = Json::Value(atoi(r["id"].c_str()));
		o["name"] = Json::Value(r["name"].c_str());

		call_data.append(o);
		r = result->next_row();
	}

	delete result;

	return call_data;
}
