/*
 * builders.h
 */

#include <json/value.h>
#include "../database/database.h"

#pragma once
#define EXTRACT_SIZE 2048
enum value_type { V_STRING, V_INT, V_BOOL };

typedef void(*fixture)(Json::Value&);

void fix_rename(Json::Value &object, const char *key, const char *new_key);
void fix_remove(Json::Value &object, const char *key);
void fix_type(Json::Value &object, const char *key, value_type type);

Json::Value build_object_from_row(row_t& row, fixture fix = NULL);
Json::Value build_array_from_query(QueryResult *query, fixture fix = NULL);
Json::Value build_simple_array_from_query(QueryResult *query, const char *key, value_type type = V_STRING );

std::string build_in_string(Json::Value &array);
bool validate_array_vector(Json::Value &array, std::vector<int> &vector);
bool validate_value_in_vector(int value, std::vector<int> &vector);

inline std::vector<int> build_simple_int_vector_from_query(QueryResult *query, const char *key)
{
	std::vector<int> v;
	row_t r = query->next_row();
	while (!r.empty())
	{

		v.push_back(atoi(r[key].c_str()));
		r = query->next_row();
	}

	return v;
}

std::map<int, int> build_simple_int_map_from_query(QueryResult *query, const char *key_key, const char *value_key);

int extract_string(char *buffer, Json::Value &object, const char *key, bool null = false, Database *escape_db = NULL);
int extract_int(int *buffer, Json::Value &object, const char *key, bool null = false);
int extract_bool(bool *buffer, Json::Value &object, const char *key, bool null = false);


