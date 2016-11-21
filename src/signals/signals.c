#define _GNU_SOURCE

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

#include "signals.h"

static void sigint(int signum, siginfo_t* siginfo, void* data)
{
  puts("\n");
  data = data;
  signum = signum;
  siginfo = siginfo;
} 

void set_sigacts(void)
{
  struct sigaction sigact;
  sigact.sa_sigaction = sigint;
  
  if (sigaction(SIGINT, &sigact, NULL))
    errx(1, "Sigaction has failed");
}
