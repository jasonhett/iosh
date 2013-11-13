/*
 * author: zachary.mcfeely@uky.edu (Zacahry S. mcFeely)
 * File: token.h
 *
 * 
 */
#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

using namespace std;

	/*
	Token Types:
	0	0000	metachar
	1	0001	word
	2	0010	string
	3	0011	variable_name
	4	0100	end-of-line
	5	0101	setprompt
	6	0110	setvar
	7	0111	echocmd
	8	1000	parsecmd
	9	1001	showchild
	10	1010	cd
	11	1011	exit
	12  1100	BAD SYNTAX
	13	1101	Comment
	14	1110	NULL - init
	*/

class token{
 public:
  token(){
	value = "UNDEFINED";
	usage = "UNDEFINED";
	type = 14; //NULL
  }
  string getValue() const{return value;}
  string getUsage() const{return usage;}
  int getType() const{return type;}

  void setValue(string inVal){value = inVal;}
  void setUsage(string inStr){usage = inStr;}
  void setType(int inType){type = inType;}
  

 private:
	string value;
	string usage;
	int type;
};	


#endif