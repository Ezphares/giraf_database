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
			char type[EXTRACT_SIZE];
			_database->escape(type, object["type"].asCString());
			int object_id = object["id"].asInt();

			if(strcmp(object["type"].asCString(), "application") == 0)
			{
				if(object.isMember("settings") && profile != 0)	_database->escape(settings, object["settings"].asCString());

				snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `application` WHERE `id`=%d", object_id);
				result = _database->send_query(query);
				r = result->next_row();
				delete result;

				if (r.empty())
				{
					errors.append(Json::Value("Illegal application"));
					return Json::Value(Json::nullValue);
				}

			}
			else
			{
				snprintf(query, API_BUFFER_SIZE, "SELECT `id` FROM `pictogram` WHERE `id`=%d", object_id); //TODO Limit this to visible and public pics
				result = _database->send_query(query);
				r = result->next_row();
				delete result;

				if (r.empty())
				{
					errors.append(Json::Value("Illegal pictogram"));
					return Json::Value(Json::nullValue);
				}
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
			char type[EXTRACT_SIZE];
			_database->escape(type, object["type"].asCString());
			int object_id = object["id"].asInt();

			snprintf(query, API_BUFFER_SIZE, "DELETE FROM `%s_%s` WHERE `%s_id`=%d AND `%s_id`=%d;", actor, type, actor, actor_id, type, object_id);
			result = _database->send_query(query);
			delete result;
		}
	}
	return Json::Value(Json::nullValue);
}