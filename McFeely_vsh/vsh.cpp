#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include "token.h"
#include "vsh_var.h"

#define BUFLEN 256 //If editing BUFLEN, then should also edit ARGLEN in parser.cpp
using namespace std;

extern char **environ; //array of pointer to environment variable strings of the form name=value;

void parser(const string*, vsh_var& vshell); //command parser function
 
int main()
{
	vsh_var vshell; //The verbose shell environment variables
	string lastStr = "void"; // Will record the users most recent input
	while(1){
		char in[BUFLEN]; 
		cout << vshell.getPrompt();
		fgets(in, BUFLEN, stdin); //Get user input
		string* str = new string(in); //Convert user input to string pointer
		
		string testEOF = *str;
		if(feof(stdin)){// Exit on Ctrl-d
			if(testEOF.size() > 1 && testEOF != lastStr && lastStr != "void"){ //If EOF on same line as user input and the new input does not equal the old input
				unsigned found = testEOF.find_first_not_of(" \t"); //find whitespaces
				if (found!=std::string::npos) //if there is input besided whitespaces
					parser(str, vshell); 
			}
			cout << endl;
			return 0;
		}
		
		if(*str != "\n") {//skip to next iteration if the user inputs no data
			parser(str, vshell); 
		}
		lastStr = *str;
		delete str;
	}
}







