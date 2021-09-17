#pragma once
#include <string>
#include <vector>
#include <mysql.h>
#include <iostream>

class SQLBase
{
public:
	//sets host user port and pass
	SQLBase(std::string h, int p, std::string u, std::string pa);
	SQLBase( ); //sets error flag to true

	//return 0 = fail
	//return 1 = success
	//return 2 = SQL initialization failed
	int connect();
	int connectS();
	//changes connection parameters
	bool changeConnection(std::string h, int p, std::string u, std::string pa);

	//gets connection function
	MYSQL* getConnection();
protected:
	std::string host;
	std::string username;
	std::string password;
	int port;

	MYSQL* connection;

	bool error = false; //error flag false by default
};

