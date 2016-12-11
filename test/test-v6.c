/*
 *
 * This program checks if your system is able to create IPv6 sockets (AF_INET6)
 * if the sockets is created, so you operating system is able to support IPv6.
 * Otherwise, is not able to support IPv6.
 *
 * Jul 19, 2003
 * leo@ubiobio.cl
 */


#include<stdio.h> /* fprintf() */
#include<stdlib.h> /* exit() */
#include<sys/types.h> /* SOCK_STREAM */
#include<sys/socket.h> /* socket(), AF_INET6 */
#include<unistd.h> /* close() */

int
main (int argc, char *argv[])
{
	int sd = 0;
	if ((sd = socket (AF_INET6, SOCK_STREAM, 0)) == -1) {
			fprintf (stdout, "Only IPv4 sockets are allowed\n");
			exit (1);
	} else {
			close(sd);
			fprintf(stdout, "IPv6 sockets are allowed\n");
			exit (0);
	}
	return 0;
}
