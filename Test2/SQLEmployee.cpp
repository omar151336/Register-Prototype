#include "SQLEmployee.h"

/*
* SQL Employees Table Columns
*  10 Columns:
*	  Key
*	  Name
*	  Number
*	  Code
*	  Privilege
*	  TransactionsWTD
*	  TransactionsMTD
*	  RegisterOpensWTD
*	  RegisterOpensMTD
*	  MoneyMade
*/


//return 0 = good
//return 1 = no connection
//return 2 = connection failed
//return 3 = number in use
//return 4 = code in use
int SQLEmployee::addEmployee(std::string name, int number, int code, int privilege)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}


	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//checks if Number is already in database, exits if true
	std::string query = "SELECT * FROM Employees WHERE Number = " + std::to_string(number) + ";";
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{

		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'EMPLOYEE NUMBER'" << std::endl;
			return 2;
		}
	}
	//store result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'EMPLOYEE NUMBER'" << std::endl;
			return 2;
		}
	}

	//check result
	if ( result != NULL)
	{
		if ( result->row_count != 0 )
		{
			std::cerr << "Employee Number already in system" << std::endl;
			return 3;
		}
	}

	//checks if passcode is already in database, exits if true
	query = "SELECT * FROM Employees WHERE Code = " + std::to_string(code) + ";";
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{

		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'EMPLOYEE CODE'" << std::endl;
			return 2;
		}
	}
	//store result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'EMPLOYEE CODE'" << std::endl;
			return 2;
		}
	}

	//check result
	if ( result != NULL )
	{
		if ( result->row_count != 0 )
		{
			std::cerr << "Employee Code already in system" << std::endl;
			return 4;
		}
	}

	//add employee query
	query = "INSERT INTO Employees (Name, Number, Code, Privilege, TransactionsWTD, TransactionsMTD, RegisterOpensWTD, RegisterOpensMTD, MoneyMade) VALUES ('" + name + "'," + std::to_string(number) + "," + std::to_string(code) + "," + std::to_string(privilege) + ",0,0,0,0,0.0);";

	//execute query
	status = NET_ASYNC_NOT_READY;
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{

	}
	//checks if execute error
	if ( status == NET_ASYNC_ERROR )
	{
		std::cerr << "Error: execute query 'INSERT INTO Employees'" << std::endl;
		return 2;
	}


	//frees memory
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return 0;
}

//return 0 = good
//return 1 = no connection
//return 2 = connection failed
int SQLEmployee::removeEmployee(int number)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//delete employee statement
	std::string query = "DELETE FROM Employees WHERE Number = " + std::to_string(number) + "; ";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute" << std::endl;
			return 2;
		}
	}
	//stores result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result" << std::endl;;
			return 2;
		}
	}

	if ( status == NET_ASYNC_COMPLETE )
	{
		//frees result
		while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
		{

		}
		return 0;
	}
	else
	{
		std::cerr << "not updated" << std::endl;
		return 2;
	}
}

//gets employee using their number or name depending on if an int or string is passed
//if there's an error return employee object with Employee.isError() == true
Employee SQLEmployee::getEmployee(int employeeNumber)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return Employee(true);
	}

	//get employee whose number matches
	std::string query = "SELECT * FROM Employees WHERE Number = " + std::to_string(employeeNumber) + ";";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			return Employee(true);
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			return Employee(true);
		}

	}

	//checks if more than one or no results exists for employee number
	int numrows = mysql_num_rows(result);
	if ( numrows > 1 || numrows == 0 )
	{
		if ( numrows > 1 )
			std::cerr << "Too many results for Employee Number" << std::endl;
		if ( numrows == 0 )
			std::cerr << "No Matching Employee Number" << std::endl;
		return Employee(true);
	}
	int num_col = mysql_num_fields(result);

	//gets row data
	while ( (status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE_NO_MORE_RESULTS )
		{
			break;
		}
	}

	//stores employee data
	u_int key = std::atoi(row[0]);
	std::string na = row[1];
	int eNumber = std::atoi(row[2]);
	int eCode = std::atoi(row[3]);
	int priv = std::atoi(row[4]);
	int tw = std::atoi(row[5]);
	int tMo = std::atoi(row[6]);
	int rw = std::atoi(row[7]);
	int rm = std::atoi(row[8]);
	double mm = std::stod(row[9]);

	//creates employee using stored data
	Employee e(na, eNumber, eCode, priv, tw, tMo, rw, rm, mm);

	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return e;
}
Employee SQLEmployee::getEmployee(std::string name)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return Employee(true);
	}

	//get employee whose name matches
	std::string query = "SELECT * FROM Employees WHERE Name = '" + name + "';";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			return Employee(true);
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			return Employee(true);
		}

	}

	//checks if more than one or no results exists for employee number
	int numrows = mysql_num_rows(result);
	if ( numrows > 1 || numrows == 0 )
	{
		if ( numrows > 1 )
			std::cerr << "Too many results for Employee Number" << std::endl;
		if ( numrows == 0 )
			std::cerr << "No Matching Employee Number" << std::endl;
		return Employee(true);
	}
	int num_col = mysql_num_fields(result);

	//gets row data
	while ( (status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE_NO_MORE_RESULTS )
		{
			break;
		}
	}

	//stores employee data
	u_int key = std::atoi(row[0]);
	std::string na = row[1];
	int eNumber = std::atoi(row[2]);
	int eCode = std::atoi(row[3]);
	int priv = std::atoi(row[4]);
	int tw = std::atoi(row[5]);
	int tMo = std::atoi(row[6]);
	int rw = std::atoi(row[7]);
	int rm = std::atoi(row[8]);
	double mm = std::stod(row[9]);

	//creates employee using stored data
	Employee e(na, eNumber, eCode, priv, tw, tMo, rw, rm, mm);

	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return e;
}


//returns empty vector if error or no products
std::vector<Employee> SQLEmployee::getAllEmployees()
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection Established" << std::endl;
		std::vector<Employee> e;
		return e;
	}

	//gets every row in employee table
	std::string query = "SELECT * FROM Employees;";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			std::vector<Employee> e;
			return e;
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			std::vector<Employee> e;
			return e;
		}

	}
	int numrows = mysql_num_rows(result);
	if ( numrows == 0 )
	{
		std::cerr << "no results found" << std::endl;
		std::vector<Employee> e;
		return e;
	}
	int num_col = mysql_num_fields(result);


	//list used to store employees
	std::vector<Employee> list;

	// fetch each row one by one from the result set
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				//stores employee data from row
				u_int key = std::atoi(row[0]);
				std::string na = row[1];
				int eNumber = std::atoi(row[2]);
				int eCode = std::atoi(row[3]);
				int priv = std::atoi(row[4]);
				int tw = std::atoi(row[5]);
				int tMo = std::atoi(row[6]);
				int rw = std::atoi(row[7]);
				int rm = std::atoi(row[8]);
				double mm = std::atof(row[9]);

				//creates employee object and adds it to list
				Employee e(na, eNumber, eCode, priv, tw, tMo, rw, rm, mm);
				list.push_back(e);
			}
		}
		else if ( status == NET_ASYNC_ERROR )
		{
			//if error return empty vector
			std::cerr << "Error: Fetch row error" << std::endl;
			std::vector<Employee> e;
			return e;

		}

	}

	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return list;
}
//returns vector of employees whose name contains the passed string
//returns empty vector if error or no products
std::vector<Employee> SQLEmployee::getEmployeesByName(std::string name)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection Established" << std::endl;
		std::vector<Employee> e;
		return e;
	}

	//gets all employees whose Name contains the string name
	std::string query = "SELECT * FROM Employees WHERE Name LIKE '%" + name + "%';";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			std::vector<Employee> e;
			return e;
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			std::vector<Employee> e;
			return e;
		}

	}
	int numrows = mysql_num_rows(result);
	if ( numrows == 0 )
	{
		std::cerr << "no results found" << std::endl;
		std::vector<Employee> e;
		return e;
	}
	int num_col = mysql_num_fields(result);


	//list used to store employees
	std::vector<Employee> list;

	// fetch each row one by one from the result set
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				//stores employee data from row
				u_int key = std::atoi(row[0]);
				std::string na = row[1];
				int eNumber = std::atoi(row[2]);
				int eCode = std::atoi(row[3]);
				int priv = std::atoi(row[4]);
				int tw = std::atoi(row[5]);
				int tMo = std::atoi(row[6]);
				int rw = std::atoi(row[7]);
				int rm = std::atoi(row[8]);
				double mm = std::atof(row[9]);


				//creates employee object and adds it to list
				Employee e(na, eNumber, eCode, priv, tw, tMo, rw, rm, mm);
				list.push_back(e);
			}
		}
		else if ( status == NET_ASYNC_ERROR )
		{
			//if error return empty vector
			std::cerr << "Error: Fetch row error" << std::endl;
			std::vector<Employee> e;
			return e;

		}

	}

	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return list;
}

//return 0 = good
//return 1 = no connection
//return 2 = connection error
int SQLEmployee::changePrivilege(int employeeNumber, int priv)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//sets employee with number to new privilege
	std::string query = "UPDATE Employees SET Privilege = " + std::to_string(priv) + " WHERE Number = '" + std::to_string(employeeNumber) + "';";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute" << std::endl;
			return 2;
		}
	}

	//store results
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		;
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result" << std::endl;;
			return 2;
		}
	}

	if ( status == NET_ASYNC_COMPLETE )
	{
		//frees result
		while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
		{

		}
		return 0;
	}
	else
	{
		std::cerr << "not updated" << std::endl;
		return 2;
	}
}

//Pass int to check money of that privilege level
//return -1 in error
double SQLEmployee::checkMoneyMade( )
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return -1;
	}

	//gets every row from column money made
	std::string query = "SELECT MoneyMade FROM Employees;";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			return -1;
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			return -1;
		}

	}
	int numrows = mysql_num_rows(result);
	if ( numrows == 0 )
	{
		std::cerr << "no results found" << std::endl;
		return -1;
	}
	int num_col = mysql_num_fields(result);


	//adds up all results to total
	double total = 0.0;
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				double cur = atof(row[0]);
				total += cur;
			}
		}
		else if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "error: fetch row error" << std::endl;
			return -1;
		}
	}
	return total;


}
double SQLEmployee::checkMoneyMade(int privilege)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return -1;
	}

	//gets row from every moneymade column that matches privilege
	std::string query = "SELECT MoneyMade FROM Employees WHERE privilege =" + std::to_string(privilege) + ";";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			return -1;
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			return -1;
		}

	}
	int numrows = mysql_num_rows(result);
	if ( numrows == 0 )
	{
		std::cerr << "no results found" << std::endl;
		return -1;
	}
	int num_col = mysql_num_fields(result);

	//adds up all results to total
	double total = 0.0;
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				double cur = atof(row[0]);
				total += cur;
			}
		}
		else if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "error: fetch row error" << std::endl;
			return -1;
		}
	}
	return total;
}

//pass 1 = week
//pass 2 = month
//returns negative if error
int SQLEmployee::checkTotalTransactions(int weekOrMonth)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return -1;
	}

	//selects if searching for transactions of week or month
	std::string wm;
	switch ( weekOrMonth )
	{
		case 1:
			wm = "WTD";
			break;
		case 2:
			wm = "MTD";
			break;
	}

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;


	std::string query = "SELECT Transactions" + wm + " FROM Employees; ";
	//execute query
	while ( status == NET_ASYNC_NOT_READY )
	{
		status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ));
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'EMPLOYEE NUMBER'" << std::endl;
			return -2;
		}
	}

	//store result
	status = mysql_store_result_nonblocking(connection, &result);
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'EMPLOYEE NUMBER'" << std::endl;
			return -2;
		}
	}

	//adds up all transactions to total
	int total = 0;
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				int cur = atoi(row[0]);
				total += cur;
			}
		}
		else if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "error: fetch row error" << std::endl;
			return -2;
		}
	}
	return total;

}
//pass 1 = week
//pass 2 = month
//returns negative if error
int SQLEmployee::checkRegisterOpens(int weekOrMonth)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return -1;
	}

	//selects if searching for register opens of week or month
	std::string wm;
	switch ( weekOrMonth )
	{
		case 1:
			wm = "WTD";
			break;
		case 2:
			wm = "MTD";
			break;
	}


	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	std::string query = "SELECT RegisterOpens" + wm + " FROM Employees;";
	//execute query
	while ( status == NET_ASYNC_NOT_READY )
	{
		status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ));
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'EMPLOYEE NUMBER'" << std::endl;
			return -2;
		}
	}

	//store result
	status = mysql_store_result_nonblocking(connection, &result);
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'EMPLOYEE NUMBER'" << std::endl;
			return 2;
		}
	}

	//adds up all register opens to total
	int total = 0;
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				int cur = atoi(row[0]);
				total += cur;
			}
		}
		else if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "error: fetch row error" << std::endl;
			return -2;
		}
	}
	return total;

}

//checks info of employee whose number is passed
//pass 1 = week
//pass 2 = month
//returns negative if error
int SQLEmployee::checkIndividualTransactions(int weekOrMonth, int employeeNumber)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//selects if searching for transactions of week or month
	std::string wm;
	switch ( weekOrMonth )
	{
		case 1:
			wm = "WTD";
			break;
		case 2:
			wm = "MTD";
			break;
	}


	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//searches for transactions from specific employee
	std::string query = "SELECT Transactions" + wm + " FROM Employees WHERE Number = " + std::to_string(employeeNumber) + ";";
	//execute query
	while ( status == NET_ASYNC_NOT_READY )
	{
		status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ));
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'EMPLOYEE NUMBER'" << std::endl;
			return 2;
		}
	}

	//store result
	status = mysql_store_result_nonblocking(connection, &result);
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'EMPLOYEE NUMBER'" << std::endl;
			return 2;
		}
	}


	//adds up transactions to total
	int total = 0;
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				int cur = atoi(row[0]);
				total += cur;
			}
		}
		else if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "error: fetch row error" << std::endl;
			return -1;
		}
	}
	return total;

}
//checks info of employee whose number is passed
//pass 1 = week
//pass 2 = month
//returns negative if error
int SQLEmployee::checkIndividualRegisterOpens(int weekOrMonth, int employeeNumber)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return -1;
	}

	//selects if searching for register opens of week or month
	std::string wm;
	switch ( weekOrMonth )
	{
		case 1:
			wm = "WTD";
			break;
		case 2:
			wm = "MTD";
			break;
	}


	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//searches for register opens from specific employee
	std::string query = "SELECT RegisterOpens" + wm + " FROM Employees WHERE Number = " + std::to_string(employeeNumber) + ";";
	//execute query
	while ( status == NET_ASYNC_NOT_READY )
	{
		status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ));
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'EMPLOYEE NUMBER'" << std::endl;
			return -2;
		}
	}

	//store result
	status = mysql_store_result_nonblocking(connection, &result);
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'EMPLOYEE NUMBER'" << std::endl;
			return -2;
		}
	}


	//adds up register opens to total
	int total = 0;
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				int cur = atoi(row[0]);
				total += cur;
			}
		}
		else if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "error: fetch row error" << std::endl;
			return -2;
		}
	}
	return total;

}

//reset transactions/register opens of the week
int SQLEmployee::resetWeek( )
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return -1;
	}

	//sets all weekly transactions and register opens to 0
	std::string query = "UPDATE Employees SET RegisterOpensWTD = 0, TransactionsWTD = 0;";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			return -1;
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			return -1;
		}

	}

	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}

	return 1;
}
//reset transactions/register opens of the Month
int SQLEmployee::resetMonth( )
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return -1;
	}

	//sets all monthly transactions, register opens, and money made to 0
	std::string query = "UPDATE Employees SET RegisterOpensMTD = 0, TransactionsMTD = 0, MoneyMade = 0.0;";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			return -1;
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			return -1;
		}

	}

	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}

	return 1;
}

//updates all info passed of the employee whose number is passed
int SQLEmployee::updateEmployee(int employeeNumber, std::string name, int employeeCode, int transwtd, int transmtd, int registerwtd, int registermtd)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//updates employee with matching number
	std::string query = "UPDATE Employees SET Name = '" + name + "', Code = " + std::to_string(employeeCode) + ", TransactionsWTD = " + std::to_string(transwtd) + ", TransactionsMTD = " + std::to_string(transmtd) + ", RegisterOpensWTD = " + std::to_string(registerwtd) + ", RegisterOpensMTD = " + std::to_string(registermtd) + "WHERE Number = " + std::to_string(employeeNumber) + "; ";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute" << std::endl;
			return 2;
		}
	}
	//stores result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{

		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result" << std::endl;;
			return 2;
		}
	}

	if ( status == NET_ASYNC_COMPLETE )
	{
		//free result
		while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
		{

		}
		return 0;
	}
	else
	{
		std::cerr << "not updated" << std::endl;
		return 2;
	}
}
//updates employee information whose number is passed to match that of employee object
int SQLEmployee::updateEmployee(Employee emp, int number)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//updates employee with matching number to employee object
	std::string query = "UPDATE Employees SET Name = '" + emp.getName() + "', Code = " + std::to_string(emp.getCode()) + ", Number = " + std::to_string(emp.getNumber()) + ", Privilege = " + std::to_string(emp.getPrivilege()) + ", TransactionsWTD = " + std::to_string(emp.getTransactionsWTD()) + ", TransactionsMTD = " + std::to_string(emp.getTransactionsMTD()) + ", RegisterOpensWTD = " + std::to_string(emp.getRegisterOpensWTD()) + ", RegisterOpensMTD = " + std::to_string(emp.getRegisterOpensMTD()) + ", MoneyMade =" + std::to_string(emp.getMoneyMade()) + " WHERE Number = " + std::to_string(number) + "; ";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute" << std::endl;
			return 2;
		}
	}
	//stores result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{

		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result" << std::endl;;
			return 2;
		}
	}

	if ( status == NET_ASYNC_COMPLETE )
	{
		//free result
		while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
		{

		}
		return 0;
	}
	else
	{
		std::cerr << "not updated" << std::endl;
		return 2;
	}
}

//updates employee's transactions by 1 and adds money to total money made by employee
int SQLEmployee::transactionMade(int employeeNumber, double money)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//adds money made to employee and increases transaction trackers by 1
	std::string query = "UPDATE Employees SET MoneyMade = MoneyMade + " + std::to_string(money) + ", TransactionsWTD = TransactionsWTD + 1, TransactionsMTD = TransactionsMTD + 1 WHERE Number = '" + std::to_string(employeeNumber) + "';";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute" << std::endl;
			return 2;
		}
	}

	//store result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		;
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result" << std::endl;;
			return 2;
		}
	}

	if ( status == NET_ASYNC_COMPLETE )
	{
		//frees result
		while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
		{

		}
		return 0;
	}
	else
	{
		std::cerr << "not updated" << std::endl;
		return 2;
	}
}
//increases employee's register opened info by 1
int SQLEmployee::registerOpened(int employeeNumber)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//increases register data by 1 for employee whose number is passed
	std::string query = "UPDATE Employees SET RegisterOpensWTD = RegisterOpensWTD + 1, RegisterOpensMTD = RegisterOpensMTD + 1 WHERE Number = '" + std::to_string(employeeNumber) + "';";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute" << std::endl;
			return 2;
		}
	}

	//stores result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		;
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result" << std::endl;;
			return 2;
		}
	}

	if ( status == NET_ASYNC_COMPLETE )
	{
		//frees result
		while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
		{

		}
		return 0;
	}
	else
	{
		std::cerr << "not updated" << std::endl;
		return 2;
	}
}

//returns employee whose login code was passed
//if there's an error return employee object with Employee.isError() == true
Employee SQLEmployee::login(int code)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return Employee(true);
	}

	//gets employee data that matches code
	std::string query = "SELECT * FROM Employees WHERE Code = " + std::to_string(code) + ";";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			return Employee(true);
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			return Employee(true);
		}

	}

	//checks if multiple employees were found or if no employees were found, returns error if true
	int numrows = mysql_num_rows(result);
	if ( numrows > 1 || numrows == 0 )
	{
		if ( numrows > 1 )
			std::cerr << "Too many results for Employee Code" << std::endl;
		if ( numrows == 0 )
			std::cerr << "No Matching Employee Code" << std::endl;
		return Employee(true);
	}
	int num_col = mysql_num_fields(result);


	//store row data
	while ( (status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_COMPLETE_NO_MORE_RESULTS )
		{
			break;
		}
	}

	//stores employee data
	u_int key = std::atoi(row[0]);
	std::string na = row[1];
	int eNumber = std::atoi(row[2]);
	int eCode = std::atoi(row[3]);
	int priv = std::atoi(row[4]);
	int tw = std::atoi(row[5]);
	int tMo = std::atoi(row[6]);
	int rw = std::atoi(row[7]);
	int rm = std::atoi(row[8]);
	double mm = std::stod(row[9]);

	//creates employee
	Employee e(na, eNumber, eCode, priv, tw, tMo, rw, rm, mm);

	//frees result data
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return e;
}