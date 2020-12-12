#ifndef SEMAPHORE_H
#define SEMAPHORE_H


#define DROITS 0641




// crée une sémaphore avec pour val n
// la clé est générée à partir du fichier passé en paramètre
// l'ID de la clé est passé en paramètre
int semCreate(int n, char const *fichier, int ID);

// détruit le semaphore d'id semId
void semDestroy(int semId);

//crée un set de 1 sémaphore anonyme avec pour val n
int semCreateAnon(int n);

//récupère un sémaphore à partir de la clé passé en paramètre
int semRecover(char const *fichier, int ID);

// récupère un sémaphore par sa clé
int semRecoverCle(int cle);


// Accesseur =================================================================

// passe la valeur du sémaphore à n
void semSetVal(int semId, int n);

// revoie la valeur du sémaphore 
int semGetVal(int semId);

// revoie le nbr de processus en attente avec l'opération Zéro
int semGetZcnt(int semId);

// revoie le nompre de processus qui attendent une augmentation de semval
int semGetNcnt(int semId);


// Signal, Wait et Zero ======================================================

// envoie un wait au sémaphore
void semWait(int semId);

// envoie un signal au sémaphore
void semSignal(int semId);

// envoie opération Zéro
void semZero(int semId);

// quand n processus arrivent à la barrière, ils se débloquent
void barriere(int semId, int n);


#endif
