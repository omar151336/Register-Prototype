#pragma once

#include "Product.h"
#include "SQLBase.h"
#include <functional>

/*
*  SQL Products Table Columns
*   9 Columns:
*      InternalKey
*      SerialNumber
*      Category
*      Name
*      Provider
*      Cost
*      Price
*      Profit
*      QuantitySold
*/


class SQLProduct : public SQLBase
{
public:
	//use same constructor as SQLBase
	using SQLBase::SQLBase;

	//adds product to Product table
	//return 0 = good
	//return 1 = no connection
	//return 2 = connection failed
	//return 3 = SN in use
	//return 4 = name in use
	int addProduct(std::string serialNumber, int category, std::string name, std::string provider, double cost, double price);
	//Put a function first if action needs to be done during call
	//Not used in this program, but is an option
	//and other functions can be edited to do the same
	int addProduct(std::function<void()> func, std::string serialNumber, int category, std::string name, std::string provider, double cost, double price);

	//removes product using its serial number
	//return 0 = good
	//return 1 = no connection
	//return 2 = async error
	int removeProduct(std::string serialNumber);


	//if there's an error return product object with Product.isError() == true
	Product getProductBySN(std::string serialNumber);

	//returns empty vector if error or no products
	std::vector<Product> getProductsByName(std::string name);

	//returns empty vector if error or no products
	std::vector<Product> getProductsByProvider(std::string provider);

	//return 0 = good
	//return 1 = no connection
	//return 2 = async error
	int changeSerialByName(std::string name, std::string sn);

	//TODO if needed
	//int changeCategoryByName(std::string name, int cat);
	//int changeCategoryBySN(std::string sn, int cat);

	//TODO if needed
	//std::string findProvidorFromSN();
	//std::string findProvidorFromName();




	/*Synchronous versions
	* faster than standard functions
	* not used in this program,
	* but can be if speed becomes an issue
	*/

	//return 0 = good
	//return 1 = no connection
	//return 2 = Add failed
	//return 3 = SN in use
	//return 4 = name in use
	int addProductS(std::string serialNumber, int category, std::string name, std::string provider, double cost, double price);

	//if there's an error return product object with Product.isError() == true
	Product getProductBySNS(std::string serialNumber);

	//returns empty vector if error or no products
	std::vector<Product> getProductsByNameS(std::string name);

	//returns empty vector if error or no products
	std::vector<Product> getProductsByProviderS(std::string provider);

	//return 0 = good
	//return 1 = no connection
	//return 2 = Add failed
	int changeSerialByNameS(std::string name, std::string sn);

	//TODO if needed
	//int changeCategoryByNameS(std::string name, int cat);
	//int changeCategoryBySNS(std::string sn, int cat);

	//std::string findProvidorFromSNS();
	//std::string findProvidorFromNameS();


private:

};

