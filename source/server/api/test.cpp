#define BOOST_TEST_MODULE api
#include <boost/test/included/unit_test.hpp>

#include "api.h"
#include <json/reader.h>


BOOST_AUTO_TEST_SUITE(api_calls)

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

	const char *auth_fail = "{\"data\":null, \"action\":null, \"auth\":{\"username\":\"\", \"password\":\"\"}}";
	reader.parse(API().handle_request(auth_fail), check);
	BOOST_CHECK(strcmp(check["status"].asCString(), STATUS_AUTH) == 0);

	const char *auth_succeed = "{\"data\":null, \"action\":null, \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(auth_succeed), check);
	BOOST_CHECK(strcmp(check["status"].asCString(), STATUS_OK) == 0);
	BOOST_CHECK(check.isMember("session") == 0);

}

BOOST_AUTO_TEST_CASE(api_read)
{
	Json::Value check;
	Json::Reader reader;

	const char *profile_list = "{\"data\":{\"type\":\"profile\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("name"), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("role"), 0);

	const char *profile_fail = "{\"data\":{\"type\":\"profile\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *profile_succeed = "{\"data\":{\"type\":\"profile\", \"view\":\"details\", \"ids\":[1]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(profile_succeed), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), 0);

	const char *user_list = "{\"data\":{\"type\":\"user\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(user_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);

	const char *user_fail = "{\"data\":{\"type\":\"user\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(user_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *user_succeed = "{\"data\":{\"type\":\"user\", \"view\":\"details\", \"ids\":[1]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(user_succeed), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), 0);

	const char *department_list = "{\"data\":{\"type\":\"department\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), 0);

	const char *department_fail = "{\"data\":{\"type\":\"department\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *department_succeed = "{\"data\":{\"type\":\"department\", \"view\":\"details\", \"ids\":[1]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(department_succeed), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), 0);

	const char *application_list = "{\"data\":{\"type\":\"application\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);

	const char *application_fail = "{\"data\":{\"type\":\"application\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *application_succeed = "{\"data\":{\"type\":\"application\", \"view\":\"details\", \"ids\":[1]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(application_succeed), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), 0);

	const char *pictogram_list = "{\"data\":{\"type\":\"pictogram\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(pictogram_list), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), 0);

	const char *pictogram_fail = "{\"data\":{\"type\":\"pictogram\", \"view\":\"details\", \"ids\":[1000, 42]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(pictogram_fail), check);
	BOOST_CHECK_EQUAL(strcmp(check["status"].asCString(), STATUS_ACCESS), 0);

	const char *pictogram_succeed = "{\"data\":{\"type\":\"pictogram\", \"view\":\"details\", \"ids\":[1]}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}";
	reader.parse(API().handle_request(pictogram_succeed), check);
	BOOST_CHECK_EQUAL(check["data"].isArray(), 0);
	BOOST_CHECK_EQUAL(check["data"][0u].isMember("id"), 0);

}

BOOST_AUTO_TEST_CASE(api_update)
{

}

BOOST_AUTO_TEST_CASE(api_delete)
{

}

BOOST_AUTO_TEST_CASE(api_create)
{

}

BOOST_AUTO_TEST_SUITE_END()
