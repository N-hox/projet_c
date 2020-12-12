#ifndef TUBE_H
#define TUBE_H value


// Ouvrir, fermer, créer détruire ============================================

// crée un pipe nommé dans le système de  fichiers
void createNamedPipe(char const *nom);

// retire le pipe nommé du système de fichiers
void destroyNamedPipe(char const *nom);

// ouvre un pipe à partir d'un tableau de fs
void creerPipe(int pipefd[2]);

// Ouvre en lecture ou en écriture un pipe nommé (mod = O_WRONLY ou O_RDONLY) 
int ouvrirNamedPipe(char const *nomTube, int mode);

// ferme un pipe via son descripteur
void fermerPipe(int tube);


// envoyer ====================================================================

// envoie un type de data dont on précise la taille
void sendDataGeneric(int tube, void const *buff, int size);

//envoyer un entier
void sendInt(int tube, int n);

// envoyer une chaine (d'abord la taille, puis la chaine)
void sendStr(int tube, char const *str);


// recevoir ==================================================================

// recevoir n'import quel type de donnée du moment que l'on connait la taille
void recieveDataGeneric(int tube, void *buff, int size);

// recevoir un entier
int recieveInt(int tube);

// on recoit la taille de la chaine, puis la chaine a lire
char *recieveStr(int tube);


#endif