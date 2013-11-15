#include <iostream>
#include "global.h"

std::vector<token> vTokens;
std::string sPrompt = "ios";
std::string sCurDir = "";
bool bDebugFlag = false;

using namespace std;

void printError(string s){
    cout << "Error: " << s << endl;
}

bool runcmd(string cmd_passed){
    //build cmd array and args
    int num_of_args = 2;
    int iter = 1;
    while(vTokens[iter].getType() == WORD){
        num_of_args++;
        iter++;
    } 
    const char* cmd = cmd_passed.c_str();
    char* args[num_of_args];

    char *cmdstr = new char [cmd_passed.length()+1];
    strcpy (cmdstr, cmd_passed.c_str());

    args[0] = cmdstr;
    args[num_of_args-1] = NULL;
    for(int i=1; i<num_of_args-1;i++){
        string s = vTokens[i].getValue();
        char *cstr = new char [s.length()+1];
        strcpy (cstr, s.c_str());
        args[i] = cstr;
    }

    if(fork()){   
        wait(0);
    }
    else {
        execvp(cmd,args);
        cout << "Command not found: " << cmd << endl;
        exit(EXIT_SUCCESS);
    }
    return true;
}

bool parser(){
    int firstType = vTokens[0].getType();
    string firstValue = vTokens[0].getValue();
    if(firstValue != "exit" && firstValue != "quit"){
        switch( firstType ){
            case METACHAR:
                if( firstValue == "#" )
                    return true;
                else
                    printError("metachar default");
                break;
            case WORD:
                if( firstValue == "setprompt" ) {
                    if(vTokens[1].getType()==STRING)
                        sPrompt = vTokens[1].getValue()+ " ";
                    else
                        printError("Usage: setprompt <string>");
                }
                else if( firstValue == "debug" ) { 
                    if(vTokens[1].getType()==WORD) {
                        if(vTokens[1].getValue()=="on"){
                            bDebugFlag = true;
                        }
                        else if (vTokens[1].getValue()=="off"){
                            bDebugFlag = false;
                        }
                        else
                            printError("Usage: debug on (or off)");
                    }
                    else
                        printError("Usage: debug on (or off)");
                    break;
                }
                else if( firstValue == "chdir" ) { 
                    string s = vTokens[1].getValue();
                    char *cstr = new char [s.length()+1];
                    strcpy (cstr, s.c_str());
                    int retValue;
                    retValue = chdir(cstr);
                    if(retValue){
                        printError("chdir error");
                    }
                }
                else {
                    //attempt to run the command
                    //i removed the error message call
                    //because i didnt like the "Error: " part
                    if(!runcmd(firstValue)){
                        cout << "No Command found: " << firstValue << endl;
                        cout << "Usage: <command> <arguments>" << endl;   

                    };
                }
                break;
            default:
                printError("firstType default");
                break;
        }
        return true;
    }
    else {
        return false;
    }
}
void printPrompt(){
    if(sCurDir == "")
    {
        cout << sPrompt << ":~$ ";
    }
    else{
        cout << sPrompt << ":~" << sCurDir << "$ ";
    }
}
int main(){

    int lookahead;
    do{
            // reset vTokens
            vTokens.clear();
            printPrompt();
            do{
                lookahead = yylex();
            } while(lookahead != ENDOFLINE);

            if(bDebugFlag){
                // possible move this stuff to a function and add more
                cout << "Tokens: " << endl;
                for( int i = 0; i < vTokens.size(); i++ ){
                    cout << "   Token " << i << ":" << endl
                     << "       Type: " << vTokens[i].getType() << endl
                     << "       Value: " << vTokens[i].getValue() << endl;

                }
            }
    } while (parser());
    
    return 0;
}

