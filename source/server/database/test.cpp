#define BOOST_TEST_MODULE database
#include <boost/test/included/unit_test.hpp>
#include <cstring>

#include "database.h"

BOOST_AUTO_TEST_SUITE(database_lib)

BOOST_AUTO_TEST_CASE( classes )
{
	Database fail_db("localhost", 1500, "test", "giraf", "admin"); //instantiate fail database (wrong port, wrong password
	fprintf(stderr, "Unit test: Expecting logged error\n  ");
	BOOST_CHECK_EQUAL(fail_db.connect_database(), 1);
	Database db("localhost", 3306, "test", "giraf", "123456"); //working db
	BOOST_CHECK_EQUAL(db.connect_database(), 0);
	fprintf(stderr, "Unit test: Expecting logged error\n  ");
	BOOST_CHECK_EQUAL(db.send_query("THIS BETTER FAIL"), (void *)NULL); //query is dumb. Should fail.
	db.send_query("DROP TABLE IF EXISTS `test`");
	db.send_query("CREATE TABLE `test` (`id` INT AUTO_INCREMENT, `name` VARCHAR(25), PRIMARY KEY (`id`))");
	db.send_query("INSERT INTO `test` (`name`) VALUES('Herp')");
	QueryResult *res = db.send_query("SELECT `name` from `test` WHERE `name` = 'Herp'");
	row_t row = res->next_row();
	BOOST_CHECK(strcmp(row["name"].c_str(), "Herp") == 0);
	BOOST_CHECK_EQUAL(res->next_row().size(), 0);

	db.disconnect_database();
}

BOOST_AUTO_TEST_SUITE_END()
