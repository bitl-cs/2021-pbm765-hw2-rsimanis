all: hw2.exe

hw2.exe: hw2.c
	gcc -Wall -Wextra -Werror -fno-common -o hw2.exe hw2.c

clean:
	rm hw2.exe
