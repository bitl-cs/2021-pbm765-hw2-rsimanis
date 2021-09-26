all: hw2.exe

hw2.exe: hw2.o read_utils.o person_lists.o
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -o hw2.exe hw2.o read_utils.o person_lists.o

hw2.o: hw2.c
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -c hw2.c

read_utils.o: read_utils.c
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -c read_utils.c

person_lists.o: person_lists.c
	gcc -Wall -Wextra -Werror -std=c90 -fno-common -c person_lists.c

clean:
	rm hw2.exe hw2.o read_utils.o person_lists.o
