#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "directory.h"
#include "file.h"

typedef enum
{
    CMD_INVALID,
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
    CMD_EXIT
} CommandType;

CommandType parseCommand(const char *cmd);
void executeCommand(CommandType cmd, char *arg1, char *arg2);

#endif
