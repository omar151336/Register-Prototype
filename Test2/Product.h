#pragma once
#include <string>
#include <iostream>
#include <mysql.h>

/* Product Category List:
* Grocery			1
* Frozen Food		2
* Household Supply	3
* Liquor			4
* Beer				5
* Wine				6
* Cigar				7
* Cigarette			8
* NO TAX		   -1
*/


class Product
{
public:
	//Constructors
	Product(std::string sn, std::string n, std::string cat, double c, double p);
	Product(u_int k, std::string sn, std::string n, std::string cat, std::string prov, double c, double p);
	Product(std::string n, std::string cat, double c, double p);
	//used when Product gives an error, sets error flag to true
	Product( );
	Product(bool err );

	//prints Product's information
	bool print();

	//getters
	std::string getSerialNumber();
	std::string getName();
	std::string getCategory( );
	std::string getProvider( );
	double getCost();
	double getPrice( );
	double getCategoryTax( );
	double getCategoryTax(std::string cat );
	bool isError( );

	//setters
	bool setSerialNumber(std::string sn);
	bool setName(std::string n);
	bool setCategory(std::string c);
	bool setCost(double c);
	bool setPrice(double p);



private:
	std::string serialNumber;
	std::string name, category, provider;
	double cost, price;

	u_int key; // in case the product key is needed
	bool error = false; //errors are false by default
};

