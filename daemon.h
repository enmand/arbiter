#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include <fcntl.h>
#include <unistd.h>

#if __STDC_VERSION__ >= 199901L // c99
#include <stdbool.h>
#else
typedef enum {false=0, true} bool;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARBITER_START 1
#define ARBITER_STOP 0
#define ARBITER_PFILE "arbiter.pid"

// prototypes for deamon and locking
void daemonize(int);

void _setpidf(pid_t);
pid_t _getpidf(void);
bool _rmpid();

void _start();
bool _stop();
void _handlesig(int);

#endif