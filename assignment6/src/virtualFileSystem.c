#include "../inc/virtualFileSystem.h"

char *virtualDisk = NULL;
FreeBlock *freeListHead = NULL;
FileNode *root = NULL;
FileNode *cwd = NULL;
static FreeBlock *freeListTail = NULL;

void initializeFreeList()
{
    FreeBlock *prev = NULL;
    for (int index = 0; index < NUM_BLOCKS; index++)
    {
        FreeBlock *node = (FreeBlock *)malloc(sizeof(FreeBlock));
        node->id = index;
        node->prev = prev;
        node->next = NULL;
        if (!freeListHead)
        {
            freeListHead = node;
        }
        if (prev)
        {
            prev->next = node;
        }
        prev = node;
    }
    freeListTail = prev;
}

void initializeFileSystem()
{
    virtualDisk = (char *)malloc(BLOCK_SIZE * NUM_BLOCKS);
    if (!virtualDisk)
    {
        printf("Virtual disk allocation failed.\n");
        exit(1);
    }

    initializeFreeList();

    root = (FileNode *)calloc(1, sizeof(FileNode));
    if (!root)
    {
        printf("Root directory allocation failed.\n");
        exit(1);
    }

    root->name = strdup("/");
    if (!root->name)
    {
        printf("Memory allocation failed for root name.\n");
        free(root);
        exit(1);
    }

    root->isDirectory = 1;
    root->dataBlocks = NULL;
    root->blockCount = 0;
    root->size = 0;
    root->child = NULL;
    root->sibling = NULL;
    root->parent = NULL;

    cwd = root;
    printf("Compact VFS ready. Type 'exit' to quit.\n");
}


void cleanup()
{
    FreeBlock *cur = freeListHead;
    while (cur)
    {
        FreeBlock *temp = cur;
        cur = cur->next;
        free(temp);
    }
    free(virtualDisk);
    printf("Memory released. Exiting program...\n");
}

FileNode *findNode(const char *name)
{
    FileNode *child = cwd->child;
    while (child)
    {
        if (strcmp(child->name, name) == 0)
        {
            return child;
        }
        child = child->sibling;
    }
    return NULL;
}

void insertNode(FileNode *newNode)
{
    if (!cwd->child)
    {
        cwd->child = newNode;
    }
    else
    {
        FileNode *temp = cwd->child;
        while (temp->sibling)
        {
            temp = temp->sibling;
        }
        temp->sibling = newNode;
    }
}

void mkdirCmd(const char *name)
{
    if (findNode(name))
    {
        printf("Name already exists in current directory.\n");
        return;
    }
    FileNode *directory = (FileNode *)malloc(sizeof(FileNode));
    directory->name = strdup(name);
    directory->isDirectory = 1;
    directory->dataBlocks = NULL;
    directory->blockCount = 0;
    directory->size = 0;
    directory->child = NULL;
    directory->sibling = NULL;
    directory->parent = cwd;
    insertNode(directory);
    printf("Directory '%s' created successfully.\n", name);
}

void rmdirCmd(const char *name)
{
    FileNode *target = findNode(name);
    if (!target || !target->isDirectory)
    {
        printf("Directory not found.\n");
        return;
    }
    if (target->child)
    {
        printf("Directory not empty.Remove files first\n");
        return;
    }

    FileNode *cur = cwd->child;
    FileNode *prev = NULL;
    while (cur)
    {
        if (cur == target)
        {
            if (prev)
            {
                prev->sibling = cur->sibling;
            }
            else
            {
                cwd->child = cur->sibling;
            }
            free(cur->name);
            free(cur);
            printf("Directory removed successfully.\n");
            return;
        }
        prev = cur;
        cur = cur->sibling;
    }
}

void createCmd(const char *name)
{
    if (findNode(name))
    {
        printf("File name already exists in current directory.\n");
        return;
    }
    FileNode *file = (FileNode *)malloc(sizeof(FileNode));
    if (!file)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    file->name = strdup(name);
    file->isDirectory = 0;
    file->dataBlocks = NULL;
    file->blockCount = 0;
    file->size = 0;
    file->child = NULL;
    file->sibling = NULL;
    file->parent = cwd;

    insertNode(file);
    printf("File '%s' created successfully.\n", name);
}

void writeCmd(const char *name, const char *content)
{
    FileNode *file = findNode(name);
    if (!file || file->isDirectory)
    {
        printf("File not found.\n");
        return;
    }

    int contentSize = strlen(content);
    int totalSize = contentSize;
    int blocksNeeded = (contentSize + BLOCK_SIZE - 1) / BLOCK_SIZE;

    file->dataBlocks = (char **)malloc(blocksNeeded * sizeof(char *));
    file->blockCount = blocksNeeded;

    char *src = (char *)content;
    for (int index = 0; index < blocksNeeded; index++)
    {
        if (!freeListHead)
        {
            printf("Disk full.\n");
            return;
        }

        FreeBlock *block = freeListHead;
        freeListHead = freeListHead->next;
        if (freeListHead)
        {
            freeListHead->prev = NULL;
        }
        char *dest = virtualDisk + (block->id * BLOCK_SIZE);
        file->dataBlocks[index] = dest;
        int bytesThisBlock = (contentSize > BLOCK_SIZE) ? BLOCK_SIZE : contentSize;
        for (int counter = 0; counter < bytesThisBlock; counter++)
        {
            *(dest + counter) = *(src + counter);
        }
        src += bytesThisBlock;
        contentSize -= bytesThisBlock;

        free(block);
    }

    file->size = totalSize;
    printf("Data written successfully (size = %d bytes).\n", file->size);
}

void readCmd(const char *name)
{
    FileNode *file = findNode(name);
    if (!file || file->isDirectory || !file->dataBlocks)
    {
        printf("File not found or empty.\n");
        return;
    }

    int remaining = file->size;  

    for (int index = 0; index < file->blockCount; index++)
    {
        char *ptr = file->dataBlocks[index];
        int bytesThisBlock = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;

        for (int counter = 0; counter < bytesThisBlock; counter++)
        {
            putchar(*(ptr + counter));
        }
        remaining -= bytesThisBlock;
    }
    putchar('\n');
}

void deleteCmd(const char *name)
{
    FileNode *file = findNode(name);
    if (!file || file->isDirectory)
    {
        printf("File not found.\n");
        return;
    }

    if (file->dataBlocks && file->blockCount > 0)
    {
        for (int index = 0; index < file->blockCount; index++)
        {
            long offset = *(file->dataBlocks + index) - virtualDisk;
            int blockIndex = (int)(offset / BLOCK_SIZE);

            FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));
            block->id = blockIndex;
            block->prev = freeListTail;
            block->next = NULL;

            if (freeListTail)
            {
                freeListTail->next = block;
            }
            else
            {
                freeListHead = block;
            }
            freeListTail = block;
        }

        free(file->dataBlocks);
        file->dataBlocks = NULL;
        file->blockCount = 0;
    }

    FileNode *cur = cwd->child;
    FileNode *prev = NULL;
    while (cur)
    {
        if (cur == file)
        {
            if (prev)
            {
                prev->sibling = cur->sibling;
            }
            else
            {
                cwd->child = cur->sibling;
            }
            free(cur->name);
            free(cur);
            printf("File deleted successfully.\n");
            return;
        }
        prev = cur;
        cur = cur->sibling;
    }
}

void lsCmd()
{
    FileNode *child = cwd->child;
    if (!child)
    {
        printf("(empty)\n");
        return;
    }

    while (child)
    {
        printf("%s%s\n", child->name, child->isDirectory ? "/" : "");
        child = child->sibling;
    }
}

void cdCmd(const char *name)
{
    if (strcmp(name, "..") == 0)
    {
        if (cwd->parent)
        {
            cwd = cwd->parent;
        }
        printf("Moved up.\n");
        return;
    }

    FileNode *dir = findNode(name);
    if (!dir || !dir->isDirectory)
    {
        printf("Directory not found.\n");
        return;
    }
    cwd = dir;
    printf("Moved to %s.\n", name);
}

void pwdCmd()
{
    const FileNode *stack[256];
    int depth = 0;
    const FileNode *temp = cwd;

    while (temp)
    {
        stack[depth++] = temp;
        temp = temp->parent;
    }
    printf("/");
    for (int index = depth - 2; index >= 0; index--)
    {
        printf("%s", stack[index]->name);
        if (index > 0)
        {
            printf("/");
        }
    }
    printf("\n");
}


void dfCmd()
{
    int freeCount = 0;
    FreeBlock *temp = freeListHead;

    while (temp)
    {
        freeCount++;
        temp = temp->next;
    }

    int usedBlocks = NUM_BLOCKS - freeCount;
    double usagePercent = ((double)usedBlocks / NUM_BLOCKS) * 100.0;

    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeCount);
    printf("Disk Usage: %.2f%%\n", usagePercent);
}

CommandType parseCommand(const char *cmd)
{
    if (strcmp(cmd, "mkdir") == 0)
        return CMD_MKDIR;
    if (strcmp(cmd, "rmdir") == 0)
        return CMD_RMDIR;
    if (strcmp(cmd, "create") == 0)
        return CMD_CREATE;
    if (strcmp(cmd, "write") == 0)
        return CMD_WRITE;
    if (strcmp(cmd, "read") == 0)
        return CMD_READ;
    if (strcmp(cmd, "delete") == 0)
        return CMD_DELETE;
    if (strcmp(cmd, "ls") == 0)
        return CMD_LS;
    if (strcmp(cmd, "cd") == 0)
        return CMD_CD;
    if (strcmp(cmd, "pwd") == 0)
        return CMD_PWD;
    if (strcmp(cmd, "df") == 0)
        return CMD_DF;
    if (strcmp(cmd, "exit") == 0)
        return CMD_EXIT;
    return CMD_INVALID;
}

void executeCommand(CommandType cmdType, char *arg1, char *arg2)
{
    switch (cmdType)
    {
        case CMD_MKDIR:
            mkdirCmd(arg1);
            break;
        case CMD_RMDIR:
            rmdirCmd(arg1);
            break;
        case CMD_CREATE:
            createCmd(arg1);
            break;
        case CMD_WRITE:
            writeCmd(arg1, arg2);
            break;
        case CMD_READ:
            readCmd(arg1);
            break;
        case CMD_DELETE:
            deleteCmd(arg1);
            break;
        case CMD_LS:
            lsCmd();
            break;
        case CMD_CD:
            cdCmd(arg1);
            break;
        case CMD_PWD:
            pwdCmd();
            break;
        case CMD_DF:
            dfCmd();
            break;
        case CMD_EXIT:
            cleanup();
            exit(0);
        default:
            printf("Invalid command.Please Try again\n");
    }
}
