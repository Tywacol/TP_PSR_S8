#include "libreseau.c"

void envoiMessageUDP(int s, void * handle, unsigned char * message, int taille) {
    struct addrinfo * resultat = handle;
    /* Envoi du message */
    int nboctets = sendto(s, message, taille, 0, resultat->ai_addr, resultat->ai_addrlen);
    if (nboctets < 0) {
        perror("messageUDPgenerique.sento");
        exit(EXIT_FAILURE);
    }

    /* Fermeture de la socket d'envoi */
}

int initSocket(char * hote, char * service, void ** handle) {
    struct addrinfo precisions, * resultat, * origine;
    int statut;
    int s;

    /* Creation de l'adresse de socket */
    memset( & precisions, 0, sizeof precisions);
    precisions.ai_family = AF_UNSPEC;
    precisions.ai_socktype = SOCK_DGRAM;

    // creation addr

    resultat = malloc(sizeof(struct addrinfo));

    statut = getaddrinfo(hote, service, & precisions, & origine);

    if (statut < 0) {
        perror("messageUDPgenerique.getaddrinfo");
        exit(EXIT_FAILURE);
    }

    struct addrinfo * p;

    for (p = origine, resultat = origine; p != NULL; p = p -> ai_next)
        if (p -> ai_family == AF_INET6) {
            resultat = p;
            break;
        }

    /* Creation d'une socket */
    s = socket(resultat -> ai_family, resultat -> ai_socktype, resultat -> ai_protocol);
    if (s < 0) {
        perror("messageUDPgenerique.socket");
        exit(EXIT_FAILURE);
    }

    /* Option sur la socket */
    int vrai = 1;

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, & vrai, sizeof(vrai)) < 0) {
        perror("initialisationServeurUDPgenerique.setsockopt (BROADCAST)");
        exit(-1);
    }

    * handle = resultat;

    return s;
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Syntaxe : %s <message>\n", argv[0]);
        exit(EXIT_FAILURE);        
    }

    //char * hote = argv[1];
    char * message = argv[1];
    char * service = "4000";
    void * handle;
    int s = initSocket("255.255.255.255", service, &handle);
    if (s < 0) {
        exit(EXIT_FAILURE);
    }
    while (1) {
        envoiMessageUDP(s, handle, (unsigned char * ) message, strlen(message));
        sleep(5);
    }

    /* Liberation de la structure d'informations */
    freeaddrinfo(handle);
    return 0;
}
