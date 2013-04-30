/*
 * API.h
 */

#pragma once

#define STATUS_OK "OK"
#define STATUS_SYNTAX "SYNTAXERROR"
#define STATUS_STRUCTURE "BADREQUEST"
#define STATUS_AUTH "AUTHFAILED"
#define STATUS_ACCESS "ACCESSDENIED"

const unsigned int BUFFER_SIZE = 4096;

enum rights_t : char {CREATE = 0, READ = 1, UPDATE = 2, DELETE = 3};
enum rights_v : char {NONE = 0, EMULATE = 1, TRUE = 3};

#include <json/value.h>
#include <vector>
#include <map>
#include <cstring>
#include "../database/database.h"

class API
{
public:
	API();
	virtual ~API();

public:
	const char *handle_request(const char *json);

private:
	// RETURN:
	// >0: User ID
	// -1: Not authenticated
	int authenticate(Json::Value &auth);

	int authenticate_user_pass(Json::Value &username, Json::Value &password);
	int authenticate_certificate(Json::Value &certificate);
	// int authenticate_session(Json::Value session_key); // TODO Future work


	int validate_top(Json::Value &request, Json::Value &errors);
	int validate_auth(Json::Value &auth, Json::Value &errors);
	int validate_read(Json::Value &data, Json::Value &errors);
	int validate_write(Json::Value &data, Json::Value &errors);
	int validate_update(Json::Value &data, Json::Value &errors);
	int validate_delete(Json::Value &data, Json::Value &errors);




	std::map<int, rights_v> get_rights(int user_id, const char *data_type, std::vector<int> ids, rights_t rights);

	/*
	 * ALL API CALLS GO HERE:
	 */

	int api_read(Json::Value &request, Json::Value &response, Json::Value &errors);
	Json::Value read_profile_list(Json::Value &data, int user, Json::Value &errors);
	Json::Value read_profile_details(Json::Value &data, int user, Json::Value &errors);

	Json::Value update_profile(Json::Value &data, int user, Json::Value &errors);
	Json::Value create_profile(Json::Value &data, int user, Json::Value &errors);
	Json::Value delete_profile(Json::Value &data, int user, Json::Value &errors);
	// ETC, ETC


private:
	Database *_database;

};

