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

bool runcmd(int cmd_poss){
    //build cmd array and args
    string cmd_passed = vTokens[cmd_poss].getValue();
    int num_of_args = 2;
    int iter = cmd_poss+1;
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
    for(int i=cmd_poss+1; i<num_of_args-1;i++){
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
    int commandpos =0;
    bool haspipe = false;
    
    if(firstValue != "exit" && firstValue != "quit"){
        switch( firstType ){
            case ENDOFFILE:
                return false;
            case METACHAR:
                if( firstValue == "#" )
                    return true;
                else
                    printError("metachar default");
                break;
            case 260:
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
                                commandpos = i+1;
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
                        }
                   }
                   if(hasoutfile){
                        char *ocstrfile = new char [ofilename.length()+1];
                        strcpy (ocstrfile, ofilename.c_str());
                        
                        //create file always
                        FILE * pFile;
                        pFile = fopen(ocstrfile, "w");
                        fclose(pFile);

                        if ((ofd = open(ocstrfile, O_WRONLY)) < 0) { 
                            perror("outFile Open"); 
                            hasoutfile = false;
                        } 
                   }
                   //infile successfully opened, build args list
                   if(hasinfile && !hasoutfile){
                       int fdhold = dup(0);
                       dup2(ifd, 0);
                        if(!runcmd(commandpos)){
                            cout << "No Command found: " << firstValue << endl;
                            cout << "Usage: <command> <arguments>" << endl;   
                        };
                      //redirect io
                       dup2(fdhold, 0);
                   }
                   //output file successfully open
                   if(hasoutfile && !hasinfile){
                       int fdhold = dup(1);
                       dup2(ofd, 1);
                        if(!runcmd(commandpos)){
                            cout << "No Command found: " << firstValue << endl;
                            cout << "Usage: <command> <arguments>" << endl;   
                        };
                      //redirect io
                       dup2(fdhold, 1);
                   }
                   if(hasinfile && hasoutfile){

                       //read file into memory
                       char buf[512]; 
                       int nbytes; /* number of bytes read */ 
                       /* Open file fd ... */ 
                       /* Then read up to 512 bytes from file fd */ 
                       if ((nbytes = read(ifd, buf, sizeof(buf))) < 0) { 
                            perror("inFile read"); 
                       } 

                       int fdhold = dup(1);
                       int fdhold2 = dup(0);
                       dup2(ofd, 1);
                       dup2(ifd, 0);
                        if(!runcmd(commandpos)){
                            cout << "No Command found: " << firstValue << endl;
                            cout << "Usage: <command> <arguments>" << endl;   
                        };
                      //redirect io
                       dup2(fdhold, 1);
                       dup2(fdhold2, 0);
                   }
                }
                else {
                    //attempt to run the command
                    if(!runcmd(commandpos)){
                        cout << "No Command found: " << firstValue << endl;
                        cout << "Usage: <command> <arguments>" << endl;   
                    };
                }
                break;
            default:
                cout<<firstValue;
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
            } while(lookahead != ENDOFLINE && lookahead !=0);

            if(bDebugFlag){
                // possible move this stuff to a function and add more
                cout << "Tokens: " << endl;
                for( int i = 0; i < vTokens.size(); i++ ){
                    cout << "   Token " << i << ":" << endl
                     << "       Type: " << vTokens[i].getType() << endl
                     << "       Value: " << vTokens[i].getValue() << endl;

                }
            }
    } while (parser()&& lookahead!=0);
    
    return 0;
}

