#include "../inc/virtualFileSystem.h"

int main()
{
    initializeFileSystem();

    char input[256];
    while (1)
    {
        printf("%s > ", cwd->name);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char *cmd = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, "\"");

        if (!cmd)
        {
            continue;
        }
        CommandType type = parseCommand(cmd);
        executeCommand(type, arg1, arg2);
    }
}
