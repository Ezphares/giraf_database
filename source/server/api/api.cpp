/*
 * API.cpp
 */

#include "api.h"
#include <json/reader.h>
#include <json/writer.h>


API::API() {
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

	if (validate_top(root, errors) < 0)
	{
		response["status"] = Json::Value(STATUS_STRUCTURE);

		goto done;
	}

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
	response["errors"] = errors;
	return writer.write(response).c_str();
}

int API::authenticate(Json::Value &auth)
{
	return 1; // TODO
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
