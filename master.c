#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "myassert.h"

#include "master_client.h"
#include "master_worker.h"
#include "tubes.h"
#include "semaphore.h"


/************************************************************************
 * Données persistantes d'un master
 ************************************************************************/

// on peut ici définir une structure stockant tout ce dont le master
// a besoin
struct leMaster
	{
		int nbWorker; // le nombre de workers
		workerP *worker; // un tableau de workers
	};


/************************************************************************
 * Usage et analyse des arguments passés en ligne de commande
 ************************************************************************/

static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


/************************************************************************
 * boucle principale de communication avec le client
 ************************************************************************/
void loop(/* paramètres */)
{
	demandeClient = 0; 
    // boucle infinie :
    // - ouverture des tubes (cf. rq client.c)
	int tube1 = ouvrirNamedPipe(TUBE_MtoC, O_RDONLY);
	int tube2 = ouvrirNamedPipe(TUBE_CtoM, O_WRONLY);
    // - attente d'un ordre du client (via le tube nommé)
	demandeClient = recieveInt(tube1);
    // - si ORDER_STOP
    //       . envoyer ordre de fin au premier worker et attendre sa fin
    //       . envoyer un accusé de réception au client
    // - si ORDER_COMPUTE_PRIME
    //       . récupérer le nombre N à tester provenant du client
    //       . construire le pipeline jusqu'au nombre N-1 (si non encore fait) :
    //             il faut connaître le plus nombre (M) déjà enovoyé aux workers
    //             on leur envoie tous les nombres entre M+1 et N-1
    //             note : chaque envoie déclenche une réponse des workers
    //       . envoyer N dans le pipeline
    //       . récupérer la réponse
    //       . la transmettre au client
    // - si ORDER_HOW_MANY_PRIME
    //       . transmettre la réponse au client
    // - si ORDER_HIGHEST_PRIME
    //       . transmettre la réponse au client
    // - fermer les tubes nommés
    // - attendre ordre du client avant de continuer (sémaphore : précédence)
    // - revenir en début de boucle
    //
    // il est important d'ouvrir et fermer les tubes nommés à chaque itération
    // voyez-vous pourquoi ?
}


/************************************************************************
 * Fonction principale
 ************************************************************************/

int main(int argc, char * argv[])
{
    if (argc != 1)
        usage(argv[0], NULL);

	int demandeClient = 0;

    // - création des sémaphores
	
	int semClientMaster = semCreate(1, FICHIER_CM, ID_CM);
	printf("(Master) semaphore client créé\n");
    // - création des tubes nommés
	createNamedPipe(TUBE_MtoC);
	createNamedPipe(TUBE_CtoM);
	printf("(Master) tubes clients créés\n");
    // - création du premier worker

    // boucle infinie
    loop(/* paramètres */);

    // destruction des tubes nommés, des sémaphores, ...

	destroyNamedPipe(TUBE_MtoC);
	destroyNamedPipe(TUBE_CtoM);

	semDestroy(semRecover(FICHIER_CM, ID_CM));

    return EXIT_SUCCESS;
}

// N'hésitez pas à faire des fonctions annexes ; si les fonctions main
// et loop pouvaient être "courtes", ce serait bien
