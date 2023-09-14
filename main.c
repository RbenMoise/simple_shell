#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "command.h"
#include "string.h"
#include "_putchar.h"

/**
 * main - Simple shell entry point.
 * @ac: int, The program's arguments count.
 * @argv: A pointer to the program's arguments.
 *
 * Return: (0) EXIT_SUCCESS on success or (1)
 *         EXIT_FAILURE on failure.
 */
int main(int ac, char **argv)
{
	Command_t cmd;
	int status;
	pid_t pid;
	char *commandStr;
	char cmdPath[50];

	cmd.ppathname = argv[0];
	cmd.args = NULL;
	cmd.args_size = cmd.args_count = 0;
	commandStr = NULL;
	if (ac > 1)
	{
		if (isExecutable(argv[1], cmdPath))
		{
			argv[1] = cmdPath;
			pid = fork();
			if (pid == -1)
			{
				perror(argv[0]);
				exit(EXIT_FAILURE);
			}
			else if (pid == 0)
			{
				parseCommand(argv, &cmd);
				executeCommand(&cmd);
			}
			else
			{
				wait(&status);
				if (cmd.args != NULL)
					free(cmd.args);
				exit(status);
			}
		}
		else
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	while (1)
	{
		commandStr = handlePrompt();
		commandStr = strtok(commandStr, "\n");

		pid = fork();
		if (pid == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			cmd.args_count = 0;
			parseCommandLine(commandStr, &cmd);
			executeCommand(&cmd);
		}
		else
		{
			wait(&status);
			free(commandStr);
			commandStr = NULL;
		}
	}

	free(cmd.args);
	return (0);
}



/**
 * handlePrompt - Handles the command value entered in the
 *                prompt.
 * Return: A pointer to the line got from the prompt
 */
char *handlePrompt(void)
{
	ssize_t read_len;
	size_t line_len;
	char *line;
	char *msg = "#cisfun$ ";

	while (*msg != '\0')
		_putchar(*msg++);

	line = NULL;
	read_len = line_len = 0;

	while ((read_len = getline(&line, &line_len, stdin)) != -1)
		return (line);

	if (read_len == -1)
		perror("getline: ");
	return (NULL);
}
