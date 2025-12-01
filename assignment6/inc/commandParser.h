#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "directory.h"
#include "file.h"

typedef enum
{
    INVALID,
    MKDIR,
    RMDIR,
    CREATE,
    WRITE,
    READ,
    DELETE,
    LS,
    CD,
    PWD,
    DF,
    EXIT
} CommandType;

CommandType parseCommand(const char *input);
void executeCommand(CommandType cmd, char *arg1, char *arg2);

#endif
