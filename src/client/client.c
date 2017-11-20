#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>

#define SERVER_PORT 1095

int main(int argc, char **argv) {
  int s, rc, i;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *h;

  // assert correct cli usage
  if(argc < 3) {
    printf("Usage: %s <server> <data_0> ... <data_n> \n", argv[0]);
    exit(EXIT_FAILURE);
  }
  // resolve ip of host
  h = gethostbyname(argv[1]);
  if (h == NULL) {
    printf("%s: unknown host '%s' \n", argv[0], argv[1]);
    exit(EXIT_FAILURE);
  }
  printf ("%s: streaming to '%s' (IP : %s) \n", argv[0], h->h_name,
    inet_ntoa (*(struct in_addr *) h->h_addr_list[0]));
  remoteServAddr.sin_family = h->h_addrtype;
  memcpy((char *) &remoteServAddr.sin_addr.s_addr, h->h_addr_list[0],
    h->h_length);
  remoteServAddr.sin_port = htons(SERVER_PORT);

  // spawn socket
  s = socket (AF_INET, SOCK_DGRAM, 0); // TODO this should be SOCK_STREAM
  if(s < 0) {
     printf ("%s: unable to open socket (%s) \n", argv[0], strerror(errno));
     exit(EXIT_FAILURE);
  }
  // bind ports
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);
  rc = bind (s, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if(rc < 0) {
    printf ("%s: unable to bind port (%s)\n", argv[0], strerror(errno));
    exit (EXIT_FAILURE);
  }
  // stream data
  for(i = 2; i < argc; i++) {
    rc = sendto(s, argv[i], strlen(argv[i])+1, 0, 
      (struct sockaddr *) &remoteServAddr, sizeof(remoteServAddr));
    if (rc < 0) {
       printf("%s: unable to send data %d\n", argv[0], i-1);
       close(s);
       exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}