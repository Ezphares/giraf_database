Json::Value build_object_from_row(row_t& row, fixture fix)
{
	Json::Value o(Json::objectValue);
	for (row_t::iterator it = row.begin(); it != row.end(); it++)
	{
		o[it->first] = Json::Value(it->second);
	}
	if (fix != NULL) fix(o);
	return o;
}