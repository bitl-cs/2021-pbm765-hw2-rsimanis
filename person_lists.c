#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"
#include "read_utils.h"
#include "person_lists.h"

Person* create_person(char* name, int generation)
{
	Person* p = (Person*) malloc(sizeof(Person));
	strcpy(p->name, name);
	p->generation = generation;
	p->father = NULL;
	p->mother = NULL;
	p->next = NULL;

	return p;
}

void print_persons(int fd, Person** person_lists)
{
	int i, first;
	Person* p;

	first = 1;
	for (i = 0; i < MAX_LISTS; i++) {
		if ((p = person_lists[i]) == NULL)
			continue;
		if (!first)
			write(fd, "\n", 1);
		else if (first)
			first = 0;
		do {
			write(fd, p->name, strlen(p->name));
			write(fd, "\n", 1);
			p = p->next;
		} while (p != NULL);
	}
}

int add_person(char* name, char* father_name, char* mother_name, Person** person_lists)
{
	if (name == NULL || name[0] == '\0') 
		return -1;
	
	int ret_code, new_list;
	int known_mother, known_father;
	int mother_set, father_set;
	int mother_exists, father_exists;
	int person_list, mother_list, father_list;

	Person* person;
	Person* mother;
	Person* father;

	if (father_name == NULL || father_name[0] == '\0')
		known_father = 0;
	else
		known_father = 1;
	if (mother_name == NULL || mother_name[0] == '\0')
		known_mother = 0;
	else
		known_mother = 1;

	mother_set = 0; father_set = 0;
	mother_exists = 0; father_exists = 0; 
	person_list = -1; mother_list = -1; father_list = -1;
	person = NULL; mother = NULL; father = NULL;

	ret_code = find_person(name, &person, person_lists);
	if (ret_code >= 0) {
		person_list = ret_code;
		mother = person->mother;
		father = person->father;
		if (mother != NULL) {
			if (known_mother && !equal_names(mother_name, mother->name))
				return -1; /* error: single person cant have more than one mother and father */
			mother_set = 1;
			/* mother_exists = 1; */
			mother_list = person_list;
		}
		else if (mother_known) {
			ret_code = find_person(mother_name, &mother, person_lists);
			if (ret_code >= 0) {
				mother_list = ret_code;
				mother_exists = 1;
			}
		}
		if (father != NULL) {
			if (known_father && !equal_names(father_name, father->name))
				return -1; /* error: single person cant have more than one mother and father */
			father_set = 1;
			/* father_exists = 1; */
			father_list = person_list;
		}
		else if (father_known) {
			ret_code = find_person(father_name, &father, person_lists);
			if (ret_code >= 0) {
				father_list = ret_code;
				father_exists = 1;
			}
		}

		if (mother_known && father_known) {
			if (mother_set && father_set)
				return 0;
			if (mother_set) {
				if (father_exists) {
					if (father->generation != mother->generation)
						return -1; /* error: parents must come from the same generation */
					merge_lists(person_list, father_list);
					person->father = father;
				}
				else {
					father = create_person(father_name, mother->generation);
					insert_person(father, person_list, person_lists);
				}
			}
			else if (father_set) {
				if (mother_exists) {
					if (father->generation != mother->generation)
						return -1; /* error: parents must come from the same generation */
					merge_lists(person_list, mother_list);
					person->mother = mother;
				}
				else {
					mother = create_person(mother_name, father->generation);
					insert_person(mother, person_list, person_lists);
				}
			}
			else {
				if (mother_exist && father_exist) {
					if (father->generation != mother->generation)
						return -1; /* error: parents must come from the same generation */
					if (person->generation - father->generation != 1)
						return -1; /* error: parents must be one generation above person */
					if (mother_list == father_list) {
						if (mother_list != person_list) {
							merge_lists(mother_list, person_list);
						}
					}
					else {
						if (mother_list == person_list) {
							merge_lists(father_list, person_list);
						}
						else if (father_list == person_list) {
							merge_lists(mother_list, person_list);
						}
						else {
							ret_code = merge_lists(mother_list, person_list);
							merge_lists(ret_code, father_list);
						}
					}
				}
				else if (mother_exist) {
					if (person->generation - mother->generation != 1)
						return -1; /* error: parents must be one generation above person */
					father = create_person(father_name, mother->generation);
					insert_person(father, person_list, person_lists);
					if (mother_list != person_list) {
						merge_lists(mother_list, person_list);
					}
				} 
				else if (father_exist) {
					if (person->generation - father->generation != 1)
						return -1; /* error: parents must be one generation above person */
					mother = create_person(mother_name, father->generation);
					insert_person(mother, person_list, person_lists);
					if (father_list != person_list) {
						merge_lists(father_list, person_list);
					}
				}
				else {
					mother = create_person(mother_name, person->generation - 1);
					father = create_person(father_name, person->generation - 1);
					insert_person(mother, person_list, person_lists);
					insert_person(father, person_list, person_lists);
				}
				person->mother = m;
				person->father = father;	
			}
		}
		else if (mother_known) {
			if (!mother_set) {
				if (mother_exists) {
					if (person->generation - mother->generation != 1)
						return -1; /* error: must be one generation above */
					if (person_list != mother_list)
						merge_lists(mother_list, person_list);
				}
				else {
					mother = create_person(mother_name, person->generation - 1);
					insert_person(mother, person_list, person_lists);
				}
				person->mother = mother;
			}
		}
		else if (father_known) {
			if (!father_set) {
				if (father_exists) {
					if (person->generation - father->generation != 1)
						return -1; /* error: must be one generation above */
					if (person_list != father_list)
						merge_lists(father_list, person_list);
				}
				else {
					father = create_person(father_name, person->generation - 1);
					insert_person(father, person_list, person_lists);
				}
				person->father = father;
			}
		}
	}
	else {
		person = create_person(name, 0);
		if (mother_known) {
			ret_code = find_person(mother_name, &mother, person_lists);
			if (ret_code >= 0) {
				mother_exists = 1;
				mother_list = ret_code;
			}
		}
		if (father_known) {
			ret_code = find_person(father_name, &father, person_lists);
			if (ret_code >= 0) {
				father_exists = 1;
				father_list = ret_code;
			}
		}

		if (mother_known && father_known) {
			if (mother_exists && father_exists) {
				if (mother->generation == father->generation)
					return -1; /* error: parents cant be from same generation */
				person->generation = mother->generation + 1;
				insert_person(person, mother_list, person_lists);
				if (mother_list != father_list)
					merge_lists(mother_list, father_list);
			}
			else if (mother_exists) {
				father = create_person(father_name, mother->generation);
				person->generation = mother->generation + 1;
				insert_person(person, mother_list, person_lists);
				insert_person(father, mother_list, person_lists);
			}
			else if (father_exists) {
				mother = create_person(mother_name, father->generation);
				person->generation = father->generation + 1;
				insert_person(person, father_list, person_lists);
				insert_person(mother, father_list, person_lists);
			}
			else {
				person_list = insert_list(person, person_lists);
				mother = create_person(mother_name, person->generation - 1);	
				father = create_person(father_name, person->generation - 1);	
				insert_person(mother, person_list, person_lists);
				insert_person(father, person_list, person_lists);
			}
			person->mother = mother;
			person->father = father;
		}
		else if (mother_known) {
			if (mother_exists) {
				person->generation = mother->generation + 1;
				insert_person(person, mother_list, person_lists);
			}
			else {
				person_list = insert_list(person, person_lists);
				mother = create_person(mother_name, person->generation - 1);
				insert_person(mother, person_list, person_lists);
			}
			person->mother = mother;
		}
		else if (father_known) {
			if (father_exists) {
				person->generation = father->generation + 1;
				insert_person(person, father_list, person_lists);
			}
			else {
				person_list = insert_list(person, person_lists);
				father = create_person(father_name, person->generation - 1);
				insert_person(father, person_list, person_lists);
			}
			person->father = father;
		}
		else {
			insert_list(person);
		}
	}

	return 0;
}

int read_persons(int fd, Person** person_lists)
{
	int read_ret_code, add_ret_code;
	int mother_count, father_count;
	char name[MAX_LINE];
	char person_name[MAX_LINE];
	char mother_name[MAX_LINE];
	char father_name[MAX_LINE];

	/* if first line is not NAME
		   return ERROR (invalid first record)
	*/
	mother_name[0] = '\0'; father_name[0] = '\0';
	mother_count = 0; father_count = 0;

	read_ret_code = read_line(STDIN_FILENO, name);
	if (read_ret_code < 0)
		return read_ret_code; /* error: reading error */
	if (read_ret_code > 0)
		return -4; /* error: record must start with NAME */
	strcpy(person_name, name);

	while ((read_ret_code = read_line(STDIN_FILENO, name)) != -1) {
		if (read_ret_code < -1)
			return read_ret_code;
		if (read_ret_code == 0) {
			add_ret_code = add_person(person_name, father_name, mother_name, person_lists);
			if (add_ret_code < 0)
				return add_ret_code; /* error */
			mother_name[0] = '\0'; father_name[0] = '\0';
			mother_count = 0; father_count = 0;
			strcpy(person_name, name);
		}
		else if (read_ret_code == 1) {
			mother_count++;	
			if (mother_count > 1)
				return -5; /* error: a single person can have at most one parent of each gender */
			strcpy(mother_name, name);
		}
		else if (read_ret_code == 2) {
			father_count++;
			if (father_count > 1)
				return -5; /* error: a single person can have at most one parent of each gender */
			strcpy(father_name, name);
		}
	}
	add_ret_code = add_person(person_name, father_name, mother_name, person_lists);
	if (add_ret_code < 0)
		return add_ret_code; /* error */

	return 0;
}

/* Searches for a person with name in person_lists.
If person found, returns list index and saves this person's pointer in person.
If no person found, returns -1, and sets person to NULL */
int find_person(char* name, Person** person, Person** person_lists)
{
	int i;
	Person* p;

	if (name == NULL)
		return -1;

	for (i = 0; i < MAX_LISTS; i++) {
		if ((p = person_lists[i]) == NULL)
			continue;
		do {
			if (equal_names(name, p->name)) {
				*person = p;
				return i;
			}
			p = p->next;
		} while (p != NULL);
	}
	*person = NULL; /* maybe not needed? */
	return -1;
}


/* Merges two lists (list1, list2) in person_lists. List indexes are provided.
On error returns -1, otherwise - 0. */
int merge_lists(int list1, int list2, Person** person_lists)
{
	Person* res;
	Person* prev;
	Person* p1 = person_lists[list1];
	Person* p2 = person_lists[list2];

	if (p1 == NULL || p2 == NULL || list1 == list2)
		return -1;

	if (p1->generation <= p2->generation) {
		res = p1;
		p1 = p1->next;
	}
	else {
		res = p2;
		p2 = p2->next;
	}
	prev = res;

	while (p1 != NULL || p2 != NULL) {
		if (p1 != NULL && (p2 == NULL || p1->generation <= p2->generation)) {
			prev->next = p1;
			prev = p1;
			p1 = p1->next;
		}
		else {
			prev->next = p2;
			prev = p2;
			p2 = p2->next;
		}
	}

	person_lists[list1] = res;
	person_lists[list2] = NULL;

	return 0;
}

/* Inserts person into index-th list of person_lists. Assumes that generation is set. 
On error returns -1, otherwise - 0. */
int insert_person(Person* person, int index, Person** person_lists)
{
	int gen;
	Person* p;
	Person* prev;

	if (index < 0 || index >= MAX_LISTS)
		return -1;

	p = person_lists[index];
	if (p == NULL)
		return -1;

	gen = person->generation;
	if (gen <= p->generation) {
		person->next = p;
		person_lists[index] = person;
		return 0;
	} 
	prev = p;
	p = p->next;
	while (p != NULL) {
		if (gen <= p->generation) {
			prev->next = person;
			person->next = p;
			return 0;
		}
		prev = p;
		p = p->next;
	}
	prev->next = person;
	person->next = NULL;

	return 0;
}

/* Inserts person into person_lists. Returns list index. Returns -1 on error. Returns -2 if person_lists is full. */
int insert_list(Person* person, Person** person_lists)
{
	int i;

	if (person == NULL)
		return -1;

	for (i = 0; i < MAX_LISTS && person_lists[i] != NULL; i++)
		;
	if (i == MAX_LISTS)
		return -2;	
	person_lists[i] = person;	

	return i;
}

/* Returns 1 if name1 and name2 are equal, otherwise returns 0. */
int equal_names(char* name1, char* name2)
{
	int len1, len2;

	len1 = strlen(name1);
	len2 = strlen(name2);
	if (len1 != len2)
		return 0;
	if (strncmp(name1, name2, len1) == 0)
		return 1;
	return 0;
}















