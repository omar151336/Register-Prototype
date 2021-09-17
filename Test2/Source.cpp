#include <iostream>
#include <fstream>
#include <filesystem>
#include <mysql.h>
#include "SQLProduct.h"
#include "SQLEmployee.h"
#include <windows.h>
#include "Handler.h"
//#include <conio.h>

//using namespace std;
// Defining Constant Variables
#define SERVER "localhost"  // As your code and database are on same computer
#define USER "root" // user name by which you want to access the database ( database user name- not server)
#define PASSWORD "password"
#define DATABASE "store"
#define PORT 3306  // the database name in the database server

void run_show(MYSQL* connect, std::string query)
// This
{
	MYSQL_RES* res_set; //Result set object to store output table from the query
	MYSQL_ROW row; // row variable to process each row from the result set.

	int j = mysql_query(connect, query.c_str( ));  // execute the query, returns Zero for success. Nonzero if an error occurred. details at https://dev.mysql.com/doc/refman/5.7/en/mysql-query.html
	if ( j != 0 )
		std::cerr << "not ok" << std::endl;
	res_set = mysql_store_result(connect); //reads the entire result of a query, allocates a MYSQL_RES structure, details at: https://dev.mysql.com/doc/refman/5.7/en/mysql-store-result.html
	int numrows = mysql_num_rows(res_set); // get number of rows in output table/ result set
	int num_col = mysql_num_fields(res_set); // get number of columns
	int i = 0;

	while ( ((row = mysql_fetch_row(res_set)) != NULL) ) // fetch each row one by one from the result set
	{
		std::cout << std::endl << "okkk" << std::endl;
		i = 0;
		while ( i < num_col )
		{ // print every row
//if(row[i])
			if ( row[i] == NULL )
				std::cout << "NULL ";
			else
				std::cout << row[i] << " ";  //cells are separated by space

			i++;
		}
		std::cout << std::endl; // print a new line after printing a row
	}
	std::cout << std::endl << "oddd" << std::endl;
}

void productTest( )
{
	SQLProduct p(SERVER, PORT, USER, PASSWORD);

	if ( p.connect( ) == 1 )
		std::cout << "YESSS" << std::endl;
	else
		std::cout << "NOOOO" << std::endl;
	//"4099100103069", "Grocery", "Peanut Delight Crunchy", "Aldi", 0, 1.59, 2.49

	if ( p.addProduct("84145218", 1, "fdsafdas", "Aldi", 1.59, 2.49) != 0 )
	{
		std::cout << "not added" << std::endl;
	}
	else std::cout << "yes" << std::endl;

	if ( p.addProduct("8764456", 1, "grethtryej", "Aldi", 1.59, 2.49) != 0 )
	{
		std::cout << "not added" << std::endl;
	}
	if ( p.addProduct("4099100103070", 1, "Peanut Delight munchy", "Aldi", 1.59, 2.49) != 0 )
	{
		std::cout << "not added" << std::endl;
	}
	if ( p.addProduct("4099100103071", 1, "Peanut Delight bunchy", "Aldi", 1.59, 2.49) != 0 )
	{
		std::cout << "not added" << std::endl;
	}
	if ( p.addProduct("409910010505", 1, "nutt Delight bunchy", "Aldi", 1.59, 2.49) != 0 )
	{
		std::cout << "not added" << std::endl;
	}
	//run_show(p.getConnection(), "select * from Products;");
	p.getProductBySN("4099100103069").print( );

	std::string s;
	std::string d;
	std::cout << "name  ";
	std::cin >> s;

	std::vector<Product> l = p.getProductsByName(s);
	auto i = l.begin( );
	while ( i != l.end( ) )
	{
		i->print( );
		++i;
	}
	p.changeSerialByName("nutt Delight bunchy", "8000069");

	l = p.getProductsByName(s);
	i = l.begin( );
	while ( i != l.end( ) )
	{
		i->print( );
		++i;
	}
}

bool plist( )
{
	SQLProduct p(SERVER, PORT, USER, PASSWORD);

	if ( p.connect( ) == 1 )
		std::cout << "YESSS" << std::endl;
	else
		std::cout << "NOOOO" << std::endl;

	std::vector<Product> list = p.getProductsByProvider("al");
	if ( list.empty( ) )
	{
		std::cout << "No Products Found" << std::endl;
		return false;
	}
	int snlen = 0;
	int namelen = 0;
	int catlen = 8;
	for ( auto i : list )
	{
		int temp;
		if ( snlen < (temp = i.getSerialNumber( ).length( )) )
			snlen = temp;
		if ( namelen < (temp = i.getName( ).length( )) )
			namelen = temp;
		if ( catlen < (temp = i.getCategory( ).length( )) )
			catlen = temp;
	}
	std::cout << "*************";
	for ( int i = 0; i < (snlen + namelen + catlen); i++ )
	{
		if ( i == ((snlen + namelen + catlen) / 2) )
			std::cout << "Product*List";
		std::cout << "*";
	}
	std::cout << "*************" << std::endl;
	std::cout << "| " << std::setw(snlen + 2) << "Serial Number" << " | " << std::setw(namelen + 2) << "Name" << " | ";
	std::cout << std::setw(catlen + 2) << "Category" << " | " << std::setw(8) << "Price" << " | " << std::setw(8) << "Total" << " |" << std::endl;
	for ( auto i : list )
	{
		double tot = i.getPrice() * i.getCategoryTax( );
		std::cout << "| " << std::setw(snlen + 2) << i.getSerialNumber( ) << " | " << std::setw(namelen + 2) << i.getName( ) << " | ";
		std::cout << std::setw(catlen + 2) << i.getCategory( ) << " | " << std::setw(8) << i.getPrice( ) << " | " << std::setw(8) << tot << " |" << std::endl;
	}

	std::cout << "*******************";
	for ( int i = 0; i < (snlen + namelen + catlen); i++ )
	{
		std::cout << "*";
	}
	std::cout << "*******************" << std::endl;

	return true;
}

bool listprint(bool verbose, std::vector<Employee> list)
{
	//system("cls");

	std::cout << std::left;
	std::cout << "*************************************";
	if ( verbose )
		std::cout << "********************************************************";
	std::cout << std::endl;

	std::cout << "| " << std::setw(10) << "Name" << " |  " << std::setw(6) << "Number" << "  | ";
	if ( verbose )
		std::cout << " " << std::setw(4) << "Code" << "  | ";
	std::cout << std::setw(9) << "Privilege" << " |   ";
	if ( verbose )
	{
		std::cout << std::setw(12) << "Transactions" << " | ";
		std::cout << std::setw(14) << "Register Opens" << " | " << std::setw(10) << "Money Made" << " |" << std::endl;
		std::cout << "|------------|----------|--------|-----------|----WTD-|---MTD-|----WTD-|---MTD-|------------|";
	}
	std::cout << std::endl;

	for ( auto e : list )
	{
		std::cout << std::left << "| " << std::setw(10) << e.getName( ) << " | " << std::right << std::setw(8) << e.getNumber( ) << " | ";
		if ( verbose )
			std::cout << std::setw(6) << e.getCode( ) << " | ";
		std::cout << std::setw(9) << e.getPrivilege( ) << " |";
		if ( verbose )
		{
			std::cout << std::setw(7) << e.getTransactionsWTD( ) << " |" << std::setw(6) << e.getTransactionsMTD( ) << " |";
			std::cout << std::setw(7) << e.getRegisterOpensWTD( ) << " |" << std::setw(6) << e.getRegisterOpensMTD( ) << " |";
			std::cout << std::setw(11) << e.getMoneyMade( ) << " |";
		}
		std::cout << std::endl;
	}

	std::cout << std::left;
	std::cout << "*************************************";
	if ( verbose )
		std::cout << "********************************************************";
	std::cout << std::endl;
	system("pause");
	return true;

	//sEmployee
	return true;
}

void printtest( )
{
	system("cls");
	SQLEmployee e(SERVER, PORT, USER, PASSWORD);
	if ( e.connect( ) == 1 )
		std::cout << "YESSS" << std::endl;
	else
		std::cout << "NOOOO" << std::endl;
	std::vector<Employee> list = e.getAllEmployees( );

	//listprint(true, list);
	//listprint(false, list);
}
void etest( )
{
	SQLEmployee e(SERVER, PORT, USER, PASSWORD);
	e.connect( );
	e.addEmployee("super", 69, 15133, 6);
	if ( e.connect( ) == 1 )
		std::cout << "YESSS" << std::endl;
	else
		std::cout << "NOOOO" << std::endl;

	e.addEmployee("Esam", 1, 0223, 0);
	e.addEmployee("Omar", 3, 1513, 2);
	e.addEmployee("Wanda", 4, 1234, 2);

	e.getEmployee(3).print( );

	e.getEmployee("Mo").print( );
	std::string n;
	//std::cout << "name: ";
	//std::cin >> n;
	std::vector<Employee> list = e.getEmployeesByName("m");
	listprint(true, list);
	auto i = list.begin( );
	while ( i != list.end( ) )
	{
		i->print( );
		++i;
	}
	std::cout << std::endl << std::endl;

	std::cout << e.checkMoneyMade( ) << std::endl;
	std::cout << e.checkMoneyMade(2) << std::endl;

	e.changePrivilege(1, 9);

	e.getEmployee(1).print( );
	e.changePrivilege(1, 0);
	e.getEmployee(1).print( );

	e.transactionMade(3, 1.20);
	e.getEmployee(3).print( );
	e.resetWeek( );
	e.getEmployee(4).print( );
	std::cout << "enter code: ";
	int code;
	std::cin >> code;
	Employee emp = e.login(code);
	if ( !emp.isError( ) )
		emp.print( );
}

int main(int argc, char* argv[])
{
	//Set Window size
	SMALL_RECT windowSize;
	windowSize.Top = 0;
	windowSize.Left = 0;
	windowSize.Bottom = 30;
	windowSize.Right = 95;
	COORD a;
	a.X = 130;
	a.Y = 300;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(hConsole, a);
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	//gets directory for error logs
	std::string errorDir;
	if ( argc > 1 )
		errorDir = argv[1];
	else
		errorDir = ".\\LOGS";

	std::filesystem::create_directories(errorDir);

	FILE* errLog;

	std::string fullDir = errorDir + "\\error.log";
	freopen_s(&errLog, fullDir.c_str( ), "w", stderr);

	Handler h;
	h.start( );
	//plist( );
	//printtest( );

	return 0;
}