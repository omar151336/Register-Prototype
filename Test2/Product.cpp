#include "Product.h"



Product::Product(u_int k, std::string sn, std::string n, std::string cat, std::string prov, double c, double p)
{
	key = k;
	serialNumber = sn;
	name = n;
	category = cat;
	provider = prov;
	cost = c;
	price = p;
}
Product::Product(std::string sn, std::string n, std::string cat, double c, double p)
{
	serialNumber = sn;
	name = n;
	category = cat;
	cost = c;
	price = p;
}
Product::Product(std::string n, std::string cat, double c, double p)
{
	serialNumber = "n/a";
	name = n;
	category = cat;
	cost = c;
	price = p;
}
Product::Product( )
{
	serialNumber = "n/a";
	error = true;
}
Product::Product(bool err )
{
	error = err;
}

bool Product::print( )
{
	std::cout << serialNumber << " " << category << " " << name << " " << provider << " " << cost << " " << price << " " << std::endl;
	return true;
}

std::string Product::getSerialNumber( )
{
	return Product::serialNumber;
}
std::string Product::getName( )
{
	return Product::name;
}
std::string Product::getCategory( )
{
	return Product::category;
}
std::string Product::getProvider( )
{
	return Product::provider;
}
double Product::getCost( )
{
	return Product::cost;
}
double Product::getPrice( )
{
	return Product::price;
}

//helper for hash() function
uint64_t constexpr mix(char m, uint64_t s)
{
	return ((s << 7) + ~(s >> 3)) + ~m;
}
//used to turn a string into a hash
//for getCategoryTax() switch case
uint64_t constexpr hash(const char* m)
{
	return (*m) ? mix(*m, hash(m + 1)) : 0;
}
//gets hash of product's category and compares it to list
//returns tax of category
double Product::getCategoryTax( )
{
	switch ( hash(category.c_str( )) )
	{
		case hash("Grocery"):
			return 1.02;
			break;
		case hash("Frozen Food"):
			return 1.02;
			break;
		case hash("Household Supply"):
			return 1.1;
			break;
		case hash("Liquor"):
			return 1.1;
			break;
		case hash("Beer"):
			return 1.1;
			break;
		case hash("Wine"):
			return 1.1;
			break;
		case hash("Cigar"):
			return 1;
			break;
		case hash("Cigarette"):
			return 1;
			break;
		case hash("NO TAX"):
			return 1;
			break;
	}
}
//gets hash of passed string and compares it to list
//returns tax of category
double Product::getCategoryTax( std::string cat)
{
	switch ( hash(cat.c_str( )) )
	{
		case hash("Grocery"):
			return 1.02;
			break;
		case hash("Frozen Food"):
			return 1.02;
			break;
		case hash("Household Supply"):
			return 1.1;
			break;
		case hash("Liquor"):
			return 1.1;
			break;
		case hash("Beer"):
			return 1.1;
			break;
		case hash("Wine"):
			return 1.1;
			break;
		case hash("Cigar"):
			return 1;
			break;
		case hash("Cigarette"):
			return 1;
			break;
		case hash("NO TAX"):
			return 1;
			break;
	}
}


bool Product::setSerialNumber(std::string sn)
{
	Product::serialNumber = sn;
	return true;
}
bool Product::setName(std::string n)
{
	Product::name = n;
	return true;

}
bool Product::setCategory(std::string c)
{
	category = c;
	return true;
}
bool Product::setCost(double c)
{
	Product::cost = c;
	return true;
}
bool Product::setPrice(double p)
{
	Product::price = p;
	return true;
}
bool Product::isError( )
{
	return error;
}
