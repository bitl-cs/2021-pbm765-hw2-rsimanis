#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "person_lists.h"

Person* create_person(char* name, int generation)
{
	Person* p = (Person*) malloc(sizeof(Person));
	strcpy(p->name, name);
	p->generation = generation;
	p->gender = -1;
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
	
	int ret_code;
	int mother_known, father_known; /* indicates whether mother_name or father_name (respectively) is given) */
	int mother_set, father_set; /* indicates whether a person already has parent attached to it */
	int mother_exists, father_exists; /* indicates whether a mother or father (respectively) is already present in one of family trees */
	int person_list, mother_list, father_list; /* index of a list, in which the respective person is located (exists) */

	Person* person;
	Person* mother;
	Person* father;

	if (father_name == NULL || father_name[0] == '\0')
		father_known = 0;
	else
		father_known = 1;
	if (mother_name == NULL || mother_name[0] == '\0')
		mother_known = 0;
	else
		mother_known = 1;

	mother_set = 0; father_set = 0;
	mother_exists = 0; father_exists = 0; 
	person_list = -1; mother_list = -1; father_list = -1;
	person = NULL; mother = NULL; father = NULL;

	if (mother_known && father_known)
		if (equal_names(mother_name, father_name))
			return E_PARRENT_TWO_GENDERS;
	if (mother_known) {
		if (equal_names(name, mother_name))
			return E_LOOPS;
		ret_code = find_person(mother_name, &mother, person_lists);
		if (ret_code >= 0) {
			if (mother->gender == 1)
				return E_PARRENT_TWO_GENDERS;
			mother_list = ret_code;
			mother_exists = 1;
		}
	}
	if (father_known) {
		if (equal_names(name, father_name))
			return E_LOOPS;
		ret_code = find_person(father_name, &father, person_lists);
		if (ret_code >= 0) {
			if (father->gender == 0)
				return E_PARRENT_TWO_GENDERS;
			father_list = ret_code;
			father_exists = 1;
		}
	}

	ret_code = find_person(name, &person, person_lists);
	if (ret_code >= 0) {
		person_list = ret_code;
		if (person->mother != NULL) {
			if (mother_known && !equal_names(mother_name, person->mother->name))
				return E_PARRENT_LIMIT; /* error: single person cant have more than one mother and father */
			mother_set = 1;
		}
		if (person->father != NULL) {
			if (father_known && !equal_names(father_name, person->father->name))
				return E_PARRENT_LIMIT; /* error: single person cant have more than one mother and father */
			father_set = 1;
		}

		if (mother_known && father_known) {
			if (mother_set && father_set)
				return 0;
			if (mother_set) {
				if (father_exists) {
					if (father_list == person_list) {
						if (person->generation - father->generation != 1)
							return E_PARRENT_ABOVE; /* error: parents must come from the same generation */
					}
					else {
						ret_code = merge_lists(person_list, father_list, person->generation - father->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
				}
				else {
					father = create_person(father_name, person->generation - 1);
					ret_code = insert_person(father, person_list, person_lists);
					if (ret_code < 0)
						return ret_code;
				}
				person->father = father;
				father->gender = 1;
			}
			else if (father_set) {
				if (mother_exists) {
					if (mother_list == person_list) {
						if (person->generation - mother->generation != 1)
							return E_PARRENT_ABOVE; /* error: parents must come from the same generation */
					}
					else {
						ret_code = merge_lists(person_list, mother_list, person->generation - mother->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
				}
				else {
					mother = create_person(mother_name, person->generation - 1);
					ret_code = insert_person(mother, person_list, person_lists);
					if (ret_code < 0)
						return ret_code;
				}
				person->mother = mother;
				mother->gender = 0;
			}
			else {
				if (mother_exists && father_exists) {
					if (mother_list == person_list && father_list == person_list) {
						if (person->generation - father->generation != 1)
							return E_PARRENT_ABOVE; /* error: parents must be one generation above person */
						if (person->generation - mother->generation != 1)
							return E_PARRENT_ABOVE; /* error: parents must be one generation above person */
					}
					else if (mother_list == person_list) {
						if (person->generation - mother->generation != 1)
							return E_PARRENT_ABOVE; /* error: parents must be one generation above person */
						ret_code = merge_lists(person_list, father_list, person->generation - father->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
					else if (father_list == person_list) {
						if (person->generation - father->generation != 1)
							return E_PARRENT_ABOVE; /* error: parents must be one generation above person */
						ret_code = merge_lists(person_list, mother_list, person->generation - mother->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
					else {
						ret_code = merge_lists(person_list, mother_list, person->generation - mother->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
						ret_code = merge_lists(person_list, father_list, person->generation - father->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
						
				}
				else if (mother_exists) {
					father = create_person(father_name, person->generation - 1);
					ret_code = insert_person(father, person_list, person_lists);
					if (ret_code < 0)
						return ret_code;

					if (mother_list == person_list) {
						if (person->generation - mother->generation != 1)
							return E_PARRENT_ABOVE; /* error: parents must be one generation above person */
					}
					else {
						ret_code = merge_lists(person_list, mother_list, person->generation - mother->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
				} 
				else if (father_exists) {
					mother = create_person(mother_name, person->generation - 1);
					ret_code = insert_person(mother, person_list, person_lists);
					if (ret_code < 0)
						return ret_code;

					if (father_list == person_list) {
						if (person->generation - father->generation != 1)
							return E_PARRENT_ABOVE; /* error: parents must be one generation above person */
					}
					else {
						ret_code = merge_lists(person_list, father_list, person->generation - father->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
				}
				else {
					mother = create_person(mother_name, person->generation - 1);
					father = create_person(father_name, person->generation - 1);
					ret_code = insert_person(mother, person_list, person_lists);
					if (ret_code < 0)
						return ret_code;
					ret_code = insert_person(father, person_list, person_lists);
					if (ret_code < 0)
						return ret_code;
				}
				person->mother = mother;
				person->father = father;	
				mother->gender = 0;
				father->gender = 1;
			}
		}
		else if (mother_known) {
			if (!mother_set) {
				if (mother_exists) {
					if (mother_list == person_list) {
						if (person->generation - mother->generation != 1)
							return E_PARRENT_ABOVE; /* error: must be one generation above */
					}
					else {
						ret_code = merge_lists(person_list, mother_list, person->generation - mother->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
				}
				else {
					mother = create_person(mother_name, person->generation - 1);
					ret_code = insert_person(mother, person_list, person_lists);
					if (ret_code < 0)
						return ret_code;
				}
				person->mother = mother;
				mother->gender = 0;
			}
		}
		else if (father_known) {
			if (!father_set) {
				if (father_exists) {
					if (father_list == person_list) {
						if (person->generation - father->generation != 1)
							return E_PARRENT_ABOVE; /* error: must be one generation above */
					}
					else {
						ret_code = merge_lists(person_list, father_list, person->generation - father->generation - 1, person_lists);
						if (ret_code < 0)
							return ret_code;
					}
				}
				else {
					father = create_person(father_name, person->generation - 1);
					ret_code = insert_person(father, person_list, person_lists);
					if (ret_code < 0)
						return ret_code;
				}
				person->father = father;
				father->gender = 1;
			}
		}
	}
	else {
		person = create_person(name, 0);
		if (mother_known && father_known) {
			if (mother_exists && father_exists) {
				person->generation = mother->generation + 1;
				ret_code = insert_person(person, mother_list, person_lists);
				if (ret_code < 0)
					return ret_code;
				if (mother_list == father_list) {
					if (mother->generation != father->generation)
						return E_PARRENT_GENERATION; /* error: parents must be from same generation */
				}
				else {
					ret_code = merge_lists(mother_list, father_list, person->generation - father->generation - 1, person_lists);
					if (ret_code < 0)
						return ret_code;
				}
			}
			else if (mother_exists) {
				father = create_person(father_name, mother->generation);
				person->generation = mother->generation + 1;
				ret_code = insert_person(person, mother_list, person_lists);
				if (ret_code < 0)
					return ret_code;
				ret_code = insert_person(father, mother_list, person_lists);
				if (ret_code < 0)
					return ret_code;
			}
			else if (father_exists) {
				mother = create_person(mother_name, father->generation);
				person->generation = father->generation + 1;
				ret_code = insert_person(person, father_list, person_lists);
				if (ret_code < 0)
					return ret_code;
				ret_code = insert_person(mother, father_list, person_lists);
				if (ret_code < 0)
					return ret_code;
			}
			else {
				person_list = insert_list(person, person_lists);
				if (person_list < 0)
					return person_list;
				mother = create_person(mother_name, person->generation - 1);	
				father = create_person(father_name, person->generation - 1);	
				ret_code = insert_person(mother, person_list, person_lists);
				if (ret_code < 0)
					return ret_code;
				ret_code = insert_person(father, person_list, person_lists);
				if (ret_code < 0)
					return ret_code;
			}
			person->mother = mother;
			person->father = father;
			mother->gender = 0;
			father->gender = 1;
		}
		else if (mother_known) {
			if (mother_exists) {
				person->generation = mother->generation + 1;
				ret_code = insert_person(person, mother_list, person_lists);
				if (ret_code < 0)
					return ret_code;
			}
			else {
				person_list = insert_list(person, person_lists);
				if (person_list < 0)
					return person_list;
				mother = create_person(mother_name, person->generation - 1);
				ret_code = insert_person(mother, person_list, person_lists);
				if (ret_code < 0)
					return ret_code;
			}
			person->mother = mother;
			mother->gender = 0;
		}
		else if (father_known) {
			if (father_exists) {
				person->generation = father->generation + 1;
				ret_code = insert_person(person, father_list, person_lists);
				if (ret_code < 0)
					return ret_code;
			}
			else {
				person_list = insert_list(person, person_lists);
				if (person_list < 0)
					return person_list;
				father = create_person(father_name, person->generation - 1);
				ret_code = insert_person(father, person_list, person_lists);
				if (ret_code < 0)
					return ret_code;
			}
			person->father = father;
			father->gender = 1;
		}
		else {
			ret_code = insert_list(person, person_lists);
			if (ret_code < 0)
				return ret_code;
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

	mother_name[0] = '\0'; father_name[0] = '\0';
	mother_count = 0; father_count = 0;

	read_ret_code = read_line(fd, name);
	if (read_ret_code < 0)
		return read_ret_code; /* error: reading error */
	if (read_ret_code > 0)
		return E_INVALID_RECORD_START; /* error: record must start with NAME */
	strcpy(person_name, name);

	while ((read_ret_code = read_line(fd, name)) != -1) {
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
				return E_PARRENT_LIMIT; /* error: a single person can have at most one parent of each gender */
			strcpy(mother_name, name);
		}
		else if (read_ret_code == 2) {
			father_count++;
			if (father_count > 1)
				return E_PARRENT_LIMIT; /* error: a single person can have at most one parent of each gender */
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

/* merges final_list and adjusted_list into one list by incrementing each person's generation by diff inside adjusted_list */
int merge_lists(int final_list, int adjusted_list, int diff, Person** person_lists)
{
	Person* res;
	Person* prev;
	Person* fl = person_lists[final_list];
	Person* al = person_lists[adjusted_list];

	if (fl == NULL || al == NULL || final_list == adjusted_list)
		return E_MERGE_ARGUMENTS; /* error */

	if (fl->generation <= al->generation + diff) {
		res = fl;
		fl = fl->next;
	}
	else {
		res = al;
		al->generation += diff;
		al = al->next;
	}
	prev = res;

	while (fl != NULL || al != NULL) {
		if (fl != NULL && (al == NULL || fl->generation <= al->generation + diff)) {
			prev->next = fl;
			prev = fl;
			fl = fl->next;
		}
		else {
			prev->next = al;
			prev = al;
			al->generation += diff;
			al = al->next;
		}
	}

	person_lists[final_list] = res;
	person_lists[adjusted_list] = NULL;

	return 0;
}

/* Inserts person into index-th list of person_lists. Assumes that generation is set. 
On error returns negative value, otherwise - 0. */
int insert_person(Person* person, int index, Person** person_lists)
{
	int gen;
	Person* p;
	Person* prev;

	if (index < 0 || index >= MAX_LISTS)
		return E_INSERT_PERSON_ARGUMENTS; /* error */

	p = person_lists[index];
	if (p == NULL)
		return E_INSERT_PERSON_ARGUMENTS; /* error */

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

/* Inserts person into person_lists. Returns list index. On error returns negative value */
int insert_list(Person* person, Person** person_lists)
{
	int i;

	if (person == NULL)
		return E_INSERT_LIST_ARGUMENTS;

	for (i = 0; i < MAX_LISTS && person_lists[i] != NULL; i++)
		;
	if (i == MAX_LISTS)
		return E_INSERT_LIST_FULL;	
	person_lists[i] = person;	

	return i;
}

/* Returns 1 if name1 and name2 are equal, otherwise - 0. */
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

/* deallocates memory for lists inside person_lists */
void free_person_lists(Person** person_lists)
{
	int i;
	Person* p; 
	Person* temp;

	for (i = 0; i < MAX_LISTS; i++) {
		if ((p = person_lists[i]) == NULL)
			continue;
		do {
			temp = p->next;
			free(p);
			p = temp;
		} while (p != NULL);
	}
}


int read_line(int fd, char* buffer)
{
	char c;
	int i, kw, ws;

	if (read(fd, &c, 1) == 0)
		return -1;
	while (c == '\n')
		if (read(fd, &c, 1) == 0)
			return -1;

	if (c == 'N') {
		if (!validate_keyword(fd, 3, "AME"))
			return E_INVALID_KEYWORD;
		kw = 0;
	}
	else if (c == 'M') {
		if (!validate_keyword(fd, 5, "OTHER"))
			return E_INVALID_KEYWORD;
		kw = 1;
	}
	else if (c == 'F') {
		if (!validate_keyword(fd, 5, "ATHER"))
			return E_INVALID_KEYWORD;
		kw = 2;
	}
	else
		return E_INVALID_KEYWORD;

	if (read(fd, &c, 1) == 0 || c != ' ')
		return E_INVALID_NAME;
	if (read(fd, &c, 1) == 0 || c == '\n' || c == ' ' || c == '\t')
		return E_INVALID_NAME;
	ws = 0;
	buffer[0] = c;
	for (i = 1; read(fd, &c, 1) != 0 && c != '\n'; i++) {
		if (c == ' ') {
			ws++;
			if (ws > 1)
				return E_INVALID_NAME;
		}
		else if (ws > 0)
			ws = 0;
		buffer[i] = c;
	}
	if (ws > 0)
		return E_INVALID_NAME;
	buffer[i] = '\0'; 	

	return kw;
}

/* helper function for read_line() */
int validate_keyword(int fd, int len, char* ending)
{
	int c, i;
	char temp[7];

	for (i = 0; i < len && read(fd, &c, 1) != 0 && c != '\n'; i++)
		temp[i] = c;
	temp[i] = '\0';
	if (i < len || strncmp(temp, ending, len) != 0)
		return 0;
	return 1;
}

/* prints error message that corresponds to error_code to file descriptor fd */
void print_error(int fd, int error_code)
{
	char* error_msg;

	switch (error_code) {
		case E_INVALID_KEYWORD:
			error_msg = "Invalid keyword";
			break;
		case E_INVALID_NAME:
			error_msg = "Invalid person name";
			break;
		case E_PARRENT_LIMIT:
			error_msg = "Each person can have at most one parent of each gender";
			break;
		case E_PARRENT_GENERATION:
			error_msg = "Both parents must come from the same generation";
			break;
		case E_PARRENT_ABOVE:
			error_msg = "Parents must be one generation above person";
			break;
		case E_INVALID_RECORD_START:
			error_msg = "Person record must start with keyword NAME";
			break;
		case E_MERGE_ARGUMENTS:
			error_msg = "merge_lists(): Invalid arguments";
			break;
		case E_INSERT_PERSON_ARGUMENTS:
			error_msg = "insert_person(): Invalid arguments";
			break;
		case E_INSERT_LIST_ARGUMENTS:
			error_msg = "insert_list(): Invalid arguments";
			break;
		case E_INSERT_LIST_FULL:
			error_msg = "insert_list(): Person lists data structure is full";
			break;
		case E_LOOPS:
			error_msg = "A person can't be an ancestor of themselves";
			break;
		case E_PARRENT_TWO_GENDERS:
			error_msg = "A single person cannot be both father and mother";
			break;
		default:
			error_msg = "Couldn't find a matching error message";
	}

	write(fd, error_msg, strlen(error_msg));
	write(fd, "\n", 1);
}
