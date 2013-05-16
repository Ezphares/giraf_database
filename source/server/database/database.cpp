/*
 * database.cpp

 */

#include "database.h"
#include <cstring>

Database::Database(const char *address, unsigned int port,
		const char *database, const char *username,
		const char *password) :
		_address(address),
		_database(database),
		_username(username),
		_password(password),
		_port(port),
		_conn(NULL){ }

Database::~Database()
{
	if(_conn != NULL)
		disconnect_database();
}

QueryResult *Database::send_query(const char *query)
{
	if(mysql_query(_conn, query))
	{
		fprintf(stderr, "Error %u: %s\n", mysql_errno(_conn), mysql_error(_conn));
		return NULL;
	}
	QueryResult *r = new QueryResult(mysql_store_result(_conn));

	return r;
}

void Database::disconnect_database()
{
	mysql_close(_conn);
	_conn = NULL;
}

int Database::connect_database()
{
	_conn = mysql_init(NULL);
	if(_conn == NULL)
	{
		fprintf(stderr, "Error %u: %s\n", mysql_errno(_conn), mysql_error(_conn));
		return 1;
	}

	if(mysql_real_connect(_conn, _address, _username, _password, _database, _port, "/var/run/mysqld/mysqld.sock", 0) == NULL)
	{
		fprintf(stderr, "Error %u: %s\n", mysql_errno(_conn), mysql_error(_conn));
		return 1;
	}

	return 0;
}

void Database::escape(char *buffer, const char *input)
{
	mysql_real_escape_string(_conn, buffer, input, strlen(input));
}

unsigned int Database::insert_id()
{
	unsigned int id = mysql_insert_id(_conn);
	return id;
}
