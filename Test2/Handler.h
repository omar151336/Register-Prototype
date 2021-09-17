#pragma once

#include "SQLProduct.h"
#include "SQLEmployee.h"
#include <fstream>
#include <ctime>
#include <filesystem>
#include <boost/json.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
namespace json = boost::json;

//used for transaction functions
enum moneyType {cash, card, fs};

class Handler
{
public:
	Handler( );

	//starts the handler
	int start( );

	//used by start function
	Employee login( ); //logs in using login SQL user then switches to higher privilege connection based on employee privilege
	bool logout( ); //logs user out and empties employee variable
	//main menu loop
	//return 0 on logout
	//returns 1 on exit
	int mainMenu( );

	//transaction classes
	bool startTransaction( ); // starts transaction
	std::vector<Product> getTransactionProducts( ); //creates list of products to be sold
	void transactionPayment(std::vector<Product> list, double total); //handles payments for products
	bool createReceipt(std::vector<Product> list, double pMethods[], double total, double change); //creates receipt to put in .json file
	void transactionProductPrint(std::vector<Product> list); //prints a list of products in list
	double getTotalPrice(std::vector<Product> list); //gets a total price of objects in list

	//employee print classes
	//pass string to search for employees that contain that string in their name
	bool printEmployeeList( bool verbose );
	bool printEmployeeList( bool verbose, std::string name);



	//Employee update classes handler
	bool chooseItemToUpdate(Employee selected);
	//adds employee to Employee SQL table
	bool addEmployee( );

	//product classes
	//if error return product where product.isError() == true
	Product getProduct(unsigned long sn); //gets product from Sn
	Product getProduct(std::string name); //gets product from name
	//adds product to product SQL table
	bool addProduct( );
	//searches for product by providers
	bool searchProduct(std::string provider);

	//resets monthly data and add information to a .json file
	bool resetMonth( );




protected:


	//employee update classes
	bool employeeUpdateALL( ); //updates all employee info in SQL table to most recent information
	bool employeeUpdatePrivilege(Employee selected); //updates selected employee's privilege

	Employee emp; //current logged in employee

	//used for updating SQL tables
	SQLEmployee sEmployee;
	SQLProduct sProduct;

	//username and password for logging into SQL
	std::string user;
	std::string pw;

};

