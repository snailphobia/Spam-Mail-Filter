build: main

utils: utils.c string_functions.c
	gcc -Wall utils.c -o utils.o
	gcc -Wall string_functions.c -o string_functions.o

main: main.c utils.o string_functions.o
	gcc -Wall main.c utils.o string_functions.o -o spam_detector -lm
pack:
	zip -r cs.zip README.md cs/cs.sh cs/checkpatch.pl cs/spelling.txt cs/const_structs.checkpatch cs/tests/

clean:
	rm -f spam string_functions.o utils.o
.PHONY: build pack clean
