#ifndef _INTEGER_H_
#define _INTEGER_H_
#include "BigNumber.h"

class Integer : virtual public Number
{
public:
	Integer();
	~Integer();
	Integer(string newNum);
	Integer(const char*);
	Integer(string strNum, string varName, vector<int> newNum, vector<int> newDenom, int isDecimal, int sign);
	Integer(string newNum, vector<int>& newDemon, int newSign, int decimal);
	Integer(const Integer& Integer);
	Integer& operator=(const Integer& num1);
	friend ostream& operator<<(ostream& out, const Integer& number);
};

#endif