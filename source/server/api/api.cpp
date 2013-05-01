/*
 * API.cpp
 */

#include "api.h"
#include <json/reader.h>
#include <json/writer.h>


API::API()
{
	_database = new Database("127.0.0.1", 3306, "giraf", "giraf", "123456"); // TODO: Database
	// TODO Auto-generated constructor stub

}

API::~API() {
	delete _database;
}


std::string API::handle_request(const char *json)
{
	Json::Value root, response(Json::objectValue), errors(Json::arrayValue);
	Json::Reader reader;
	Json::FastWriter writer;

	response["data"] = Json::Value(Json::nullValue);
	response["status"] = Json::Value(STATUS_OK);
	response["session"] = Json::Value(Json::nullValue);

	do
	{
		if (!reader.parse(json, root))
		{
			// Parsing failed
			response["status"] = Json::Value(STATUS_SYNTAX);
			errors.append(Json::Value("Request was not valid JSON"));

			break;
		}

		if (validate_top(root, errors) < 0 || validate_auth(root["auth"], errors) < 0)
		{
			response["status"] = Json::Value(STATUS_STRUCTURE);

			break;
		}

		_database->connect_database();

		if (root["action"].isNull())
		{
			int user = authenticate(root["auth"]);
			if (user) create_session(response, user);
		}
		else
		{
			const char* action = root["action"].asCString();
			if (strcmp(action, "create") == 0)
			{
				//TODO
			}
			else if (strcmp(action, "read") == 0)
			{
				api_read(root, response, errors);
			}
			else if (strcmp(action, "update") == 0)
			{
				//TODO
			}
			else if (strcmp(action, "delete") == 0)
			{
				//TODO
			}
			else
			{
				errors.append("\"action\" was not null or a CRUD action");
				response["status"] = Json::Value(STATUS_STRUCTURE);

				break;
			}
		}
	}
	while (0); /* break abuse */

	_database->disconnect_database();
	response["errors"] = errors;
	std::string a = writer.write(response).c_str(); // Pointer hack. JsonCPP does weird stuff.

	return a;
}

void API::create_session(Json::Value &response, int user)
{
	Json::Value session(Json::objectValue);
	session["user"] = Json::Value(user);
	session["session"] = Json::Value("");

	char buffer [API_BUFFER_SIZE];

	sprintf(buffer, "SELECT `id` "
						"FROM `profile` "
						"WHERE `user_id`=%d;",
			user);

	QueryResult *result = _database->send_query(buffer);
	row_t r = result->next_row();

	if (r.empty()) session["profile"] = Json::Value(Json::nullValue);
	else session["profile"] = Json::Value(atoi(r["id"].c_str()));

	response["session"] = session;

	return;
}

int API::authenticate(Json::Value &auth)
{
	char buffer [API_BUFFER_SIZE];

	if (auth.isMember("username"))
	{
		char username [strlen(auth["username"].asCString()) * 2 + 1];
		char password [strlen(auth["password"].asCString()) * 2 + 1];

		_database->escape(username, auth["username"].asCString());
		_database->escape(password, auth["password"].asCString());

		sprintf(buffer, "SELECT `id` "
							"FROM `user` "
							"WHERE `username`='%s' AND `password`='%s';",
							username, password);
	}
	else if (auth.isMember("certificate"))
	{
		char cert [strlen(auth["certificate"].asCString()) * 2 + 1];

		_database->escape(cert, auth["certificate"].asCString());

		sprintf(buffer, "SELECT `id` "
							"FROM `user` "
							"WHERE `certificate`='%s';",
							cert);
	}

	QueryResult *result = _database->send_query(buffer);
	row_t row = result->next_row();
	delete result;

	if (!row.empty()) return atoi(row["id"].c_str());
	return -1;
}

int API::validate_top(Json::Value &request, Json::Value &errors)
{
	if (!request.isObject()) // This check SHOULD be redundant, due to json syntax, but just to be safe.
	{
		errors.append(Json::Value("Request was not an object"));
	}

	if (!request.isMember("auth"))
	{
		errors.append(Json::Value("\"auth\" key missing"));
	}
	else
	{
		if (!request["auth"].isObject())
		{
			errors.append(Json::Value("\"auth\" was not an object"));
		}
	}

	if (!request.isMember("action"))
	{
		errors.append(Json::Value("\"action\" key missing"));
	}
	else
	{
		if (!request["action"].isString() && !request["action"].isNull())
		{
			errors.append(Json::Value("\"action\" was not an a string or null"));
		}
	}

	if (!request.isMember("data"))
	{
		errors.append(Json::Value("\"data\" key missing"));
	}
	else
	{
		if (!request["data"].isObject() && !request["action"].isNull())
		{
			errors.append(Json::Value("\"data\" was not an object"));
		}
	}

	if (errors.empty()) return 0;
	else return -1;
}

int API::validate_auth(Json::Value &auth, Json::Value &errors)
{
	if (auth.isMember("username") && auth.isMember("password"))
	{
		if (!auth["username"].isString()) errors.append(Json::Value("\"auth\":\"username\" is not a string"));
		if (!auth["password"].isString()) errors.append(Json::Value("\"auth\":\"password\" is not a string"));
	}
	else if (auth.isMember("certificate"))
	{
		if (!auth["certificate"].isString()) errors.append(Json::Value("\"auth\":\"certificate\" is not a string"));
	}
	/*else if (auth.isMember("session"))
	{
		if (!auth["session"].isString()) errors.append(Json::Value("\"auth\":\"session\" is not a string"));
	}*/
	else errors.append(Json::Value("\"auth\" object invalid"));

	if (errors.empty()) return 0;
	else return -1;
}
