main: main.o ls.o pinfo.o parseexec.o autocomplete.o redirect.o
	g++ main.o ls.o pinfo.o  parseexec.o  autocomplete.o redirect.o -o main
main.o: main.cpp
	g++ -c main.cpp
ls.o:	ls.cpp
	g++ -c ls.cpp
pinfo.o: pinfo.cpp
	g++ -c pinfo.cpp
parseexec.o: parseexec.cpp	
	g++ -c parseexec.cpp
autocomplete.o: autocomplete.cpp
	g++ -c autocomplete.cpp
redirect.o: redirect.cpp
	g++ -c redirect.cpp

clean:
	rm -rf *.o
