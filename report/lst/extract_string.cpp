int extract_string(char *buffer, Json::Value &object, const char *key, bool null, Database *escape_db)
{
	if (object.isMember(key))
	{
		if (!object[key].isString()) return -1;
		const char *raw_value = object[key].asCString();
		char value[EXTRACT_SIZE];
		memset(value, 0, EXTRACT_SIZE);

		if (escape_db != NULL) escape_db->escape(value, raw_value);
		else std::strncpy(value, raw_value, EXTRACT_SIZE - 1);

		unsigned int length = std::min(EXTRACT_SIZE - 3u, (unsigned int)strlen(value));
		std::strncpy(buffer + 1, value, length);
		buffer[0] = '\'';
		buffer[length + 1] = '\'';
		buffer[length + 2] = '\0';
	}
	else
	{
		if (null) strncpy(buffer, "NULL", 5);
		else return -1;
	}
	return 0;
}