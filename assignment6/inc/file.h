#ifndef FILE_H
#define FILE_H

#include "directory.h"

void createCmd(const char *filename);
void writeCmd(const char *filename, const char *content);
void readCmd(const char *filename);
void deleteCmd(const char *filename);
void dfCmd(void);
void cleanup(void);

#endif
