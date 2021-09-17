#include "SQLProduct.h"

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


//adds product to Product table
//return 0 = good
//return 1 = no connection
//return 2 = connection failed
//return 3 = SN in use
//return 4 = name in use
int SQLProduct::addProduct(std::string serialNumber, int category, std::string name, std::string provider, double cost, double price)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}
	std::string cat;
	//sets category according to category number given
	switch ( category )
	{
		case 1:
			cat = "Grocery";
			break;
		case 2:
			cat = "Frozen Food";
			break;
		case 3:
			cat = "Household Supply";
			break;
		case 4:
			cat = "Liquor";
			break;
		case 5:
			cat = "Beer";
			break;
		case 6:
			cat = "Wine";
			break;
		case 7:
			cat = "Cigar";
			break;
		case 8:
			cat = "Cigarette";
			break;
		case -1:
			cat = "NO TAX";
			break;
	}


	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//checks if serial number is already in database, exits if true
	std::string query = "SELECT * FROM Products WHERE SerialNumber = " + serialNumber + ";";
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{

		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'SERIAL NUMBER'" << std::endl;
			return 2;
		}
	}

	//store result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'SERIAL NUMBER'" << std::endl;
			return 2;
		}
	}

	//check result
	if ( result != NULL )
	{
		if ( result->row_count != 0 )
		{
			std::cerr << "Serial Number already in system" << std::endl;
			return 3;
		}
	}


	//checks if name is already in database, exits if true
	query = "SELECT * FROM Products WHERE Name = '" + name + "';";
	//execute query
	status = NET_ASYNC_NOT_READY;
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'NAME CHECK'" << std::endl;
			return 2;
		}
	}
	//store result
	status = NET_ASYNC_NOT_READY;
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		;
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'NAME CHECK'" << std::endl;;
			return 2;
		}
	}
	if ( result != NULL )
	{
		if ( result->row_count != 0 )
		{
			std::cerr << "Name already in system" << std::endl;
			return 4;
		}
	}



	//inserts product into table
	query = "INSERT INTO Products (SerialNumber, Category, Name, Provider, Cost, Price) VALUES ('" + serialNumber + "','" + cat + "','" + name + "','" + provider + "'," + std::to_string(cost) + "," + std::to_string(price) + ");";

	status = NET_ASYNC_NOT_READY;
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{

	}
	if ( status == NET_ASYNC_ERROR )
	{
		std::cerr << "Error: execute query 'INSERT INTO Products'" << std::endl;
		return 2;
	}


	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return 0;
}
//Put a function first if action needs to be done during call
//Not used in this program, but is an option
//and other functions can be edited to do the same
int SQLProduct::addProduct(std::function<void( )> func, std::string serialNumber, int category, std::string name, std::string provider, double cost, double price)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}
	std::string cat;
	//sets category according to category number given
	switch ( category )
	{
		case 1:
			cat = "Grocery";
			break;
		case 2:
			cat = "Frozen Food";
			break;
		case 3:
			cat = "Household Supply";
			break;
		case 4:
			cat = "Liquor";
			break;
		case 5:
			cat = "Beer";
			break;
		case 6:
			cat = "Wine";
			break;
		case 7:
			cat = "Cigar";
			break;
		case 8:
			cat = "Cigarette";
			break;
		case -1:
			cat = "NO TAX";
			break;
	}


	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//checks if serial number is already in database, exits if true
	std::string query = "SELECT * FROM Products WHERE SerialNumber = " + serialNumber + ";";
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		//function to execute between calls
		func();
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'SERIAL NUMBER'" << std::endl;
			return 2;
		}
	}

	//store result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		func( );
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'SERIAL NUMBER'" << std::endl;
			return 2;
		}
	}

	//check result
	if ( result != NULL )
	{
		if ( result->row_count != 0 )
		{
			std::cerr << "Serial Number already in system" << std::endl;
			return 3;
		}
	}


	//checks if name is already in database, exits if true
	query = "SELECT * FROM Products WHERE Name = '" + name + "';";
	//execute query
	status = NET_ASYNC_NOT_READY;
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		func( );
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query execute 'NAME CHECK'" << std::endl;
			return 2;
		}
	}
	//store result
	status = NET_ASYNC_NOT_READY;
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		func( );
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: store result 'NAME CHECK'" << std::endl;;
			return 2;
		}
	}
	if ( result != NULL )
	{
		if ( result->row_count != 0 )
		{
			std::cerr << "Name already in system" << std::endl;
			return 4;
		}
	}



	//inserts product into table
	query = "INSERT INTO Products (SerialNumber, Category, Name, Provider, Cost, Price) VALUES ('" + serialNumber + "','" + cat + "','" + name + "','" + provider + "'," + std::to_string(cost) + "," + std::to_string(price) + ");";

	status = NET_ASYNC_NOT_READY;
	//execute query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		func( );
	}
	if ( status == NET_ASYNC_ERROR )
	{
		std::cerr << "Error: execute query 'INSERT INTO Products'" << std::endl;
		return 2;
	}


	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{
		func( );
	}
	return 0;
}

//removes product using its serial number
//return 0 = good
//return 1 = no connection
//return 2 = async error
int SQLProduct::removeProduct(std::string serialNumber)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//deletes object with matching serial number
	std::string query = "DELETE FROM Products WHERE SerialNumber = " + serialNumber + "; ";

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


//if there's an error return product object with Product.isError() == true
Product SQLProduct::getProductBySN(std::string serialNumber)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return Product(true);
	}

	//gets product with matching serial number
	std::string query = "SELECT * FROM Products WHERE SerialNumber = '" + serialNumber + "';";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			return Product(true);
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			return Product(true);
		}

	}

	//checks if no or too many results were returned
	//returns product with error flag if true
	int numrows = mysql_num_rows(result);
	if ( numrows > 1 || numrows == 0 )
	{
		if ( numrows > 1 )
			std::cerr << "Too many results for Serial Number" << std::endl;
		if ( numrows == 0 )
			std::cerr << "No Matching Serial Number" << std::endl;
		return Product(true);
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

	//stores row data
	u_int key = std::atoi(row[0]);
	std::string sn = row[1];
	std::string cat = row[2];
	std::string name = row[3];
	std::string prov = row[4];
	double cost = std::atof(row[5]);
	double price = std::atof(row[6]);
	double profit = std::atof(row[7]);
	int sold = std::atoi(row[8]);

	//adds row data to product object
	Product p(key, sn, name, cat, prov, cost, price);

	//frees result
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return p; //returns product
}

//returns empty vector if error or no products
std::vector<Product> SQLProduct::getProductsByName(std::string name)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection Established" << std::endl;
		std::vector<Product> p;
		return p;
	}

	//gets products whose Name contains string name
	std::string query = "SELECT * FROM Products WHERE Name LIKE '%" + name + "%';";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			std::vector<Product> p;
			return p;
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			std::vector<Product> p;
			return p;
		}

	}
	//checks if no result given
	//returns empty list if true
	int numrows = mysql_num_rows(result);
	if ( numrows == 0 )
	{
		std::cerr << "no results found" << std::endl;
		std::vector<Product> p;
		return p;
	}
	int num_col = mysql_num_fields(result);


	//creates list of products
	std::vector<Product> list;

	//goes through all rows returned
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY ) // fetch each row one by one from the result set
	{
		if ( status == NET_ASYNC_COMPLETE)
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				//stores product data
				u_int key = std::atoi(row[0]);
				std::string sn = row[1];
				std::string cat = row[2];
				std::string name = row[3];
				std::string prov = row[4];
				double cost = std::atof(row[5]);
				double price = std::atof(row[6]);
				double profit = std::atof(row[7]);
				int sold = std::atoi(row[8]);

				//insets data into product object and add object to list
				Product p(key, sn, name, cat, prov, cost, price);
				list.push_back(p);
			}
		}
		//if error return empty list
		else if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: Fetch row error" << std::endl;
			std::vector<Product> p;
			return p;

		}

	}

	//frees result and returns list
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return list;
}

//returns empty vector if error or no products
std::vector<Product> SQLProduct::getProductsByProvider(std::string provider)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection Established" << std::endl;
		std::vector<Product> p;
		return p;
	}

	//gets products whose Name contains string name
	std::string query = "SELECT * FROM Products WHERE Provider LIKE '%" + provider + "%';";

	MYSQL_RES* result;
	MYSQL_ROW row;
	net_async_status status = NET_ASYNC_NOT_READY;

	//executes query
	while ( (status = mysql_real_query_nonblocking(connection, query.c_str( ), query.size( ))) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: query error" << std::endl;
			std::vector<Product> p;
			return p;
		}

	}

	//stores query result
	while ( (status = mysql_store_result_nonblocking(connection, &result)) == NET_ASYNC_NOT_READY )
	{
		if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: result store error" << std::endl;
			std::vector<Product> p;
			return p;
		}

	}
	//checks if no result given
	//returns empty list if true
	int numrows = mysql_num_rows(result);
	if ( numrows == 0 )
	{
		std::cerr << "no results found" << std::endl;
		std::vector<Product> p;
		return p;
	}
	int num_col = mysql_num_fields(result);


	//creates list of products
	std::vector<Product> list;

	//goes through all rows returned
	while ( ((status = mysql_fetch_row_nonblocking(result, &row)) == NET_ASYNC_COMPLETE) || status == NET_ASYNC_NOT_READY ) // fetch each row one by one from the result set
	{
		if ( status == NET_ASYNC_COMPLETE )
		{
			if ( row == NULL )
			{
				break;
			}
			else
			{
				//stores product data
				u_int key = std::atoi(row[0]);
				std::string sn = row[1];
				std::string cat = row[2];
				std::string name = row[3];
				std::string prov = row[4];
				double cost = std::atof(row[5]);
				double price = std::atof(row[6]);
				double profit = std::atof(row[7]);
				int sold = std::atoi(row[8]);

				//insets data into product object and add object to list
				Product p(key, sn, name, cat, prov, cost, price);
				list.push_back(p);
			}
		}
		//if error return empty list
		else if ( status == NET_ASYNC_ERROR )
		{
			std::cerr << "Error: Fetch row error" << std::endl;
			std::vector<Product> p;
			return p;

		}

	}

	//frees result and returns list
	while ( mysql_free_result_nonblocking(result) != NET_ASYNC_COMPLETE )
	{

	}
	return list;
}

//return 0 = good
//return 1 = no connection
//return 2 = async error
int SQLProduct::changeSerialByName(std::string name, std::string sn)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	//updates product's serial number using name
	std::string query = "UPDATE Products SET SerialNumber = " + sn + " WHERE Name = '" + name + "';";

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

	if ( status == NET_ASYNC_COMPLETE)
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


//Synchronous versions

//return 0 = good
//return 1 = no connection
//return 2 = Add failed
//return 3 = SN in use
//return 4 = name in use
int SQLProduct::addProductS(std::string serialNumber, int category, std::string name, std::string provider, double cost, double price)
{
	//checks if valid connection exists
	if (!connection)
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}
	std::string cat;

	switch (category) {
	case 1:
		cat = "Grocery";
		break;
	case 2:
		cat = "Frozen Food";
		break;
	case 3:
		cat = "Household Supply";
		break;
	case 4:
		cat = "Liquor";
		break;
	case 5:
		cat = "Beer";
		break;
	case 6:
		cat = "Wine";
		break;
	case 7:
		cat = "Cigar";
		break;
	case 8:
		cat = "Cigarette";
		break;
	}


	//checks if serial number is already in database, exits if true
	std::string query = "SELECT * FROM Products WHERE SerialNumber = '" + serialNumber + "';";
	MYSQL_RES* result;
	int j = mysql_real_query(connection, query.c_str(), query.size());
	result = mysql_store_result(connection);
	if (mysql_num_rows(result) != 0)
	{
		std::cerr << "Serial Number already in system" << std::endl;
		return 3;
	}
	//checks if name is already in database, exits if true
	query = "SELECT * FROM Products WHERE Name = '" + name + "';";
	result;
	j = mysql_real_query(connection, query.c_str(), query.size());
	result = mysql_store_result(connection);
	if (mysql_num_rows(result) != 0)
	{
		std::cerr << "Name already in system" << std::endl;
		return 4;
	}


	//adds product
	query = "INSERT INTO Products (SerialNumber, Category, Name, Provider, Cost, Price) VALUES ('" + serialNumber + "','" + cat + "','" + name + "','" + provider + "'," + std::to_string(cost) + "," + std::to_string(price) + ");";


	int i = mysql_real_query(connection, query.c_str(), query.size());
	if (i == 0)
	{
		return 0;
	}
	else
	{
		std::cerr << "not added" << std::endl;
		return 2;
	}

}

//if there's an error return product object with Product.isError() == true
Product SQLProduct::getProductBySNS(std::string serialNumber)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return Product(true);
	}

	//gets product by serial number
	std::string query = "SELECT * FROM Products WHERE SerialNumber = '" + serialNumber + "';";

	MYSQL_RES* result;
	MYSQL_ROW row;

	int j = mysql_real_query(connection, query.c_str(), query.size()); //execute
	result = mysql_store_result(connection);	//store
	//checks if number of rows is 0 or more than 1
	//returns product with error flag if true
	int numrows = mysql_num_rows(result);
	if (numrows > 1 || numrows == 0)
	{
		std::cerr << "too many or no matching serial number" << std::endl;
		Product p;
		return p;
	}
	int num_col = mysql_num_fields(result);
	//store row data
	row = mysql_fetch_row(result);

	//stores row data to be used
	u_int key = std::atoi(row[0]);
	std::string sn = row[1];
	std::string cat = row[2];
	std::string name = row[3];
	std::string prov = row[4];
	double cost = std::atof(row[5]);
	double price = std::atof(row[6]);
	double profit = std::atof(row[7]);
	int sold = std::atoi(row[8]);


	//adds data into product object
	Product p(key, sn, name, cat, prov, cost, price);

	return p; //returns product object
}

//returns empty vector if error or no products
std::vector<Product> SQLProduct::getProductsByNameS(std::string name)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		std::vector<Product> p;
		return p;
	}

	std::string query = "SELECT * FROM Products WHERE Name LIKE '%" + name + "%';";

	MYSQL_RES* result;
	MYSQL_ROW row;

	int j = mysql_real_query(connection, query.c_str(), query.size());
	result = mysql_store_result(connection);
	int numrows = mysql_num_rows(result);
	if (numrows == 0)
	{
		std::cerr << "no results found" << std::endl;
		std::vector<Product> p;
		return p;
	}
	int num_col = mysql_num_fields(result);

	std::vector<Product> list;
	while (((row = mysql_fetch_row(result)) != NULL)) // fetch each row one by one from the result set
	{
		u_int key = std::atoi(row[0]);
		std::string sn = row[1];
		std::string cat = row[2];
		std::string name = row[3];
		std::string prov = row[4];
		double cost = std::atof(row[5]);
		double price = std::atof(row[6]);
		double profit = std::atof(row[7]);
		int sold = std::atoi(row[8]);


		Product p(key, sn, name, cat, prov, cost, price);
		list.push_back(p);

	}


	return list;
}

//returns empty vector if error or no products
std::vector<Product> SQLProduct::getProductsByProviderS(std::string provider)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		std::vector<Product> p;
		return p;
	}

	std::string query = "SELECT * FROM Products WHERE Provider LIKE '%" + provider + "%';";

	MYSQL_RES* result;
	MYSQL_ROW row;

	int j = mysql_real_query(connection, query.c_str(), query.size());
	result = mysql_store_result(connection);
	int numrows = mysql_num_rows(result);
	if (numrows == 0)
	{
		std::cerr << "no results found" << std::endl;
		std::vector<Product> p;
		return p;
	}
	int num_col = mysql_num_fields(result);

	std::vector<Product> list;
	while (((row = mysql_fetch_row(result)) != NULL)) // fetch each row one by one from the result set
	{
		u_int key = std::atoi(row[0]);
		std::string sn = row[1];
		std::string cat = row[2];
		std::string name = row[3];
		std::string prov = row[4];
		double cost = std::atof(row[5]);
		double price = std::atof(row[6]);
		double profit = std::atof(row[7]);
		int sold = std::atoi(row[8]);


		Product p(key, sn, name, cat, prov, cost, price);
		list.push_back(p);

	}


	return list;
}

//return 0 = good
//return 1 = no connection
//return 2 = Add failed
int SQLProduct::changeSerialByNameS(std::string name, std::string sn)
{
	//checks if valid connection exists
	if ( !connection )
	{
		std::cerr << "Error: No connection established" << std::endl;
		return 1;
	}

	std::string query = "UPDATE Products SET SerialNumber = " + sn + " WHERE Name = '" + name + "';";

	MYSQL_RES* result;
	MYSQL_ROW row;

	int i = mysql_real_query(connection, query.c_str(), query.size());
	if (i == 0)
	{
		return 0;
	}
	else
	{
		std::cerr << "not updated" << std::endl;
		return 2;
	}

}

