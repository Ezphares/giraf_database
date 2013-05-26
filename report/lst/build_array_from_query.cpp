Json::Value build_array_from_query(QueryResult *query, fixture fix)
{
	Json::Value a(Json::arrayValue);
	row_t r = query->next_row();
	while (!r.empty())
	{
		a.append(build_object_from_row(r, fix));

		r = query->next_row();
	}

	return a;
}