#define BOOST_TEST_MODULE api
#include <boost/test/included/unit_test.hpp>

#include "api.h"
#include <json/reader.h>


BOOST_AUTO_TEST_SUITE(api_calls)

int id_profile;
int id_department;
int id_user;
int id_application;
int id_pictogram;
int id_category;

BOOST_AUTO_TEST_CASE(api_builders)
{
	std::vector<int> vec;
	vec.push_back(1);
	vec.push_back(3);

	BOOST_CHECK_EQUAL(validate_value_in_vector(1, vec), true);
	BOOST_CHECK_EQUAL(validate_value_in_vector(2, vec), false);

	Json::Value a1(Json::arrayValue);
	a1.append(Json::Value(1));
	a1.append(Json::Value(3));

	BOOST_CHECK_EQUAL(validate_array_vector(a1, vec), true);

	a1.append(Json::Value(2));

	BOOST_CHECK_EQUAL(validate_array_vector(a1, vec), false);

	Json::Value o1(Json::objectValue);
	o1["test"] = Json::Value("1");

	BOOST_CHECK_EQUAL(o1["test"].isString(), true);
	BOOST_CHECK_EQUAL(o1["test"].isInt(), false);

	fix_type(o1, "test", V_INT);

	BOOST_CHECK_EQUAL(o1["test"].isString(), false);
	BOOST_CHECK_EQUAL(o1["test"].isInt(), true);

	fix_rename(o1, "test", "newtest");

	BOOST_CHECK_EQUAL(o1.isMember("test"), false);
	BOOST_CHECK_EQUAL(o1.isMember("newtest"), true);

	fix_remove(o1, "newtest");

	BOOST_CHECK_EQUAL(o1.isMember("newtest"), false);

}

BOOST_AUTO_TEST_CASE(api_structure)
{
	Json::Value check;
	Json::Reader reader;

	const char *no_key = "{}";
	reader.parse(API().handle_request(no_key), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *no_action = "{\"auth\":{}, \"data\":null}";
	reader.parse(API().handle_request(no_action), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *no_data = "{\"auth\":{}, \"action\":null}";
	reader.parse(API().handle_request(no_data), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *no_auth = "{\"data\":null, \"action\":null}";
	reader.parse(API().handle_request(no_auth), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *structure = "{\"data\":null, \"action\":null, \"auth\":{\"username\":\"\", \"password\":\"\"}}";
	reader.parse(API().handle_request(structure), check);
	BOOST_CHECK(strcmp(check["status"].asCString(), STATUS_STRUCTURE) != 0);

}

BOOST_AUTO_TEST_CASE(api_null)
{
	Json::Value check;
	Json::Reader reader;

	const char *auth_fail = "{\"data\":null, \"action\":null, \"auth\":{\"username\":\"flerp\", \"password\":\"derp\"}}";
	reader.parse(API().handle_request(auth_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_AUTH), 0);

	const char *auth_succeed = "{\"data\":null, \"action\":null, \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(auth_succeed), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);
	BOOST_CHECK_EQUAL(check.isMember("session"), true);

}

BOOST_AUTO_TEST_CASE(api_create)
{
	Json::Value check;
	Json::Reader reader;
	char query[API_BUFFER_SIZE];

	const char *profile_fail = "{\"data\":{\"type\":\"profile\", \"values\":[{\"name\": 1}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *profile_success = "{\"data\":{\"type\":\"profile\", \"values\":[{\"name\": \"Jeppe\", \"department\": 1, \"role\": 2, \"address\": \"Selma Lagerlofs Vej 300\"}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_success), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);
	id_profile = check["data"][0u].asInt();

	const char *user_fail = "{\"data\":{\"type\":\"user\", \"values\":[{\"username\": 1}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(user_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *user_success = "{\"data\":{\"type\":\"user\", \"values\":[{\"username\": \"Jeppe\", \"password\":\"Jeppe\", \"profile\":%d}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, user_success, id_profile);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);
	id_user = check["data"][0u].asInt();

	const char *department_fail = "{\"data\":{\"type\":\"department\", \"values\":[{\"name\": 1}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *department_success = "{\"data\":{\"type\":\"department\", \"values\":[{\"name\": \"Jeppes Crib\", \"address\": \"Selma Lagerlofs Vej 300\", \"phone\": \"12345678\", \"email\": \"test@test.com\", \"topdepartment\": 1}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_success), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);
	id_department = check["data"][0u].asInt();

	const char *application_fail = "{\"data\":{\"type\":\"application\", \"values\":[{\"name\": 1}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *application_success = "{\"data\":{\"type\":\"application\", \"values\":[{\"name\": \"Jeppe\", \"version\": \"1\", \"icon\": \"2\", \"package\": \"SelmaSelma\", \"activity\": \"herpie\", \"description\": \"hurr\"}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_success), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);
	id_application = check["data"][0u].asInt();

	const char *category_fail = "{\"data\":{\"type\":\"category\", \"values\":[{\"name\": 1}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(category_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *category_success = "{\"data\":{\"type\":\"category\", \"values\":[{\"name\": \"Jeppe\", \"colour\": \"red\"}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(category_success), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);
	id_category = check["data"][0u].asInt();

	const char *pictogram_fail = "{\"data\":{\"type\":\"pictogram\", \"values\":[{\"name\": 1}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(pictogram_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *pictogram_success = "{\"data\":{\"type\":\"pictogram\", \"values\":[{\"name\": \"Jeppe\", \"public\": false, \"categories\":[%d]}]}, \"action\":\"create\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, pictogram_success, id_category);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);
	id_pictogram = check["data"][0u].asInt();

}

BOOST_AUTO_TEST_CASE(api_read)
{
	Json::Value check;
	Json::Reader reader;
	char query[API_BUFFER_SIZE];

	const char *profile_list = "{\"data\":{\"type\":\"profile\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("name"), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("role"), true);

	const char *profile_fail = "{\"data\":{\"type\":\"profile\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *profile_succeed = "{\"data\":{\"type\":\"profile\", \"view\":\"details\", \"ids\":[%d]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, profile_succeed, id_profile);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

	const char *user_list = "{\"data\":{\"type\":\"user\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(user_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);

	const char *user_fail = "{\"data\":{\"type\":\"user\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(user_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *user_succeed = "{\"data\":{\"type\":\"user\", \"view\":\"details\", \"ids\":[%d]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, user_succeed, id_user);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

	const char *department_list = "{\"data\":{\"type\":\"department\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

	const char *department_fail = "{\"data\":{\"type\":\"department\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *department_succeed = "{\"data\":{\"type\":\"department\", \"view\":\"details\", \"ids\":[%d]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, department_succeed, id_department);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

	const char *application_list = "{\"data\":{\"type\":\"application\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);

	const char *application_fail = "{\"data\":{\"type\":\"application\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *application_succeed = "{\"data\":{\"type\":\"application\", \"view\":\"details\", \"ids\":[%d]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, application_succeed, id_application);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

	const char *pictogram_list = "{\"data\":{\"type\":\"pictogram\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(pictogram_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

	const char *pictogram_fail = "{\"data\":{\"type\":\"pictogram\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(pictogram_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *pictogram_succeed = "{\"data\":{\"type\":\"pictogram\", \"view\":\"details\", \"ids\":[%d]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, pictogram_succeed, id_pictogram);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

	const char *category_list = "{\"data\":{\"type\":\"category\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(category_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

	const char *category_fail = "{\"data\":{\"type\":\"category\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(category_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *category_succeed = "{\"data\":{\"type\":\"category\", \"view\":\"details\", \"ids\":[%d]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, category_succeed, id_category);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), true);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), true);

}

BOOST_AUTO_TEST_CASE(api_update)
{
	Json::Value check;
	Json::Reader reader;
	char query[API_BUFFER_SIZE];

	const char *profile_fail = "{\"data\":{\"type\": \"profile\", \"values\": [{\"id\": 1042, \"value\":{\"name\": \"snuffti\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *profile_success = "{\"data\":{\"type\": \"profile\", \"values\": [{\"id\": %d, \"value\":{\"name\": \"snuffti\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, profile_success, id_profile);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *user_fail = "{\"data\":{\"type\": \"user\", \"values\": [{\"id\": 1042, \"value\":{\"password\": \"snuffti\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *user_success = "{\"data\":{\"type\": \"user\", \"values\": [{\"id\": %d, \"value\":{\"password\": \"snuffti\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, profile_success, id_user);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *department_fail = "{\"data\":{\"type\": \"department\", \"values\": [{\"id\": 1042, \"value\":{\"name\": \"snuffti\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *department_success = "{\"data\":{\"type\": \"department\", \"values\": [{\"id\": %d, \"value\":{\"name\": \"snuffti\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, department_success, id_department);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *application_fail = "{\"data\":{\"type\": \"application\", \"values\": [{\"id\": 1042, \"value\":{\"version\": \"2.0\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *application_success = "{\"data\":{\"type\": \"application\", \"values\": [{\"id\": %d, \"value\":{\"version\": \"2.0\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, application_success, id_application);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *category_fail = "{\"data\":{\"type\": \"category\", \"values\": [{\"id\": 1042, \"value\":{\"version\": \"2.0\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(category_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *category_success = "{\"data\":{\"type\": \"category\", \"values\": [{\"id\": %d, \"value\":{\"name\": \"Barbara\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, category_success, id_category);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *pictogram_fail = "{\"data\":{\"type\": \"pictogram\", \"values\": [{\"id\": 1042, \"value\":{\"version\": \"2.0\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(pictogram_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *pictogram_success = "{\"data\":{\"type\": \"pictogram\", \"values\": [{\"id\": %d, \"value\":{\"sound\": \"Barbara\"}}]}, \"action\":\"update\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, pictogram_success, id_pictogram);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

}

BOOST_AUTO_TEST_CASE(api_delete)
{
	Json::Value check;
	Json::Reader reader;
	char query[API_BUFFER_SIZE];

	const char *user_fail = "{\"data\":{\"type\": \"user\", \"ids\":[1042]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(user_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *user_success = "{\"data\":{\"type\": \"user\", \"ids\":[%d]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, user_success, id_user);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *department_fail = "{\"data\":{\"type\": \"department\", \"ids\":[1042]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *department_success = "{\"data\":{\"type\": \"department\", \"ids\":[%d]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, department_success, id_department);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *application_fail = "{\"data\":{\"type\": \"application\", \"ids\":[1042]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *application_success = "{\"data\":{\"type\": \"application\", \"ids\":[%d]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, application_success, id_application);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0); //Applications cannot be deleted, unlink instead.

	const char *profile_fail = "{\"data\":{\"type\": \"profile\", \"ids\":[1042]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *profile_success = "{\"data\":{\"type\": \"profile\", \"ids\":[%d]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, profile_success, id_profile);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *category_fail = "{\"data\":{\"type\": \"category\", \"ids\":[1042]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(category_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *category_success = "{\"data\":{\"type\": \"category\", \"ids\":[%d]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, category_success, id_category);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

	const char *pictogram_fail = "{\"data\":{\"type\": \"pictogram\", \"ids\":[1042]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(pictogram_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *pictogram_success = "{\"data\":{\"type\": \"pictogram\", \"ids\":[%d]}, \"action\":\"delete\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	snprintf(query, API_BUFFER_SIZE, pictogram_success, id_pictogram);
	reader.parse(API().handle_request(query), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_OK), 0);

}

BOOST_AUTO_TEST_SUITE_END()
