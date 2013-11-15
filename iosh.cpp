#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
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
    bool haspipe = false;
    if(firstValue != "exit" && firstValue != "quit"){
        switch( firstType ){
            case METACHAR:
                if( firstValue == "#" )
                    return true;
                else
                    printError("metachar default");
                break;
            case WORD:
                //test for piping
                for(int i=1; i<vTokens.size()-1; i++){
                if(vTokens[i].getValue() == "<" || vTokens[i].getValue() == ">"){
                         haspipe = true;
                    }
                }
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
                } else if (haspipe) {
                   cout<<"has pipe"<<endl;
                   
                   int inmetacharpos = -1;
                   int outmetacharpos = -1;
                   int ifd;
                   int ofd;
                   //find optional input file
                   bool hasinfile = false;
                   string ifilename;
                   for(int i=0; i<vTokens.size();i++){
                        if(vTokens[i].getValue() == "<"){
                            if((i-1)>=0){
                                ifilename = vTokens[i-1].getValue();
                                hasinfile = true;
                                inmetacharpos = i;
                            } else {
                                printError("No inFile given");
                            }
                        }
                   }
                   if(hasinfile){
                        char *icstrfile = new char [ifilename.length()+1];
                        strcpy (icstrfile, ifilename.c_str());
                        if ((ifd = open(icstrfile, O_RDONLY)) < 0) { 
                            perror("inFile Open"); 
                            hasinfile = false;
                        } 
                   }

                   //find optional outfile
                   bool hasoutfile = false;
                   string ofilename;
                   for(int i=0; i<vTokens.size();i++){
                        if(vTokens[i].getValue() == ">"){
                            ofilename = vTokens[i+1].getValue();
                            hasoutfile = true;
                            outmetacharpos = i;
                        }
                   }
                   if(hasoutfile){
                        char *ocstrfile = new char [ofilename.length()+1];
                        strcpy (ocstrfile, ofilename.c_str());
                        if ((ofd = open(ocstrfile, O_WRONLY)) < 0) { 
                            perror("outFile Open"); 
                            hasoutfile = false;
                        } 
                   }
                   //infile successfully opened, build args list
                   if(hasinfile){
                       //build agrs from file;

                   }
                   //output file successfully open
                   if(hasoutfile){
                      //overwrite stdio
                       int fdhold = dup(1);
                       dup2(ofd, 1);
                      //run cmd
                        if(!runcmd(firstValue)){
                            cout << "No Command found: " << firstValue << endl;
                            cout << "Usage: <command> <arguments>" << endl;   
                        };
                      //redirect io
                       dup2(fdhold, 1);
                   }
                }
                else {
                    //attempt to run the command
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

