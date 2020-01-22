#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h> 
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
    if ((msgid = msgget(CLE_MSG, IPC_PRIVATE | IPC_EXCL | 0666)) == 1) {
        perror("msgget");
        exit(1);
    }

    action.sa_handler = hand;
    sigaction(SIGINT, &action, NULL);
    int i  = 0;
    while (1) {
      clock_t begin = clock();

      /* on est dans le fils */ /* reception du message du père : message de type 2 */
        msgrcv(msgid, &message, TAILLE_MSG, 2, 0); /* affichage contenu message */
        printf("message de pong = %s\n", message.texte); /* preparation de l'ack : message de type 2 */
        message.type = 1;
	if (i == 0) {
		strcpy(message.texte, "msg reçu par ping\n");
		i = 1;
	} else {
		strcpy(message.texte, "msg reçu par ping 2\n");
		i = 0;
	}
        /* envoi de l'ack au père */
        msgsnd(msgid, &message, TAILLE_MSG, 0);

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Temps aller-retour : %f\n", time_spent);
    }
    return 0;
}
