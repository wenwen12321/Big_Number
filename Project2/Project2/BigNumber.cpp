#include "BigNumber.h"
#include <string>
#include <iostream>
using namespace std;
#define MAX 5000
Number::Number()
{
	num.push_back(0);
	sign = 1;
	dot1 = 0;
	isDecimal = 0;
	strnum = "0";
	denom.push_back(0);
}

Number::~Number()
{
	num.clear();
	num.shrink_to_fit();
	denom.clear();
	denom.shrink_to_fit();
}

Number::Number(string newNum)
{
	dot1 = 0;
	isDecimal = 0;
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

Number::Number(const char *a)
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

Number::Number(string strnum, string varName, vector<int> newNum, vector<int> newDenom, int isDecimal, int sign)
{
	dot1 = 0;
	this->isDecimal = isDecimal;
	this->sign = sign;
	this->strnum = strnum;
	this->varName = varName;
	num.assign(newNum.begin(), newNum.end());
	denom.assign(newDenom.begin(), newDenom.end());
}

Number::Number(string newNum, vector<int>& newDemon, int newSign, int decimal)
{
	dot1 = 0;
	isDecimal = decimal;
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

Number::Number(const Number & number)
{
	num.assign(number.num.begin(), number.num.end());
	sign = number.sign;
	dot1 = number.dot1;
	strnum = number.strnum;
	varName = number.varName;
	isDecimal = number.isDecimal;
	denom.assign(number.denom.begin(), number.denom.end());
}

Number &Number::operator !()
{
	string one = "1";
	if (this->num.size() == 1 && this->num[0] == 0)
	{
		*this = Number(one);
		return *this;
	}
	Number result = *this;
	Number mark = *this, use = Number(one);
	mark = mark - use;
	int set = 0;
	for (;; mark = mark - use)
	{
		set = 0;
		for (int i = 0; i<mark.num.size(); i++)
		{
			if (mark.num[i] == 0) { set++; }
		}
		if (set == mark.num.size())
		{
			break;
		}
		else
		{
			*this = *this*mark;
		}
	}
	return *this;
}

Number & Number::operator=(const Number & num1)
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


void mul(vector<int>& num, vector<int> c)
{
	vector <int> temp;
	temp.assign(num.begin(), num.end());
	num.clear();
	for (int i = 0; i < c.size() + temp.size(); i++)
	{
		num.push_back(0);
	}
	for (int i = 0; i < temp.size(); i++)
	{
		for (int j = 0; j < c.size(); j++)
		{
			num[i + j] += temp[i] * c[j];
		}
	}
	int len = num.size();
	int count = 0;
	for (int i = 0; i < num.size(); i++)
	{
		if (i == (len - 1) && num[i] > 9)
		{
			num.push_back(0);
		}
		if (num[i] > 9)
		{
			num[i + 1] += num[i] / 10;
			num[i] %= 10;
		}
	}
	while (num.size() > 1 && num[num.size() - 1] == 0)
	{
		num.pop_back();
	}
}

void add(vector<int>& num, vector<int> c)
{
	vector<int> temp;
	temp.assign(num.begin(), num.end());
	num.clear();
	for (int i = 0; i < temp.size() + 1; i++)
	{
		num.push_back(0);
	}
	while (temp.size() > c.size())
	{
		c.push_back(0);
	}
	for (int i = 0; i < temp.size(); i++)
	{
		num[i] += temp[i] + c[i];
	}
	int len = num.size();
	for (int i = 0; i < num.size(); i++)
	{
		if (i == (len - 1) && num[i] > 9)
		{
			num.push_back(0);
		}
		if (num[i] > 9)
		{
			num[i + 1] += num[i] / 10;
			num[i] %= 10;
		}
	}
	while (num.size() > 1 && num[num.size() - 1] == 0)
	{
		num.pop_back();
	}
}

void sub(string & dividend, vector<int>& temp)
{
	int size, borrow = 0;
	while (dividend.size() > temp.size())
	{
		temp.push_back(0);
	}
	for (int i = 0; i < dividend.size(); i++) {
		int v = dividend[i] - temp[i] - 48 - borrow;
		if (v < 0) {
			borrow = 1;
			v += 10;
		}
		else {
			borrow = 0;
		}
		dividend[i] = v + 48;
	}
	while (dividend.size() > 0 && dividend[dividend.size() - 1] == '0')
	{
		dividend.pop_back();
	}
}

string div(vector<int>& num1, vector<int>& num2, int dot)
{
	int len = num1.size();
	string dividend;
	string ans = "";
	for (int i = len - 1; i >= -dot; i--)
	{
		if (i == -1)
		{
			ans += '.';
		}
		dividend = (i >= 0 ? (char)(num1[i] + 48) : '0') + dividend;
		for (int j = 9; j >= 0; j--)
		{
			vector <int> temp, c;
			temp.assign(num2.begin(), num2.end());
			if (j == 9) {
				j = j;
			}
			c.push_back(j);
			mul(temp, c);

			if (compare(temp, dividend))
			{
				sub(dividend, temp);
				ans.push_back((char)(j + 48));
				break;
			}
		}
	}
	while (ans[0] == '0' && ans[1] >= '0' && ans[1] <= '9')
	{
		ans.erase(0, 1);
	}
	return ans;
}

bool compare(vector<int>& temp, string dividend)
{
	if (temp.size() > dividend.size())
	{
		return false;
	}
	else if (temp.size() < dividend.size())
	{
		return true;
	}
	else
	{
		for (int i = temp.size() - 1; i >= 0; i--)
		{
			if (((int)(dividend[i]) - 48) > temp[i])
			{
				return true;
			}
			else if (((int)(dividend[i]) - 48) < temp[i])
			{
				return false;
			}
		}
		return true;
	}
}

Number root(vector<int>& num)
{
	vector <int> divisor;
	string dividend = "";
	string ans = "";
	int len = num.size() - 1;
	int sign;
	int k = 1;
	len -= (num.size() % 2) == 0 ? 1 : 0;
	for (int i = len; i >= -200; i -= 2)
	{
		if (i == 0) {
			i = i;
		}
		if ((i == len) && (num.size() % 2 == 0))
		{
			dividend = (char)(num[i + 1] + 48);
			dividend = (char)(num[i] + 48) + dividend;
		}
		else if ((i == len) && (num.size() % 2 == 1))
		{
			dividend = (char)(num[i] + 48) + dividend;
		}
		else if (i >= 0)
		{
			dividend = (char)(num[i + 1] + 48) + dividend;
			dividend = (char)(num[i] + 48) + dividend;
		}
		else
		{
			dividend = "00" + dividend;
		}
		for (int j = 9; j >= 0; j--)
		{
			divisor.insert(divisor.begin(), j);
			vector<int> temp, c;
			temp.assign(divisor.begin(), divisor.end());
			c.push_back(j);
			mul(temp, c);
			if (compare(temp, dividend))
			{
				add(divisor, c);
				sub(dividend, temp);
				ans.push_back((char)(j + 48));
				break;
			}
			divisor.erase(divisor.begin());
		}
		if (i == 0)
		{
			ans += ".";
		}
	}
	while (ans[0] == '0' && ans[1] >= '0' && ans[1] <= '9')
	{
		ans.erase(0, 1);
	}
	if (ans == "0")
	{
		sign = 1;
	}
	return Number(ans);
}

Number operator+(const Number& add1, const Number& add2)
{
	Number num1 = add1;
	Number num2 = add2;
	Number ans;
	if (num1.denom.size() != num2.denom.size()) 
	{
		mul(num1.denom, add2.denom);
		mul(num1.num, add2.denom);
		mul(num2.denom, add1.denom);
		mul(num2.num, add1.denom);
	}
	else 
	{
		int same = 1;
		for (int i = 0; i < num1.denom.size(); i++) 
		{
			if (num1.denom[i] != num2.denom[i]) 
			{
				same = 0;
				break;
			}
		}
		if (same == 1) 
		{
			mul(num1.denom, add2.denom);
			mul(num1.num, add2.denom);
			mul(num2.denom, add1.denom);
			mul(num2.num, add1.denom);
		}
	}
	int temp = 0;//進位值
	ans.denom.assign(num1.denom.begin(), num1.denom.end());
	ans.isDecimal = add1.isDecimal || add2.isDecimal ? 1 : 0;
	if ((num1.sign == 1 && num2.sign == 1) || (num1.sign == -1 && num2.sign == -1))
	{
		if (num1.num.size() > num2.num.size())//如 size 不同把 size 小的前面補 0
		{
			for (int i = 0; num1.num.size() != num2.num.size(); i++)
			{
				num2.num.push_back(0);
			}

		}
		else if (num1.num.size() < num2.num.size())//如 size 不同把 size 小的前面補 0
		{
			for (int i = 0; num1.num.size() != num2.num.size(); i++)
			{
				num1.num.push_back(0);
			}
		}

		for (int i = 0; i < num1.num.size() - 1; i++)//新增一個全 0 的陣列
		{
			ans.num.push_back(0);
		}

		for (int i = 0; i < num1.num.size(); i++)//從個位數開始加
		{
			ans.num[i] = temp + num1.num[i] + num2.num[i];//temp 是進位值

			temp = ans.num[i] / 10;
			ans.num[i] %= 10;

			if (i == num1.num.size() - 1 && temp > 0)
			{
				ans.num.push_back(temp);
				temp = 0;
			}
		}

		if (num1.sign == 1 && num2.sign == 1)
		{
			ans.sign = 1;
			return ans;
		}
		else if (num1.sign == -1 && num2.sign == -1)
		{
			ans.sign = -1;
			return ans;
		}
	}
	else if (num1.sign == 1 && num2.sign == -1)
	{
		num2.sign = 1;
		ans = num1 - num2;
		return ans;
	}
	else if ((num1.sign == -1 && num2.sign == 1))
	{
		num1.sign = 1;
		ans = num2 - num1;
		return ans;
	}
}

Number operator-(const Number& sub1, const Number& sub2)
{
	Number num1 = sub1;
	Number num2 = sub2;
	Number ans;
	if (num1.denom.size() != num2.denom.size())
	{
		mul(num1.denom, sub2.denom);
		mul(num1.num, sub2.denom);
		mul(num2.denom, sub1.denom);
		mul(num2.num, sub1.denom);
	}
	else
	{
		int same = 1;
		for (int i = 0; i < num1.denom.size(); i++)
		{
			if (num1.denom[i] != num2.denom[i])
			{
				same = 0;
				break;
			}
		}
		if (same == 1)
		{
			mul(num1.denom, sub2.denom);
			mul(num1.num, sub2.denom);
			mul(num2.denom, sub1.denom);
			mul(num2.num, sub1.denom);
		}
	}
	ans.denom.assign(num1.denom.begin(), num1.denom.end());
	ans.isDecimal = sub1.isDecimal || sub2.isDecimal ? 1 : 0;
	bool sub_Compare = true;

	if (num1.sign == 1 && num2.sign == 1)
	{
		for (int i = 0; ; i++)
		{
			if (num1.num.size() > num2.num.size())//如 size 不同把 size 小的前面補 0
			{
				num2.num.push_back(0);
			}
			else if (num1.num.size() < num2.num.size())//如 size 不同把 size 小的前面補 0
			{
				num1.num.push_back(0);
			}
			else if (num1.num.size() == num2.num.size())
			{
				break;
			}
		}

		for (int i = 0; i < num1.num.size() - 1; i++)//新增一個全 0 的陣列
		{
			ans.num.push_back(0);
		}

		for (int i = num1.num.size() - 1; i >= 0; i--)
		{
			if (num1.num[i] > num2.num[i])
			{
				ans.sign = 1;
				break;
			}
			else if (num1.num[i] < num2.num[i])
			{
				ans = num2 - num1;
				ans.sign = -1;
				for (int j = ans.num.size() - 1; j > 0; j--)
				{
					if (ans.num[j] != 0)
					{
						break;
					}
					else
					{
						ans.num.erase(ans.num.end() - 1);
					}
				}
				return ans;
				break;
			}
		}

		for (int i = 0; i < num1.num.size(); i++)//開始減
		{
			if (num1.num[i] < num2.num[i])
			{
				ans.num[i] = 10 + num1.num[i] - num2.num[i];

				for (int j = i + 1; j < num1.num.size(); j++)
				{
					if (num1.num[j] != 0)
					{
						num1.num[j]--;
						break;
					}
					else
					{
						num1.num[j] = 9;
					}
				}
			}
			else
			{
				ans.num[i] = num1.num[i] - num2.num[i];
			}
		}

		for (int j = ans.num.size() - 1; j > 0; j--)
		{
			if (ans.num[j] != 0)
			{
				break;
			}
			else
			{
				ans.num.erase(ans.num.end() - 1);
			}
		}
		return ans;
	}
	else if (num1.sign == -1 && num2.sign == 1)
	{
		num1.sign = 1;
		ans = num1 + num2;
		ans.sign = -1;
		return ans;
	}
	else if (num1.sign == 1 && num2.sign == -1)
	{
		num2.sign = 1;
		ans = num1 + num2;
		ans.sign = 1;
		return ans;
	}
	else if (num1.sign == -1 && num2.sign == -1)
	{
		num2.sign = 1;
		num1.sign = 1;
		ans = num2 - num1;
		return ans;
	}
}


Number operator/(const Number& div1, const Number& div2)
{
	int dot = div1.isDecimal == 1 || div2.isDecimal == 1 ? -100 : 0;
	int decimal = div1.isDecimal == 1 || div2.isDecimal == 1 ? 1 : 0;
	Number num1 = div1;
	Number num2 = div2;
	if (num1.dot1 > num2.dot1)
	{
		for (int i = 0; i < num1.dot1 - num2.dot1; i++)
		{
			num2.num.insert(num2.num.begin(), 0);
		}
	}
	else if (num2.dot1 > num1.dot1)
	{
		for (int i = 0; i < num2.dot1 - num1.dot1; i++)
		{
			num1.num.insert(num1.num.begin(), 0);
		}
	}
	int len = num1.num.size();
	string dividend;
	string ans = "";
	int sign = num1.sign * num2.sign;
	for (int i = len - 1; i >= dot; i--)
	{
		if (i == -1)
		{
			ans += '.';
		}
		dividend = (i >= 0 ? (char)(num1.num[i] + 48) : '0') + dividend;
		for (int j = 9; j >= 0; j--)
		{
			vector <int> temp, c;
			temp.assign(num2.num.begin(), num2.num.end());
			if (j == 9) {
				j = j;
			}
			c.push_back(j);
			mul(temp, c);

			if (compare(temp, dividend))
			{
				sub(dividend, temp);
				ans.push_back((char)(j + 48));
				break;
			}
		}
	}
	while (ans[0] == '0' && ans[1] >= '0' && ans[1] <= '9')
	{
		ans.erase(0, 1);
	}
	if (ans == "0")
	{
		sign = 1;
	}
	return Number(ans, num1.denom, sign, decimal);
}

Number operator*(const Number& a, const Number& b)
{
	int decimal = a.isDecimal == 1 || b.isDecimal == 1 ? 1 : 0;
	vector<int>result;
	Number num1 = a;
	Number num2 = b;
	int fpoint = num1.dot1 + num2.dot1;
	int sign = 0;
	if (num1.sign != num2.sign) { sign = -1; }
	else { sign = 1; }
	result.resize(num1.num.size() + num2.num.size(), 0);
	for (int i = 0; i < num1.num.size(); ++i)
	{
		if (num1.num[i] == 0)
		{
			continue;
		}
		else
		{
			for (int j = 0; j < num2.num.size(); ++j)
			{
				result[i + j] += (num1.num[i] * num2.num[j]);
			}
		}
	}
	for (int use = 0, i = 0; i < num1.num.size() + num2.num.size(); ++i)
	{
		result[i] += use;
		use = (result[i] / 10);
		result[i] %= 10;
	}
	string ans = "";
	for (int i = 0; i < result.size(); i++)
	{
		ans += result[i] + '0';
	}
	reverse(ans.begin(), ans.end());
	int mark = 0;
	string ansReal = "";
	for (int i = 0; i < ans.size(); i++)
	{
		if (ans[i] == '0' && mark == 0 && i < ans.size() - 1)
		{
			continue;
		}
		else if (i == ans.size() - 1 && ans[i] == '0')
		{
			ansReal += ans[i];
		}
		else
		{
			mark = 1;
			ansReal += ans[i];
		}
	}
	/*if (fpoint > 0)
	{
		ansReal.insert(ansReal.end() - fpoint, '.');
	}*/
	if (ansReal == "0")
	{
		sign = 1;
	}
	mul(num1.denom, num2.denom);
	return Number(ansReal, num1.denom, sign, decimal);
}

Number operator^(const Number & num1, const Number & num2)
{
	if (num2.num.size() == 1 && num2.num[0] == 0)
	{
		string one = "1";
		return Number(one);
	}

	int sign = num1.sign;
	long int n = 0;
	Number temp1, temp2, num;
	num = num1;
	if (num2.sign == -1)
	{
		num.num.swap(num.denom);
	}
	vector<int> a, b;
	a.assign(num.num.begin(), num.num.end());
	b.assign(num.denom.begin(), num.denom.end());
	temp1 = num2;
	temp2 = num2;
	vector<int>().swap(temp1.denom);
	temp2.denom.swap(temp2.num);
	vector<int>().swap(temp2.denom);
	temp1.denom.push_back(1);
	temp2.denom.push_back(1);
	temp2 = temp1 / temp2;
	temp2.sign = sign;

	if ((temp2.isDecimal == 1 && temp2.num[0] != 5 && temp2.denom.size() > 2))
	{
		if ((temp2.denom.size() == 2 && temp2.denom[0] != 0 && temp2.denom[1] != 1) && (temp2.denom.size() == 1 && temp2.denom[0] != 1))
		{
			cout << "指數請輸入0.5的倍數" << endl;
			return Number();
		}
		else if (temp2.denom.size() > 2) {
			cout << "指數請輸入0.5的倍數" << endl;
			return Number();
		}
	}

	if (temp2.isDecimal && temp2.denom.size() == 1 && temp2.denom[0] == 1)
	{
		temp2.num.insert(temp2.num.begin(), 0);
		temp2.denom.insert(temp2.denom.begin(), 0);
	}
	int s = 1, mode = 0;
	if (temp2.num[0] == 5 && temp2.isDecimal)
	{
		mode = 1;
	}
	
	
	Number temp, two;
	string tw = "2";
	two = Number(tw);
	if (temp2.isDecimal)
	{
		vector<int> c;
		c.push_back(2);
		mode ? mul(temp2.num, c), temp2.num.erase(temp2.num.begin()), temp2.denom.erase(temp2.denom.begin()) : temp2.num.erase(temp2.num.begin());
	}
	if (temp2.num[0] % 2 == 0)
	{
		sign = 1;
	}
	temp2.isDecimal = 0;
	vector<int> bit;
	while (temp2.num.size() > 1 || (temp2.num[0] != 1 && temp2.num.size() == 1))
	{
		(temp2.num[0] % 2 == 0) ? bit.push_back(0) : bit.push_back(1);
		two.dot1 = temp2.dot1;
		temp2 = temp2 / two;
	}
	bit.push_back(1);
	/*for (int i = 0; i < temp2.num.size(); i++, s *= 10)
	{
	n = n + temp2.num[i] * s;
	}
	if (temp2.isDecimal)
	{
	n = mode ? n * 2 / 10 : n / 10;
	}
	if (n % 2 == 0)
	{
	sign = 1;
	}
	int count = 31;
	while (((n >> count--) & 1) == 0);
	vector <int> bit;
	for (int i = count + 1; i >= 0; i--)
	{
	bit.push_back(((n >> i) & 1));
	}*/
	for (int i = bit.size() - 2; i >= 0; i--)
	{
		if (bit[i])
		{
			mul(num.num, num.num);
			mul(num.num, a);
			mul(num.denom, num.denom);
			mul(num.denom, b);
		}
		else
		{
			mul(num.num, num.num);
			mul(num.denom, num.denom);
		}
	}

	if (mode)
	{
		if (num.sign == -1)
		{
			cout << "負數不能開根號" << endl;
			return Number();
		}
		Number c, d;
		c = num;
		d = num;
		vector<int>().swap(c.denom);
		d.denom.swap(d.num);
		vector<int>().swap(d.denom);
		c.denom.push_back(1);
		d.denom.push_back(1);
		num = root(c.num) / root(d.num);
		if (num.denom.size() > 101)
		{
			for (int i = 0; i < 100; i++)
			{
				num.denom.erase(num.denom.begin());
			}
		}
	}
	num.sign = sign;
	num.isDecimal = num1.isDecimal || num2.isDecimal ? 1 : 0;
	return num;
}

ostream & operator<<(ostream & out, const Number & number)
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



void inToPostfix(char* infix, char* postfix) {

	char stack[MAX] = { '\0' };
	char sign = '\0';
	char temp[MAX] = { '\0' };
	int i, j, top, tempn = 0;   //tempn(括號) 
	vector <int> powern;
	for (i = 0, j = 0, top = 0; infix[i] != '\0'; i++) switch (infix[i]) {
		if (i > 1000) 
		{
			break;
		}
	case '(':              // 運算子堆疊 
		stack[++top] = infix[i];
		tempn++;
		break;
	case '!':
		postfix[j++] = ' ';
		postfix[j++] = '!';
		break;
	case '+': case '-': case '*': case '/': case '%': case '^':

		if (infix[i] == '+' || infix[i] == '-')
		{
			if ((infix[i - 1] < 48 || infix[i - 1] > 57) && infix[i - 1] != ')' && infix[i - 1] != '!' && infix[i + 1] != '(')
			{
				if (sign == '\0')
				{
					sign = infix[i];
				}
				else if ((infix[i] == '+' && sign == '+') || (infix[i] == '-' && sign == '-'))
				{
					sign = '+';
				}
				else
				{
					sign = '-';
				}
				break;
			}
			if ((infix[i - 1] < 48 || infix[i - 1] > 57) && infix[i + 1] == '(' && infix[i - 1] != ')')
			{
				postfix[j++] = ' ';
				postfix[j++] = '0';
				temp[tempn] = infix[i];
				break;
			}
		}

		if (infix[i] == '^')
		{
			powern.push_back(tempn);
			break;
		}
		while (powern.size() > 0 && (powern[powern.size() - 1] == tempn))
		{
			postfix[j++] = ' ';
			postfix[j++] = '^';
			powern.pop_back();
		}

		while (priority(stack[top]) >= priority(infix[i])) {
			postfix[j++] = ' ';
			postfix[j++] = stack[top--];
		}
		stack[++top] = infix[i]; // 存入堆疊 
		break;
	case ')':
		while (powern.size() > 0 && powern[powern.size() - 1] == tempn)
		{
			postfix[j++] = ' ';
			postfix[j++] = '^';
			powern.pop_back();
		}
		tempn--;
		while (stack[top] != '(' && top >= 0) { // 遇 ) 輸出至 ( 
			postfix[j++] = ' ';
			postfix[j++] = stack[top--];
		}
		if (temp[tempn] != '\0')
		{
			postfix[j++] = ' ';
			postfix[j++] = temp[tempn];
			temp[tempn] = '\0';
		}
		top--;  // 不輸出 ( 
		break;
	default:  // 運算元直接輸出 
		if (infix[i] != '.' && infix[i - 1] != '.' && (infix[i - 1] < 48 || infix[i - 1] > 57))
		{
			postfix[j++] = ' ';
		}
		if (sign != '\0')
		{
			postfix[j++] = sign;
			sign = '\0';
		}
		postfix[j++] = infix[i];
		break;
	}
	while (powern.size() > 0)
	{
		postfix[j++] = ' ';
		postfix[j++] = '^';
		powern.pop_back();
	}
	while (top > 0) {
		postfix[j++] = ' ';
		postfix[j++] = stack[top--];
	}
}

int priority(char op) {
	switch (op) {
	case '+': case '-': return 1;
	case '*': case '/': case '%': return 2;
	case '^': return 3;
	case '!': return 4;
	default:            return 0;
	}
}


void splitString(string& temp, vector<string>& cal, string& c)
{
	int pos1 = 0, pos2 = temp.find(c);
	while (string::npos != pos2)
	{
		if (temp.substr(pos1, pos2 - pos1) != "")
			cal.push_back(temp.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = temp.find(c, pos1);
	}
	if (pos1 != temp.size())
	{
		cal.push_back(temp.substr(pos1));
	}
}


void calBigNum(vector<string>& cal, Number& number)
{
	//計算
	vector <Number> num1;
	int error = 0;
	for (int i = 0; i < cal.size(); i++)
	{
		if ((cal[i][0] >= 48 && cal[i][0] <= 57) || (cal[i][1] >= 48 && cal[i][1] <= 57))
		{
			num1.push_back(Number(cal[i]));
		}
		else if (cal[i].size() == 1)
		{
			switch (cal[i][0])
			{
			case '+':
				if (num1[num1.size() - 1].isDecimal || num1[num1.size() - 2].isDecimal)
				{
					Number a, b;
					a = num1[num1.size() - 2];
					b = num1[num1.size() - 1];
					mul(num1[num1.size() - 1].num, a.denom);
					mul(num1[num1.size() - 1].denom, a.denom);
					mul(num1[num1.size() - 2].num, b.denom);
					mul(num1[num1.size() - 2].denom, b.denom);
					num1[num1.size() - 2] = num1[num1.size() - 2] + num1[num1.size() - 1];
				}
				else
				{
					num1[num1.size() - 2] = num1[num1.size() - 2] + num1[num1.size() - 1];
				}
				num1.pop_back();
				break;
			case '-':
				if (num1[num1.size() - 1].isDecimal || num1[num1.size() - 2].isDecimal)
				{
					Number a, b;
					a = num1[num1.size() - 2];
					b = num1[num1.size() - 1];
					mul(num1[num1.size() - 1].num, a.denom);
					mul(num1[num1.size() - 1].denom, a.denom);
					mul(num1[num1.size() - 2].num, b.denom);
					mul(num1[num1.size() - 2].denom, b.denom);
					num1[num1.size() - 2] = num1[num1.size() - 2] - num1[num1.size() - 1];
				}
				else
				{
					num1[num1.size() - 2] = num1[num1.size() - 2] - num1[num1.size() - 1];
				}
				num1.pop_back();
				break;
			case '/':
				if (num1[num1.size() - 1].num[0] == 0 && num1[num1.size() - 1].num.size() == 1)
				{
					error = 1;
					number = Number();
					break;
				}
				if (num1[num1.size() - 2].isDecimal == 0 && num1[num1.size() - 1].isDecimal == 0)
				{
					num1[num1.size() - 2] = num1[num1.size() - 2] / num1[num1.size() - 1];
					num1.pop_back();
				}
				else
				{
					mul(num1[num1.size() - 2].num, num1[num1.size() - 1].denom);
					mul(num1[num1.size() - 2].denom, num1[num1.size() - 1].num);
					num1[num1.size() - 2].sign *= num1[num1.size() - 1].sign;
					num1[num1.size() - 2].isDecimal = 1;
					num1[num1.size() - 2].strnum += "/" + num1[num1.size() - 1].strnum;
					num1.pop_back();
				}
				break;
			case '*':
				num1[num1.size() - 2] = num1[num1.size() - 2] * num1[num1.size() - 1];
				num1.pop_back();
				break;
			case '!':
				if (num1[num1.size() - 1].isDecimal)
				{
					Number a, b;
					a = num1[num1.size() - 1];
					b = num1[num1.size() - 1];
					vector<int>().swap(a.denom);
					b.num.swap(b.denom);
					vector<int>().swap(a.denom);
					a.denom.push_back(1);
					b.denom.push_back(1);
					num1[num1.size() - 1] = a / b;
					if (num1[num1.size() - 1].denom[0] == 1 && num1[num1.size() - 1].denom.size() == 1)
					{
						!num1[num1.size() - 1];
						break;
					}
					number = Number();
					error = 1;
					break;
				}
				!num1[num1.size() - 1];
				break;
			case '^':
				num1[num1.size() - 2] = num1[num1.size() - 2] ^ num1[num1.size() - 1];
				if (num1[num1.size() - 1].sign == -1 && num1[num1.size() - 2].num.size() == 1 && num1[num1.size() - 2].num[0] == 0)
				{
					error = 1;
					number = Number();
					break;
				}
				num1.pop_back();
			}

		}
		if (error == 1)
		{
			break;
		}
	}
	if (error == 0)
	{
		number = num1[0];
	}
	else
	{
		number = Number();
	}

}
		