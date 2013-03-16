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

To build the program, make sure boost and MySQL Connector/C is installed and setup correctly.
If non-standard settings are used for these, modify the settings section in Makefile

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
Connection unit test requires right to open listening sockets and ports `1238` and `1237` to be free.

License
==========
The report content is freely available, but publication (with source), only after agreement with the authors.

GIRAF Admin's source code is released under the [GPLv3](https://github.com/Zucka/girafAdmin/blob/master/LICENSE.md) open source license. This means that you are free to inspect the source code at any time or contribute to the project yourself.
