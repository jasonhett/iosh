#makefile
# 
# Source files:  vsh.cpp scanner.cpp parser.cpp token.h vsh_var.h
# 
# executable file: vsh
#


make: vsh.cpp parser.cpp scanner.o vsh_var.h token.h
	g++ -o vsh -W -Wall vsh.cpp parser.cpp scanner.o vsh_var.h token.h

scanner.o: scanner.cpp token.h
	g++ -c -W -Wall scanner.cpp token.h
	
clean:
	rm scanner.o token.h.gch vsh_var.h.gch
