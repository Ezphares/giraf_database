/*
 * builders.h
 */

#include <json/value.h>
#include "../database/database.h"

#pragma once

enum value_type { V_STRING, V_INT, V_BOOL };

typedef void(*fixture)(Json::Value&);

void fix_rename(Json::Value &object, const char *key, const char *new_key);
void fix_remove(Json::Value &object, const char *key);
void fix_type(Json::Value &object, const char *key, value_type type);

Json::Value build_object_from_row(row_t& row, fixture fix = NULL);
Json::Value build_array_from_query(QueryResult *query, fixture fix = NULL);
Json::Value build_simple_array_from_query(QueryResult *query, const char *key, value_type type = V_STRING );
