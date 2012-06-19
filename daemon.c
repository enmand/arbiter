#include "daemon.h"
#include <pthread.h>

void daemonize(int proc)
{
	bool stopping;
	pid_t existing;

	switch(proc)
	{
		case ARBITER_START:
			existing = _getpidf();
			if(existing != -1)
			{
				fprintf(stderr, "Failed to start. Maybe there is a process running already? Or a dangling pid-file?\n");
				exit(EXIT_FAILURE);
			} else
			{
				_start();
			}
			break;
		case ARBITER_STOP:
			stopping = _stop();
			if(!stopping)
			{
				fprintf(stderr, "Failed to stop. Is the process running?\n");
				exit(EXIT_FAILURE);
			}
			break;
	}
}

bool _stop()
{
	pid_t childpid = _getpidf();
	if(childpid == -1)
	{
		return false;
	} 
	_rmpid();

	kill(childpid, SIGHUP);
	exit(EXIT_SUCCESS);
}

void _start()
{
	pid_t pid, sid;
	
	if((pid = fork()) < 0)
	{
		exit(EXIT_FAILURE);
	}else if(pid > 0)
	{
		_setpidf(pid);
		exit(EXIT_SUCCESS);
	}

	umask(0);

	if((sid = setsid()) < 0)
	{
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
 
 	// set up signal handlers
	struct sigaction sa;
	sa.sa_handler = _handlesig;
	sigaddset(&(sa.sa_mask), SIGTERM);
	if (sigaction(SIGTERM, &sa, NULL) != 0)
	{
		exit(EXIT_FAILURE);
	}
}

void _handlesig(int sig)
{
	_stop();
}

bool _rmpid()
{
	return unlink(ARBITER_PFILE) != -1;
}

/** write the pid to a file */
void _setpidf(pid_t pid)
{
	int fd = open(ARBITER_PFILE, O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK, S_IRUSR | S_IWUSR);
	flock(fd, LOCK_EX);

	if(fd != -1)
	{
		unsigned int _pidl = sizeof(pid_t) + 2; // room for \0 and full size
		char _pidc[_pidl];
		snprintf(_pidc, _pidl, "%d", pid);
		write(fd, _pidc, strlen(_pidc));

		flock(fd, LOCK_UN);
		close(fd);
	} else
	{
		perror("open");
		kill(0, SIGTERM);
		exit(EXIT_FAILURE);
	}
}

pid_t _getpidf()
{
	struct stat _pidf;
	if(stat(ARBITER_PFILE, &_pidf) == -1)
	{
		return -1;
	}

	int fd = open(ARBITER_PFILE, O_RDONLY);
	unsigned int _pidl = sizeof(pid_t) + 2;

	char _pidc[_pidl];
	read(fd, _pidc, _pidl);

	return (pid_t)atoi(_pidc);
}
