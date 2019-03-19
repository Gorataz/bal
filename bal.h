/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
#define CST 40

struct lettre  // structure de lettre
{
    char *message ;
    struct lettre *suivant;
};

struct bal_i // structure de bal
{
    int i ; // numero du recepteur
    struct bal_i *bal_suivant ; // pointe sur la bal suivante
    struct lettre *premiere_lettre ; // pointe sur la premiere lettre de ma bal
};

struct File_bal // Creation d'une structure file bal 
{
    struct bal_i *premier;
    struct bal_i *dernier;
};

struct File_lettre // Creation d'une structure file lettre 
{
    struct lettre *premier;
    struct lettre *dernier;
    struct lettre *courant;
};

//void construire_message(char * message,char motif,int lg);

int Creer_file_bal(struct File_bal *BAL) ;

int Creer_file_lettre(struct File_lettre *LET, struct bal_i *bal) ;

int File_bal_Vide(struct File_bal *BAL) ;

int File_lettre_Vide(struct File_lettre *LET) ;

int push_bal(struct bal_i *file, struct File_bal *BAL) ;

int push_lettre(struct lettre *file, struct File_lettre *LET) ;

int pop_lettre (struct File_lettre *LET) ;