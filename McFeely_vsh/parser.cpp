#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h> //for waitpid
#include <vector>
#include <iostream>
#include "token.h"
#include "vsh_var.h"

#define ARGLEN 128 //only used for system command arguments ~ 128 max possible with input buffer of 256 defined in vsh.cpp

using namespace std;

vector<token> scan_cntrler(const string*); //Top level abstraction of the scanner functionality - could be seen as lower level of parser functionality ~ refer to scanner.cpp
void print_parsed_cmd(const vector<token>&);  //used to process command echoing and command parseing

void parser(const string* input, vsh_var& vshell){
	
	vector<token> toks;
	vector<token> tmp_toks; 
	string str = *input;
	
	//Get the tokens
	toks = scan_cntrler(input);
	
	//unsigned int numTokens = toks.size();
	if( (toks[0].getType() == 5)&&(toks[1].getType() == 2)&&(toks[2].getType() == 4) ){ //if cmd is setprompt string\n
		if(vshell.parseCmd()){print_parsed_cmd(toks);}
		if(vshell.echoCmd()){// print the command the shell is about to execute		
			cout << "+++++++++++++++++++++++++++++++++++\n";
			cout << "+\tCommand to execute: setprompt " << toks[1].getValue() << endl;
			cout << "+++++++++++++++++++++++++++++++++++\n";
		} 
		vshell.setPrompt(toks[1].getValue());
	}
	else if( (toks[0].getType() == 8) && (toks[2].getType() == 4) ){ //command parsing
		if( toks[1].getValue() == "on" ){
			toks[1].setUsage("arg");
			if(vshell.parseCmd()){print_parsed_cmd(toks);}
			if(vshell.echoCmd()){// print the command the shell is about to execute
				cout << "+++++++++++++++++++++++++++++++++++\n";
				cout << "+\tCommand to execute: parsecmd on\n";
				cout << "+++++++++++++++++++++++++++++++++++\n";
			} 
			vshell.setParseCmd(true);
		}
		else if( toks[1].getValue() == "off" ){
			toks[1].setUsage("arg");
			if(vshell.parseCmd()){print_parsed_cmd(toks);}
			if(vshell.echoCmd()){// print the command the shell is about to execute
				cout << "+++++++++++++++++++++++++++++++++++\n";
				cout << "+\tCommand to execute: parsecmd off\n";
				cout << "+++++++++++++++++++++++++++++++++++\n";
			} 
			vshell.setParseCmd(false);
		}
		else
			cout << "\tError: Bad Parse Command Syntax\n";
	}
	else if( (toks[0].getType()==6) && (toks[1].getType()==3) && (toks[2].getType()==2) && (toks[3].getType()==4) ){ //'setvar variable_name string' command
		if(vshell.parseCmd()){print_parsed_cmd(toks);}
		if(vshell.echoCmd()){// print the command the shell is about to execute
			cout << "+++++++++++++++++++++++++++++++++++\n";
			cout << "+\tCommand to execute: setvar " << toks[1].getValue() << " " << toks[2].getValue() << endl;
			cout << "+++++++++++++++++++++++++++++++++++\n";
		} 
		string tmp_envar = toks[1].getValue();
		tmp_envar = tmp_envar.substr(1);
		const char * envar = tmp_envar.c_str();
		const char * enstr = toks[2].getValue().c_str();
		int setCheck = setenv(envar,enstr,1); //Note: third param controls overwrite of pre-exsisting vars
		if(setCheck != 0){
			perror("\tERROR-> environment variable not set");
		}
	}
	else if( (toks[0].getType() == 7) && (toks[2].getType() == 4) ){ //handle command echoing
		if( toks[1].getValue() == "on" ){
			toks[1].setUsage("arg");
			if(vshell.parseCmd()){print_parsed_cmd(toks);}
			if(vshell.echoCmd()){// print the command the shell is about to execute
				cout << "+++++++++++++++++++++++++++++++++++\n";
				cout << "+\tCommand to execute: echocmd on\n";
				cout << "+++++++++++++++++++++++++++++++++++\n";
			} 
			vshell.setEchoCmd(true);
		}
		else if( toks[1].getValue() == "off" ){
			toks[1].setUsage("arg");
			if(vshell.parseCmd()){print_parsed_cmd(toks);}
			if(vshell.echoCmd()){// print the command the shell is about to execute
				cout << "+++++++++++++++++++++++++++++++++++\n";
				cout << "+\tCommand to execute: echocmd off\n";
				cout << "+++++++++++++++++++++++++++++++++++\n";
			} 
			vshell.setEchoCmd(false);
		}
		else
			cout << "\tError: Bad Echo Command Syntax\n";
	}
	else if( (toks[0].getType() == 9) && (toks[2].getType() == 4) ){ //handle child proccess reporting command
		if( toks[1].getValue() == "on" ){
			toks[1].setUsage("arg");
			if(vshell.parseCmd()){print_parsed_cmd(toks);}
			if(vshell.echoCmd()){// print the command the shell is about to execute
				cout << "+++++++++++++++++++++++++++++++++++\n";
				cout << "+\tCommand to execute: showchild on\n";
				cout << "+++++++++++++++++++++++++++++++++++\n";
			} 
			vshell.setShowChild(true);
		}
		else if( toks[1].getValue() == "off" ){
			toks[1].setUsage("arg");
			if(vshell.parseCmd()){print_parsed_cmd(toks);}
			if(vshell.echoCmd()){// print the command the shell is about to execute
				cout << "+++++++++++++++++++++++++++++++++++\n";
				cout << "+\tCommand to execute: showchild off\n";
				cout << "+++++++++++++++++++++++++++++++++++\n";
			} 
			vshell.setShowChild(false);
		}
		else
			cout << "\tError: Bad showchild Command Syntax\n";
	}
	else if( toks[0].getType() == 11 && toks[1].getType() == 4){ //exit command 
		if(vshell.parseCmd()){print_parsed_cmd(toks);}
		if(vshell.echoCmd()){// print the command the shell is about to execute
			cout << "+++++++++++++++++++++++++++++++++++\n";
			cout << "+\tCommand to execute: exit\n";
			cout << "+++++++++++++++++++++++++++++++++++\n";
		} 
		exit(EXIT_FAILURE); //- exit the shell gracefully 
		perror("\tERROR");
	}
	else if(toks[0].getType() == 13 && toks[1].getType() == 4){ //Comment line
		//Comment - Do nothing unless parsecmd is on
		if(vshell.parseCmd()){print_parsed_cmd(toks);}
	}
	else if(toks[0].getType() == 10 && toks[2].getType() == 4){ //handle the cd director command
		const char* dir_name = toks[1].getValue().c_str();
		if(vshell.parseCmd()){print_parsed_cmd(toks);}
		if(vshell.echoCmd()){// print the command the shell is about to execute
			cout << "+++++++++++++++++++++++++++++++++++\n";
			cout << "+\tCommand to execute: cd " << toks[1].getValue() << endl;
			cout << "+++++++++++++++++++++++++++++++++++\n";
		}
		int result = chdir(dir_name); //chage directories
		if(result != 0)
			perror("\tWARNING-> chdir() failed");
	}
	else if( toks[0].getType() == 1 ){ //Possible System Command
		if(vshell.echoCmd()){// print the command the shell is about to execute
			cout << "+++++++++++++++++++++++++++++++++++\n";
		}
		pid_t pid;
		int retval, child_status;
		const char* cmd = toks[0].getValue().c_str(); //--------------> Convert the command to appropriate type for exec
		unsigned int toksize = toks.size(); //------------------------> Number of tokens, including EOL
		unsigned pathfound = toks[0].getValue().find_last_of("/"); //-> '/' only present if path supplied by user
		string only_cmd_str; //---------------------------------------> to store only the command
		char* args[ARGLEN];  //---------------------------------------> user supplied arguments to the command
		if(pathfound != string::npos){ //-----------------------------> Check if user specified path
			only_cmd_str = toks[0].getValue().substr(pathfound+1); 
			args[0] = new char[only_cmd_str.size()+1]; 
			std::strcpy(args[0],only_cmd_str.c_str());
			if(vshell.echoCmd()){// print the command the shell is about to execute
				cout << "+\tCommand to execute: " << only_cmd_str;
			}
		}
		else{ //------------------------------------------------------> User did not specify path
			args[0] = new char[toks[0].getValue().size()+1]; 
			std::strcpy(args[0],cmd);
			if(vshell.echoCmd()){// print the command the shell is about to execute
				cout << "+\tCommand to execute: " << toks[0].getValue();
			}
		}
		toks[0].setUsage("cmd");
		for(unsigned i=1;i<toksize;++i){ //Loop Purpose: fill args array with all of the arguments
			if(i+1 == toksize){ //if last token
				args[i] = NULL; // When execvp encounters the NULL-Pointer, it knows it has reached end of the argument list	
			}
			else{ 	// Place the command arguments in the args array
				if(toks[i].getType()==3){ //test if token is a variable
					string varStr = toks[i].getValue();
					varStr = varStr.substr(1);
					char* varCstr; 
					const char* envar =  varStr.c_str();
					varCstr = getenv(envar);
					if(vshell.echoCmd()){// print the command the shell is about to execute
						cout << " " << varCstr;
					}
					if(varCstr!=NULL){
						args[i] = new char[strlen(varCstr)+1]; 
						std::strcpy(args[i], varCstr);
					}
					else{
						args[i] = new char[1];
						std::strcpy(args[i], "");						
					}
				}
				else{
					args[i] = new char[toks[i].getValue().size()+1];  
					std::strcpy(args[i], toks[i].getValue().c_str());
					if(vshell.echoCmd()){// print the command the shell is about to execute
						cout << " " << toks[i].getValue();
					}
				}
				toks[i].setUsage("arg");
			}
		}
		if(vshell.echoCmd()){// print the command the shell is about to execute
			cout << "\n+++++++++++++++++++++++++++++++++++\n";
		}
		if(vshell.parseCmd()){print_parsed_cmd(toks);} //Print the parsed cmd is parsecmd on
		pid = fork(); //Replicate the current process
		if (pid ==0) {
			retval = execvp(cmd,args); //run the program specified in the address space of the child
			perror("\tERROR-> Child failed to execute the command - execvp returned"); //Should not see this if command is valid
			exit(0);
		}
		else {
			retval = wait(&child_status);//block until child terminates
			if(retval == -1)
				perror("\tERROR");
			if(vshell.showChild()){ 
				cout << "+++++++++++++++++++++++++++++++++++\n";
			//	cout << "+\tParent's PID: " << getpid() << endl; //not specified in requirements
				cout << "+\tChild's PID: " << pid << endl;
				cout << "+\tChild's Exit Status: " << retval << endl;
				cout << "+++++++++++++++++++++++++++++++++++\n";
			}
			// delete[] args; /* Complier returns-> warning: deleting array âchar* args [128]â [enabled by default] */
		}	
	}
	else{
		if(vshell.parseCmd()){print_parsed_cmd(toks);}
		cout << "\tERROR: Bad Command Syntax\n";
	}

	
	return;
	
}
	
void print_parsed_cmd(const vector<token> &input){ //Used to print the parsed command if parsecmd is on
	string TTable[] = {"metachar","word","string","variable","end-of-line","setprompt","setvar","echocmd","parsecmd","showchild","cd","exit","BAD SYNTAX","Comment","NULL"}; //All of the token types ~ refer to token.h
	cout << "\n+-----------------------------------------------------------------------------------------------------------\n";
	for(unsigned i=0;i<input.size();++i){
		string strValue = input[i].getValue();
		if(i == (input.size() -1)){
			cout << "|\tToken Type = " << TTable[input[i].getType()] << "\tToken = "<< strValue << "\t\tUsage:" << input[i].getUsage() << endl;
		}
		else if (strValue.size() >= 8){
			cout << "|\tToken Type = " << TTable[input[i].getType()] << "\t\tToken = "<< strValue << "\t\tUsage:" << input[i].getUsage() << endl;
		}
		else{
			cout << "|\tToken Type = " << TTable[input[i].getType()] << "\t\tToken = "<< strValue << "\t\t\tUsage:" << input[i].getUsage() << endl;
		}
	}
	cout << "+-----------------------------------------------------------------------------------------------------------\n";
}
