#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "constants.h"

//
// This is based on the textbook example from Advanced Programming in the UNIX
// Environment, Stevens, Chapter 13
//

// Returns false if daemonization failed, true on success
int daemonize()
{
	int fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	// Clear file creation mask
	umask(0);

	// Get max number of file descriptors
	if( getrlimit(RLIMIT_NOFILE, &rl) < 0 )
		return FALSE;

	// Become session leader and detach from the TTY
	if( (pid = fork()) < 0 )
		return FALSE;
	else if( pid != 0 ) // Parent
		exit(0);
	setsid(); // Child becomes session leader

	// Ensure that opening new files will *not* reopen the TTY
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	// Ignore SIGHUP, since we're not on a TTY
	if( sigaction(SIGHUP, &sa, NULL) < 0 )
		return FALSE;

	// Now we become a child and ditch the parent, so there effectively is no
	// session, and we're detached from everything
	if( (pid = fork()) < 0 )
		return FALSE;
	else if( pid != 0 ) // Parent
		exit(0);

	// Usually daemonization also includes changing directory to root
	// In this case our tftp daemon may actually want to access files from
	// running directory, so we'll skip that step

	// Close all open file descriptors
	if( rl.rlim_max == RLIM_INFINITY )
		rl.rlim_max = 1024;
	for( unsigned int i = 0; i < rl.rlim_max; i++ )
		close(i);

	// Re-attach file descriptors 0, 1, and 2 to /dev/null so syscalls
	// will silently fail instead of crashing the daemon
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	// That *should* be stdin, stdout, and stderr, and something is very
	// wrong if other file handles are open
	if( fd0 != 0 || fd1 != 1 || fd2 != 2 )
		exit(1);

	return TRUE;
}
