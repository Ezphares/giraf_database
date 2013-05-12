giraf_database
==============

AAU GIRAF project, database server and synchronization

Group: SW603F13 <br />
Office: 5.1.16

Authors:<br />
[Barbara Albertine Flindt](https://github.com/DorkMatter)<br />
[Simon Jensen](https://github.com/sjens)<br />
[Hilmar Laksa Magnussen](https://github.com/Hilmar10)<br />
[Jeppe Blicher Tarp](https://github.com/Ezphares)<br />


Build instructions
==================
Pre-requisites: <br />
MySQL Connector/C <br />
* Download from [MySQL](http://dev.mysql.com/downloads/connector/c/)
* Headers in /usr/local/include <br />
* Libs (libmysql.so etc.) in /usr/local/lib

JsonCPP <br />
* Install instructions found [here](http://stackoverflow.com/questions/4628922/building-jsoncpp-linux-an-instruction-for-us-mere-mortals)
* Headers in /usr/local/include <br />
* Libs (renamed to libjson.so, libjson.a) in /usr/local/include


Add line /usr/local/lib to /etc/ld.so.conf and run `sudo ldconfig` <br />
Unit tests require BOOST installed

Building the program:<br />
cd to `source/server`<br />
run `make all`<br />
run `./serverapp`<br />
At the moment this should just exit gracefully.

Running the unit tests:<br />
cd to `source/server`<br />
run `make test`<br />
run `./test_connection` or `./test_database`<br />
Database unit test requires a database called `giraf` accessible by user `giraf`@`localhost` identified by `123456`.<br />
Connection unit test requires right to open listening sockets and ports `1238` and `1239` to be free.

License
==========
The report content is freely available, but publication (with source), only after agreement with the authors.

GIRAF Database's source code is released under the [GPLv3](https://github.com/Zucka/girafAdmin/blob/master/LICENSE.md) open source license. This means that you are free to inspect the source code at any time or contribute to the project yourself.
