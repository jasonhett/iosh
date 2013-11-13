#include <iostream>
#include "global.h"

std::vector<token> vTokens;
std::string sPrompt = "ios$ ";
bool bDebugFlag = false;

using namespace std;

void printError(string s){
    cout << "Error: " << s << endl;
}

void parser(){
    int firstType = vTokens[0].getType();
    string firstValue = vTokens[0].getValue();

    switch( firstType ){
        case METACHAR:
            if( firstValue == "#" )
                return;
            else
                printError("metachar default");
            break;
        case WORD:
            if( firstValue == "setprompt" ) {
                if(vTokens[1].getType()==STRING)
                    sPrompt = vTokens[1].getValue();
                else
                    printError("Usage: setprompt <string>");
            }
            else if( firstValue == "debug" ) 
                bDebugFlag = !bDebugFlag;
            break;
        default:
            printError("firstType default");
            break;
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
            parser();
    } while (vTokens[0].getValue()!="exit");
    
    return 0;
}

