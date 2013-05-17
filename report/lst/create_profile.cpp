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

		int d = object["department"].asInt();
		if (!validate_value_in_vector(d, departments))
		{
			errors.append(Json::Value("Illegal department"));
			return Json::Value(Json::nullValue);
		}

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

		char name[EXTRACT_SIZE];
		char email[EXTRACT_SIZE];
		char address[EXTRACT_SIZE];
		char phone[EXTRACT_SIZE];
		char picture[EXTRACT_SIZE];
		char settings[EXTRACT_SIZE];
		int department;
		int role;

		int err = 0;
		err += extract_string(name, object, "name", false);
		err += extract_string(email, object, "email", true);
		err += extract_string(address, object, "address", false);
		err += extract_string(phone, object, "phone", true);
		err += extract_string(picture, object, "picture", true);
		err += extract_string(settings, object, "settings", true);
		err += extract_int(&department, object, "department", false);
		err += extract_int(&role, object, "role", false);
		if (err != 0)
		{
			errors.append("Value error(s) in profile data object");
			return Json::Value(Json::nullValue);
		}

		char query[API_BUFFER_SIZE];
		snprintf(query, API_BUFFER_SIZE, "INSERT INTO `profile` (`name`, `email`, `department_id`, `role`, `address`, `phone`, `picture`, `settings`, `author`)"
											"VALUES (%s, %s, %d, %d, %s, %s, %s, %s, %d);", name, email, department, role, address, phone, picture, settings, user);
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