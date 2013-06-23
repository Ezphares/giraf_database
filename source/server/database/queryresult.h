/*
 * queryresult.h
 */

#pragma once
#include <my_global.h>
#undef min
#undef max
#include <mysql.h>
#include <map>
#include <string>


typedef std::map<std::string, std::string> row_t;

class QueryResult {
public:
	QueryResult (MYSQL_RES *result);
	virtual ~QueryResult();

public:
	row_t next_row();

private:
	MYSQL_RES *_result;
};
