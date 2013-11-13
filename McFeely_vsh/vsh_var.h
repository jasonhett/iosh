/*
 * author: zachary.mcfeely@uky.edu (Zacahry S. mcFeely)
 * File: vsh_var.h
 *
 * 
 */
#ifndef _VSH_VAR_H_
#define _VSH_VAR_H_

#include <string>

using namespace std;

class vsh_var{
 public:
  vsh_var(){ //constructor
	setPrompt("vsh% "); //dafauls prompt
	setParseCmd(false); //the state of command parsing --> if parsecmd = false --> command parsing is off
	setEchoCmd(false); //the state of command echoing --> false means echoing is off
	setShowChild(false); //the state of child proccess information reporting --> false means reporting is off
  }
	string getPrompt() const{return prompt;}
	bool parseCmd() const{return parsecmd;}
	bool echoCmd() const{return echocmd;}
	bool showChild() const{return showchild;}

	void setPrompt(string inStr){prompt = inStr;}
	void setParseCmd(bool value){parsecmd = value;}
	void setEchoCmd(bool value){echocmd = value;}
	void setShowChild(bool value){showchild = value;}
  
 private:
	string prompt; //the prompt
	bool parsecmd; 	//the state of command parsing --> if parsecmd = false --> command parsing is off
	bool echocmd; 	//the state of command echoing --> false means echoing is off
	bool showchild; //the state of child proccess information reporting --> false means reporting is off
};	

#endif