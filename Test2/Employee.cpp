#include "Employee.h"
#include <filesystem>

Employee::Employee(std::string n, int en, int ec, int p, int tw, int tm, int rw, int rm, double mm)
{
	name = n;
	employeeNumber = en;
	employeeCode = ec;
	privilege = p;
	TransactionsWtD = tw;
	TransactionsMtD = tm;
	registerOpensWtD = rw;
	registerOpensMtD = rm;
	moneyMade = mm;
}
Employee::Employee(std::string n, int en, int ec, int p)
{
	name = n;
	employeeNumber = en;
	employeeCode = ec;
	privilege = p;
	TransactionsWtD = -1;
	TransactionsMtD = -1;
	registerOpensWtD = -1;
	registerOpensMtD = -1;
	moneyMade = -1;
}
Employee::Employee(bool err)
{
	error = err;
}
Employee::Employee()
{
	error = true;
}

void Employee::print( )
{
	//prints headers
	std::cout << std::left << std::setw(10) << "Name" << "  " << std::setw(6) << "Number" << "  " << std::setw(4) << "Code" << "  " << std::setw(9) << "Privilege" << "  " << std::setw(5) << "T WTD" << "  " << std::setw(5) << "T MTD" << "  ";
	std::cout << std::setw(6) << "RO WTD" << "  " << std::setw(6) << "RO MTD" << "  " << std::setw(9) << "MoneyMade" << std::endl;
	//prints employee info
	std::cout << std::setw(10) << name << std::right << "  " << std::setw(6) << employeeNumber << "  " << std::setw(4) << employeeCode << "  " << std::setw(9) << privilege;
	std::cout << "  " << std::setw(5) << TransactionsWtD << "  " << std::setw(5) << TransactionsMtD << "  " << std::setw(6) << registerOpensWtD << "  " << std::setw(6) << registerOpensMtD << "  " << std::setw(9) << moneyMade << std::endl;
}

bool Employee::isError( )
{
	return error;
}

std::string Employee::getName( )
{
	return name;
}

int Employee::getNumber( )
{
	return employeeNumber;
}

int Employee::getCode( )
{
	return employeeCode;
}

int Employee::getPrivilege( )
{
	return privilege;
}

int Employee::getTransactionsWTD( )
{
	return TransactionsWtD;
}

int Employee::getTransactionsMTD( )
{
	return TransactionsMtD;
}

int Employee::getRegisterOpensWTD( )
{
	return registerOpensWtD;
}

int Employee::getRegisterOpensMTD( )
{
	return registerOpensMtD;
}

double Employee::getMoneyMade( )
{
	return moneyMade;
}


bool Employee::addTransaction( )
{
	TransactionsWtD++;
	TransactionsMtD++;
	return true;
}

//adds i transactions to count
bool Employee::addTransaction(int i)
{
	TransactionsWtD += i;
	TransactionsMtD += i;
	return true;
}

bool Employee::addRegisterOpens( )
{
	registerOpensWtD++;
	registerOpensMtD++;
	return true;
}

//adds i register opens to count
bool Employee::addRegisterOpens(int i )
{
	registerOpensWtD+=i;
	registerOpensMtD+=i;
	return true;
}

bool Employee::addMoney(double m)
{
	moneyMade += m;
	return true;
}

bool Employee::changeName(std::string n)
{
	name = n;
	return true;
}

bool Employee::changeNumber(int n)
{
	employeeNumber = n;
	return true;
}

bool Employee::changeCode(int c)
{
	employeeCode = c;
	return true;
}

bool Employee::changePrivilege(int p)
{
	privilege = p;
	return true;
}
