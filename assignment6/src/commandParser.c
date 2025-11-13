#include "commandParser.h"

CommandType parseCommand(const char *command)
{
    if (!command)
        return INVALID;
    if (!strcmp(command, "mkdir"))
        return MKDIR;
    if (!strcmp(command, "rmdir"))
        return RMDIR;
    if (!strcmp(command, "create"))
        return CREATE;
    if (!strcmp(command, "write"))
        return WRITE;
    if (!strcmp(command, "read"))
        return READ;
    if (!strcmp(command, "delete"))
        return DELETE;
    if (!strcmp(command, "ls"))
        return LS;
    if (!strcmp(command, "cd"))
        return CD;
    if (!strcmp(command, "pwd"))
        return PWD;
    if (!strcmp(command, "df"))
        return DF;
    if (!strcmp(command, "exit"))
        return EXIT;
    return INVALID;
}

void executeCommand(CommandType command, char *arg1, char *arg2)
{
    switch (command)
    {
        case MKDIR:
            mkdirCmd(arg1);
            break;
        case RMDIR:
            rmdirCmd(arg1);
            break;
        case CREATE:
            createCmd(arg1);
            break;
        case WRITE:
            writeCmd(arg1, arg2);
            break;
        case READ:
            readCmd(arg1);
            break;
        case DELETE:
            deleteCmd(arg1);
            break;
        case LS:
            lsCmd();
            break;
        case CD:
            cdCmd(arg1);
            break;
        case PWD:
            pwdCmd();
            break;
        case DF:
            dfCmd();
            break;
        case EXIT:
            cleanup();
            exit(0);
        default:
            printf("Invalid command.\n");
    }
}
