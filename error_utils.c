#include <unistd.h>
#include <string.h>
#include "error_utils.h"

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
			error_msg = "Parent must be generation above person";
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
		default:
			error_msg = "Couldn't find a matching error message";
	}

	write(fd, error_msg, strlen(error_msg));
	write(fd, "\n", 1);
}
