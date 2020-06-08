#include "Integer.h"
#define MAX 1000

Integer::Integer() : Number()
{

}

Integer::~Integer()
{
	num.clear();
	num.shrink_to_fit();
	denom.clear();
	denom.shrink_to_fit();
}

Integer::Integer(string newNum)
{
	dot1 = 0;
	isDecimal = 0;
	denom.push_back(1);
	strnum = newNum;
	if (newNum.find('.', 0) != string::npos)
	{
		while (newNum.find('.') != newNum.size()) 
		{
			newNum.pop_back();
		}
	}
	for (int i = newNum.size() - 1; i > 0; i--)
	{
		num.push_back((int)(newNum[i]) - 48);
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

Integer::Integer(const char *a)
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
			isDecimal = 0;
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
			isDecimal = 0;
		}
	}
	else
	{
		num.push_back(0);
		sign = 1;
		dot1 = 0;
		isDecimal = 0;
		strnum = "0";
		denom.push_back(0);
	}
}

Integer::Integer(string strNum, string varName, vector<int> newNum, vector<int> newDenom, int isDecimal, int sign)
{
	dot1 = 0;
	this->isDecimal = 0;
	this->sign = sign;
	this->strnum = strnum;
	this->varName = varName;
	num.assign(newNum.begin(), newNum.end());
	denom.assign(newDenom.begin(), newDenom.end());
}

Integer::Integer(string newNum, vector<int>& newDemon, int newSign, int decimal)
{
	dot1 = 0;
	isDecimal = 0;
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

Integer::Integer(const Integer & Integer) :Number(Integer)
{

}

Integer & Integer::operator=(const Integer & num1)
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

ostream & operator<<(ostream & out, const Integer & number)
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
	out << temp;
	return out;
}
