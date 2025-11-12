#include "commandParser.h"

int main()
{
    root = malloc(sizeof(DirectoryNode));
    strcpy(root->name, "");
    root->parent = NULL;
    root->subDirs = NULL;
    root->files = NULL;
    root->next = NULL;
    cwd = root;

    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char input[256];
    while (1)
    {
        printf("%s > ", cwd == root ? "/" : cwd->name);
        if (!fgets(input, sizeof(input), stdin))
        {
            break;
        }
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0)
        {
            continue;
        }

        char *cmd = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, "");
        if (arg2 && arg2[0] == '"')
        {
            arg2++;
            char *end = strrchr(arg2, '"');
            if (end)
            {
                *end = '\0';
            }
        }

        CommandType type = parseCommand(cmd);
        executeCommand(type, arg1, arg2);
    }
    return 0;
}
