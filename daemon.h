#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <fstream>

void _setpid(pid_t);
void daemonize(void);