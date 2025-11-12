#include "directory.h"

DirectoryNode *root = NULL;
DirectoryNode *cwd = NULL;

void mkdirCmd(const char *directoryName)
{
    if (!directoryName || strlen(directoryName) == 0)
    {
        printf("Usage: mkdir <directoryName>\n");
        return;
    }

    DirectoryNode *temp = cwd->subDirs;
    while (temp)
    {
        if (strcmp(temp->name, directoryName) == 0)
        {
            printf("Name already exists in current directory.\n");
            return;
        }
        temp = temp->next;
    }

    DirectoryNode *newDir = malloc(sizeof(DirectoryNode));
    strcpy(newDir->name, directoryName);
    newDir->parent = cwd;
    newDir->subDirs = NULL;
    newDir->files = NULL;
    newDir->next = cwd->subDirs;
    cwd->subDirs = newDir;
    printf("Directory '%s' created successfully.\n", directoryName);
}

void rmdirCmd(const char *directoryName)
{
    if (!directoryName)
    {
        printf("Usage: rmdir <directoryName>\n");
        return;
    }

    DirectoryNode *prev = NULL, *curr = cwd->subDirs;
    while (curr)
    {
        if (strcmp(curr->name, directoryName) == 0)
        {
            if (curr->subDirs || curr->files)
            {
                printf("Directory not empty. Remove files first.\n");
                return;
            }
            if (prev)
            {
                prev->next = curr->next;
            }
            else
            {
                cwd->subDirs = curr->next;
            }
            free(curr);
            printf("Directory removed successfully.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Directory not found.\n");
}

void lsCmd(void)
{
    bool empty = true;
    DirectoryNode *directory = cwd->subDirs;
    FileNode *file = cwd->files;
    while (directory)
    {
        printf("%s/\n", directory->name);
        directory = directory->next;
        empty = false;
    }
    while (file)
    {
        printf("%s\n", file->name);
        file = file->next;
        empty = false;
    }
    if (empty)
    {
        printf("(empty)\n");
    }
}

void cdCmd(const char *directoryName)
{
    if (!directoryName)
    {
        printf("Usage: cd <directoryName>\n");
        return;
    }

    if (strcmp(directoryName, "..") == 0)
    {
        if (cwd->parent)
        {
            cwd = cwd->parent;
        }
        printf("Moved to /%s\n", cwd == root ? "" : cwd->name);
        return;
    }

    DirectoryNode *temp = cwd->subDirs;
    while (temp)
    {
        if (strcmp(temp->name, directoryName) == 0)
        {
            cwd = temp;
            printf("Moved to /%s\n", cwd->name);
            return;
        }
        temp = temp->next;
    }

    printf("Directory not found.\n");
}

void pwdCmd(void)
{
    DirectoryNode *temp = cwd;
    char path[256] = "";
    while (temp)
    {
        char buf[128];
        sprintf(buf, "/%s", temp->name);
        strcat(buf, path);
        strcpy(path, buf);
        temp = temp->parent;
    }
    printf("%s\n", path[0] ? path : "/");
}
