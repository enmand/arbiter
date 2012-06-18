#include "daemon.h"

void daemonize()
{
	pid_t pid, sid;

	signal(SIGHUP, _handlesig);

	if((pid = fork()) < 0)
	{
		exit(EXIT_FAILURE);
	}else if(pid > 0)
	{
		_setpid(pid);
		exit(EXIT_SUCCESS);
	}

	umask(0);
	chdir("/");

	if((sid = setsid()) < 0)
	{
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void _handlesig(int signal)
{
	switch(sig):
	{
		case SIGHUP:
		case SIGTERM:
			_rmpid();
			break;		
	}
}

/** write the pid to a file */
void _setpid(pid_t pid)
{
	int fd = open("pid.arbiter", O_RDWR | O_CREAT , S_IRUSR | S_IWUSR);

	if(fd != -1)
	{
		unsigned int _pidl = sizeof(pid_t);
		char _pidc[_pidl];
		snprintf(_pidc, _pidl, "%d", pid);
		write(fd, _pidc, strlen(_pidc));
		close(fd);
	}
}