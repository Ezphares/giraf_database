/*
 * api_read.h
 *
 * This file contains implementation of all read calls (list and details) for all data types in the API.
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


void fix_profile_list(Json::Value &o)
{
	fix_type(o, "id", V_INT);
	fix_type(o, "role", V_INT);
	fix_type(o, "update", V_INT);
	fix_type(o, "delete", V_INT);
}

Json::Value API::read_profile_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, `role`, MAX(`update`) AS `update`, MAX(`delete`) AS `delete` FROM `profile_list` WHERE `user_id`=%d GROUP BY `id`;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_profile_list);

	delete result;
	return call_data;
}

void fix_profile_details(Json::Value &o)
{
	fix_rename(o, "department_id", "department");
	fix_rename(o, "user_id", "user");
	fix_type(o, "user", V_INT);
	fix_type(o, "id", V_INT);
	fix_type(o, "role", V_INT);
	fix_type(o, "department", V_INT);
}

Json::Value API::read_profile_details(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
	}
	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT * FROM `profile` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);
	Json::Value call_data = build_array_from_query(result, fix_profile_details);
	delete result;

	for (unsigned int i = 0; i < call_data.size(); i++)
	{
		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `child_id` FROM `guardian_of` WHERE `guardian_id`=%d;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["guardian_of"] = build_simple_array_from_query(result, "child_id", V_INT);
		delete result;
	}

	return call_data;
}

void fix_generic_list(Json::Value &o)
{
	fix_type(o, "id", V_INT);
	fix_type(o, "update", V_INT);
	fix_type(o, "delete", V_INT);
}

Json::Value API::read_department_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, MAX(`update`) AS `update`, MAX(`delete`) AS `delete` FROM `department_list` WHERE `user_id`=%d GROUP BY `id`;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_generic_list);

	delete result;
	return call_data;
}

void fix_department_details(Json::Value &o)
{
	fix_rename(o, "super_department_id", "topdepartment");
	fix_remove(o, "author");
	fix_type(o, "id", V_INT);
	fix_type(o, "topdepartment", V_INT);
}

Json::Value API::read_department_details(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");

	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
	}
	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT * FROM `department` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);
	Json::Value call_data = build_array_from_query(result, fix_department_details);
	delete result;

	for (unsigned int i = 0; i < call_data.size(); i++)
	{
		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department` WHERE `super_department_id`=%d;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["subdepartments"] = build_simple_array_from_query(result, "id", V_INT);
		delete result;
	}

	return call_data;
}

void fix_user_list(Json::Value &o)
{
	fix_type(o, "id", V_INT);
	fix_type(o, "delete", V_INT);

}

Json::Value API::read_user_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `username`, MAX(`delete`) AS `delete` FROM `user_list` WHERE `user_id`=%d GROUP BY `id`;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_user_list);

	delete result;
	return call_data;
}

void fix_user_details(Json::Value &o)
{
	fix_type(o, "id", V_INT);
}

Json::Value API::read_user_details(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `user_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
	}

	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `username` FROM `user_list` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);
	Json::Value call_data = build_array_from_query(result, fix_user_details);
	delete result;

	return call_data;
}

void fix_app_picto_list(Json::Value &o)
{
	fix_type(o, "id", V_INT);
	fix_type(o, "author", V_INT);
}

Json::Value API::read_application_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, MAX(`author`) AS `author` FROM `application_list` WHERE `user_id`=%d GROUP BY `id`;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_app_picto_list);

	delete result;
	return call_data;
}

void fix_application_details(Json::Value &o)
{
	fix_type(o, "id", V_INT);
	fix_remove(o, "user_id");
	fix_remove(o, "max");
	fix_remove(o, "direct");
}

Json::Value API::read_application_details(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `application_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
	}

	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT * FROM `application_details` WHERE `id` IN (%s);", st.c_str());

	result = _database->send_query(query);
	Json::Value call_data = build_array_from_query(result, fix_application_details);
	delete result;

	return call_data;
}

Json::Value API::read_pictogram_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, MAX(`author`) AS `author` FROM `pictogram_list` WHERE `user_id`=%d GROUP  BY `id`;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_app_picto_list);
	delete result;

	for (unsigned int i = 0; i < call_data.size(); i++)
	{
		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `extra` FROM `pictogram_extras` WHERE `id`=%d AND `is_category`=1;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["categories"] = build_simple_array_from_query(result, "extra", V_STRING);
		delete result;

		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `extra` FROM `pictogram_extras` WHERE `id`=%d AND `is_category`=0;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["tags"] = build_simple_array_from_query(result, "extra", V_STRING);
		delete result;
	}

	return call_data;
}

void fix_pictogram_details(Json::Value &o)
{
	fix_type(o, "id", V_INT);
	fix_type(o, "public", V_BOOL);
	fix_remove(o, "author");
	fix_rename(o, "inline_text", "text");
	fix_rename(o, "sound_data", "sound");
	fix_rename(o, "image_data", "image");
}

Json::Value API::read_pictogram_details(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `pictogram_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
	}

	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT * FROM `pictogram` WHERE `id` IN (%s);", st.c_str());
	result = _database->send_query(query);
	Json::Value call_data = build_array_from_query(result, fix_pictogram_details);
	delete result;

	for (unsigned int i = 0; i < call_data.size(); i++)
	{
		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `extra` FROM `pictogram_extras` WHERE `id`=%d AND `is_category`=1;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["categories"] = build_simple_array_from_query(result, "extra", V_STRING);
		delete result;

		snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `extra` FROM `pictogram_extras` WHERE `id`=%d AND `is_category`=0;", call_data[i]["id"].asInt());
		result = _database->send_query(query);
		call_data[i]["tags"] = build_simple_array_from_query(result, "extra", V_STRING);
		delete result;
	}

	return call_data;
}

void fix_category_list(Json::Value &o)
{
	fix_rename(o, "super_category_id", "topcategory");
	fix_type(o, "topcategory", V_INT);
	fix_type(o, "id", V_INT);
}

Json::Value API::read_category_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, `super_category_id` FROM `category_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	Json::Value call_data = build_array_from_query(result, fix_category_list);
	delete result;

	return call_data;
}

Json::Value API::read_category_details(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `category_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	if(validate_array_vector(data["ids"], accessible) == false)
	{
			errors.append(Json::Value("Invalid ID access"));
			return Json::Value(Json::nullValue);
	}

	const std::string &st = build_in_string(data["ids"]);
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT * FROM `category` WHERE `id` IN (%s);", st.c_str());
	result = _database->send_query(query);
	Json::Value call_data = build_array_from_query(result, fix_category_list);
	delete result;

	return call_data;
}
