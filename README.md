# Register-Prototype
 
### Instructions made for Microsoft Visual Studio 2019

* Must be run in x64
* C++ Language must be at 17 or above

### Downloads
* Download mysql server 8.0.26
  * https://dev.mysql.com/downloads/mysql/
  * Alternate Link:
    * https://downloads.mysql.com/archives/community/ 

* Download ZIP Archive of MySQL Connector/C++ 8.0.26 (x86, 64-bit)
  * https://dev.mysql.com/downloads/connector/cpp/
  * Alternate Link:
    * https://downloads.mysql.com/archives/c-cpp/

* Download Boost Library 1.76.0
  * https://www.boost.org/users/history/version_1_76_0.html

### Library Setup
* Boost's JSON library needs to be built
  1. In the boost install directory open bootstrap.bat
     * b2.exe should be created from bootstrap.bat
  2. lauch b2.exe using the flag "--with-json"

### Visual studio properties:
* Under C/C++ 
  * General
    * Additional Include Directories add
      1. $MySQL_Server_Directory$\include
      2. $Boost_directory$
* Under Linker
  * General
    * Additional Library Directories 
      1. $MySQL_Server_Directory$\lib
      2. $Boost_Directory$\stage\lib
  * Input		
    * Additional Dependencies
      1. libmysql.lib

### In exe directory
* Add
  * From $MySQL_Server_Directory$\lib
    1. libmysql.dll      
  * From $MySQL_Connector_Directory$\lib64
    1. libssl-1_1-x64.dll		
    2. libcrypto-1_1-x64.dll
	
### NOTE:
* SQL Data located in SQL Data folder
  * Included are instructions.txt, table creation files, and table fill files 
* In order to run program without changing any code the following must be true
  * MySQL Server schema must be named "store"
    * if not then all instances of `#define DATABASE "store"` must be altered to match schema
  * MySQL Server must be run from local machine
		If not then all instances of `#define SERVER "localhost"` must be altered to server IP
  * MySQL Server must have the following users with the following passwords and privileges
    * User: login`    `PW: 2752
      * "SELECT"
    * User: loggedin`        `PW: password
      * "Alter", "Alter Routine", "Create", "Create Routine"
      * "Create Temporary Tables", "Create View", "Delete", "Drop"
      * "Event", "Grant Option", "Index", "Insert", "Lock Tables"
      * "Select", "Show Databases", "Show View", "Trigger", "Update"
		
		

	
