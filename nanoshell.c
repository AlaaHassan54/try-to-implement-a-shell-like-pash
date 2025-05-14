#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

#define buf_count 1000


typedef struct {
    char *name;
    char *value;
    int exported;
} ShellVar;

ShellVar *vars = NULL;
int var_count = 0;
int last_status = 0;

void add_variable(const char *name, const char *value)
{
    for (int i = 0; i < var_count; ++i) {
	if (strcmp(vars[i].name, name) == 0) {
	    free(vars[i].value);
	    vars[i].value = strdup(value);
	    return;
	}
    }
    vars = (ShellVar *) realloc(vars, sizeof(ShellVar) * (var_count + 1));
    vars[var_count].name = strdup(name);
    vars[var_count].value = strdup(value);
    vars[var_count].exported = 0;
    var_count++;
}

void export_variable(const char *name)
{
    for (int i = 0; i < var_count; ++i) {
	if (strcmp(vars[i].name, name) == 0) {
	    setenv(name, vars[i].value, 1);
	    vars[i].exported = 1;
	    return;
	}
    }
    printf("Variable not found\n");
}

char *get_variable_value(const char *name)
{
    for (int i = 0; i < var_count; ++i) {
	if (strcmp(vars[i].name, name) == 0) {
	    return vars[i].value;
	}
    }
    return getenv(name);	
}

void substitute_variables_in_line(char *line)
{
    char new_line[buf_count * 2] = "";
    char *p = line;

    while (*p) {
	if (*p == '$') {
	    p++;
	    char var_name[100] = "";
	    int j = 0;
	    while (*p && (isalnum(*p) || *p == '_')) {
		var_name[j++] = *p++;
	    }
	    var_name[j] = '\0';
	    char *value = get_variable_value(var_name);
	    if (value)
		strcat(new_line, value);
	} else {
	    strncat(new_line, p, 1);
	    p++;
	}
    }
    strcpy(line, new_line);
}

void execute_command(char *line)
{
    substitute_variables_in_line(line);

    int argc = 0;
    char *token;
    char *saveptr;

    char *line_copy = strdup(line);
    token = strtok_r(line_copy, " ", &saveptr);
    while (token != NULL) {
	argc++;
	token = strtok_r(NULL, " ", &saveptr);
    }
    free(line_copy);

    char **argv = (char **) malloc((argc + 1) * sizeof(char *));
    if (!argv) {
        last_status=1;
	perror("malloc");
	return;
    }

    argc = 0;
    token = strtok_r(line, " ", &saveptr);
    while (token != NULL) {
	argv[argc++] = strdup(token);
	token = strtok_r(NULL, " ", &saveptr);
    }
    argv[argc] = NULL;

    execvp(argv[0], argv);

    printf("IamNotACommand_abcdefxyz: command not found\n");
    for (int i = 0; i < argc; i++)
	free(argv[i]);
    free(argv);
    last_status=1;
  			
}

int main(int argv, char **argc)
{
    char *buf = NULL;
    size_t bufsize = 0;
    ssize_t linelen;

    while ((linelen = getline(&buf, &bufsize, stdin)) != -1) {
	if (linelen > 0 && buf[linelen - 1] == '\n')
	    buf[--linelen] = '\0';
	if (linelen == 0)
	    continue;

	// Check for variable assignment
	if (strchr(buf, '=') && !strchr(buf, ' ')) {
	    char *eq = strchr(buf, '=');
	    if (eq && eq != buf && *(eq + 1) != '\0') {
		*eq = '\0';
		char *name = buf;
		char *value = eq + 1;
		add_variable(name, value);
		continue;
	    } else {
		printf("Invalid command\n");
		last_status=1;
		continue;
	    }
	}

	char *buf_copy = strdup(buf);
	substitute_variables_in_line(buf_copy);
	char *token = strtok(buf_copy, " ");

	if (!token) {
	    free(buf_copy);
	    continue;
	}

	if (strcmp(token, "echo") == 0) {
	    char *arg;
	    int first = 1;
	    while ((arg = strtok(NULL, " ")) != NULL) {
		substitute_variables_in_line(arg);
		if (!first)
		    printf(" ");
		printf("%s", arg);
		first = 0;
	    }
	    printf("\n");
	    last_status=0;
	} else if (strcmp(token, "pwd") == 0) {
	    char path[buf_count];
	    printf("%s\n", getcwd(path, buf_count));
	    last_status=0;
	} else if (strcmp(token, "cd") == 0) {
	    char *path = strtok(NULL, "\0");
	    if (chdir(path) < 0)
		printf
		    ("cd: /invalid_directory: No such file or directory\n");
		    last_status=1;
	} else if (strcmp(token, "export") == 0) {
	    char *var = strtok(NULL, " ");
	    if (var)
		export_variable(var);
	    else
		printf("Invalid command\n");
		last_status=1;
	} else if (strcmp(token, "exit") == 0) {
	    printf("Good Bye\n");
	    free(buf_copy);
	    free(buf);
	    return last_status;
	} else {
	    pid_t pid = fork();
	    if (pid > 0) {
		int status;
		wait(&status);
		last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
	    } else if (pid == 0) {
		execute_command(buf);
	    } else {
		perror("fork");
		last_status = 1;
	    }
	}

	free(buf_copy);
    }

    free(buf);
    return last_status;
}

