/*
 * builders.cpp
 */

#include "builders.h"
#include <iostream>
#include <sstream>
#include <cstring>

void fix_rename(Json::Value &object, const char *key, const char *new_key)
{
	object[new_key] = object[key];
	fix_remove(object, key);
}

void fix_remove(Json::Value &object, const char *key)
{
	object.removeMember(key);
}

void fix_type(Json::Value &object, const char *key, value_type type)
{
	if (type == V_INT) object[key] = Json::Value(atoi(object[key].asCString()));
	if (type == V_BOOL) object[key] = Json::Value((bool)!!atoi(object[key].asCString()));
}

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

Json::Value build_simple_array_from_query(QueryResult *query, const char *key, value_type type)
{
	Json::Value a(Json::arrayValue);
	row_t r = query->next_row();
	while (!r.empty())
	{
		if (type == V_STRING) a.append(Json::Value(r[key]));
		else if (type == V_INT) a.append(Json::Value(atoi(r[key].c_str())));
		else if (type == V_BOOL) a.append(Json::Value((bool)!!atoi(r[key].c_str())));

		r = query->next_row();
	}

	return a;
}

std::string build_in_string(Json::Value &array)
{
	std::stringstream str;
	for (unsigned int i = 0; i < array.size(); i++)
	{
		if (i > 0) str << ",";
		str << array[i].asInt();
	}
	return str.str();
}

bool validate_array_vector(Json::Value &array, std::vector<int> &vec)
{
	for (unsigned int i = 0; i < array.size(); i++)
	{
		if (!array[i].isInt()) return false;
		bool found = false;
		int id = array[i].asInt();
		for(std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)	if(id == *it) found = true;

		if(found == false) return false;
	}
	return true;
}

bool validate_value_in_vector(int value, std::vector<int> &vector)
{
	for(std::vector<int>::iterator it = vector.begin(); it != vector.end(); it++)	if(value == *it) return true;

	return false;
}

std::map<int, int> build_simple_int_map_from_query(QueryResult *query, const char *key_key, const char *value_key)
{
	std::map <int, int> a;
	row_t r = query->next_row();

	while(!r.empty())
	{
		a.insert(std::pair<int, int>(atoi(r[key_key].c_str()), atoi(r[value_key].c_str())));

		r = query->next_row();
	}

	return a;

}

int extract_string(char *buffer, Json::Value &object, const char *key, bool null)
{
	if (object.isMember(key))
	{
		if (!object[key].isString()) return -1;
		const char *value = object[key].asCString();
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

int extract_int(int *buffer, Json::Value &object, const char *key, bool null)
{
	if (object.isMember(key))
	{
		if (!object[key].isInt()) return -1;
		int value = object[key].asInt();
		*buffer = value;
	}
	else
	{
		if (null) *buffer = 0;
		else return -1;
	}
	return 0;
}

