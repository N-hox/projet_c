
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <stdio.h>

#include "myassert.h"
#include "semaphore.h"



// crée une semaphore avec pour val n
// la clé est générée à partir du fichier passé en paramètre
// l'ID de la clé est passé en paramètre
int semCreate(int n, char const *fichier, int ID)
{
	// on génère une clé à partir du fichier et de l'id en paramètre
	int MA_CLE = ftok(fichier, ID);
	myassert(MA_CLE != -1, "Erreur génération de la clé pour création semaphore");
	
	// on crée un semaphore avec la clé et les droits de semaphore.h
	int semId = semget(MA_CLE, 1, DROITS | IPC_CREAT | IPC_EXCL);
	myassert(semId != -1,"Erreur création semaphore avec clé");
	
	// on met la valeur à n
	semSetVal(semId, n);

	return semId;
}

// détruit le semaphore d'id semId
void semDestroy(int semId)
{
	int sysCall = semctl(semId,0,IPC_RMID);
	myassert(sysCall != -1, "Erreur destruction du semaphore");
}

//crée un set de 1 sémaphore anonyme avec pour val n
int semCreateAnon(int n)
{
	// on crée un semaphore anonyme avec les droits de semaphore.H
	int semId = semget(IPC_PRIVATE, 1, DROITS);
	myassert(semId != -1, "Erreur création semaphore anonyme");

	semSetVal(semId, n);

	return semId;
}

//récupère un sémaphore à partir de la clé passé en paramètre
int semRecover(char const *fichier, int ID)
{
	// on génère une clé à partir du fichier et de l'id en paramètre
	int MA_CLE = ftok(fichier, ID);
	myassert(MA_CLE != -1, "Erreur génération de la clé pour récupération semaphore");

	// on récupère le sémaphore à partir de la clé générée
	int semId = semget(MA_CLE, 0, 0);
	myassert(semId != -1,"Erreur récupération semaphore");
	
	return semId;
}

// récupère un sémaphore par sa clé
int semRecoverCle(int cle)
{
	myassert(cle != -1, "Erreur clé pour récupération semaphore");

	int semId = semget(cle, 0, 0);
	myassert(semId != -1,"Erreur récupération semaphore");
	
	return semId;
}


// Accesseurs ================================================================

// passe la valeur du sémaphore à n
void semSetVal(int semId, int n)
{
	int sysCall = semctl(semId, 0, SETVAL, n);
	myassert(sysCall != -1,"Erreur attribution valeur sémaphore");
}

// revoie la valeur du sémaphore 
int semGetVal(int semId)
{
	int val = semctl(semId,0,GETVAL);
	// myassert(val != -1, "Erreur récupération valeur semaphore");
	
	return(val);
}

// revoie le nbr de processus en attente avec l'opération Zéro
int semGetZcnt(int semId)
{
	int zcnt = semctl(semId,0,GETZCNT);
	myassert(zcnt != -1, "Erreur récupération ZCNT semaphore");
	
	return(zcnt);
}

// revoie le nompre de processus qui attendent une augmentation de semval
int semGetNcnt(int semId)
{
	int ncnt = semctl(semId,0,GETNCNT);
	myassert(ncnt != -1, "Erreur récupération NCNT semaphore");
	
	return(ncnt);
}


// Signal, Wait et Zero ======================================================

// envoie un wait au sémaphore
void semWait(int semId)
{
	// on crée une tructure sembuf pour la passer en paramètre de semop()
	struct sembuf sops[1];
	sops->sem_num =  0;
	sops->sem_op  = -1; // opération Wait
	sops->sem_flg =  0;
	
	int sysCall = semop(semId,sops,1);
	myassert(sysCall != -1, "Erreur, opération bloquage sur semaphore");
}

// envoie un signal au sémaphore
void semSignal(int semId)
{
	// on crée une tructure sembuf pour la passer en paramètre de semop()
	struct sembuf sops[1];
	sops->sem_num = 0;
	sops->sem_op  = 1; // opération Signal
	sops->sem_flg = 0;
	
	int sysCall = semop(semId,sops,1);
	myassert(sysCall != -1, "Erreur, opération déblocage sur sémaphore");
}

// effectue l'opération Zero sur le sémaphore (barrière)
void semZero(int semId)
{
	// on crée une tructure sembuf pour la passer en paramètre de semop()
	struct sembuf sops[1];
	sops->sem_num = 0;
	sops->sem_op  = 0; // opération ZERO
	sops->sem_flg = 0;
	
	int sysCall = semop(semId,sops,1);
	myassert(sysCall != -1, "Erreur, opération Zéro sur semaphore");
}

// quand n processus arrivent à la barrière, ils se débloquent
void barriere(int semId, int n)
{
	int nbProcessEnattente = semGetZcnt(semId);
	if (nbProcessEnattente + 1 == n)
		semSetVal(semId, 0);
	else
		semZero(semId);
}
