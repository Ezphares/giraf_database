Json::Value API::read_profile_list(Json::Value &data, int user, Json::Value &errors)
{
	char query[API_BUFFER_SIZE];
	snprintf(query, API_BUFFER_SIZE, "SELECT DISTINCT `id`, `name`, `role` FROM `profile_list` WHERE `user_id`=%d;", user);
	QueryResult *result = _database->send_query(query);

	Json::Value call_data = build_array_from_query(result, fix_profile_list);

	delete result;
	return call_data;
}