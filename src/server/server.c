#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define LOCAL_SERVER_PORT 1095
#define BUF 255

int main(int argc, char **argv) {
  int s, rc, n, len;
  struct sockaddr_in cliAddr, servAddr;
  char buffer[BUF];
  time_t time1;  // used for logging
  char loctime[BUF];
  char *ptr;
  const int y = 1;

  // create socket
  s = socket(AF_INET, SOCK_DGRAM, 0); // TODO this should be SOCK_STREAM
  // error handling
  if(s < 0) {
     printf("%s: Unable to open socket...(%s)\n", argv[0], strerror(errno));
     exit(EXIT_FAILURE);
  }

  // bind local server port
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl (INADDR_ANY);
  servAddr.sin_port = htons (LOCAL_SERVER_PORT);
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
  rc = bind(s, (struct sockaddr *)&servAddr, sizeof(servAddr));
  if (rc < 0) {
    printf("%s: Cannot bind port number %d (%s)\n", argv[0],
      LOCAL_SERVER_PORT, strerror(errno));
    exit (EXIT_FAILURE);
  }
  printf("%s: Waiting for connection on port %u\n", argv[0],
    LOCAL_SERVER_PORT);
  
  // Server loop
  while (1) {
    memset (buffer, 0, BUF);
    // Receive data
    len = sizeof(cliAddr);
    n = recvfrom(s, buffer, BUF, 0, (struct sockaddr *) &cliAddr,
      (socklen_t *) &len);
    if (n < 0) {
       printf ("%s: Cannot receive data ...\n",argv[0]);
       continue;
    }
    // logging output
    time(&time1);
    strncpy(loctime, ctime(&time1), BUF);
    ptr = strchr(loctime, '\n' );
    *ptr = '\0';
    printf("%s: Receiving from %s:UDP%u: %s \n", loctime,
      inet_ntoa(cliAddr.sin_addr), ntohs (cliAddr.sin_port), buffer);
  }
  return EXIT_SUCCESS;
}