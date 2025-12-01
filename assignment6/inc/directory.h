#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TOTAL_BLOCKS 1024
#define BLOCK_SIZE 512

typedef struct FileNode
{
    char name[50];
    char *data;
    int size;
    struct FileNode *next;
} FileNode;

typedef struct DirectoryNode
{
    char name[50];
    struct DirectoryNode *parent;
    struct DirectoryNode *subDirs;
    struct DirectoryNode *next;
    FileNode *files;
} DirectoryNode;

extern DirectoryNode *root;
extern DirectoryNode *cwd;

void mkdirCmd(const char *dirname);
void rmdirCmd(const char *dirname);
void lsCmd(void);
void cdCmd(const char *dirname);
void pwdCmd(void);

#endif
