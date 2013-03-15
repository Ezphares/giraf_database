/*
 * queryresult.cpp
 *
 *  Created on: 13 Mar 2013
 *      Author: vixen
 */

#include "queryresult.h"

QueryResult::QueryResult(MYSQL_RES *result): _result(result) { }

QueryResult::~QueryResult()
{
	mysql_free_result(_result);
}

row_t QueryResult::next_row()
{
	MYSQL_ROW row;
	MYSQL_FIELD *fields;
	row_t r;

	if(!(row = mysql_fetch_row(_result)))
	{
		return r;
	}

	int num_fields = mysql_num_fields(_result);
	fields = mysql_fetch_fields(_result);

	for(int i = 0; i < num_fields; i++)
	{
		r.insert(std::pair<std::string, std::string>(fields[i].name, row[i]));
	}

	return r;
}
