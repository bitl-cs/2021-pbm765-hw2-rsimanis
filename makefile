all: hw2.exe

hw2.exe: hw2.o read_utils.o error_utils.o person_lists.o
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -o hw2.exe error_utils.o hw2.o read_utils.o person_lists.o

hw2.o: hw2.c
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -c hw2.c

read_utils.o: read_utils.c
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -c read_utils.c

error_utils.o: error_utils.c
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -c error_utils.c

person_lists.o: person_lists.c
	gcc -std=c90 -Wall -Wextra -Werror -fno-common -c person_lists.c

clean:
	rm hw2.exe hw2.o read_utils.o error_utils.o person_lists.o
