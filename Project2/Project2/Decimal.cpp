#include "Decimal.h"
#define MAX 1000

Decimal::Decimal()
{
	num.push_back(0);
	sign = 1;
	dot1 = 0;
	isDecimal = 1;
	strnum = "0";
	denom.push_back(0);
}

Decimal::~Decimal()
{
	num.clear();
	num.shrink_to_fit();
	denom.clear();
	denom.shrink_to_fit();
}

Decimal::Decimal(string newNum)
{
	dot1 = 0;
	isDecimal = 1;
	denom.push_back(1);
	strnum = newNum;
	if (newNum.find('.', 0) != string::npos)
	{
		isDecimal = 1;
		int s = newNum.size() - 1;
		if (newNum[s] == '+')
		{
			sign = 1;
			s--;
		}
		else if (newNum[s] == '-')
		{
			sign = -1;
			s--;
		}
		else
		{
			sign = 1;
		}
		while (newNum[s--] == '0')
		{
			newNum.pop_back();
		}
		s = newNum.size() - 1;
		while (newNum[s--] != '.')
		{
			dot1++;
			denom.insert(denom.begin(), 0);
		}
	}
	for (int i = newNum.size() - 1; i > 0; i--)
	{
		if (newNum[i] != '.')
		{
			num.push_back((int)(newNum[i]) - 48);
		}
	}
	if (newNum[0] == '+')
	{
		sign = 1;

	}
	else if (newNum[0] == '-')
	{
		sign = -1;
	}
	else
	{
		sign = 1;
		num.push_back((int)(newNum[0]) - 48);
	}
	while (num[num.size() - 1] == 0 && num.size() > 1)
	{
		num.pop_back();
	}
}

Decimal::Decimal(const char *a)
{
	string str = a;
	if (str != "")
	{
		char infix[MAX] = { '\0' };
		char postfix[MAX] = { '\0' };
		int count = 0;
		for (int i = 0, j = 0; i < str.size(); i++)
		{
			if (str[i] == '(')
			{
				count++;
			}
			else if (str[i] == ')')
			{
				if (count < 1)
				{
					count = -1;
					break;
				}
				count--;
			}
			if (str[i] != ' ')
			{
				infix[j++] = str[i];
			}
		}
		int error = 0;
		for (int i = 0; infix[i] != '\0'; i++)
		{
			if ((infix[i] >= 65 && infix[i] <= 90) || (infix[i] >= 97 && infix[i] <= 122))
			{
				error = 1;
				break;
			}
		}
		if (error == 1 || count != 0)
		{
			num.push_back(0);
			sign = 1;
			dot1 = 0;
			isDecimal = 1;
			strnum = "0";
			denom.push_back(0);
		}
		else
		{
			inToPostfix(infix, postfix);
			vector <string> cal;
			string temp(postfix);
			string c = " ";
			splitString(temp, cal, c);
			calBigNum(cal, *this);
			isDecimal = 1;
		}
	}
	else
	{
		num.push_back(0);
		sign = 1;
		dot1 = 0;
		isDecimal = 1;
		strnum = "0";
		denom.push_back(0);
	}
}

Decimal::Decimal(string strNum, string varName, vector<int> newNum, vector<int> newDenom, int isDecimal, int sign)
{
	dot1 = 0;
	this->isDecimal = 1;
	this->sign = sign;
	this->strnum = strnum;
	this->varName = varName;
	num.assign(newNum.begin(), newNum.end());
	denom.assign(newDenom.begin(), newDenom.end());
}

Decimal::Decimal(string newNum, vector<int>& newDemon, int newSign, int decimal)
{
	dot1 = 0;
	isDecimal = 1;
	sign = newSign;
	strnum = newNum;
	denom.push_back(1);
	if (newNum.find('.', 0) != string::npos)
	{
		isDecimal = 1;
		int s = newNum.size() - 1;
		while (newNum[s--] == '0')
		{
			newNum.pop_back();
		}
		s = newNum.size() - 1;
		while (newNum[s--] != '.')
		{
			dot1++;
			denom.insert(denom.begin(), 0);
		}
	}
	mul(denom, newDemon);
	for (int i = newNum.size() - 1; i >= 0; i--)
	{
		if (newNum[i] != '.')
		{
			num.push_back((int)(newNum[i]) - 48);
		}
	}
}

Decimal::Decimal(const Decimal & number) :Number(number)
{
	num.assign(number.num.begin(), number.num.end());
	sign = number.sign;
	dot1 = number.dot1;
	strnum = number.strnum;
	varName = number.varName;
	isDecimal = 1;
	denom.assign(number.denom.begin(), number.denom.end());
}

Decimal & Decimal::operator=(const Decimal & num1)
{
	vector<int>().swap(num);
	num.assign(num1.num.begin(), num1.num.end());
	vector<int>().swap(denom);
	denom.assign(num1.denom.begin(), num1.denom.end());
	sign = num1.sign;
	dot1 = num1.dot1;
	strnum = num1.strnum;
	isDecimal = num1.isDecimal;
	return *this;
}

ostream & operator<<(ostream & out, const Decimal & number)
{
	int sign = number.sign;
	Number num1, num2;
	num1 = number;
	num2 = number;
	vector<int>().swap(num1.denom);
	num2.denom.swap(num2.num);
	vector<int>().swap(num2.denom);
	num1.denom.push_back(1);
	num2.denom.push_back(1);
	num1 = num1 / num2;
	num1.sign = sign;
	if (num1.sign == -1)
	{
		if (!(num1.num[0] == 0 && num1.num.size() == 1))
		{
			out << '-';
		}
	}
	string temp = "";
	for (int i = num1.num.size() - 1; i >= 0; i--)
	{
		temp += (char)num1.num[i] + 48;
	}
	if (num1.isDecimal)
	{
		temp.insert(temp.end() - num1.dot1, '.');
		while ((temp.size() - temp.find(".")) < 101)
		{
			temp += "0";
		}
	}
	out << temp;
	return out;
}
