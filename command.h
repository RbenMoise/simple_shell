#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>

/**
 * struct Command_s - Helpers Representing the parameters
 *                  to execute a program with an exec call.
 *
 * @args: Arguments vector.
 * @name: The name of program to execute and the first element of args.
 * @ppathname: The parent program path name.
 * @args_count: The number of arguments set.
 * @args_size: The max number of arguments that can set.
 */
struct Command_s
{
	char **args;
	char *name;
	char *ppathname;
	size_t args_count;
	size_t args_size;
};
typedef struct Command_s Command_t;


/**
 * parseCommandLine - Takes a string entered as a command
 *                    and parses and validates it in a command
 *                    struct pointer.
 *
 * @cmd: A double pointer to a Command_t struct.
 */
int parseCommandLine(char *line, Command_t *cmd);

void parseCommand(char **av, Command_t *cmd);


/**
 * isExecutable - Tests if a given commandName actually
 *                corresponds to an executable.
 *
 * @commandName: The comamnd name to test against.
 *
 * Return: 1 if comamndName is executable otherwise 0.
 */
char isExecutable(char *commandName, char filePath[]);


/**
 * executeCommand - Executes a given command.
 *
 * @cmd: A double pointer to a Command_t struct.
 */
void executeCommand(Command_t *cmd);


void initializeArgs(Command_t *cmd);

#endif /* COMMAND_H  */
