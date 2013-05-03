/*
 * API.h
 */

#pragma once

#define STATUS_OK "OK"
#define STATUS_SYNTAX "SYNTAXERROR"
#define STATUS_STRUCTURE "BADREQUEST"
#define STATUS_AUTH "AUTHFAILED"
#define STATUS_ACCESS "ACCESSDENIED"

#define API_BUFFER_SIZE 4096

#include <json/config.h>
#include <json/value.h>
#include <vector>
#include <map>
#include <cstring>
#include "../database/database.h"
#include "../file/fileio.h"
#include <sstream>
#include <cstdlib>
#include "builders.h"

class API
{
public:
	API();
	virtual ~API();

public:
	std::string handle_request(const char *json);

private:
	// RETURN:
	// >0: User ID
	// -1: Not authenticated
	int authenticate(Json::Value &auth);

	int authenticate_user_pass(Json::Value &username, Json::Value &password);
	int authenticate_certificate(Json::Value &certificate);
	// int authenticate_session(Json::Value session_key); // TODO Future work
	void create_session(Json::Value &response, int user);

	int validate_top(Json::Value &request, Json::Value &errors);
	int validate_auth(Json::Value &auth, Json::Value &errors);
	int validate_read(Json::Value &data, Json::Value &errors);
	int validate_create(Json::Value &data, Json::Value &errors);
	int validate_update(Json::Value &data, Json::Value &errors);
	int validate_delete(Json::Value &data, Json::Value &errors);

	/*
	 * ALL API CALLS GO HERE:
	 */

	int api_read(Json::Value &request, Json::Value &response, Json::Value &errors);
	int api_delete(Json::Value &request, Json::Value &response, Json::Value &errors);
	int api_update(Json::Value &request, Json::Value &response, Json::Value &errors);
	int api_create(Json::Value &request, Json::Value &response, Json::Value &errors);

	Json::Value read_profile_list(Json::Value &data, int user, Json::Value &errors);
	Json::Value read_profile_details(Json::Value &data, int user, Json::Value &errors);
	Json::Value update_profile(Json::Value &data, int user, Json::Value &errors);
	Json::Value create_profile(Json::Value &data, int user, Json::Value &errors);
	Json::Value delete_profile(Json::Value &data, int user, Json::Value &errors);

	Json::Value read_department_list(Json::Value &data, int user, Json::Value &errors);
	Json::Value read_department_details(Json::Value &data, int user, Json::Value &errors);
	Json::Value delete_department(Json::Value &data, int user, Json::Value &errors);

	Json::Value read_user_list(Json::Value &data, int user, Json::Value &errors);
	Json::Value read_user_details(Json::Value &data, int user, Json::Value &errors);
	Json::Value delete_user(Json::Value &data, int user, Json::Value &errors);

	Json::Value read_pictogram_list(Json::Value &data, int user, Json::Value &errors);
	Json::Value read_pictogram_details(Json::Value &data, int user, Json::Value &errors);

	Json::Value read_application_list(Json::Value &data, int user, Json::Value &errors);
	Json::Value read_application_details(Json::Value &data, int user, Json::Value &errors);
	Json::Value delete_application(Json::Value &data, int user, Json::Value &errors);
	// ETC, ETC


private:
	Database *_database;

};

