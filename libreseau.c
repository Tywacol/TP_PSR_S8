// _____ Divers ______

// Obtention des addresse IP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*
int getaddrinfo(char *name,char *service,
                struct addrinfo *hints,
                struct addrinfo **res);
*/

/* void freeaddrinfo(struct addrinfo *res); */



void socketVersClient(int s, char ** hote, char ** service) {
  struct sockaddr_storage adresse;
  socklen_t taille = sizeof adresse;
  int statut;

  /* Recupere l'adresse de la socket distante */
  statut = getpeername(s, (struct sockaddr * ) & adresse, & taille);
  if (statut < 0) {
    perror("socketVersNom.getpeername");
    exit(EXIT_FAILURE);
  }
}

  /* Recupere le nom de la machine */
  /*
  * hote = malloc(MAX_TAMPON);
  if ( * hote == NULL) {
    perror("socketVersClient.malloc");
    exit(EXIT_FAILURE);
  }
  * service = malloc(MAX_TAMPON);
  if ( * service == NULL) {
    perror("socketVersClient.malloc");
    exit(EXIT_FAILURE);
  }
  getnameinfo((struct sockaddr * ) & adresse, sizeof adresse, * hote, MAX_TAMPON, * service, MAX_TAMPON, 0);
}
*/
