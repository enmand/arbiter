#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include <fcntl.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARBITER_START 1
#define ARBITER_STOP 0
#define ARBITER_PFILE "arbiter.pid"

// prototypes for deamon and locking
void daemonize(int);

#endif