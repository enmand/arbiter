
#include <zmq.hpp>
#include <mongo/client/dbclient.h>

#include "daemon.h"

int main()
{
	daemonize();

	while(true)
	{
	}
}