#pragma warning( disable : 4996 ) 
#include "Integer.h"
#include "Decimal.h"
#define MAX 5000


void calculate(vector<string>&, int, string, string);


vector<Number> v;


int main(void) {
	char infix[MAX] = { '\0' };
	char input[MAX] = { '\0' };
	char postfix[MAX] = { '\0' };
	/*Number A = "0.5";
	Integer B = "2", C;
	string q = "2", p = "0.5";
	Decimal T = "0.3*3";
	cout << B+T << endl;*/
	/*Integer z = "123";
	Decimal w = "123.3";
	vector<Number*> nums;
	nums.push_back(&z);
	nums.push_back(&w);
	for (const auto& num : nums)
		cout << *num << endl;
	cout << (z - w) << endl;*/
	while (fgets(input, MAX, stdin))
	{
		if (input[strlen(input) - 1] == '\n')
		{
			input[strlen(input) - 1] = '\0';
		}
		cin.sync();
		vector <string> var;
		string str = "";
		string strnum = "";
		string c = "=";
		int mode = 0;
		str += input;
		if (str.find("=") != string::npos) 
		{
			mode = 2;
			if(str.find("Set Integer") != string::npos)
			{ 
				mode = 1;
				str.erase(str.find("Set Integer"), 11);
			}
			else if (str.find("Set Decimal") != string::npos)
			{
				mode = 3;
				str.erase(str.find("Set Decimal"), 11);
			}
			splitString(str, var, c);
			if (var.size() != 2 && var.size() != 1) 
			{
				cout << "變數輸入錯誤" << endl;
				memset(input, '\0', sizeof(input));
				continue;
			}
			else 
			{
				for (int i = 0; i < var[0].size(); i++) 
				{
					if (var[0][i] == ' ') 
					{
						var[0].erase(i, 1);
					}
				}
				str = var[0];
				strnum = var[1];
				
				for (int i = 0; i < v.size(); i++)
				{
					if (mode == 2 && v[i].varName == var[0]) {
						if (v[i].isDecimal == 1) 
						{
							mode = 4;
						}
						else 
						{
							mode = 2;
						}
					}
					while (var[1].find(v[i].varName) != string::npos)
					{
						var[1].insert(var[1].find(v[i].varName), "(");
						var[1].insert(var[1].find(v[i].varName) + v[i].varName.size() , ")");
						var[1].replace(var[1].begin() + var[1].find(v[i].varName), var[1].begin() + var[1].find(v[i].varName) + v[i].varName.size(),
							v[i].strnum.begin(), v[i].strnum.end());
					}
				}				
				memset(input, '\0', sizeof(input));
				for (int i = 0, j = 0; i < var[1].size(); i++)
				{
					if (var[1][i] != ' ')
					{
						input[j++] = var[1][i];
					}
				}
			}
		}
		string t = "";
		t = input;
		for (int i = 0; i < v.size(); i++)
		{
			while (t.find(v[i].varName) != string::npos)
			{
				if (v[i].sign == -1) {
					t.insert(t.find(v[i].varName), "-(");
				}
				else
					t.insert(t.find(v[i].varName), "(");
				t.insert(t.find(v[i].varName) + v[i].varName.size(), ")");
				int place = t.find(v[i].varName);
				string tt = "";
				string dot = ".0";
				t.erase(t.find(v[i].varName), v[i].varName.size());
				if (v[i].isDecimal == 1) 
				{
					tt = dot + tt;
				}
				for (int j = 0; j < v[i].num.size(); j++) 
				{
					tt = (char)(v[i].num[j] + 48) + tt;
				}
				tt += '/';
				for (int j = v[i].denom.size() - 1; j >= 0; j--)
				{
					tt += (char)(v[i].denom[j] + 48);
				}
				if (v[i].isDecimal == 1)
				{
					tt += dot;
				}
				t.insert(place, tt);
			}
		}
		memset(input, '\0', sizeof(input));
		for (int i = 0, j = 0; i < t.size(); i++)
		{
			if (t[i] != ' ')
			{
				input[j++] = t[i];
			}
		}
		if (input[0] == '\0') 
		{
			cout << "輸入為空" << endl;
			continue;
		}
		int count = 0;
		for (int i = 0, j = 0; input[i] != '\0'; i++) 
		{
			if (input[i] == '(') 
			{
				count++;
			}
			else if (input[i] == ')') 
			{
				if (count < 1) 
				{
					count = -1;
					break;
				}
				count--;
			}
			if (input[i] != ' ') 
			{
				infix[j++] = input[i];
			}
		}
		if (count != 0) 
		{
			cout << "括號不完全" << endl;
			continue;
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
		if (error == 1) 
		{
			cout << "找不到變數\n";
			memset(input, '\0', sizeof(input));
			continue;
		}

		//轉後序
		inToPostfix(infix, postfix);
		//cout << postfix << endl;
	
		//分割字串
		vector <string> cal;
		string temp(postfix);
		c = " ";
		splitString(temp, cal, c);
		/*for (int i = 0; i < cal.size(); i++)
		{
			cout << cal[i] << ' ';
		}*/
		//cout << endl;
		
		calculate(cal, mode, str, strnum);
		
		memset(infix, '\0', sizeof(infix));
		memset(postfix, '\0', sizeof(postfix));
	}


	system("pause");
	return 0;
}



void calculate(vector<string>& cal, int mode, string str, string strnum)
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
					cout << "除數不能為 0" << endl;
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
					cout << "小數不能算階乘" << endl;
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
					cout << "無效冪次，零的冪次不能為負" << endl;
					break;
				}
				else if (num1[num1.size() - 2].denom[0] == 0 && num1[num1.size() - 2].denom.size() == 1) {
					error = 1;
					break;
				}
				num1.pop_back();
				break;
			}

		}
		if (error == 1)
		{
			break;
		}
	}
	if (error == 0)
	{
		if (mode == 0) 
		{
			cout << num1[0] << endl;
		}
		else 
		{
			if (str == "") 
			{
				cout << "變數名稱無效" << endl;
			}
			else if (mode == 1)
			{
				for (int i = 0; i < v.size(); i++) 
				{
					if (v[i].varName == str) 
					{
						v[i] = Number(strnum, str, num1[0].num, num1[0].denom, 0, num1[0].sign);
						return;
					}
				}
				v.push_back(Number(strnum, str, num1[0].num, num1[0].denom, 0, num1[0].sign));
			}
			else if (mode == 2) 
			{
				for (int i = 0; i < v.size(); i++)
				{
					if (v[i].varName == str)
					{
						v[i] = Number(strnum, str, num1[0].num, num1[0].denom, 0, num1[0].sign);
						return;
					}
				}
				cout << "找不到變數" << endl;
			}
			else if (mode == 3) 
			{
				for (int i = 0; i < v.size(); i++)
				{
					if (v[i].varName == str)
					{
						v[i] = Number(strnum, str, num1[0].num, num1[0].denom, 1, num1[0].sign);
						v[i].isDecimal = 1;
						return;
					}
				}
				v.push_back(Number(strnum, str, num1[0].num, num1[0].denom, 1, num1[0].sign));
				v[v.size() - 1].isDecimal = 1;
			}
			else if (mode == 4) 
			{
				num1[0].isDecimal = 1;
				for (int i = 0; i < v.size(); i++)
				{
					if (v[i].varName == str)
					{
						v[i] = Number(strnum, str, num1[0].num, num1[0].denom, 1, num1[0].sign);
						v[i].isDecimal = 1;
						return;
					}
				}
				cout << "找不到變數" << endl;
			}
		}
	}
}
