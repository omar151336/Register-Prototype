#pragma once
#include <string>
#include <iostream>
#include <windows.h>


class Employee
{
public:
	//Constructors
	Employee(std::string n, int en, int ec, int p, int tw, int tm, int rw, int rm, double mm);
	Employee(std::string n, int en, int ec, int p);
	//used when Employee gives an error, sets error flag to true
	Employee(bool err);
	Employee();

	//print's employee's information
	void print( );

	//getters
	std::string getName( );
	int getNumber( );
	int getCode( );
	int getPrivilege( );
	int getTransactionsWTD( );
	int getTransactionsMTD( );
	int getRegisterOpensWTD( );
	int getRegisterOpensMTD( );
	double getMoneyMade( );
	bool isError( );

	//setters
	bool addTransaction( );
	bool addRegisterOpens( );
	bool addTransaction(int i );
	bool addRegisterOpens(int i );
	bool addMoney(double m);
	bool changeName(std::string n );
	bool changeNumber(int n);
	bool changeCode(int c);
	bool changePrivilege(int p);




private:
	std::string name;
	int employeeNumber, employeeCode;
	int TransactionsWtD, TransactionsMtD;
	int registerOpensWtD, registerOpensMtD;
	double moneyMade;
	int privilege;
	/*Employee Privilege key
	* 0 = cashier
	* 1 = Shift Manager
	* 2 = Manager
	* 3 = Admin
	* 4 = Owner
	*/

	bool error = false; //Error flag set false by default
};

