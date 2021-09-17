# Register-Prototype
 
Instructions made for Microsoft Visual Studio 2019

Must be run in x64
C++ Language must be at 17 or above

Download mysql server 8.0.26
	https://dev.mysql.com/downloads/mysql/
	Alternate Link:
		https://downloads.mysql.com/archives/community/ 

Download ZIP Archive of MySQL Connector/C++ 8.0.26 (x86, 64-bit)
	https://dev.mysql.com/downloads/connector/cpp/
	Alternate Link:
		https://downloads.mysql.com/archives/c-cpp/

Download Boost Library 1.76.0
	https://www.boost.org/users/history/version_1_76_0.html


Boost's JSON library needs to be built
	In the boost install directory open bootstrap.bat
	b2.exe should be created from bootstrap.bat
	lauch b2.exe using the flag "--with-json"

in Visual studio properties:
	under C/C++ 
		General
			Additional Include Directories add
				$MySQL_Server_Directory$\include
				$Boost_directory$
	under linker
		general
			Additional Library Directories 
				$MySQL_Server_Directory$\lib
				$Boost_Directory$\stage\lib
		Input		
			Additional Dependencies
				libmysql.lib

In exe directory add
	libmysql.dll			From $MySQL_Server_Directory$\lib
	libssl-1_1-x64.dll		From $MySQL_Connector_Directory$\lib64
	libcrypto-1_1-x64.dll	From $MySQL_Connector_Directory$\lib64
	
NOTE:
SQL Data located in SQL Data folder
	Included are instructions.txt, table creation files, and table fill files 
In order to run program without changing any code the following must be true
	MySQL Server schema must be named "store"
		if not then all instances of #define DATABASE "store" must be altered to match schema
	MySQL Server must be run from local machine
		If not then all instances of #define SERVER "localhost" must be altered to server IP
	MySQL Server must have the following users with the following passwords	and privileges
		user:login		pw:2752
			"SELECT"
		user:loggedin	pw:password
			"Alter"		"Alter Routine"	"Create"		"Create Routine"
			"Create Temporary Tables"	"Create View"	"Delete"	"Drop"
			"Event"	  "Grant Option"	"Index"		"Insert"	"Lock Tables"
			"Select"	"Show Databases"	"Show View"		"Trigger"	"Update"
		
		

	
