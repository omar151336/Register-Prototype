#include "Handler.h"


#define SERVER "localhost"  // As your code and database are on same computer
#define USER "login" // user name by which you want to access the database ( database user name- not server)
#define PASSWORD "2752"
#define PORT 3306  // the database name in the database server

//handler constructor
Handler::Handler( )
{
	emp = Employee(true);
}

//starts handler
int Handler::start( )
{
	//logs in and checks if login results in error
	if ( login( ).isError( ) )
	{
		std::cerr << "Log in Error" << std::endl;
		return 0;
	}
	else
	{
		//clears screen and starts main menu
		system("cls");
		while ( mainMenu( ) == 0 && emp.isError()) //if logged out return to login screen
		{
			//login again
			if ( login( ).isError( ) )
			{
				std::cerr << "Log in Error" << std::endl;
				return 0;
			}
			system("cls");
		}
		return 1;
	}
}

//logs in using login SQL user then switches to higher privilege connection based on employee privilege
Employee Handler::login()
{
	//creates connection and attempts to connect to table
	sEmployee = SQLEmployee(SERVER, PORT, USER, PASSWORD);
	if ( sEmployee.connect( ) == 0 )
	{
		std::cerr << "Error connecting to Login" << std::endl;
		std::cout << "Connection Error. \nExiting" << std::endl;
		exit(0);
	}
	//loops while user does not input 'exit' or a valid employee code
	std::string code = "";
	while ( code != "exit")
	{
		if ( code != "" )
			system("pause");
		system("cls");

		//gets user's input
		std::cout << "Enter Login: ";
		std::getline(std::cin, code);

		std::transform(code.begin( ), code.end( ), code.begin( ), tolower);
		if ( code == "exit" )
		{
			std::cout << "Exiting program" << std::endl;
			exit(0);
		}
		//checks that input is valid
		else if ( code.find_first_not_of("0123456789") != std::string::npos )
		{
			std::cout << "Invalid Input, Try Again" << std::endl;
		}
		else
		{
			//checks that employee exists and logs if if they do
			emp = sEmployee.login(stoi(code));
			if ( emp.isError( ) )
				std::cout << "No Such User, Try Again" << std::endl;
			else
			{
				//gets SQL login with more privileges and connects to product and employee table
				user = "loggedin";
				pw = "password";
				sEmployee = SQLEmployee(SERVER, PORT, user, pw);
				int i = 0;
				//tries multiple attempts to connect to employee table
				while ( i != 10 && sEmployee.connect( ) != 1 )
				{
					i++;
				}
				if ( i == 10 )
				{
					std::cerr << "SQLEmployee Connection issue" << std::endl;
					std::cout << "Error connecting to server. \nExiting" << std::endl;
					exit(0);
				}

				sProduct = SQLProduct(SERVER, PORT, user, pw);
				i = 0;
				//tries multiple attempts to connect to Product table
				while ( i != 10 && sProduct.connect( ) != 1 )
				{
					i++;
				}
				if ( i == 10 )
				{
					std::cerr << "SQLProduct Connection issue" << std::endl;
					std::cout << "Error connecting to server. \nExiting" << std::endl;
					exit(0);
				}
				//returns employee
				return emp;
			}
		}
	}
	return Employee(true);
}
//logs user out and empties relevant variables
bool Handler::logout( )
{
	user = "";
	pw = "";
	emp = Employee(true);
	sEmployee = SQLEmployee(SERVER, PORT, USER, PASSWORD);
	sProduct = SQLProduct(SERVER, PORT, USER, PASSWORD);
	return true;

}
//main menu loop
//return 0 on logout
//returns 1 on exit
int Handler::mainMenu( )
{
	std::string input;
	while ( input != "exit" && !emp.isError() )	//loops until user enters exit or current employee is not valid
	{
		std::cout << "************************************" << std::endl;//
		std::cout << "|  Enter number for option wanted  |" << std::endl;//
		std::cout << "|   1. Start Transaction           |" << std::endl;//
		std::cout << "|   2. Open Register               |" << std::endl;//
		//prints out certain options depending on user's privilege level
		if ( emp.getPrivilege( ) >= 1 )
		{
			std::cout << "|   3. View Products by Provider   |" << std::endl;//
			std::cout << "|   4. Add Product                 |" << std::endl;//
			if ( emp.getPrivilege( ) >= 2 )
			{
				std::cout << "|   5. Add Employee                |" << std::endl;//
				std::cout << "|   6. View Employee List          |" << std::endl;//
				std::cout << "|   7. Search For Employee         |" << std::endl;//
				std::cout << "|   8. Update Employee Info        |" << std::endl;//
				std::cout << "|   9. Update Employee Privilege   |" << std::endl;//
				if ( emp.getPrivilege( ) >= 3 )
				{
					std::cout << "|  10. Reset Weekly Info           |" << std::endl;//
					std::cout << "|  11. Reset Monthly Info          |" << std::endl;//
					std::cout << "|  12. Delete Employee             |" << std::endl;//
				}
			}
		}
		std::cout << "|   0. Log Out                     |" << std::endl;//
		std::cout << "************************************" << std::endl;

		//asks users to enter option
		std::cout << std::endl << "Enter Option: ";
		std::getline(std::cin, input);
		std::transform(input.begin( ), input.end( ), input.begin( ), tolower);
		std::cout << std::endl << std::endl;

		//checks that input is valid
		if ( input.find_first_not_of("0123456789") == std::string::npos )
		{
			//checks if user is entering a command above their privilege level
			bool privilegeOK = true;
			if ( stoi(input) >= 10 && emp.getPrivilege( ) < 3 )
			{
				std::cout << "Must be Admin to use command" << std::endl;
				privilegeOK = false;
			}
			else if ( stoi(input) >= 5 && emp.getPrivilege( ) < 2 )
			{
				std::cout << "Must be Manager to use command" << std::endl;
				privilegeOK = false;
			}
			else if ( stoi(input) >= 3 && emp.getPrivilege( ) < 1 )
			{
				std::cout << "Must be Shift Manager to use command" << std::endl;
				privilegeOK = false;
			}


			//do action if privilege level is valid
			if ( privilegeOK )
			{
				std::string in;
				bool verbose = false;

				switch ( stoi(input) )
				{
					case 1: //starts transaction
						startTransaction( );
						break;
					case 2: //increases register opened for employee
						emp.addRegisterOpens( );
						sEmployee.registerOpened(emp.getNumber( ));
						break;
					case 3: //searches for product
						std::cout << "Enter Provider to search for: ";
						std::getline(std::cin, in);
						searchProduct(in);
						break;
					case 4: //adds product to product table
						addProduct( );
						break;
					case 5: //adds employee to employee table
						addEmployee( );
						break;
					case 6: //asks if verbose description wanted then prints full list of employees
						std::cout << "Full description? (y/n)" << std::endl;
						std::getline(std::cin, in);
						in[0] = tolower(in[0]);
						if ( in[0] == 'y' )
							verbose = true;
						printEmployeeList(verbose);
						break;
					case 7: //asks if verbose description wanted then prints list of employees that contain inputed string
						std::cout << "Full description? (y/n)" << std::endl;
						std::getline(std::cin, in);
						in[0] = tolower(in[0]);
						if ( in[0] == 'y' )
							verbose = true;
						std::cout << "Enter Employee Name: ";
						std::getline(std::cin, in);
						printEmployeeList(verbose, in);
						break;
					case 8: //asks to search for employee to change
						std::cout << "Search Employees first? (y/n)" << std::endl;
						std::getline(std::cin, in);
						in[0] = tolower(in[0]);
						if ( in[0] == 'y' )
						{
							std::cout << "Enter Name to Search: ";
							std::getline(std::cin, in);
							printEmployeeList(verbose, in);
						}
						//asks user to input number of employee to update
						std::cout << "Enter Employee Number: ";
						std::getline(std::cin, in);
						if ( in.find_first_not_of("0123456789") == std::string::npos ) //checks that input is valid
						{
							Employee em = sEmployee.getEmployee(std::stoi(in));
							if ( em.isError( ) ) //checks if employee exists
							{
								std::cout << "Employee number does not exist." << std::endl;
							}
							else if ( em.getPrivilege( ) > emp.getPrivilege( ) ) //checks if current employee is privileged enough to edit selected employee
							{
								std::cout << "You do not have permissions to edit this Employee" << std::endl;
							}
							else //starts employee update handler
							{
								chooseItemToUpdate(em);
							}
						}
						break;
					case 9: //asks to search for employee to change
						std::cout << "Search Employees first? (y/n)" << std::endl;
						std::getline(std::cin, in);
						in[0] = tolower(in[0]);
						if ( in[0] == 'y' )
						{
							std::cout << "Enter Name to Search: ";
							std::getline(std::cin, in);
							printEmployeeList(verbose, in);
						}
						//enter employee number to edit
						std::cout << "Enter Employee Number: ";
						std::getline(std::cin, in);
						if ( in.find_first_not_of("0123456789") == std::string::npos ) //checks that input is valid
						{
							Employee em = sEmployee.getEmployee(std::stoi(in));
							if ( em.isError( ) ) //checks if employee exists
							{
								std::cout << "Employee number does not exist." << std::endl;
							}
							else if ( em.getPrivilege( ) > emp.getPrivilege( ) ) //checks if current employee is privileged enough to edit selected employee
							{
								std::cout << "You do not have permissions to edit this Employee" << std::endl;
							}
							else //starts employee privilege handler
							{
								employeeUpdatePrivilege(em);
							}
						}
						break;
					case 10: //resets employee weekly data
						sEmployee.resetWeek( );
						break;
					case 11: //resets employee monthly data and prints to .json file
						resetMonth( );
						break;
					case 12://asks to search for employee to delete first
						std::cout << "Search Employees first? (y/n)" << std::endl;
						std::getline(std::cin, in);
						in[0] = tolower(in[0]);
						if ( in[0] == 'y' )
						{
							std::cout << "Enter Name to Search: ";
							std::getline(std::cin, in);
							printEmployeeList(verbose, in);
						}
						//asks to enter number of employee to delete
						std::cout << "Enter Employee Number: ";
						std::getline(std::cin, in);
						if ( in.find_first_not_of("0123456789") == std::string::npos ) //checks that input is valid
						{
							Employee em = sEmployee.getEmployee(std::stoi(in));
							if ( em.isError( ) ) //checks if employee exists
							{
								std::cout << "Employee number does not exist." << std::endl;
							}
							else if ( em.getPrivilege( ) > emp.getPrivilege( ) ) //checks if current employee is privileged enough to edit selected employee
							{
								std::cout << "You do not have permissions to edit this Employee" << std::endl;
							}
							else
							{
								//prints data of selected employee
								em.print( );
								//asks user if they are sure they want to delete selected employee
								std::cout << "Are you sure you want to delete Employee? (y/n)" << std::endl;
								std::getline(std::cin, in);
								//attempts employee delete, prints error message if not completed
								if ( in[0] == 'y' )
								{
									if ( sEmployee.removeEmployee(em.getNumber( )) == 0 )
									{
										std::cout << "Deleted Successfully" << std::endl;
									}
									else
										std::cout << "Error deleting, try again." << std::endl;
								}
								else //cancels
								{
									std::cout << "Canceling delete" << std::endl;
								}
							}
						}
						break;
					case 0:
						logout( );
						return 0;
						break;
				}
			}
		}
		system("pause");
		system("cls");
		//log back in if logged out
		if ( input == "0" )
			login( );
	}
	return 1;
}

//employee print classes
//pass string to search for employees that contain that string in their name
//verbose = true prints extra information
bool Handler::printEmployeeList(bool verbose)
{
	//gets list of all employees
	std::vector<Employee> list = sEmployee.getAllEmployees( );

	//top separator
	std::cout << std::left;
	std::cout << "*************************************";
	if ( verbose )	//prints if verbose true
		std::cout << "********************************************************";
	std::cout << std::endl;

	//prints information row
	std::cout << "| " << std::setw(10) << "Name" << " |  " << std::setw(6) << "Number" << "  | ";
	if ( verbose )	//prints if verbose true
		std::cout << " " << std::setw(4) << "Code" << "  | ";
	std::cout << std::setw(9) << "Privilege" << " |   ";
	if ( verbose )	//prints if verbose true
	{
		std::cout << std::setw(12) << "Transactions" << " | ";
		std::cout << std::setw(14) << "Register Opens" << " | " << std::setw(10) << "Money Made" << " |" << std::endl;
		std::cout << "|------------|----------|--------|-----------|----WTD-|---MTD-|----WTD-|---MTD-|------------|";
	}
	std::cout << std::endl;


	//prints employee information
	for ( auto e : list )
	{
		std::cout << std::left << "| " << std::setw(10) << e.getName( ) << " | " << std::right << std::setw(8) << e.getNumber( ) << " | ";
		if ( verbose )	//prints if verbose true
			std::cout << std::setw(6) << e.getCode( ) << " | ";
		std::cout << std::setw(9) << e.getPrivilege( ) << " |";
		if ( verbose )	//prints if verbose true
		{
			std::cout << std::setw(7) << e.getTransactionsWTD( ) << " |" << std::setw(6) << e.getTransactionsMTD( ) << " |";
			std::cout << std::setw(7) << e.getRegisterOpensWTD( ) << " |" << std::setw(6) << e.getRegisterOpensMTD( ) << " |";
			std::cout << std::setw(11) << e.getMoneyMade( ) << " |";
		}
		std::cout << std::endl;
	}

	//prints bottom separator
	std::cout << std::left;
	std::cout << "*************************************";
	if ( verbose )	//prints if verbose true
		std::cout << "********************************************************";
	std::cout << std::endl;
	return true;
}
bool Handler::printEmployeeList(bool verbose, std::string name)
{
	//gets list of employees whose Names contains the string name
	std::vector<Employee> list = sEmployee.getEmployeesByName(name);

	//top separator
	std::cout << std::left;
	std::cout << "*************************************";
	if ( verbose )	//prints if verbose true
		std::cout << "********************************************************";
	std::cout << std::endl;

	//prints information row
	std::cout << "| " << std::setw(10) << "Name" << " |  " << std::setw(6) << "Number" << "  | ";
	if ( verbose )	//prints if verbose true
		std::cout << " " << std::setw(4) << "Code" << "  | ";
	std::cout << std::setw(9) << "Privilege" << " |   ";
	if ( verbose )	//prints if verbose true
	{
		std::cout << std::setw(12) << "Transactions" << " | ";
		std::cout << std::setw(14) << "Register Opens" << " | " << std::setw(10) << "Money Made" << " |" << std::endl;
		std::cout << "|------------|----------|--------|-----------|----WTD-|---MTD-|----WTD-|---MTD-|------------|";
	}
	std::cout << std::endl;


	//prints employee information
	for ( auto e : list )
	{
		std::cout << std::left << "| " << std::setw(10) << e.getName( ) << " | " << std::right << std::setw(8) << e.getNumber( ) << " | ";
		if ( verbose )	//prints if verbose true
			std::cout << std::setw(6) << e.getCode( ) << " | ";
		std::cout << std::setw(9) << e.getPrivilege( ) << " |";
		if ( verbose )	//prints if verbose true
		{
			std::cout << std::setw(7) << e.getTransactionsWTD( ) << " |" << std::setw(6) << e.getTransactionsMTD( ) << " |";
			std::cout << std::setw(7) << e.getRegisterOpensWTD( ) << " |" << std::setw(6) << e.getRegisterOpensMTD( ) << " |";
			std::cout << std::setw(11) << e.getMoneyMade( ) << " |";
		}
		std::cout << std::endl;
	}

	//prints bottom separator
	std::cout << std::left;
	std::cout << "*************************************";
	if ( verbose )	//prints if verbose true
		std::cout << "********************************************************";
	std::cout << std::endl;
	return true;
}

//resets monthly data and add information to a .json file
bool Handler::resetMonth( )
{
	std::vector<Employee> list = sEmployee.getAllEmployees( );

	//transacion data directory
	std::string resetdir = ".\\Monthly Transaction Data";

	//gets time
	time_t now = time(0);
	tm localt;
	localtime_s(&localt, &now);
	resetdir = resetdir + "\\" + std::to_string((localt.tm_year + 1900)); //adds year to directory path

	//creates directory
	std::filesystem::create_directories(resetdir);

	//gets string for current month
	std::string month;
	switch ( localt.tm_mon + 1 )
	{
		case 1:
			month = "01 January";
			break;
		case 2:
			month = "02 February";
			break;
		case 3:
			month = "03 March";
			break;
		case 4:
			month = "04 April";
			break;
		case 5:
			month = "05 May";
			break;
		case 6:
			month = "06 June";
			break;
		case 7:
			month = "07 July";
			break;
		case 8:
			month = "08 August";
			break;
		case 9:
			month = "09 September";
			break;
		case 10:
			month = "10 October";
			break;
		case 11:
			month = "11 November";
			break;
		case 12:
			month = "12 December";
			break;
	}

	//adds month.json to directory path
	std::string fulldir = resetdir + "\\" + month + ".json";

	//creates tree to use in json
	ptree employee;

	for ( auto i : list )
	{
		//inserts employee's transaction data into tree
		employee.put(i.getName( ) + ".Register Opens", i.getRegisterOpensMTD( ));
		employee.put(i.getName( ) + ".Transactions", i.getTransactionsMTD( ));
		employee.put(i.getName( ) + ".Money Made", i.getMoneyMade( ));
	}
	//creates json
	boost::property_tree::json_parser::write_json(fulldir, employee);

	//resets monthly data in table
	sEmployee.resetMonth( );
	return true;

}

//gets product from sn
//if error return product where product.isError() == true
Product Handler::getProduct(unsigned long sn )
{
	return sProduct.getProductBySN(std::to_string(sn));
}
//gets product from name
//if error return product where product.isError() == true
Product Handler::getProduct(std::string name)
{
	//gets employee by name
	std::vector<Product> list = sProduct.getProductsByName(name);
	if ( list.size( ) == 0 )		//none found
	{
		std::cout << "No Product Found" << std::endl;
		return Product(true);
	}
	else if ( list.size( ) == 1)	//returns employee
	{
		return list[0];
	}
	else							//more than one found
	{
		std::cout << "Too Many Products from Name" << std::endl;
		return Product(true);
	}
}
//adds product to product SQL table
bool Handler::addProduct( )
{
	//product data
	std::string serialNumber, provider, name;
	int category;
	double cost, price;
	Product p;
	std::string temp;

	std::cout << std::endl;
	system("cls");

	//product serial number
	bool loop = true;
	while ( loop )
	{
		std::cout << "Enter Product Serial Number: ";
		std::getline(std::cin, serialNumber);
		temp = serialNumber;
		std::transform(temp.begin( ), temp.end( ), temp.begin( ), tolower);
		if ( temp == "exit" )	//checks if the user is trying to exit
		{
			std::cout << "Cancel Product Add" << std::endl;
			return false;
		}
		else if ( serialNumber.find_first_not_of("0123456789") != std::string::npos )	//checks that input is valid
		{
			std::cout << "Non-Integer found, try again" << std::endl;

		}
		else loop = false;
	}

	//product name
	std::cout << std::flush;
	std::cout << "Enter Product Name: ";
	std::getline(std::cin, name);
	temp = name;
	std::transform(temp.begin( ), temp.end( ), temp.begin( ), tolower);
	if ( temp == "exit" )	//checks if the user is trying to exit
	{
		std::cout << "Cancel Product Add" << std::endl;
		return false;
	}


	//provider name
	std::cout << std::flush;
	std::cout << "Enter Provider Name: ";
	std::getline(std::cin, provider);

	temp = provider;
	std::transform(temp.begin( ), temp.end( ), temp.begin( ), tolower);
	if ( temp == "exit" )	//checks if the user is trying to exit
	{
		std::cout << "Cancel Product Add" << std::endl;
		return false;
	}



	std::cout << "Only Numbers accepted for following information" << std::endl;

	//category loop
	loop = true;
	while ( loop )
	{
		std::cout << "1. Grocery" << std::endl;
		std::cout << "2. Frozen Food" << std::endl;
		std::cout << "3. Household Supply" << std::endl;
		std::cout << "4. Liquor" << std::endl;
		std::cout << "5. Beer" << std::endl;
		std::cout << "6. Wine" << std::endl;
		std::cout << "7. Cigar" << std::endl;
		std::cout << "8. Cigarette" << std::endl;

		std::cout << "Enter Category Number: ";
		std::getline(std::cin, temp);

		std::transform(temp.begin( ), temp.end( ), temp.begin( ), tolower);
		if ( temp == "exit" )	//checks if the user is trying to exit
		{
			std::cout << "Cancel Product Add" << std::endl;
			return false;
		}
		else if ( temp.find_first_not_of("0123456789-") != std::string::npos ) //checks that input is valid
		{
			std::cout << "Non-Integer found, try again" << std::endl;

		}
		else loop = false;
		category = std::stoi(temp);

	}


	//Cost loop
	loop = true;
	while ( loop )
	{
		std::cout << "Enter Cost: $";
		std::getline(std::cin, temp);

		std::transform(temp.begin( ), temp.end( ), temp.begin( ), tolower);
		if ( temp == "exit" )	//checks if the user is trying to exit
		{
			std::cout << "Cancel Product Add" << std::endl;
			return false;
		}
		else if ( temp.find_first_not_of("0123456789.") != std::string::npos ) //checks that input is valid
		{
			std::cout << "Non-Integer found, try again" << std::endl;

		}
		else loop = false;
		cost = std::stod(temp);

	}


	//price loop
	loop = true;
	while ( loop )
	{
		std::cout << "Enter Price: $";
		std::getline(std::cin, temp);

		std::transform(temp.begin( ), temp.end( ), temp.begin( ), tolower);
		if ( temp == "exit" )	//checks if the user is trying to exit
		{
			std::cout << "Cancel Product Add" << std::endl;
			return false;
		}
		else if ( temp.find_first_not_of("0123456789.") != std::string::npos ) //checks that input is valid
		{
			std::cout << "Non-Integer found, try again" << std::endl;

		}
		else loop = false;
		price = std::stod(temp);

	}


	//adds product to table
	int err;	//tracks error number
	while ( 0 != (err = sProduct.addProduct(serialNumber, category, name, provider, cost, price)) )
	{
		if ( err == 1 || err == 2 )
		{
			std::cout << "Connection Error" << std::endl;
			return false;
		}
		if ( err == 3 )	//if serial number already in use let user change it
		{
			std::cout << "Serial Number already in use. ";
			bool loop = true;
			while ( loop )
			{
				std::cout << "Enter Serial Number: ";
				std::getline(std::cin, serialNumber);
				temp = serialNumber;
				std::transform(temp.begin( ), temp.end( ), temp.begin( ), tolower);
				if ( temp == "exit" )	//checks if the user is trying to exit
				{
					std::cout << "Cancel Product Add" << std::endl;
					return false;
				}
				else if ( temp.find_first_not_of("0123456789") != std::string::npos ) //checks that input is valid
				{
					std::cout << "Non-Integer found, try again" << std::endl;

				}
				else loop = false;
			}
		}
		if ( err == 4 ) //if name already in use let user change it
		{
			std::cout << "Product Name already in use. ";
			loop = true;
			while ( loop )
			{
				std::cout << "Enter Product Name: ";
				std::getline(std::cin, name);
				temp = name;
				std::transform(name.begin( ), name.end( ), temp.begin( ), tolower);
				if ( temp == "exit" )	//checks if the user is trying to exit
				{
					std::cout << "Cancel Product Add" << std::endl;
					return false;
				}
				else loop = false;
			}

		}
	}


	return true;
}
//searches for product by providers
bool Handler::searchProduct(std::string provider)
{
	//gets list of products whose Provider contains the string provider
	std::vector<Product> list = sProduct.getProductsByProvider(provider);
	if ( list.empty( ) )
	{
		std::cout << "No Products Found" << std::endl;
		return false;
	}

	//default spacing lengths
	int snlen = 13;
	int namelen = 4;
	int catlen = 8;
	int provlen = 8;
	//gets spacing length if any strings need more space than allocated
	for ( auto i : list )
	{
		int temp;
		if ( snlen < (temp = i.getSerialNumber( ).length( )) )
			snlen = temp;
		if ( namelen < (temp = i.getName( ).length( )) )
			namelen = temp;
		if ( catlen < (temp = i.getCategory( ).length( )) )
			catlen = temp;
		if ( provlen < (temp = i.getProvider( ).length( )) )
			provlen = temp;
	}

	//top spacer
	std::cout << "*******************";
	for ( int i = 0; i < (snlen + namelen + catlen + provlen); i++ )
	{
		std::cout << "*";
	}
	std::cout << "************************" << std::endl;

	//prints information row
	std::cout << "| " << std::setw(snlen + 2) << "Serial Number" << " | " << std::setw(namelen + 2) << "Name" << " | ";
	std::cout << std::setw(catlen + 2) << "Category" << " | " << std::setw(provlen + 2) << "Provider" << " | " << std::setw(8) << "Cost" << " | " << std::setw(8) << "Price" << " |" << std::endl;

	//prints product data
	for ( auto i : list )
	{
		std::cout << "| " << std::setw(snlen + 2) << i.getSerialNumber( ) << " | " << std::setw(namelen + 2) << i.getName() << " | ";
		std::cout << std::setw(catlen + 2) << i.getCategory() << " | " << std::setw(provlen + 2) << i.getProvider() << " | " << std::setw(8) << i.getCost() << " | " << std::setw(8) << i.getPrice() << " |" << std::endl;
	}

	//bottom spacer
	std::cout << "*******************";
	for ( int i = 0; i < (snlen + namelen + catlen + provlen); i++ )
	{
		std::cout << "*";
	}
	std::cout << "************************" << std::endl;


	return true;
}

//adds employee to Employee SQL table
bool Handler::addEmployee( )
{
	//employee data
	std::string name, number, code, priv;
	std::string temp;
	Employee e;

	//employee name
	std::cout << "Enter Name: ";
	std::getline(std::cin, name);

	temp = name;
	std::transform(temp.begin( ), temp.end( ), temp.begin( ), tolower);
	if ( temp == "exit" )	//checks if the user is trying to exit
	{
		std::cout << "Cancel Employee Add" << std::endl;
		return false;
	}

	std::cout << "Only Numbers accepted for following information" << std::endl;
	//employee number
	bool loop = true;
	while ( loop )
	{
		std::cout << "Enter Employee Number: ";
		std::getline(std::cin, number);
		temp = number;
		std::transform(number.begin( ), number.end( ), temp.begin( ), tolower);
		if ( temp == "exit" )	//checks if the user is trying to exit
		{
			std::cout << "Cancel Employee Add" << std::endl;
			return false;
		}
		else if ( number.find_first_not_of("0123456789") != std::string::npos ) //checks that input is valid
		{
			std::cout << "Non-Integer found, try again" << std::endl;

		}
		else loop = false;
	}

	//employee code
	loop = true;
	while ( loop )
	{
		std::cout << "Enter Employee Log-In Code: ";
		std::getline(std::cin, code);
		temp = code;
		std::transform(code.begin( ), code.end( ), temp.begin( ), tolower);
		if ( temp == "exit" )	//checks if the user is trying to exit
		{
			std::cout << "Cancel Employee Add" << std::endl;
			return false;
		}
		else if ( code.find_first_not_of("0123456789") != std::string::npos ) //checks that input is valid
		{
			std::cout << "Non-Integer found, try again" << std::endl;
		}
		else loop = false;
	}

	//employee privilege
	loop = true;
	while ( loop )
	{
		std::cout << "0 = Cashier" << std::endl;
		std::cout << "1 = Shift Manager" << std::endl;
		std::cout << "2 = Manager" << std::endl;
		std::cout << "3 = Admin" << std::endl;
		std::cout << "4 = Owner" << std::endl;
		std::cout << "Enter Employee Privilege: ";
		std::getline(std::cin, priv);
		temp = priv;
		std::transform(priv.begin( ), priv.end( ), priv.begin( ), tolower);
		if ( temp == "exit" )	//checks if the user is trying to exit
		{
			std::cout << "Cancel Employee Add" << std::endl;
			return false;
		}
		else if ( priv.find_first_not_of("0123456789") != std::string::npos ) //checks that input is valid
		{
			std::cout << "Non-Integer found, try again" << std::endl;
		}
		else loop = false;
	}


	//adds product to table
	int err;	//tracks error number
	while ( 0 != (err = sEmployee.addEmployee(name, std::stoi(number), std::stoi(code), std::stoi(priv))) )
	{
		if ( err == 1 || err == 2 )
		{
			std::cout << "Connection Error" << std::endl;
			return false;
		}
		if ( err == 3 ) //if number is already in use let user change it
		{
			std::cout << "Employee Number already in use. ";
			bool loop = true;
			while ( loop )
			{
				std::cout << "Enter Employee Number: ";
				std::getline(std::cin, number);
				temp = number;
				std::transform(number.begin( ), number.end( ), temp.begin( ), tolower);
				if ( temp == "exit" )	//checks if the user is trying to exit
				{
					std::cout << "Cancel Employee Add" << std::endl;
					return false;
				}
				else if ( number.find_first_not_of("0123456789") != std::string::npos ) //checks that input is valid
				{
					std::cout << "Non-Integer found, try again" << std::endl;

				}
				else loop = false;
			}
		}
		if ( err == 4 ) //if code is already in use let user change it
		{
			std::cout << "Log-In Code already in use. ";
			loop = true;
			while ( loop )
			{
				std::cout << "Enter Employee Log-In Code: ";
				std::getline(std::cin, code);
				temp = code;
				std::transform(code.begin( ), code.end( ), temp.begin( ), tolower);
				if ( temp == "exit" )	//checks if the user is trying to exit
				{
					std::cout << "Cancel Employee Add" << std::endl;
					return false;
				}
				else if ( code.find_first_not_of("0123456789") != std::string::npos ) //checks that input is valid
				{
					std::cout << "Non-Integer found, try again" << std::endl;
				}
				else loop = false;
			}

		}
	}
	return true;

}

//Employee update classes handler
bool Handler::chooseItemToUpdate(Employee selected)
{
	Employee temp = selected; //creates copy of selected employee
	std::string selection;
	while ( selection != "0" )
	{
		//prints old and changed info
		system("cls");
		std::cout << "Old Info" << std::endl;
		selected.print( );
		std::cout << std::endl << "New Info" << std::endl;
		temp.print( );
		std::cout << std::endl;

		//asks for input on what info to change
		std::cout << "What would you like to change?" << std::endl;
		std::cout << "1. Name  \n2. Employee Number  \n3. Employee Code" << std::endl;
		std::cout << "4. Privilege  \n5. Transaction Info  \n6. Register Opens" << std::endl;
		std::cout << "0. Confirm Changes" << std::endl;
		std::cout << "-1. Exit Without Saving" << std::endl;
		std::cout << "Enter Number Wanted: ";
		std::getline(std::cin, selection);

		//checks that input is valid
		if ( selection.find_first_not_of("-0123456") != std::string::npos )
		{
			std::cout << "Selection not valid, try again." << std::endl;
		}
		//switch for accesing selected option
		std::string input;
		switch ( stoi(selection) )
		{
			case -1:		//exit without saving
				return false;
				break;
			case 0:			//saves employee data and exits
				sEmployee.updateEmployee(temp, selected.getNumber( ));
				return true;
				break;
			case 1:			//enter a name
				std::cout << "Enter New Name: ";
				std::getline(std::cin, input);
				temp.changeName(input);
				break;
			case 2:			//enter number
				std::cout << "Enter New Employee Number: ";
				std::getline(std::cin, input);

				if ( input.find_first_not_of("0123456789") != std::string::npos )
				{
					std::cout << "Non-Integer found, try again" << std::endl;

				}
				else temp.changeNumber(std::stoi(input));
				break;
			case 3:			//enter code
				std::cout << "Enter New Employee Code: ";
				std::getline(std::cin, input);

				if ( input.find_first_not_of("0123456789") != std::string::npos )
				{
					std::cout << "Non-Integer found, try again" << std::endl;

				}
				else temp.changeCode(std::stoi(input));
				break;
			case 4:			//enter privilege
				std::cout << "Enter New Privilege: ";
				std::getline(std::cin, input);

				if ( input.find_first_not_of("0123456789") != std::string::npos )
				{
					std::cout << "Non-Integer found, try again" << std::endl;

				}
				else temp.changePrivilege(std::stoi(input));
				break;
			case 5:			//enter transactions to add
				std::cout << "Enter Number of Transactions to add: ";
				std::getline(std::cin, input);

				if ( input.find_first_not_of("-0123456789") != std::string::npos )
				{
					std::cout << "Non-Integer found, try again" << std::endl;

				}
				else temp.addTransaction(std::stoi(input));
				break;
			case 6:			//enter register opens to add
				std::cout << "Enter Number of RegisterOpens to add: ";
				std::getline(std::cin, input);

				if ( input.find_first_not_of("-0123456789") != std::string::npos )
				{
					std::cout << "Non-Integer found, try again" << std::endl;

				}
				else temp.addRegisterOpens(std::stoi(input));
				break;
		}
	}

}

//updates all employee info in SQL table to most recent information
bool Handler::employeeUpdateALL( )
{
	if ( sEmployee.updateEmployee(emp, emp.getNumber( )) == 0 )
		return true;
	else
		return false;
}

//updates selected employee's privilege
bool Handler::employeeUpdatePrivilege(Employee selected)
{
	bool loop = true;
	while ( loop )
	{
		//prints different privilege level and asks for user's input
		system("cls");
		std::cout << "0 = Cashier" << std::endl;
		std::cout << "1 = Shift Manager" << std::endl;
		std::cout << "2 = Manager" << std::endl;
		std::cout << "3 = Admin" << std::endl;
		std::cout << "4 = Owner" << std::endl;
		std::cout << "Enter Employee Privilege: ";
		std::string input;
		std::getline(std::cin, input);
		//checks if user wants to exit
		if ( tolower(input[0]) == 'e' )
		{
			std::cout << "Exiting" << std::endl;
			return false;
		}
		//checks that input is valid
		else if ( input.find_first_not_of("01234") != std::string::npos )
		{
			std::cout << "Invalid Input" << std::endl;
		}
		//checks that promotion is not promoting target higher than current logged in employee's privilege
		else if(std::stoi(input) > emp.getPrivilege() )
		{
			std::cout << "Can not promote higher than your position" << std::endl;
		}
		//changes privilege
		else
		{
			selected.changePrivilege(std::stoi(input));
			if(sEmployee.updateEmployee(selected, selected.getNumber( )) != 0)
			{
				std::cout << "Error Updating Employee" << std::endl;
			}
			return true;
		}
		system("pause");
	}

}

// starts transaction
bool Handler::startTransaction( )
{
	//gets list of products to buy
	std::vector<Product> list = getTransactionProducts();

	//clears screen and prints list + total price
	system("cls");
	transactionProductPrint(list);
	std::cout << std::setprecision(2) << std::fixed;
	double total = getTotalPrice(list);
	std::cout << "Total: $" << total << std::endl << std::endl;

	//starts payment handler
	transactionPayment(list, total);

	//adds money made to employee total and increase number of transacitons
	emp.addMoney(total);
	emp.addTransaction( );
	sEmployee.transactionMade(emp.getNumber( ), total);


	return true;
}
//creates list of products to be sold
std::vector<Product> Handler::getTransactionProducts( )
{
	bool loop = true;
	std::vector<Product> list;
	std::vector<std::string> catlist = {"Grocery","Frozen Food", "Household Supply","Liquor","Beer","Wine","Cigar","Cigarette","NO TAX"};

	while ( loop )
	{
		std::string input;
		system("cls");
		//prints list and total if not empty
		if ( !list.empty( ) )
		{
			transactionProductPrint(list);
			std::cout << "Total: $" << std::setprecision(2) << getTotalPrice(list) << std::endl;
			std::cout << "Enter \"Payment\" to start payment process" << std::endl;
		}

		std::cout << "Product Entry" << std::endl;

		std::cout << "Enter Serial Number: ";
		std::getline(std::cin, input);
		//checks if 'P'ayment was entered
		if ( input[0] == 'p' || input[0] == 'P' )
		{
			loop = false;
			break;
		}

		//checks if serial number matches product and add to list if it does
		Product search = sProduct.getProductBySN(input);
		if ( !search.isError( ) )
		{
			list.push_back(search);
		}
		else
		{
			//temp product to use for filling in product info
			Product temp(false);
			std::cout << "Serial Number not found. Enter remaining information" << std::endl;
			temp.setSerialNumber(input);

			//get product category
			bool loop2 = true;
			while ( loop2 )
			{
				std::cout << "Category: ";
				std::getline(std::cin, input);
				for(int i = 1; i < input.size(); i++ )
				{
					//makes letter lowercase
					input[i] = tolower(input[i]);
					//makes letter uppercase if they are after a space
					if(input[i-1] == ' ' )
						input[i] = toupper(input[i]);
				}
				//makes first letter uppercase
				input[0] = toupper(input[0]);

				//checks that category exists, loop if invalid input
				for ( auto i : catlist )
				{
					if ( i == input )
					{
						loop2 = false;
					}
				}
				if ( loop2 )
				{
					std::cout << "Invalid Category." << std::endl;
				}
			}
			temp.setCategory(input); //sets category

			//gets product's name
			std::cout << "Name: ";
			std::getline(std::cin, input);
			temp.setName(input);

			//gets product price
			loop2 = true;
			while ( loop2 )
			{
				std::cout << "price: ";
				std::getline(std::cin, input);

				//checks that valid input is used, loop if not
				if ( input.find_first_not_of("0123456789.") != std::string::npos )
				{
					std::cout << "Invalid input, try again" << std::endl;

				}
				else
				{
					loop2 = false;
				}
			}
			temp.setPrice(std::stod(input)); //sets product price
			list.push_back(temp);	//adds product to list

		}


	}
	return list;
}
//handles payments for products
void Handler::transactionPayment(std::vector<Product> list, double total)
{
	std::string input;
	double methods[3] = {0,0,0}; //payment method vector initilized to 0
	int m = 0;
	double amountRemaining = total; //amount remaining of total transaction
	//gets total for a foodstamp purchase (food stamps do not get taxed)
	double fstot = 0;
	for ( auto i : list )
	{
		if ( i.getCategory( ) == "Grocery" || i.getCategory( ) == "Frozen Food" )
		{
			fstot += i.getPrice( );
		}
	}

	//payment loop, exits when amount remaining is less than 0
	bool loop = true;
	while ( loop && amountRemaining >= 0.00)
	{
		//selects payment type
		std::cout << "Enter Payment Type" << std::endl;
		std::cout << "1 = Cash   2 = Card  3 = Food Stamp" << std::endl;
		std::getline(std::cin, input);
		//loop again if invalid input is selected
		if ( input.find_first_not_of("123") != std::string::npos )
		{
			std::cout << "Invalid input, try again" << std::endl;

		}
		else
		{
			std::string input2;
			m = (moneyType)(std::stoi(input) - 1);

			//if foodstamp and foodstamp total not reduced to 0
			if ( m == fs  && fstot >= 0)
			{
				bool loop2 = true;
				while ( loop2 )
				{
					//prints total needed to pay and asks for amount to pay
					//if 'B'ack is entered then return to previous menu
					std::cout << "Food Stamp Total: $" << fstot << std::endl;
					std::cout << "Payment: $";
					std::getline(std::cin, input2);

					if ( tolower(input2[0]) == 'b' )
					{
						loop2 = false;
						m--;
					}
					//checks if input is not valid
					else if ( input2.find_first_not_of("1234567890.") != std::string::npos )
					{
						std::cout << "Invalid input, try again" << std::endl;

					}
					else
					{
						double fspay = std::stod(input2);
						//checks that foodstamp payment is not more than total and adjusts payment amount accordingly
						if ( fspay > fstot )
							fspay = fstot;
						fstot = fstot - fspay;

						//adds payment amount to method array
						methods[m] += fspay;
						amountRemaining -= fspay * list.begin( )->getCategoryTax("Grocery");	//removes paid fs from amount total, accounting for tax difference
						loop2 = false;

					}
				}


			}
			//if cash selected
			else if ( m == cash )
			{
				bool loop2 = true;
				while ( loop2 )
				{
					//prints total needed to pay and asks for amount to pay
					//if 'B'ack is entered then return to previous menu
					std::cout << "Remaining: $" << amountRemaining << std::endl;
					std::cout << "Payment: $";
					std::getline(std::cin, input2);

					if ( tolower(input2[0]) == 'b' )
					{
						loop2 = false;
						m--;
					}
					//checks if input is not valid
					else if ( input2.find_first_not_of("1234567890.") != std::string::npos )
					{
						std::cout << "Invalid input, try again" << std::endl;

					}
					else
					{
						//subtracts amount paid from amount remaining and adds cash paid to method array
						double cashAmount = std::stod(input2);
						methods[m] += cashAmount;
						amountRemaining -= cashAmount;
						loop2 = false;
					}
				}

			}
			//if card selected
			else if ( m == card )
			{
				bool loop2 = true;
				while ( loop2 )
				{
					//prints total needed to pay and asks for amount to pay
					//if 'B'ack is entered then return to previous menu
					std::cout << "Remaining: $" << amountRemaining << std::endl;
					std::cout << "Payment: $";
					std::getline(std::cin, input2);

					if ( tolower(input2[0]) == 'b' )
					{
						loop2 = false;
						m--;
					}
					//checks if input is not valid
					else if ( input2.find_first_not_of("1234567890.") != std::string::npos )
					{
						std::cout << "Invalid input, try again" << std::endl;

					}
					else
					{
						//subtracts amount paid from amount remaining and adds credit paid to method array
						double cardAmount = std::stod(input2);
						methods[m] += cardAmount;
						amountRemaining -= cardAmount;
						loop2 = false;
					}
				}
			}

			//clears screen and prints product list
			system("cls");
			transactionProductPrint(list);
			//prints total and amount remaining to pay
			std::cout << "Total: $" << total << "\tRemaining: $";
			if ( amountRemaining < 0.00 )
			{
				std::cout << "0.00" << std::endl;
			}
			else
			{
				std::cout << amountRemaining << std::endl;

			}
			//prints method and amount paid using method
			for ( int i = 0; i < 3; i++ )
			{
				if ( methods[i] != 0 )
				{
					switch ( i )
					{
						case 0:
							std::cout << "Cash: $" << methods[i] << "\t";
							break;
						case 1:
							std::cout << "Card: $" << methods[i] << "\t";
							break;
						case 2:
							std::cout << "Food Stamps: $" << methods[i] << "\t";
							break;
					}
				}
			}
			std::cout << std::endl << std::endl;


		}

	}
	//prints change that must be given back to customer
	std::cout << "Change: $" << abs(amountRemaining) << std::endl << std::endl;

	//prints receipt to .json file
	createReceipt(list, methods, total, abs(amountRemaining));

}
//creates receipt to put in .json file
bool Handler::createReceipt(std::vector<Product> list, double pMethods[], double total, double change)
{
	//receipt base directory
	std::string receiptdir = ".\\Receipt";

	//gets current time
	time_t now = time(0);
	tm localt;
	localtime_s(&localt, &now);
	receiptdir = receiptdir + "\\" + std::to_string((localt.tm_year + 1900)); //adds year to directory path

	//gets month to use in directory path
	std::string month;
	switch ( localt.tm_mon + 1 )
	{
		case 1:
			month = "01 January";
			break;
		case 2:
			month = "02 February";
			break;
		case 3:
			month = "03 March";
			break;
		case 4:
			month = "04 April";
			break;
		case 5:
			month = "05 May";
			break;
		case 6:
			month = "06 June";
			break;
		case 7:
			month = "07 July";
			break;
		case 8:
			month = "08 August";
			break;
		case 9:
			month = "09 September";
			break;
		case 10:
			month = "10 October";
			break;
		case 11:
			month = "11 November";
			break;
		case 12:
			month = "12 December";
			break;
	}

	//gets remaining time data and converts them to strings
	int day = localt.tm_mday;
	int hour = localt.tm_hour;
	int min = localt.tm_min;
	int sec = localt.tm_sec;
	std::string dayS = std::to_string(day);
	std::string hourS = std::to_string(hour);
	std::string minS = std::to_string(min);
	std::string secS = std::to_string(sec);
	//adds 0 to begining of string if 0-9 in order to make directory easily organized by name
	if ( day < 10 )
		dayS = "0" + dayS;
	if ( hour < 10 )
		hourS = "0" + hourS;
	if ( min < 10 )
		minS = "0" + minS;
	if ( sec < 10 )
		secS = "0" + secS;

	receiptdir = receiptdir + "\\" + month + "\\" + dayS;	//finishes full receipt directory
	std::filesystem::create_directories(receiptdir);		//creates directory

	//full file name HH-MM-SS.json
	std::string fulldir = receiptdir + "\\" + hourS + "-" + minS + "-" + secS + ".json";

	//creates tree to use in json
	ptree Products;
	int counter = 1; //counter used to keep track of how many products in receipt

	for ( auto i : list )
	{
		//ostringstream used to keep double precision to 2
		std::ostringstream converterP;
		converterP.precision(2);
		converterP << std::fixed << i.getPrice();

		std::ostringstream converterT;
		converterT.precision(0);
		converterT << std::fixed << (i.getCategoryTax( ) - 1) * 100;

		std::ostringstream converterTo;
		converterTo.precision(2);
		converterTo << std::fixed << (i.getPrice( ) * i.getCategoryTax( ));

		//inserts product data into products subtree
		Products.put("Products." + std::to_string(counter) + ": " + i.getName() + ".Category", i.getCategory());
		Products.put("Products." + std::to_string(counter) + ": " + i.getName( ) + ".Serial Number", i.getSerialNumber( ));
		Products.put("Products." + std::to_string(counter) + ": " + i.getName( ) + ".Price", "$" + converterP.str());
		Products.put("Products." + std::to_string(counter) + ": " + i.getName( ) + ".Tax", converterT.str() + "%");
		Products.put("Products." + std::to_string(counter) + ": " + i.getName( ) + ".Total", "$" + converterTo.str());

		counter++;
	}

	std::ostringstream converter;
	converter.precision(2);
	converter << std::fixed << total;
	//puts total price in payment subtree
	Products.put("Payment.Amount", "$" + converter.str());
	converter.str("");
	converter.clear( );

	//adds payment methods used to payment subtree
	for ( int i = 0; i < 3; i++ )
	{
		if ( pMethods[i] != 0 )
		{
			//gets string for method used
			std::string method;
			switch ( i )
			{
				case 0:
					method = "Cash";
					break;
				case 1:
					method = "Card";
					break;
				case 2:
					method = "Food Stamps";
					break;
			}
			converter << std::fixed << std::setprecision(2);
			converter << std::fixed << pMethods[i];
			//adds payment type and amount to payment subtree
			Products.put("Payment.Types." + method, "$" + converter.str());
			converter.str("");
			converter.clear( );
		}
	}

	converter.precision(2);
	converter << std::fixed << change;
	//puts change owed into payment subtree
	Products.put("Payment.Change", "$" + converter.str());
	converter.str("");
	converter.clear( );

	//creates json file
	boost::property_tree::json_parser::write_json(fulldir, Products);

	return true;
}
//prints a list of products in list
void Handler::transactionProductPrint(std::vector<Product> list)
{
	//default spacing lengths
	int snlen = 12;
	int namelen = 4;
	int catlen = 8;
	//gets spacing length if any strings need more space than allocated
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

	//top spacer
	std::cout << "*************";
	for ( int i = 0; i < (snlen + namelen + catlen); i++ )
	{
		if ( i == ((snlen + namelen + catlen) / 2) )
			std::cout << "Product*List";
		std::cout << "*";
	}
	std::cout << "*************" << std::endl;

	//prints information row
	std::cout << std::setprecision(2) << std::fixed;
	std::cout << "| " << std::setw(snlen + 2) << "Serial Number" << " | " << std::setw(namelen + 2) << "Name" << " | ";
	std::cout << std::setw(catlen + 2) << "Category" << " | " << std::setw(8) << "Price" << " | " << std::setw(8) << "Total" << " |" << std::endl;

	//prints product data
	for ( auto i : list )
	{
		double tot = i.getPrice( ) * i.getCategoryTax( );
		std::cout << "| " << std::setw(snlen + 2) << i.getSerialNumber( ) << " | " << std::setw(namelen + 2) << i.getName( ) << " | ";
		std::cout << std::setw(catlen + 2) << i.getCategory( ) << " | " << std::setw(8) << i.getPrice( ) << " | " << std::setw(8) << tot << " |" << std::endl;
	}

	//bottom spacer
	std::cout << "*******************";
	for ( int i = 0; i < (snlen + namelen + catlen); i++ )
	{
		std::cout << "*";
	}
	std::cout << "*******************" << std::endl;


}
//gets a total price of objects in list
double Handler::getTotalPrice(std::vector<Product> list)
{
	double total = 0;
	for ( auto i : list )
	{
		total += i.getCategoryTax( ) * i.getPrice( ); //multiplies i's price by the applicable tax and adds to total
	}
	return total;
}