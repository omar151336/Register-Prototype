#pragma once
#include "SQLBase.h"
#include "Employee.h"

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


class SQLEmployee : public SQLBase
{
public:
	//use same constructor as SQLBase
	using SQLBase::SQLBase;

	//return 0 = good
	//return 1 = no connection
	//return 2 = connection failed
	//return 3 = number in use
	//return 4 = code in use
	int addEmployee(std::string name, int number, int code, int privilege);

	//return 0 = good
	//return 1 = no connection
	//return 2 = connection failed
	int removeEmployee(int number);

	//gets employee using their number or name depending on if an int or string is passed
	//if there's an error return employee object with Employee.isError() == true
	Employee getEmployee(int employeeNumber );
	Employee getEmployee(std::string name );

	//returns empty vector if error or no products
	std::vector<Employee> getAllEmployees();
	//returns vector of employees whose name contains the passed string
	//returns empty vector if error or no products
	std::vector<Employee> getEmployeesByName(std::string name);

	//return 0 = good
	//return 1 = no connection
	//return 2 = connection error
	int changePrivilege(int employeeNumber, int priv);

	//pass int to check money of that privilege level
	//return -1 in error
	double checkMoneyMade( );
	double checkMoneyMade(int privilege );

	//pass 1 = week
	//pass 2 = month
	//returns negative if error
	int checkTotalTransactions(int weekOrMonth);
	int checkRegisterOpens(int weekOrMonth);

	//checks info of employee whose number is passed
	//pass 1 = week
	//pass 2 = month
	//returns negative if error
	int checkIndividualTransactions(int weekOrMonth, int employeeNumber);
	int checkIndividualRegisterOpens(int weekOrMonth, int employeeNumber);

	//reset transactions/register opens of the week
	int resetWeek( );
	//reset transactions/register opens of the week
	int resetMonth( );

	//updates all info passed of the employee whose number is passed
	//return 0 = good
	//return 1 = no connection
	//return 2 = connection failed
	int updateEmployee(int employeeNumber, std::string name, int employeeCode, int transwtd, int transmtd, int registerwtd, int registermtd);
	//updates employee information whose number is passed to match that of employee object
	int updateEmployee(Employee emp, int number);

	//updates employee's transactions by 1 and adds money to total money made by employee
	//return 0 = good
	//return 1 = no connection
	//return 2 = connection failed
	int transactionMade( int employeeNumber, double money );
	//increases employee's register opened info by 1
	//return 0 = good
	//return 1 = no connection
	//return 2 = connection failed
	int registerOpened(int employeeNumber);

	//returns employee whose login code was passed
	//if there's an error return employee object with Employee.isError() == true
	Employee login(int code);


private:

};

