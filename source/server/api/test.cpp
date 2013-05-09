#define BOOST_TEST_MODULE api
#include <boost/test/included/unit_test.hpp>

#include "api.h"

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
	reader.parse(API().handle_request(no_data), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *structure = "{\"data\":null, \"action\":null, \"auth\":{\"username\":\"\", \"password\":\"\"}}";
	reader.parse(API().handle_request(no_data), check);
	BOOST_CHECK(strcmp(check["status"].asCString(), STATUS_STRUCTURE) != 0);

}

BOOST_AUTO_TEST_CASE(api_null)
{

}

BOOST_AUTO_TEST_CASE(api_read)
{

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
