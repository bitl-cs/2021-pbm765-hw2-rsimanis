#ifndef _PERSON_LIST_H
#define _PERSON_LIST_H

#include "constants.h"

typedef struct _Person {
	char name[MAX_LINE];
	int generation;
	struct _Person* father;
	struct _Person* mother;
	struct _Person* next;
} Person;

void print_persons(int fd, Person** person_lists);
int add_person(char* name, char* father, char* mother, Person** person_lists);
int read_persons(int fd, Person** person_lists);

#endif
