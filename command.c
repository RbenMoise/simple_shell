#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include<stddef.h>
#include<sys/stat.h>
#include <wchar.h>
#include <errno.h>

#include "command.h"
#include "string.h"
#include "getenv.h"


void parseCommand(char **av, Command_t *cmd)
{
	int i;
	if (cmd->args_size == 0)
		initializeArgs(cmd);

	i = 1;
	cmd->args_count = 0;
	while(av[i] != NULL)
	{
		if(cmd->args_count >= cmd->args_size)
		{
			cmd->args = realloc(cmd->args, (cmd->args_size + 4) * sizeof(char *));
			cmd->args_size += 4;
		}
		cmd->args[i - 1] = av[i];
		i++;
		cmd->args_count++;
	}

	if (cmd->args_count >= cmd->args_size)
        {
                cmd->args = realloc(cmd->args, (cmd->args_size + 1) * sizeof(char *));
                cmd->args_size += 1;
        }

        cmd->args[i] = NULL;
        cmd->args_count += 1;
}


/**
 * parseCommandLine - Takes a string entered as a command
 *                    and parses and validates it in a command
 *                    struct pointer.
 *
 * @line: The command input.
 *
 * Return: int, -1 if something went wrong otherwise 1;
 */
int parseCommandLine(char *line, Command_t *cmd)
{
	int i;
	char *token;
	char cmdPath[50];
	char tmp[100];

	if (cmd->args_size == 0)
		initializeArgs(cmd);

        i = 0;
	token = strtok(_strcpy(tmp, line), " ");
	if (token == NULL)
		return (-1);

	if (!isExecutable(token, cmdPath))
        {
                perror(cmd->ppathname);
                return (-1);
        }
        cmd->args[i++] = cmdPath;
        cmd->args_count = i;

	/**
	 * Reset the strtok internal pointer
	 * to avoid dealing with values from isExecutable
	 * calls.
	 */
	strtok(line, " ");
	while ((token = strtok(NULL, " ")))
        {
                if (cmd->args_count >= cmd->args_size)
                {
                        cmd->args = realloc(cmd->args, (cmd->args_size + 4) * sizeof(char *));
                        cmd->args_size += 4;
                }

                cmd->args[i++] = token;
                cmd->args_count++;
        }

	if (cmd->args_count >= cmd->args_size)
	{
		cmd->args = realloc(cmd->args, (cmd->args_size + 1) * sizeof(char *));
		cmd->args_size += 1;
	}

	cmd->args[i] = NULL;
	cmd->args_count += 1;
	return (1);
}


/**
 * initializeArgs - Allocates memory for the command arguments.
 * @cmd: A pointer to Command_t struct.
 */
void initializeArgs(Command_t *cmd)
{
	cmd->args = malloc(8 * sizeof(char *));
	if (cmd->args == NULL)
	{
		perror(cmd->ppathname);
		exit(EXIT_FAILURE);
	}
	cmd->args_size = 8;
	cmd->args_count = 0;
}


/**
 * isExecutable - Tests if a given commandName actually
 *                corresponds to an executable.
 *
 * @commandName: The comamnd name to test against.
 * @filePath: A double pointer to an array holding
 *            the absolute path to the program file.
 *
 * Return: 1 if comamndName is executable otherwise 0.
 */
char isExecutable(char *commandName, char filePath[])
{
	char *path, *dir, *_filePath;
	struct stat st;

	if (stat(commandName, &st) == 0)
		return (1);
	path = _getenv("PATH");
        if (path != NULL)
        {
                dir = strtok(path, ":");
                while (dir != NULL)
                {
                        _filePath = filePath;
                        _filePath = _stpcpy(_filePath, dir);
                        _filePath = _stpcpy(_filePath, "/");
                        _filePath = _stpcpy(_filePath, commandName);
                        if (stat(filePath, &st) == 0)
                                return (1);

                        filePath[0] = '\0';
                        dir = strtok(NULL, ":");
                }
        }
	return (0);
}


/**
 * executeCommand - Executes a given command.
 */
void executeCommand(Command_t *cmd)
{
	if (execve(cmd->args[0], cmd->args, environ) == -1)
	{
		perror(cmd->ppathname);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
