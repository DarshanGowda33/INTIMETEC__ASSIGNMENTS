#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

typedef enum
{
    CMD_MKDIR,
    CMD_RMDIR,
    CMD_CREATE,
    CMD_WRITE,
    CMD_READ,
    CMD_DELETE,
    CMD_LS,
    CMD_CD,
    CMD_PWD,
    CMD_DF,
    CMD_EXIT,
    CMD_INVALID
} CommandType;

typedef struct FreeBlock
{
    int id;
    struct FreeBlock *prev;
    struct FreeBlock *next;
} FreeBlock;

typedef struct FileNode
{
    char *name;
    int isDirectory;
    int size;
    char **dataBlocks;
    int blockCount;
    struct FileNode *child;
    struct FileNode *sibling;
    struct FileNode *parent;
} FileNode;

extern char *virtualDisk;
extern FreeBlock *freeListHead;
extern FileNode *root;
extern FileNode *cwd;

void initializeFileSystem();
void cleanup();

void mkdirCmd(const char *name);
void rmdirCmd(const char *name);
void createCmd(const char *name);
void writeCmd(const char *name, const char *content);
void readCmd(const char *name);
void deleteCmd(const char *name);
void lsCmd();
void cdCmd(const char *name);
void pwdCmd();
void dfCmd();

CommandType parseCommand(const char *cmd);
void executeCommand(CommandType cmdType, char *arg1, char *arg2);
