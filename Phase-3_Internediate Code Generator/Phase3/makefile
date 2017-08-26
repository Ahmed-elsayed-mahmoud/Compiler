all: 
	flex lex.l
	bison -y -d syntax.y
	g++ -std=c++11 lex.yy.c y.tab.c

exec: all
	./a.out

java:
	javac test.java
	javap -c test.class

error: 
	bison --verbose syntax.y

custom_run: all
	./a.out code.txt
	java -jar jasmin.jar byteCode.j
	java test

jasmine_temp:
	java -jar jasmin.jar byteCode.j

run:
	java test

exec_jasm: exec jasmine_temp run
