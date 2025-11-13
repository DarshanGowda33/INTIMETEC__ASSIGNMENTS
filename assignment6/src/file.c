#include "file.h"

static int usedBlocks = 0;

static int countUsedBlocksDir(DirectoryNode *dir)
{
    if (!dir)
    {
        return 0;
    }
    int total = 0;

    FileNode *file = dir->files;
    while (file)
    {
        if (file->size > 0)
        {
            int blocks = (file->size + BLOCK_SIZE - 1) / BLOCK_SIZE;
            total += blocks;
        }
        file = file->next;
    }

    DirectoryNode *directory = dir->subDirs;
    while (directory)
    {
        total += countUsedBlocksDir(directory);
        directory = directory->next;
    }
    return total;
}

void createCmd(const char *filename)
{
    if (!filename)
    {
        printf("Usage: create <filename>\n");
        return;
    }

    FileNode *file = cwd->files;
    while (file)
    {
        if (strcmp(file->name, filename) == 0)
        {
            printf("Name already exists in current directory.\n");
            return;
        }
        file = file->next;
    }

    FileNode *newFile = malloc(sizeof(FileNode));
    strcpy(newFile->name, filename);
    newFile->data = NULL;
    newFile->size = 0;
    newFile->next = cwd->files;
    cwd->files = newFile;
    printf("File '%s' created successfully.\n", filename);
}

void writeCmd(const char *filename, const char *content)
{
    if (!filename || !content)
    {
        printf("Usage: write <filename> \"content\"\n");
        return;
    }

    FileNode *file = cwd->files;
    while (file)
    {
        if (strcmp(file->name, filename) == 0)
        {
            free(file->data);
            file->size = (int)strlen(content);
            file->data = malloc(file->size + 1);
            strcpy(file->data, content);
            printf("Data written successfully (size=%d bytes).\n", file->size);
            return;
        }
        file = file->next;
    }
    printf("File not found.\n");
}

void readCmd(const char *filename)
{
    if (!filename)
    {
        printf("Usage: read <filename>\n");
        return;
    }

    FileNode *file = cwd->files;
    while (file)
    {
        if (strcmp(file->name, filename) == 0)
        {
            if (file->data)
            {
                printf("%s\n", file->data);
            }
            else
            {
                printf("(empty)\n");
            }
            return;
        }
        file = file->next;
    }
    printf("File not found.\n");
}

void deleteCmd(const char *filename)
{
    FileNode *prev = NULL, *curr = cwd->files;
    while (curr)
    {
        if (strcmp(curr->name, filename) == 0)
        {
            if (prev)
            {
                prev->next = curr->next;
            }
            else
            {
                cwd->files = curr->next;
            }
            free(curr->data);
            free(curr);
            printf("File deleted successfully.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("File not found.\n");
}

void dfCmd()
{
    int usedBlocks = countUsedBlocksDir(root);
    int freeBlocks = TOTAL_BLOCKS - usedBlocks;
    double usage = 0.0;
    if (TOTAL_BLOCKS > 0)
    {
        usage = ((double)usedBlocks / (double)TOTAL_BLOCKS) * 100.0;
    }
    printf("Total Blocks: %d\n", TOTAL_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usage);
}

void cleanup()
{
    printf("Memory released. Exiting program...\n");
}
