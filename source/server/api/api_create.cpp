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
	result = _database->send_query(query);
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
	std::vector<unsigned int> added_ids;
	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		char name[strlen(object["name"].asCString())*2 + 3];
		char email[std::max((unsigned int)strlen(object["email"].asCString())*2 + 3, 5u)];
		char address[std::max((unsigned int)strlen(object["address"].asCString())*2 + 3, 5u)];
		char phone[std::max((unsigned int)strlen(object["phone"].asCString())*2 + 3, 5u)];
		char picture[std::max((unsigned int)strlen(object["picture"].asCString())*2 + 3, 5u)];
		char settings[std::max((unsigned int)strlen(object["settings"].asCString())*2 + 3, 5u)];
		int department;
		int role;

		if(!data.isMember("name"))
		{
			errors.append(Json::Value("Name required on profile."));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!data["name"].isString())
			{
				errors.append(Json::Value("Name is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(name+1, object["name"].asCString());
			name[0] = '"';
			name[strlen(name)] = '"';
		}

		if(data.isMember("email"))
		{
			if(!data["email"].isString())
			{
				errors.append(Json::Value("Email is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(email+1, object["email"].asCString());
			email[0] = '"';
			email[strlen(email)] = '"';
		}
		else strncpy(email, "NULL", 5);

		if(data.isMember("address"))
		{
			if(!data["address"].isString())
			{
				errors.append(Json::Value("Address is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(address+1, object["address"].asCString());
			address[0] = '"';
			address[strlen(address)] = '"';
		}
		else strncpy(address, "NULL", 5);

		if(data.isMember("phone"))
		{
			if(!data["phone"].isString())
			{
				errors.append(Json::Value("Phone is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(phone+1, object["phone"].asCString());
			phone[0] = '"';
			phone[strlen(phone)] = '"';
		}
		else strncpy(phone, "NULL", 5);

		if(data.isMember("picture"))
		{
			if(!data["picture"].isString())
			{
				errors.append(Json::Value("Picture is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(picture+1, object["picture"].asCString());
			picture[0] = '"';
			picture[strlen(picture)] = '"';
		}
		else strncpy(picture, "NULL", 5);

		if(data.isMember("settings"))
		{
			if(!data["settings"].isString())
			{
				errors.append(Json::Value("Settings is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(settings+1, object["settings"].asCString());
			settings[0] = '"';
			settings[strlen(settings)] = '"';
		}
		else strncpy(settings, "NULL", 5);

		if(!data.isMember("department"))
		{
			errors.append(Json::Value("Department required"));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!data["department"].isInt())
			{
				errors.append(Json::Value("Department is not an integer"));
				return Json::Value(Json::nullValue);
			}
			department = data["department"].asInt();
		}

		if(!data.isMember("role"))
		{
			errors.append(Json::Value("Role required"));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!data["role"].isInt())
			{
				errors.append(Json::Value("Role is not an integer"));
				return Json::Value(Json::nullValue);
			}
			role = data["role"].asInt();
		}

		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `profile` (`name`, `email`, `department_id`, `role`, `address`, `phone`, `picture`, `settings`)"
											"VALUES (%s, %s, %d, %d, %s, %s, %s, %s);", name, email, department, role, address, phone, picture, settings);
		QueryResult *result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;

		if(data.isMember("guardian_of"))
		{
			for (unsigned int i = 0; i < data["guardian_of"].size(); i++)
			{
				int child = data["guardian_of"][i].asInt();
				int guardian = added_ids.back();
				snprintf(query, API_BUFFER_SIZE, "INSERT INTO `guardian_of` (`guardian_id`, `child_id`)"
													"VALUES (%d, %d);", guardian, child);
				QueryResult *result = _database->send_query(query);
				delete result;
			}
		}
	}

	return call_data;
}

Json::Value API::create_department(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `department_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> departments = build_simple_int_vector_from_query(result, "id");
	delete result;

	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];

		int d = object["topdepartment"].asInt();
		if (!validate_value_in_vector(d, departments))
		{
			errors.append(Json::Value("Illegal topdepartment"));
			return Json::Value(Json::nullValue);
		}

	}

	Json::Value call_data(Json::arrayValue);
	std::vector<unsigned int> added_ids;
	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];
		char name[strlen(object["name"].asCString())*2 + 3];
		char email[strlen(object["email"].asCString())*2 + 3];
		char address[strlen(object["address"].asCString())*2 + 3];
		char phone[strlen(object["phone"].asCString())*2 + 3];
		int top_department = data["topdepartment"].asInt();

		if(!data.isMember("name"))
		{
			errors.append(Json::Value("Name required on department."));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!data["name"].isString())
			{
				errors.append(Json::Value("Name is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(name+1, object["name"].asCString());
			name[0] = '"';
			name[strlen(name)] = '"';
		}

		if(!data.isMember("email"))
		{
			errors.append(Json::Value("Email required on department."));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!data["email"].isString())
			{
				errors.append(Json::Value("Email is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(email+1, object["email"].asCString());
			email[0] = '"';
			email[strlen(email)] = '"';
		}

		if(!data.isMember("address"))
		{
			errors.append(Json::Value("Address required on department."));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!data["address"].isString())
			{
				errors.append(Json::Value("Address is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(address+1, object["address"].asCString());
			address[0] = '"';
			address[strlen(address)] = '"';
		}

		if(!data.isMember("phone"))
		{
			errors.append(Json::Value("Phone required on department."));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!data["phone"].isString())
			{
				errors.append(Json::Value("Phone is not a string"));
				return Json::Value(Json::nullValue);
			}

			_database->escape(phone+1, object["phone"].asCString());
			phone[0] = '"';
			phone[strlen(phone)] = '"';
		}

		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `department` (`name`, `email`, `address`, `phone`, `super_department_id`)"
													"VALUES (%s, %s, %s, %s, %d);", name, email, address, phone, top_department);
		QueryResult *result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;
	}

	return call_data;
}

Json::Value API::create_user(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];

	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id` FROM `profile_list` WHERE `user_id`=%d AND update=1;", user);
	QueryResult *result = _database->send_query(query);
	std::vector<int> accessible = build_simple_int_vector_from_query(result, "id");
	delete result;

	Json::Value call_data(Json::arrayValue);
	std::vector<unsigned int> added_ids;
	for(unsigned int i = 0; i < data["values"].size(); i++)
	{
		Json::Value &object = data["values"][i];
		char profile[strlen(object["profile"].asCString())*2 + 3];
		char username[strlen(object["username"].asCString())*2 + 3];
		char password[strlen(object["password"].asCString())*2 + 3];
		char certificate[strlen(object["certificate"].asCString())*2 + 3];

		if(!object.isMember("profile"))
		{
			errors.append(Json::Value("Profile required"));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!object["profile"].isInt())
			{
				errors.append(Json::Value("Profile is not an integer"));
				return Json::Value(Json::nullValue);
			}
			_database->escape(profile+1, object["profile"].asCString());
			profile[0] = '"';
			profile[strlen(profile)] = '"';
		}

		if(!object.isMember("username"))
		{
			errors.append(Json::Value("Username required"));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(!object["username"].isString())
			{
				errors.append(Json::Value("Username is not a string"));
				return Json::Value(Json::nullValue);
			}
			_database->escape(username+1, object["username"].asCString());
			username[0] = '"';
			username[strlen(username)] = '"';
		}

		if(!object.isMember("password") && !object.isMember("certificate"))
		{
			errors.append(Json::Value("You must specify either a password or certificate"));
			return Json::Value(Json::nullValue);
		}
		else
		{
			if(object.isMember("password"))
			{
				if(!object["password"].isString())
				{
					errors.append(Json::Value("Password is not a string"));
					return Json::Value(Json::nullValue);
				}
				_database->escape(password+1, object["password"].asCString());
				password[0] = '"';
				password[strlen(password)] = '"';
			}
			else strncpy(password, "NULL", 5);

			if(object.isMember("certificate"))
			{
				if(!object["certificate"].isString())
				{
					errors.append(Json::Value("Certificate is not a string"));
					return Json::Value(Json::nullValue);
				}
				_database->escape(certificate+1, object["certificate"].asCString());
				certificate[0] = '"';
				certificate[strlen(certificate)] = '"';
			}
			else strncpy(certificate, "NULL", 5);
		}

		int p = object["profile"].asInt();
		if (!validate_value_in_vector(p, accessible))
		{
			errors.append(Json::Value("Illegal profile(s)"));
			return Json::Value(Json::nullValue);
		}

		snprintf(query, API_BUFFER_SIZE, "SELECT COUNT (*) AS `createable` FROM `profile` WHERE `id`=%d AND `user_id`=NULL;", p);
		QueryResult *result = _database->send_query(query);

		row_t r = result->next_row();
		if(atoi(r["createable"].c_str()) != 1)
		{
			errors.append(Json::Value("User already exists"));
			return Json::Value(Json::nullValue);
		}

		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `user` (`username`, `password`, `certificate`)"
											"VALUES (%s, %s, %s);", username, password, certificate);
		result = _database->send_query(query);
		added_ids.push_back(_database->insert_id());
		call_data.append(Json::Value(added_ids.back()));
		delete result;

		snprintf(query, API_BUFFER_SIZE, "UPDATE `profile` SET `user_id`=%d WHERE `id`=%d", added_ids.back(), p);
		result = _database->send_query(query);
		delete result;
	}

	return call_data;
}

