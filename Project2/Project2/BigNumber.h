#ifndef _BIGNUMBER_H_
#define _BIGNUMBER_H_
#include <iostream>
#include <string>
#include <vector>
using namespace std;



class Number
{
public:
	Number();
	~Number();
	Number(string newNum);
	Number(const char*);
	Number(string strNum, string varName, vector<int> newNum, vector<int> newDenom, int isDecimal, int sign);
	Number(string newNum, vector<int>& newDemon, int newSign, int decimal);
	Number(const Number& number);
	vector<int> num, denom;
	int sign, dot1, isDecimal;
	string strnum, varName;
	friend Number operator+(const Number& add1, const Number& add2);
	friend Number operator-(const Number& sub1, const Number& sub2);
	friend Number operator/(const Number& num1, const Number& num2);
	friend Number operator*(const Number& num1, const Number& num2);
	friend Number operator^(const Number& num1, const Number& num2);
	Number& operator!();
	Number& operator=(const Number& num1);
	friend ostream& operator<<(ostream& out, const Number& number);
};

void mul(vector<int>&, vector<int> c);
string div(vector<int>& num1, vector<int>& num2, int dot);
void sub(string&, vector<int>&);
bool compare(vector<int>&, string);
Number root(vector<int>& num);
void inToPostfix(char*, char*); // 中序轉後序 
int priority(char); // 運算子優先權
void splitString(string&, vector<string>&, string&);
void calBigNum(vector<string>& cal, Number& number);
#endif