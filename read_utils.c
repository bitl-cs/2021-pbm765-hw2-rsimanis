#include <string.h>
#include <unistd.h>
#include "read_utils.h"

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
		if (!validate_keyword(fd, 4, "AME "))
			return -2;
		kw = 0;
	}
	else if (c == 'M') {
		if (!validate_keyword(fd, 6, "OTHER "))
			return -2;
		kw = 1;
	}
	else if (c == 'F') {
		if (!validate_keyword(fd, 6, "ATHER "))
			return -2;
		kw = 2;
	}
	else
		return -2;

	if (read(fd, &c, 1) == 0 || c == '\n' || c == ' ' || c == '\t')
		return -3;
	ws = 0;
	buffer[0] = c;
	for (i = 1; read(fd, &c, 1) != 0 && c != '\n'; i++) {
		if (c == ' ') {
			ws++;
			if (ws > 1)
				return -3;
		}
		else if (ws > 0)
			ws = 0;
		buffer[i] = c;
	}
	if (ws > 0)
		return -3;
	buffer[i] = '\0'; 	

	return kw;
}

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
