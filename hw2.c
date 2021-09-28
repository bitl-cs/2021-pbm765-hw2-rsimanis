#include <stdlib.h>
#include <unistd.h>
#include "person_lists.h"

int main()
{
	int ret_code, i;
	Person* person_lists[MAX_LISTS];

	for (i = 0; i < MAX_LISTS; i++)
		person_lists[i] = NULL;
	
	ret_code = read_persons(STDIN_FILENO, person_lists);	
	if (ret_code < -1) {
		print_error(STDOUT_FILENO, ret_code);
		return -1;
	}
	print_persons(STDOUT_FILENO, person_lists);
	free_person_lists(person_lists);

	return 0;
}


