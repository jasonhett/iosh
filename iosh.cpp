#include <iostream>
#include "global.h"

std::vector<token> vTokens;
std::string sPrompt = "ios$ ";
bool bDebugFlag = false;

using namespace std;

void printError(string s){
    cout << "Error: " << s << endl;
}

bool runcmd(string cmd_passed){
    //build cmd array and args
    int num_of_args =0;
    int iter =1;
    while(vTokens[iter].getType() == WORD){
        num_of_args++;
    } 
    const char* cmd = cmd_passed.c_str();
    char* args[num_of_args];
    for(int i=1; i<num_of_args;i++){
        string s = vTokens[i].getValue();
        char * cstr = new char [s.length()+1];
        strcpy (cstr, s.c_str());
        args[i-1] = cstr;
    }

    if(!fork()){   
        execvp(cmd,args);
    }
    else {
        wait(0);
    }
    return true;
}

bool parser(){
    int firstType = vTokens[0].getType();
    string firstValue = vTokens[0].getValue();
    if(firstValue != "exit"){
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
                    bDebugFlag = !bDebugFlag;
                }
                else {
                    //attempt to run the command
                    if(!runcmd(firstValue)){
                        printError("No Command found: '"+ firstValue +"' Usage: <command> <arguments>");   
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

int main(){

    int lookahead;
    do{
            // reset vTokens
            vTokens.clear();
            cout << sPrompt;
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

