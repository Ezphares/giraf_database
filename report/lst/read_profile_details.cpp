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