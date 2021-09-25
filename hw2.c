#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "read_utils.h"
#include "person_lists.h"

int main()
{
	/*char name[MAX_LINE];
	int kw;*/

	Person* head = (Person*) malloc(sizeof(Person));
	Person* p1 = (Person*) malloc(sizeof(Person));
	Person* p2 = (Person*) malloc(sizeof(Person));

	head->next = p1;
	p1->next = p2;
	p2->next = NULL;

	strcpy(head->name, "janis");
	strcpy(p1->name, "peteris");
	strcpy(p2->name, "andris");

	/* print_persons(1, head); */

	/*while ((kw = read_line(STDIN_FILENO, name)) >= 0) {
		printf("%d\n", kw);
		Person p;
		strcpy(p.name, name);
		printf("%s\n", p.name);	
	}
	printf("%d\n", kw);*/
	return 0;
}
