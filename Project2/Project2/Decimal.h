#ifndef _DECIMAL_H_
#define _DECIMAL_H_
#include "BigNumber.h"

class Decimal : virtual public Number
{
public:
	Decimal();
	~Decimal();
	Decimal(string newNum);
	Decimal(const char*);
	Decimal(string strNum, string varName, vector<int> newNum, vector<int> newDenom, int isDecimal, int sign);
	Decimal(string newNum, vector<int>& newDemon, int newSign, int decimal);
	Decimal(const Decimal& Integer);
	Decimal& operator=(const Decimal& num1);
	friend ostream& operator<<(ostream& out, const Decimal& number);
};

#endif