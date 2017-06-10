#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cmsis_os.h"
#include "shell.h"
#include "telnet.h"

typedef bool (*shell_func)(int argc, char **argv);

struct shell_command 
{
  char name[16];
  shell_func funcptr;
};

static bool shell_exit(int argc, char **argv);
static bool shell_help(int argc, char **argv);

// Must be sorted in ascending order.
const struct shell_command commands[] = 
{
  {"EXIT", shell_exit},
  {"HELP", shell_help},
};

static bool shell_exit(int argc, char **argv)
{
  // Exit the shell interpreter.
  return false;
}

static bool shell_help(int argc, char **argv)
{
  uint32_t i;

  // Loop over each shell command.
  for (i = 0; i < sizeof(commands) / sizeof(struct shell_command); ++i)
  {
    telnet_printf("%s\n", commands[i].name);
  }

  return true;
}

// Parse out the next non-space word from a string.
// str    Pointer to pointer to the string
// word   Pointer to pointer of next word.
// Returns 0:Failed, 1:Successful
static int shell_parse(char **str, char **word)
{
  // Skip leading spaces.
  while (**str && isspace((unsigned char) **str)) (*str)++;

  // Set the word.
  *word = *str;

  // Skip non-space characters.
  while (**str && !isspace((unsigned char) **str)) (*str)++;

  // Null terminate the word.
  if (**str) *(*str)++ = 0;

  return (*str != *word) ? 1 : 0;
}

// Attempt to execute the shell command.
static bool shell_command(char *cmdline)
{
  int i;
  char *argv[8];
  int argc = 0;
  bool rv = true;
  struct shell_command *command;

  // Parse the command and any arguments into an array.
  for (i = 0; i < (sizeof(argv) / sizeof(char *)); ++i)
  {
    shell_parse(&cmdline, &argv[i]);
    if (*argv[i] != 0) ++argc;
  }

  // Search for a matching command.
  command = (struct shell_command *) bsearch(argv[0], commands, sizeof(commands) / sizeof(struct shell_command), sizeof(struct shell_command), (int(*)	(const void*, const void*)) strcasecmp);

  // Call the command if found.
  if (command) rv = (command->funcptr)(argc, argv);

  return rv;
}

// Runs the shell by printing the prompt and processing each shell command typed in.
void shell_process() 
{
  char cmdbuf[64];

  // Tell the user the shell is starting.
  telnet_printf("Starting Shell...\n");

  // Send a prompt.
  telnet_puts("> ");
  telnet_flush();

  // Get a string.
  while (telnet_gets(cmdbuf, sizeof(cmdbuf), 0, true))
  {
    // Process the line as a command.
    if (!shell_command(cmdbuf)) break;

    // Send a prompt.
    telnet_puts("> ");
    telnet_flush();
  }
}

