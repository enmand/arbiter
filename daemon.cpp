#include "daemon.h"

void daemonize()
{
	pid_t pid, sid;

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

void _setpid(pid_t pid)
{
	std::ofstream pidfile;

	pidfile.open("pid.arbiter");
	pidfile << pid;
	pidfile.close();
}