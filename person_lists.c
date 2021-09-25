#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"
#include "read_utils.h"
#include "person_lists.h"

void print_persons(int fd, Person** person_lists)
{
	int i;
	Person* p;

	for (i = 0; i < MAX_LISTS && (p = person_lists[i]) != NULL; i++) {
		do {
			write(fd, p->name, strlen(p->name));
			write(fd, "\n", 1);
			p = p->next;
		} while (p != NULL);
		write(fd, "\n", 1);
	}
}

int add_person(char* name, char* father, char* mother, Person** person_lists)
{
	if (name == NULL)
		return -1;
	
	int ret_code;
	int known_mother, known_father;
	int person_exists, mother_exists, father_exists;
	int person_list, mother_list, father_list;

	Person* p;
	Person* mother;
	Person* father;

	if (father == NULL || father[0] == '\0')
		known_father = 0;
	else
		known_father = 1;
	if (mother == NULL || mother[0] == '\0')
		known_mother = 0;
	else
		known_mother = 1;
	
	/*
	search for this person (name) through person lists
		if this person exists
			set person_list
			set pointer *p to person
			if this persons mother is set (*p)->mother
				if known_mother
					return ERROR (a person cant have two mothers)
				mother_exists = 1
				find this persons mother, store it in mother
				set mother_list
			if this persons father is set
				if known_father
					return ERROR (a person cant have two fathers)
				father_exists = 1
				find this persons father, store it in father
				set father_list

			if both mother and father exists
				return 0
			if mother exists
				if father is known
					create father
					add father to mother_list 
			else if father exists 
				if mother is known 
					create mother
					add mother to father_list
			else
				if father is known
					create father
					add father to person_list
				if mother is known
					create mother
					add mother to person_list 
		else
			create a new person, store it in p
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
						add person to newly created list
					else
						add person to mother_list
				else if mother exists
					add person to mother_list, one generation after the mother
					add father to mother_list, same generation as mother
				else if father exists
					add person to father_list, one generation after the father
					add mother to father_list, same generation as father 
				else
					create a new list
					add person to it (generation 0)
					add mother and father to it (generation -1)
			else if mother is known
				if mother exists
					add person to mother_list, one generation after the mother
				else
					create a new list
					add person to it (generation 0)
					add mother to it (generation -1)	
			else if father is set
				if father exists
					add person to father_list, one generation after the father
				else
					create a new list
					add person to it (generation 0)
					add father to it (generation -1)
			else
				create a new list
				add person to it

	*/
	return 0;
}

int read_persons(int fd, Person** person_lists)
{
	int ret_code;
	int mother_count, father_count;
	char name[MAX_LINE];

	/* if first line is not NAME
		   return ERROR (invalid first record)
	*/
	while ((ret_code = read_line(STDIN_FILENO, name)) != -1) {
		if (ret_code < -1)
			return ret_code;
		/* 
			if ret_code = 0 
				add_person()
				mother_count = 0; father_count = 0	
			else if ret_code = 1
				mother_count++
				if mother_count > 1
					return ERROR (person cant have more than one mother)
			else if ret_code = 2
				father_count++
				if father_count > 1
					return ERROR (person cant have more than one father
		*/
	}
	/* call add_person() */

	return 0;
}
