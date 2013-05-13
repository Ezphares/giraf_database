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
