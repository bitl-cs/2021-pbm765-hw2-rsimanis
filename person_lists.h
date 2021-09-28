#ifndef _PERSON_LIST_H
#define _PERSON_LIST_H

#define	MAX_LISTS	1000
#define	MAX_LINE	256

/* error codes */
#define		E_INVALID_KEYWORD		-2
#define		E_INVALID_NAME			-3
#define		E_PARRENT_LIMIT			-4
#define		E_PARRENT_GENERATION		-5
#define		E_PARRENT_ABOVE			-6
#define		E_INVALID_RECORD_START		-7
#define		E_MERGE_ARGUMENTS		-8
#define		E_INSERT_PERSON_ARGUMENTS	-9
#define		E_INSERT_LIST_ARGUMENTS		-10
#define		E_INSERT_LIST_FULL		-11
#define		E_LOOPS				-12
#define		E_PARRENT_TWO_GENDERS		-13


typedef struct _Person {
	char name[MAX_LINE];
	int generation;
	int gender;
	struct _Person* father;
	struct _Person* mother;
	struct _Person* next;
} Person;

Person* create_person(char* name, int generation);
void print_persons(int fd, Person** person_lists);
int add_person(char* name, char* father, char* mother, Person** person_lists);
int read_persons(int fd, Person** person_lists);
int find_person(char* name, Person** person, Person** person_lists);
int merge_lists(int final_list, int adjusted_list, int diff, Person** person_lists);
int insert_person(Person* person, int list_index, Person** person_lists);
int insert_list(Person* person, Person** person_lists);
int equal_names(char* name1, char* name2);
void free_person_lists(Person** person_lists);
void print_error(int fd, int error_code);
int read_line(int fd, char* buffer);
int validate_keyword(int fd, int len, char* ending);

#endif
