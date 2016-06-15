#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cmsis_os.h"
#include "shell.h"
#include "leds.h"

// Thread function prototype.
static void shell_thread(void const *arg);

// RTOS related resources.
osThreadDef(shell_thread, osPriorityNormal, 1, 0);

// Prototype for a shell command handler function.
typedef bool (*shell_func)(int argc, char **argv);

// Structure for a shell command.
struct shell_command 
{
  char name[16];
  shell_func funcptr;
};

// Predefines for shell commands we support.
static bool shell_exit(int argc, char **argv);
static bool shell_help(int argc, char **argv);
static bool shell_led(int argc, char **argv);

// Structure listing the name and function associated with each command.
// Commands must be sorted in ascending order.
const struct shell_command commands[] = 
{
  {"exit", shell_exit},
  {"help", shell_help},
  {"led", shell_led}
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
    printf("%s\n", commands[i].name);
  }

  return true;
}

static bool shell_led(int argc, char **argv)
{
  bool help = false;

  // Should turn an LED on or off?
  if (argc == 3)
  {
    uint32_t led_color = LED_COUNT;
    uint32_t led_mode = 2;

    // Convert the color to an index.
    if (!strcasecmp(argv[1], "green"))
    {
      led_color = LED_GREEN;
    }
    else if (!strcasecmp(argv[1], "orange"))
    {
      led_color = LED_ORANGE;
    }
    else if (!strcasecmp(argv[1], "red"))
    {
      led_color = LED_RED;
    }
    else if (!strcasecmp(argv[1], "blue"))
    {
      led_color = LED_BLUE;
    }

    // Parse the on/off string.
    if (!strcasecmp(argv[2], "on"))
    {
      led_mode = 1;
    }
    else if (!strcasecmp(argv[2], "off"))
    {
      led_mode = 0;
    }

    // Did we correctly parse the LED color and mode?
    if ((led_color < LED_COUNT) && (led_mode < 2))
    {
      // Set the LED color and mode.
      leds_set_value(led_color, led_mode);
    }
    else
    {
      printf("Unknown led command.\n");
      help = true;
    }
  }
  else
  {
    help = true;
  }

  // Do we need to display help?
  if (help)
  {
      printf("LED commands:\n");
      printf("  led [green|orange|red|blue] [on|off]\n");
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
  while (**str && isspace(**str)) (*str)++;

  // Set the word.
  *word = *str;

  // Skip non-space characters.
  while (**str && !isspace(**str)) (*str)++;

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

  // Use a binary search to find the command to execute.
  command = (struct shell_command *) bsearch(argv[0], commands, sizeof(commands) / sizeof(struct shell_command), 
                                             sizeof(struct shell_command), (int(*)  (const void*, const void*)) strcasecmp);

  // Call the command if found.
  if (command)
    rv = (command->funcptr)(argc, argv);
  else
    printf("Unknown shell command: %s\n", argv[0]);

  return rv;
}

bool shell_gets(char *buff, int len, int tocase, bool echo)
{
  int c;
  int i;

  i = 0;
  for (;;)
  {
    // Get a char from the incoming stream.
    c = fgetc(stdin);

    // End of stream?
    if (c < 0) return false;

    // Convert to upper/lower case?
    if (tocase > 0) c = toupper(c);
    if (tocase < 0) c = tolower(c);

    // End of line?
    if (c == '\r') break;

    // Back space?
    if (((c == '\b') && i) || ((c == 0x7f) && i))
    {
      i--;
      if (echo) { fputc(c, stdout); fputc(' ', stdout); fputc(c, stdout); }
    }

    // Visible chars.
    if ((c >= ' ') && (c < 0x7f) && (i < (len - 1)))
    {
      buff[i++] = c;
      if (echo) fputc(c, stdout);
    }
  }

  // Null terminate.
  buff[i] = 0;
  if (echo)
  {
    fputs("\r\n", stdout);
  }

  return true;
}

// Runs the shell by printing the prompt and processing each shell command typed in.
static void shell_run(void)
{
  static char cmdbuf[128];

  // Indicate the shell is starting.
  printf("\n\nShell starting...\n");

  // Display a prompt.
  fputs("> ", stdout);

  // Get a string from input, echoing characters.
  while (shell_gets(cmdbuf, sizeof(cmdbuf), 0, true))
  {
    // Process the line as a command.
    if (!shell_command(cmdbuf)) break;

    // Display a prompt.
    fputs("> ", stdout);
  }

  // Indicate the shell is exiting.
  printf("Shell exiting!\n");
}

// Shell thread.
static void shell_thread(const void *arg)
{
  // Loop forever.
  for (;;)
  {
    // Run the shell.
    shell_run();
  }
}

// Shell initialization.
void shell_init(void)
{
  // Start up the shell thread.
  osThreadCreate(osThread(shell_thread), NULL);
}

