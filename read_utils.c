#include <string.h>
#include <unistd.h>
#include "read_utils.h"
#include "error_utils.h"

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
			return E_INVALID_KEYWORD;
		kw = 0;
	}
	else if (c == 'M') {
		if (!validate_keyword(fd, 6, "OTHER "))
			return E_INVALID_KEYWORD;
		kw = 1;
	}
	else if (c == 'F') {
		if (!validate_keyword(fd, 6, "ATHER "))
			return E_INVALID_KEYWORD;
		kw = 2;
	}
	else
		return E_INVALID_KEYWORD;

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
