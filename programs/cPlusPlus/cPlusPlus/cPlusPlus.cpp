// cPlusPlus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string> //string

using namespace std;

int main(void)
{
	string str, str2;
	int num;
	
	cout << "Enter your name: ";
	cin >> str;

	cout << "Hi there, " << str << endl;

	cin >> str2;
	cout << "num is: " << str2 << endl;

	//to grab everthing entered on the line,
	// use getline

	getline(cin, str);
	cout << "str is: " << str << endl;

	//cin.getline(str, 80, '\n');   non-stl version
	
	cout << "enter a string: ";
	char ch;
	do
	{
		cin.get(ch);
		cout << ch << endl;
	} while (ch != '\n');

	return EXIT_SUCCESS;

}//end main