#include "bal.h"

//Fonction de creation d'une file (de taille nulle)
int Creer_file_bal(struct File_bal *BAL) 
{
    BAL->premier=NULL;
    BAL->dernier=NULL;
    return 0;
}

//Fonction de creation d'une file (de taille nulle)
// on crée le lien entre la bal et la file de lettre
int Creer_file_lettre(struct File_lettre *LET, struct bal_i *bal) 
{
    LET->premier=NULL;
    LET->dernier=NULL;
    LET->courant=NULL;
    bal->premiere_lettre=LET->premier ;
    return 0;
}


/////////////////////////////////////////////////////////
/*
Nous definissons maintenant les primitives des structures de donnees de type file que sont :
    "push" (aussi appelee "enqueue" ou "enfiler")
    "pop" (aussi appelee "dequeue" ou "defiler")
    "isempty" (ou "estVide")
*/

// verifie si la BAL est vide (si des bal ont été créé)
int File_bal_Vide(struct File_bal *BAL) //Primitive des files : isEmpty - Definition d'une fonction permettant de verifier si une file est de taille nulle ou non
{
    return (BAL->premier==NULL);
}

//verifie si il y a des lettres dans la bal
int File_lettre_Vide(struct File_lettre *LET) 
{
    return (LET->premier==NULL);
}

// ajoute une bal dans la BAL (file de boite aux lettres)
int push_bal(struct bal_i *element, struct File_bal *BAL)  
{
    
    if (File_bal_Vide(BAL))  
    /*
    Attention ici il y avait : if(File_bal_vide(element)) or element est une structure de type
    struct bal_i* et File_bal_Vide demande une structure de type struct File_bal*
    Il a donc fallut remplacer element par BAL
    */
    {
        // si la file est vide, premier et dernier pointent sur le meme element

        BAL->dernier=malloc(sizeof(struct bal_i));
        BAL->dernier=element;
        element->premiere_lettre= NULL;
        element->bal_suivant=NULL;
        BAL->premier=BAL->dernier;
        printf("vide\n");
    }
    //sinon, faire le raccordement a l’interieur de la structure chainee
    //et modifier l−>dernier
    else
    {
        struct bal_i *nouveau;
        nouveau=BAL->dernier; // struct temporaire qui garde le dernier element de la liste
        BAL->dernier=malloc(sizeof(struct bal_i));
        element->premiere_lettre= NULL;
        element->bal_suivant=NULL;
        nouveau->bal_suivant=BAL->dernier;
        printf("pas vide\n");
    }
    return 0;
}

//Ajoute une lettre dans la file de lettre d'une bal
int push_lettre(struct lettre *file, struct File_lettre *LET) 
{
    if (File_lettre_Vide(LET))
    {
        // si la file est vide, premier et dernier pointent sur le meme element
        LET->dernier=malloc(sizeof(struct lettre));
        LET->dernier=file; // file contient le message et un pointeur sur null
        LET->premier=LET->dernier;
    }
    
    else
    {
        /*sinon, faire le raccordement a l’interieur de la structure chainee
        et modifier l−>dernier*/
        struct lettre *nouveau;
        nouveau=LET->dernier; // struct temporaire qui garde le dernier element de la liste
        nouveau->suivant=malloc(sizeof(struct lettre)); // allocation memoire
        nouveau->suivant=file; // on ajoute l'lement en fin de liste 
        LET->dernier=nouveau->suivant; // on actualise le pointeur dernier 
    }
    return 0;
}


// on recupere toutes les lettres, on affiche leur message et on supprime l'espace memoire de la lettre
void pop_lettre (struct File_lettre *LET)
{
    LET->courant=LET->premier;
    struct lettre *elmout; 
    while (LET->courant!=NULL) 
    //Il y avait, je pense, une erreur dans la version précédente : courant=!NULL a la place de courant!=NULL
    {
        printf("%s\n",LET->courant->message);
        elmout=LET->courant;
        LET->courant=LET->courant->suivant;
        free(elmout);
    }
}

void main(int argc,char **argv) 
{
    //Recuperation des options :


    // quoi faire ?
    //Etape 1 : Creer une bal
    //Etape 2 : Creer une lettre pour la bal 1
    
    
    //Realisation 
    //Etape 1 - creation d'une bal
    struct File_bal Ensemble;
    struct bal_i BAL;
    BAL.i=1; //Il y aura un autre moyen de changer 'automatiquement' cette valeur -> cf recuperation des options dans le terminal
    Creer_file_bal(&Ensemble); //Creation de la structure de BALs
    push_bal(&BAL,&Ensemble); //On a push dans la file de BALs une premiere BAL
    //Donc normalement ici on a une bal dans une file.
    //On cherche maintenant a creer une lettre et la push dans cette bal
    //
    //Etape 2
    //
    struct File_lettre ssEnsemble1;
    struct lettre lettre1;
    lettre1.message="coucou";
    Creer_file_lettre(&ssEnsemble1,&BAL); //syntaxe correcte est peut-etre Creer_file_lettre(struct File_lettre* ssEnsemble1, struct bal_i* BAL1); ?
    push_lettre(&lettre1,&ssEnsemble1);
    //Verifions maintenant que tout ce passe bien:
    //
    //Verifications
    //

    printf("%s\n",ssEnsemble1.premier->message);
    /*
    Pb ici : on a rien après Ensemble.premier. ce qui veut dire qu'il n'y a pas de liens creer 
    entre la structures de boites aux lettres et un element de cette structure.
    -> Ou est-ce que ce lien est cense se faire ? =
    Je n'ai peut etre pas fait de push ? :o bah si en fait j'en ai fait un
    Ok donc finalement il faut faire un '->' car c'est un pointeur : on fait pointer vers 
    un element distant. (Pourquoi un '.' avant alors ?)
    Premier essai pour recuperer le message : erreur de segmentation ; faut il faire un
    pop ou peut-on recuperer directement ?
    Deuxieme essai : on recupere le numero de la lettre(?) ca marche !
    Troisieme essai : toujours une erreur de segmentation.
    Quatrieme essai : en passant par ssEnsemble1.premier->message ca marche !
    */
    printf("%s\n",lettre1.message);

    /*
    Definition des operations possibles :
    - Creation d'une structure de boites aux lettres :
        struct File_bal Nom_de_l_ensemble;
        Creer_file_bal(&Nom_de_l_ensemble);

    - Creation d'une boite aux lettres :
        struct bal_i nom_boite_aux_lettres;

    - Lier une boite aux lettres avec un ensemble :
        push_bal(&nom_boite_aux_lettres,&Nom_de_l_ensemble);

    - Creation d'une structure de lettres (sous ensemble d'une boite aux lettres) :
        struct File_lettre Ensemble_de_lettres;
        Creer_file_lettre(&Ensemble_de_lettres,&BAL_mere);

    - Creation d'une lettre :
        struct lettre lettre_A;
    
    - Lier une lettre et un ensemble de lettres :
        push_lettre(&lettre_A,&Ensemble_de_lettres);

    - Lire le numero de la boite aux lettres:
        Nom_de_l_ensemble.premier->i;
    
    - Lire un message d'une boite aux lettres specifiques :
        Ensemble_de_lettres.courant->message;
    
    //Essayer de passer par un "pointeur" temporaire
    */
}
