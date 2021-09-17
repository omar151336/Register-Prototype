#include "SQLBase.h"

#define DATABASE "store" //CHANGE DATABASE NAME TO NAME USED BY MYSQL

SQLBase::SQLBase(std::string h, int p, std::string u, std::string pa)
	: host(h), port(p), username(u), password(pa)
{
}
SQLBase::SQLBase( )
{
	error = true;
}

bool SQLBase::changeConnection(std::string h, int p, std::string u, std::string pa)
{
	//assigns new parameters
	host = h;
	username = u;
	password = pa;
	port = p;

	//connects using new parameters
	if ( connect( ) == 1 )
		return true;
	else
		return false;

}

//return 0 = fail
//return 1 = success
//return 2 = SQL initialization failed
int SQLBase::connect()
{

	connection = mysql_init(NULL); // Initialize the object

	if (!connection)  // Check if the connect object was properly initialized
	{
		std::cerr << "mysql Initialization Failed";
		return -1;
	}

	//connects to server asynchronously
	net_async_status status = NET_ASYNC_NOT_READY;
	while (status == NET_ASYNC_NOT_READY)
	{
		//function can be inserted here to do between calls, not needed but is an option


		//attempts connection
		status = mysql_real_connect_nonblocking(connection, SQLBase::host.c_str(), SQLBase::username.c_str(), SQLBase::password.c_str(), DATABASE, port, NULL, 0);
		if (status == NET_ASYNC_COMPLETE)
		{
			return 1;
		}
		else if (status == NET_ASYNC_ERROR)
		{
			std::cerr << "connection error" << std::endl;
			return 0;
		}
	}
	return 2;
}

//return 0 = fail
//return 1 = success
//return 2 = SQL initialization failed
int SQLBase::connectS()
{

	connection = mysql_init(NULL); // Initialize the object

	if (!connection)  // Check if the connect object was properly initialized
	{
		std::cerr << "mysql Initialization Failed";
		return 2;
	}

	//connects to server, checks if connection worked
	connection = mysql_real_connect(connection, SQLBase::host.c_str(), SQLBase::username.c_str(), SQLBase::password.c_str(), DATABASE, port, NULL, 0);
	if (connection)
	{
		return 1;
	}
	else return 0;
}

MYSQL* SQLBase::getConnection()
{
	return connection;
}
