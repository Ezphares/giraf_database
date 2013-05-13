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
	int profile = 0;
	int department = 0;
	char query[API_BUFFER_SIZE];
	QueryResult *result;
	row_t r;

	if(data.isMember("profile"))
	{
		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND `update`=1;", user);
		result = _database->send_query(query);
		std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
		delete result;

		profile = data["profile"].asInt();
		if (!validate_value_in_vector(profile, accessible))
		{
			errors.append(Json::Value("Illegal profile"));
			return Json::Value(Json::nullValue);
		}
	}

	if(data.isMember("department"))
	{
		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department_list` WHERE `user_id`=%d AND `update`=1;", user);
		result = _database->send_query(query);
		std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
		delete result;

		department = data["department"].asInt();
		if (!validate_value_in_vector(department, accessible))
		{
			errors.append(Json::Value("Illegal department"));
			return Json::Value(Json::nullValue);
		}
	}

	int actor_id = profile + department;
	const char *actor = profile != 0 ? "profile" : "department";

	if(data.isMember("link"))
	{
		for(unsigned int i = 0; i < data["link"].size(); i++)
		{
			Json::Value &object = data["link"][i];
			char *settings = NULL;
			char *type;
			_database->escape(type, object["type"].asCString());
			int object_id = object["id"].asInt();

			if(strcmp(object["type"].asCString(), "application") == 0)
			{
				if(object.isMember("settings") && profile != 0)	_database->escape(settings, object["settings"].asCString());

			}
			else
			{
				//TODO: we need to update the view to include public pictograms!
				/*
				snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `pictogram_list` WHERE `user_id`=%d OR `public`=1;", user);
				result = _database->send_query(query);
				std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
				delete result;

				int pictogram = data["id"].asInt();
				if (!validate_value_in_vector(pictogram, accessible))
				{
					errors.append(Json::Value("Illegal pictogram"));
					return Json::Value(Json::nullValue);
				}
				*/
			}

			snprintf(query, API_BUFFER_SIZE, "SELECT * FROM `%s_%s` WHERE `%s_id`=%d AND `%s_id`=%d;", actor, type, actor, actor_id, type, object_id);
			result = _database->send_query(query);
			r = result->next_row();
			delete result;

			if (!r.empty())
			{
				if (settings == NULL) continue;

				snprintf(query, API_BUFFER_SIZE, "UPDATE `%s_%s` SET `settings`='%s' WHERE `%s_id`=%d AND `%s_id`=%d;", actor, type, settings, actor, actor_id, type, object_id);
				result = _database->send_query(query);
				delete result;
			}
			else
			{
				if (settings == NULL)
					snprintf(query, API_BUFFER_SIZE, "INSERT INTO `%s_%s` (`%s_id`, `%s_id`) VALUES (%d, %d);", actor, type, actor, type, actor_id, object_id);
				else
					snprintf(query, API_BUFFER_SIZE, "INSERT INTO `%s_%s` (`%s_id`, `%s_id`, `settings`) VALUES (%d, %d, '%s');", actor, type, actor, type, actor_id, object_id, settings);
				result = _database->send_query(query);
				delete result;
			}
		}
	}

	if(data.isMember("unlink"))
	{
		for(unsigned int i = 0; i < data["link"].size(); i++)
		{
			Json::Value &object = data["link"][i];
			char *type;
			_database->escape(type, object["type"].asCString());
			int object_id = object["id"].asInt();

			snprintf(query, API_BUFFER_SIZE, "DELETE FROM `%s_%s` WHERE `%s_id`=%d AND `%s_id`=%d;", actor, type, actor, actor_id, type, object_id);
			result = _database->send_query(query);
			delete result;
		}
	}
	return Json::Value(Json::nullValue);
}
