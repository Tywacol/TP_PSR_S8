#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct sigaction action;

#define NB 10
#define CLE_MSG (key_t) 1000
#define TAILLE_MSG 20

/* définition de la structure des messages */
typedef struct {
    long type;
    char texte[TAILLE_MSG];
}
Msg_requete;

int msgid;


void hand(int sig) {
  if (sig == SIGINT) {
    printf("SIGINT recu. Fin du programme..\n");
    msgctl(msgid, IPC_RMID, NULL);
    exit(SIGINT);
    }
  }

 int main() {

    Msg_requete message; /* création de la FDM */
    if ((msgid = msgget(CLE_MSG, IPC_CREAT | IPC_EXCL | 0666)) == 1) {
        perror("msgget");
        exit(1);
    }

    action.sa_handler = hand;
    sigaction(SIGINT, &action, NULL);

    while (1) {
      /* on est dans le fils */ /* reception du message du père : message de type 1 */
        msgrcv(msgid, &message, TAILLE_MSG, 1, 0); /* affichage contenu message */
        printf("message de ping : %s\n", message.texte); /* preparation de l'ack : message de type 2 */
        message.type = 2;
        strcpy(message.texte, "msg reçu par pong\n");
        /* envoi de l'ack au père */
        msgsnd(msgid, &message, TAILLE_MSG, 0);
    }
    return 0;
}
