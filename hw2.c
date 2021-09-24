#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define	MAX_LEN	256

int read_line(int fd, char* buffer);
int validate_keyword(FILE* fp, int len, char* ending);

int main()
{
	char name[MAX_LEN];

	int kw = read_line(STDIN_FILENO, name);
	printf("%d\n", kw);
	if (kw >= 0)
		printf("%s\n", name);	
	return 0;
}

int read_line(int fd, char* buffer)
{
	FILE* fp;
	int c, i, kw, ws;

	fp = fdopen(fd, "r");
	c = fgetc(fp);
	if (c == '\n' || c == EOF)
		return -1;
	if (c == 'N') {
		if (!validate_keyword(fp, 4, "AME "))
			return -2;
		kw = 0;
	}
	else if (c == 'M') {
		if (!validate_keyword(fp, 6, "OTHER "))
			return -2;
		kw = 1;
	}
	else if (c == 'F') {
		if (!validate_keyword(fp, 6, "ATHER "))
			return -2;
		kw = 2;
	}
	else
		return -2;

	c = fgetc(fp);
	if (c == ' ' || c == '\t')
		return -3;
	i = 0;
	ws = 0;
	buffer[i] = c;
	for (i = 1; (c = fgetc(fp)) != EOF && c != '\n'; i++) {
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

int validate_keyword(FILE* fp, int len, char* ending)
{
	int c, i;
	char temp[7];

	for (i = 0; i < len && (c = fgetc(fp)) != EOF && c != '\n'; i++)
		temp[i] = c;
	temp[i] = '\0';
	if (i < len || strncmp(temp, ending, len) != 0)
		return 0;
	return 1;
}






