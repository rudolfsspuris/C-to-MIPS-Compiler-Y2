CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include

all : clean bin/c_compiler

debug: bin/c_compiler
	gdb --args ./bin/c_compiler --translate c_translator/formative/f10.c -o c_translator.py

src/parser.tab.cpp src/parser.tab.hpp : src/parser.y
	bison -v -d src/parser.y -o src/parser.tab.cpp

src/lexer.yy.cpp : src/lexer.flex src/parser.tab.hpp
	flex -o src/lexer.yy.cpp  src/lexer.flex

bin/c_compiler : src/c_compiler.o src/parser.tab.o src/lexer.yy.o src/parser.tab.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/c_compiler $^

clean :
	rm -f src/*.o
	rm -f bin/*
	rm -f src/*.tab.cpp
	rm -f src/*.tab.hpp
	rm -f src/*.yy.cpp
	rm -f src/parser.output
	# rm -f tmp/compiler/mips/*.s
	# rm -f tmp/compiler/*.c
	# rm -f tmp/formative/*.c
	rm -f tmp/*.c
	rm -f tmp/*.s
