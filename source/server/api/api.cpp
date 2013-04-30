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
	// TODO Auto-generated destructor stub
}


/*
 * NOTE ON GOTO:
 * While GOTO is normally a bad idea, in this particular case it is the only sensible way to avoid
 * the code becoming a mess. We know and acknowledge that you should be VERY careful with gotos,
 * and all other options were considered.
 */

const char *API::handle_request(const char *json)
{
	Json::Value root, response(Json::objectValue), errors(Json::arrayValue);
	Json::Reader reader;
	Json::FastWriter writer;

	response["errors"] = errors;
	response["data"] = Json::Value(Json::nullValue);
	response["status"] = Json::Value(STATUS_OK);

	if (!reader.parse(json, root))
	{
		// Parsing failed
		response["status"] = Json::Value(STATUS_SYNTAX);
		errors.append(Json::Value("Request was not valid JSON"));

		goto done;
	}

	if (validate_top(root, errors) < 0 || validate_auth(root["auth"], errors) < 0)
	{
		response["status"] = Json::Value(STATUS_STRUCTURE);

		goto done;
	}

	_database->connect_database();

	if (root["action"].isNull())
	{
		// TODO
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

			goto done;
		}
	}

	done:

	_database->disconnect_database();
	response["errors"] = errors;
	return writer.write(response).c_str();
}

int API::authenticate(Json::Value &auth)
{
	char buffer [BUFFER_SIZE];

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
		char cert [strlen(auth["certifiecate"].asCString()) * 2 + 1];

		_database->escape(cert, auth["certificate"].asCString());

		sprintf(buffer, "SELECT `id` "
							"FROM `user` "
							"WHERE `certificate`='%s';",
							cert);
	}

	QueryResult *result = _database->send_query(buffer);
	result->next_row();

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
		if (!request["data"].isObject())
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
