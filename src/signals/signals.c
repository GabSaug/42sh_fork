#define _GNU_SOURCE

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <readline/readline.h>

#include "signals.h"
#include "main.h"
#include "my_string.h"

int g_in_readline;

static void sigint(int signum, siginfo_t* siginfo, void* data)
{
  if (!g_in_readline)
    puts("");
  else
  {
    char* ps = get_PS();
    write(STDERR_FILENO, "\n", 1);
    write(STDERR_FILENO, ps, my_strlen(ps));
    rl_line_buffer[0] = ' ';
    rl_line_buffer[1] = '\0';
    rl_point = 0;
    rl_end = 0;
  }
  data = data;
  signum = signum;
  siginfo = siginfo;
}

void set_sigacts(void)
{
  struct sigaction sigact;
  sigact.sa_sigaction = sigint;
  sigemptyset(&(sigact.sa_mask));
  sigact.sa_flags = 0;
  if (sigaction(SIGINT, &sigact, NULL))
    errx(1, "Sigaction has failed");
}
