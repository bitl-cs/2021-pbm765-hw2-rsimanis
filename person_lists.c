#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"
#include "read_utils.h"
#include "person_lists.h"

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
	if (name == NULL)
		return -1;
	
	int ret_code;
	int known_mother, known_father;
	int mother_set, father_set;
	int person_exists, mother_exists, father_exists;
	int person_list, mother_list, father_list;

	Person* p;
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
	
	/*
	search for this person (name) through person lists
		if this person exists
			set person_list
			set pointer *p to person
			if persons mother is set (*p)->mother
				mother_set = 1
				if known_mother && mother_name != persons mother name
					return ERROR (a person cant have two mothers)
				mother_exists = 1
				store mother in the mother pointer here
				find mothers list
				set mother_list
			if this persons father is set
				father_set = 1
				if known_father && father_name != persons father name
					return ERROR (a person cant have two fathers)
				father_exists = 1
				store it in father pointer here
				find its list
				set father_list

			if both mother and father are known
				if both mother and father are set
					return 0
				else if mother is set
					if father exists
						if father generation != mother generation
							return ERROR (parents must come from the same generation)
						merge person_list and father_list
						set persons father
					else
						create father
						add father to person_list  // could just insert before mother
						set persons father
				else if father is set
					if mother exists
						if father generation != mother generation
							return ERROR (parents must come from the same generation)
						merge person_list and mother_list
						set persons mother
					else
						create mother
						add mother to person_list
						set persons mother
				else
					if both mother and father exist
						if mother_generation != father_generation
							return ERROR (parents must come from the same generation)
						if mother_list = father_list
							merge person_list with mother_list 
						else
							merge person_list, mother_list, father_list
						set persons mother and father
					else if mother exist
						if mothers generation is not one above persons generation
							return ERROR (parents must be one generation above a pesons generation)
						merge person_list and mother_list
						create father
						add father to newly created list (same generation as mother)
						set persons mother and father
					else if father exist
						if fathers generation is not one above persons generation
							return ERROR (parents must be one generation above a pesons generation)
						merge person_list and father_list
						create mother
						add mother to newly created list (same generation as father)
						set persons mother and father
					else
						create mother
						create father
						add mother to person list, one generation above person
						add father to person list, one generation above person
						set persons mother and father
			else if mother is known
				if mother is set 
					return 0
				else
					if mother exists
						if mothers generation is not one above persons generation
							return ERROR (parents must be one generation above a persons generation)
						merge person_list, mother_list
						set persons mother
					else
						create mother
						add mother to person_list, one generation above person

			else if father is known
				if father is set 
					return 0
				else
					if father exists
						if fathers generation is not one above persons generation
							return ERROR (parents must be one generation above a persons generation)
						merge person_list, father_list
						set persons father
					else
						create father
						add father to person_list, one generation above person

			else
				return 0

		else
			create a new person, store it in p
			set its name
			if mother is known
				find mother
				if mother exists
					set mother_exists = 1
					set mother to point at it
					set mother_list
			if father is known
				find father
				if father exists
					set father_exists = 1
					set father to point at it
					set father_list

			if both mother and father is known
				if both mother and father exists
					if their generations dont match
						return ERROR (parents for a single person must be from one generation)
					if mother and father are from different lists
						merge those lists.. merge(list_index1, list_index2)
						add person to newly created list, one generation above mothers generation
					else
						add person to mother_list, one generation above mothers list
					set persons mother and father
					set persons generation
				else if mother exists
					set persons generation (one below mother)
					set persons mother
					add person to mother_list
					create father
					set fathers name
					set fathers generation (same as mother)
					set persons father
					add father to mother_list
				else if father exists
					set persons generation (one below father)
					set persons father
					add person to father_list
					create mother
					set mothers name
					set mothers generation (same as father)
					set persons mother
					add mother to father_list
				else
					set persons generation (0)
					add person to data structure
					set person_list
					create mother
					set mothers name
					set mother generation (-1)
					set persons mother
					add mother to person_list 
					create father
					set fathers name
					set fathers generation (-1)
					set persons father
					add father to person_list
			else if mother is known
				if mother exists
					set persons generation (one below mother)
					set persons mother
					add person to mother_list
				else
					set persons generation (0)
					add person to data_structure
					set person_list
					create mother
					set mothers name
					set mothers generation (-1)
					set persons mother
					add mother to person_list
			else if father is known 
				if father exists
					set persons generation (one below father)
					set persons father
					add person to father_list
				else
					set persons generation (0)
					add person to data_structure
					set person_list
					create father
					set fathers name
					set fathers generation (-1)
					set persons father
					add father to person_list

			else
				set persons generation (0)
				add person to data_structure

	*/
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

/* Searches for a person with name in person_lists. Stops when encounters first empty list.
If person found, returns list index and saves this person's pointer in person.
int find_person(char* name, Person* person, Person** person_lists).
If person not found, returns -1, and sets person to NULL */
int find_person(char* name, Person** person, Person** person_lists)
{
	int i, len_needle, len_haystack;
	Person* p;

	len_needle = strlen(name);
	for (i = 0; i < MAX_LISTS && (p = person_lists[i]) != NULL; i++) {
		do {
			len_haystack = strlen(p->name);	
			if (len_needle != len_haystack) {
				p = p->next;
				continue;
			}
			if (strncmp(name, p->name, len_haystack) == 0) {
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
On error returns -1 */
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





















