#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include "myassert.h"


// Ouvrir, fermer, créer détruire ============================================

// crée un pipe dans le système de  fichiers
void createNamedPipe(char const *nom)
{
	int sysCall = mkfifo(nom,0720);
	myassert(sysCall != -1, "erreur mkfifo");
}

// retire le pipe du système de fichiers
void destroyNamedPipe(char const *nom)
{
	int sysCall = unlink(nom);
	myassert(sysCall != -1, "erreur, unlink pipe");
}

// ouvre un pipe à partir d'un tableau de fs
void creerPipe(int pipefd[2])
{
	int sysCall = pipe(pipefd);
	myassert(sysCall != -1, "Erreur ouverture pipe");
}

// Ouvre en lecture ou en écriture un pipe nommé (mod = O_WRONLY ou O_RDONLY) 
int ouvrirNamedPipe(char const *nomTube, int mode)
{
	int tube = open(nomTube,mode);
	myassert(tube != -1,"Erreur ouverture Tube");
	return tube;
}

// ferme un pipe via son descripteur
void fermerPipe(int tube)
{
	int sysCall = close(tube);
	myassert(sysCall != -1,"erreur fermeture pipe");
}


// envoie ====================================================================

//  envoie un type de data dont on précise la taille
void sendDataGeneric(int tube, void const *buff, int size)
{
	int sysCall = write(tube, buff, size);
	myassert(sysCall == size,"erreur write tube"); 
}

// envoyer un entier
void sendInt(int tube, int n)
{
	sendDataGeneric(tube, &n, sizeof(int));
}

// envoyer une chaine 
// (on envoie d'abord la taille de la chaine, puis la chaine)
void sendStr(int tube, char const *str)
{
	int taille = strlen(str);

	sendDataGeneric(tube,&taille,sizeof(int));
	sendDataGeneric(tube,str,taille+1);
}


// réception =================================================================

// recevoir n'import quel type de donnée du moment que l'on connait la taille
void recieveDataGeneric(int tube, void *buff, int size)
{
	int sysCall = read(tube, buff, size);
	myassert(sysCall == size,"erreur read tube");
}

// on recoit un entier
int recieveInt(int tube)
{
	int n;
	recieveDataGeneric(tube,&n,sizeof(int));
	return n;
}

// on recoit un entier qui correspond à strlen()
// puis on recoit la chaine a lire
char *recieveStr(int tube)
{
	int n;

	recieveDataGeneric(tube,&n,sizeof(int));

	char *str = malloc(n+1);
	myassert(str != NULL,"erreur alloc chaine");
	recieveDataGeneric(tube,str,n+1);
	return str;
}

