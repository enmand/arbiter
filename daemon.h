#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototypes for deamon and locking
void _setpid(pid_t);
void _handlesig(int);
void daemonize(void);