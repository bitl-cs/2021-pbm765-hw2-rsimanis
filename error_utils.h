#ifndef _ERROR_UTILS_H
#define _ERROR_UTILS_H

/* error codes */
#define		E_INVALID_KEYWORD				-2
#define		E_INVALID_NAME					-3
#define		E_PARRENT_LIMIT					-4
#define		E_PARRENT_GENERATION			-5
#define		E_PARRENT_ABOVE					-6
#define		E_INVALID_RECORD_START			-7
#define		E_MERGE_ARGUMENTS				-8
#define		E_INSERT_PERSON_ARGUMENTS		-9
#define		E_INSERT_LIST_ARGUMENTS			-10
#define		E_INSERT_LIST_FULL				-11


void print_error(int fd, int error_code);

#endif
