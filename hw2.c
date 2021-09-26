#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "read_utils.h"
#include "person_lists.h"

int main()
{
	Person** person_lists;
	
	person_lists = (Person**) calloc(MAX_LISTS, sizeof(Person*)); 
	
	Person* a1 = (Person*) malloc(sizeof(Person)); 
	Person* a2 = (Person*) malloc(sizeof(Person)); 
	Person* a3 = (Person*) malloc(sizeof(Person)); 
	Person* a4 = (Person*) malloc(sizeof(Person)); 
	Person* a5 = (Person*) malloc(sizeof(Person)); 

	Person* b1 = (Person*) malloc(sizeof(Person)); 
	Person* b2 = (Person*) malloc(sizeof(Person)); 
	Person* b3 = (Person*) malloc(sizeof(Person)); 
	Person* b4 = (Person*) malloc(sizeof(Person)); 

	Person* c1 = (Person*) malloc(sizeof(Person)); 
	Person* c2 = (Person*) malloc(sizeof(Person)); 
	Person* c3 = (Person*) malloc(sizeof(Person)); 

	a1->generation = 5;
	a2->generation = 10;
	a3->generation = 15;
	a4->generation = 21;
	a5->generation = 26;
	b1->generation = 2;
	b2->generation = 3;
	b3->generation = 20;
	b4->generation = 25;
	c1->generation = 6;
	c2->generation = 11;
	c3->generation = 30;

	a1->next = a2;
	a2->next = a3;
	a3->next = a4;
	a4->next = a5;
	a5->next = NULL;
	b1->next = b2;
	b2->next = b3;
	b3->next = b4;
	b4->next = NULL;
	c1->next = c2;
	c2->next = c3;
	c3->next = NULL;

	strcpy(a1->name, "janis");
	strcpy(a2->name, "peteris");
	strcpy(a3->name, "andris");
	strcpy(a4->name, "aivars");
	strcpy(a5->name, "edgars");
	strcpy(b1->name, "elza");
	strcpy(b2->name, "anna");
	strcpy(b3->name, "alise");
	strcpy(b4->name, "ance");
	strcpy(c1->name, "oskars");
	strcpy(c2->name, "kristaps");
	strcpy(c3->name, "raivis");

	person_lists[0] = a1;
	person_lists[1] = b1;
	person_lists[2] = c1;

	printf("%d\n", equal_names("ab", "abc"));
	printf("%d\n", equal_names("abc", "ab"));
	printf("%d\n", equal_names("ab", "ab"));
	printf("\n");
	printf("\n");

	Person* i1 = (Person*) malloc(sizeof(Person));
	i1->generation = 22;
	strcpy(i1->name, "insert1");
	Person* i2 = (Person*) malloc(sizeof(Person));
	i2->generation = 25;
	strcpy(i2->name, "insert2");
	int list = insert_list(i1, person_lists);
	int index = insert_person(i2, list, person_lists);

	printf("%d\n", list);
	printf("%d\n", index);
	printf("\n");

	Person* found;
	index = find_person("oskars", &found, person_lists);
	printf("%d\n", index);
	if (index >= 0)
		printf("%d\n", found->generation);

	print_persons(1, person_lists);

	/*merge_lists(0, 1, person_lists);
	merge_lists(0, 2, person_lists);
	print_persons(1, person_lists);*/

	free(a1);
	free(a2);
	free(a3);
	free(a4);
	free(a5);
	free(b1);
	free(b2);
	free(b3);
	free(b4);
	free(c1);
	free(c2);
	free(c3);
	free(i1);
	free(person_lists);

	return 0;
}


