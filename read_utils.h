#ifndef _READ_UTILS_H
#define _READ_UTILS_H

int read_line(int fd, char* buffer);
int validate_keyword(int fd, int len, char* ending);

#endif
