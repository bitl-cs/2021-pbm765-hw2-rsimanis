all: hw2.exe

hw2.exe: hw2.o person_lists.o
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -o hw2.exe hw2.o person_lists.o

hw2.o: hw2.c
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -c hw2.c

person_lists.o: person_lists.c
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -c person_lists.c

clean:
	rm hw2.exe hw2.o person_lists.o
