GCC = g++ -std=c++0x

OBJECTS = iosh.o lex.yy.o

HEADERS = global.h

iosh: $(OBJECTS)
	$(GCC) $^ -o iosh

lex.yy.o: global.h
	$(GCC) -c lex.yy.c

iosh.o: global.h iosh.cpp
	$(GCC) -c iosh.cpp

clean:
	rm -rf *.o iosh

run: iosh
	./iosh
