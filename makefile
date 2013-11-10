#OBJECT = example.o parser.o
OBJECTS = iosh.o

#HEADERS = example.h parser.h

iosh: $(OBJECTS)
	g++ -std=c++0x $^ -o iosh

clean:
	rm -rf *.o iosh

run: iosh
	./iosh
