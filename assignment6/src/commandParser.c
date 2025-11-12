#include "commandParser.h"

CommandType parseCommand(const char *cmd)
{
    if (!cmd)
        return CMD_INVALID;
    if (!strcmp(cmd, "mkdir"))
        return CMD_MKDIR;
    if (!strcmp(cmd, "rmdir"))
        return CMD_RMDIR;
    if (!strcmp(cmd, "create"))
        return CMD_CREATE;
    if (!strcmp(cmd, "write"))
        return CMD_WRITE;
    if (!strcmp(cmd, "read"))
        return CMD_READ;
    if (!strcmp(cmd, "delete"))
        return CMD_DELETE;
    if (!strcmp(cmd, "ls"))
        return CMD_LS;
    if (!strcmp(cmd, "cd"))
        return CMD_CD;
    if (!strcmp(cmd, "pwd"))
        return CMD_PWD;
    if (!strcmp(cmd, "df"))
        return CMD_DF;
    if (!strcmp(cmd, "exit"))
        return CMD_EXIT;
    return CMD_INVALID;
}

void executeCommand(CommandType cmd, char *arg1, char *arg2)
{
    switch (cmd)
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
            printf("Invalid command.\n");
    }
}
