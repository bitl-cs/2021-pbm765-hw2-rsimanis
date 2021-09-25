#ifndef _PERSON_LIST_H
#define _PERSON_LIST_H

#include "constants.h"

typedef struct _Person {
	int generation;
	int known_mother;
	int known_father;
	char name[MAX_LEN];
	struct _Person* next;
} Person;

void print_persons(int fd, Person** person_lists);
int add_person(char* name, char* father, char* mother, Person** person_lists);
int read_persons(int fd, Person** person_lists);

#endif
