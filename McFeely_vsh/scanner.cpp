#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "token.h"

using namespace std;

bool isVarName (const string);
vector<token> find_metachars(string);
vector<token> scanner(const string);


vector<token> scan_cntrler(const string* input){ //This function provides a top-level abstraction for the scanner functionality
	vector<token> toks; //return value
	vector<token> tmp_toks;//temporay storage
	token t;	//a single token used to handle exceptions and EOL
	string str = *input;
	
	//Clean up any whitespace before command
	unsigned foundWhite = str.find_first_not_of(" \t");
	if(foundWhite != 0)
		str = str.substr(foundWhite);
	
	//Get the tokens
	bool EOL = false; //generic EOL used to signal the "proccessing" loop to stop proccessing input
	if(str[0] == '%'){ //Comment line
		EOL = true; 
		t.setValue(str);
		t.setType(13);  //Comment type
		t.setUsage("Comment");
		toks.push_back(t);
	}
	while(!EOL){ //This is the proccessing loop
		//seperate user input by spaces
		unsigned found = str.find_first_of(" \t"); 
		
		if(str[0] == '"'){ //check for strings          
			unsigned str_end = str.find('"',1);
			string str_variable = str.substr(0,str_end+1);
		
			//clean up any extra spaces between possible tokens
			str = str.substr(str_end+1);
			str.erase( 0, str.find_first_not_of( " \t" ) );
			
			//call scanner to identify tokens
			tmp_toks = scanner(str_variable);
			//add tokens returned by scanner to current tokens list
			toks.insert( toks.end(), tmp_toks.begin(), tmp_toks.end() );
			if(str.length() <= 1){
				EOL = true;
			}
		}
		else if(found != string::npos){ 
			string tmp_str = str.substr(0,found);
			//call scanner to identify tokens
			tmp_toks = scanner(tmp_str);
			//add tokens returned by scanner to current tokens list
			toks.insert( toks.end(), tmp_toks.begin(), tmp_toks.end() );
			
			//clean up any extra spaces between possible tokens
			str = str.substr(found);
			str.erase( 0, str.find_first_not_of( " \t" ));
		}
		else{
			//call scanner to identify tokens
			tmp_toks = scanner(str);
			//add tokens returned by scanner to current tokens list
			toks.insert( toks.end(), tmp_toks.begin(), tmp_toks.end() );
			EOL = true;
		}
	}
	//Last token is End-Of-Line
	t.setValue("end-of-line");
	t.setType(4); 
	t.setUsage("EOL");
	toks.push_back(t);

	return toks;	
}

vector<token> scanner(const string input){
	
	//declare return vector 
	vector<token> toks;
	token t;
	string in_str = input;
	
	//Chomp the newline and any other possiable gibberish off the end of the input string
	bool whiteTok = false; //token contains only whitespace
	string whitespaces (" \t\n\r");	
	unsigned found = in_str.find_last_not_of(whitespaces);
	if (found!=std::string::npos)
		in_str.erase(found+1);
	else
		whiteTok = true; //token contains only whitespace
	
	//--start scanning input--//
	
	//find any metacharacters
	string metachars ("|><&");
	unsigned metaFind = in_str.find_first_of(metachars);

	//Check for variable name tokens
	if(in_str[0] == '$'){	
		if(metaFind != string::npos){	//take care of case when metachar directly after variable
			string varStr = in_str.substr(0,metaFind); //varStr is just the variable string
			in_str = in_str.substr(metaFind); //part of string left over containing metachar(s)
			if(isVarName(varStr)){ //check that varStr has appropriate syntax
				t.setValue(varStr);
				t.setType(3); //variable_name
				t.setUsage("variable_name");
				toks.push_back(t);
			}
			else{
				t.setValue(in_str);
				t.setType(12);	//Bad Syntax
				t.setUsage("Bad_Syntax");
				toks.push_back(t);
			}
			vector<token> temp_tokens; //temp storage for metachar(s)/words from metachar string part
			temp_tokens = find_metachars(in_str);
			toks.insert( toks.end(), temp_tokens.begin(), temp_tokens.end() ); //add metachars and words to list
		}
		else if(isVarName(in_str)){ //check that varStr has appropriate syntax
			t.setValue(in_str);
			t.setType(3);
			t.setUsage("variable_name");
			toks.push_back(t);
		}
		else{
			t.setValue(in_str);
			t.setType(12);	//Bad Syntax
			t.setUsage("Bad_Syntax");
			toks.push_back(t);
		}
	}
	else if(in_str == "setprompt"){
		t.setValue(in_str);
		t.setType(5);
		t.setUsage("setprompt");
		toks.push_back(t);
	}
	else if(in_str == "setvar"){
		t.setValue(in_str);
		t.setType(6);
		t.setUsage("setvar");
		toks.push_back(t);
	}
	else if(in_str == "echocmd"){
		t.setValue(in_str);
		t.setType(7);
		t.setUsage("echocmd");
		toks.push_back(t);
	}
	else if(in_str == "parsecmd"){
		t.setValue(in_str);
		t.setType(8);
		t.setUsage("parsecmd");
		toks.push_back(t);
	}
	else if(in_str == "showchild"){
		t.setValue(in_str);
		t.setType(9);
		t.setUsage("showchild");
		toks.push_back(t);
	}
	else if(in_str == "cd"){
		t.setValue(in_str);
		t.setType(10);
		t.setUsage("cmd");
		toks.push_back(t);
	}
	else if(in_str == "exit"){
		t.setValue(in_str);
		t.setType(11);
		t.setUsage("get me out of here");
		toks.push_back(t);
	}
	else if(in_str[0] == '"'){
		in_str = in_str.substr(1);
		in_str.erase(in_str.length()-1);
		t.setValue(in_str);
		t.setType(2);
		t.setUsage("string");
		toks.push_back(t);
	}
	else if(metaFind != string::npos){ //metachars
		vector<token> temp_tokens;
		temp_tokens = find_metachars(in_str);
		toks.insert( toks.end(), temp_tokens.begin(), temp_tokens.end() );
	}
	else if(!whiteTok){ //if not a token with only whitespace
		t.setValue(in_str);
		t.setType(1);
		t.setUsage("word");
		toks.push_back(t);
	}
	
	return toks;
}

vector<token> find_metachars(string metastr){
	vector<token> toks;
	token t;
	string meta_part;
	string metachars ("|><&");
	unsigned metaFind = metastr.find_first_of(metachars);
	
	bool loop = true;
	while(loop){
		if((metastr[0] == '|') || (metastr[0] == '>' )|| (metastr[0] == '<') || (metastr[0] == '&')){
			meta_part = metastr.substr(0,1); 
			t.setValue(meta_part);
			t.setType(0);
			t.setUsage("metachar");
			toks.push_back(t);
			metastr = metastr.substr(1);
		}
		else{ 
			meta_part = metastr.substr(0,metaFind); //meta_part can be either a word or a metachar
			t.setValue(meta_part);
			t.setType(1);
			toks.push_back(t);
			metastr = metastr.substr(metaFind);
		}
		metaFind = metastr.find_first_of(metachars);
		if(metaFind == string::npos){
			if(metastr.length() > 0){
				t.setValue(metastr);
				t.setType(1);
				t.setUsage("word");
				toks.push_back(t);
			}
			loop = false;
		}
	}
	return toks;
}

bool isVarName (const string str){
	char* cstr = new char[str.length()+1];
	strcpy (cstr, str.c_str());

	bool goodSyntax = true;

	size_t len = strlen(cstr);
	if(cstr[0] == '$'){
		for(unsigned int i=1;i<len;++i){
			if(!(isalpha(cstr[i]))){
				goodSyntax = false;
				i=len;
			}
		}
	}
	else{ goodSyntax = false; }
	
	return goodSyntax;
}
