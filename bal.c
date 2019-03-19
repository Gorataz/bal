#include "bal.h"

// Pour que se soit plus clair j'éceis BAL pour les files de boites aux lettres
// LET pour le files de lettres
// file pour les lettres et bal 


void construire_message(char * message,char motif,int lg, int num) 
{
	int i;
	if (num<10) {
		message[0]='-' ;
		message[1]='-' ;
		message[2]='-' ;
		sprintf(&message[3],"%d",num );
	}
	else if (num<100) {
		message[0]='-' ;
		message[1]='-' ;
		sprintf(&message[2],"%d",num );
	}
	else if (num<1000) {
		message[0]='-' ;
		sprintf(&message[1],"%d",num ); // comme printf mais on ecrit en premier argument ou on veut ecrire
	}
		
	for (i=4; i<lg; i++)
   	{
		message[i]=motif; 
	}
	
}

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
    bal->premiere_lettre = LET->premier ;
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
int push_bal(struct bal_i *file, struct File_bal *BAL)  
{
    
    if (File_bal_Vide(file))
    {
        // si la file est vide, premier et dernier pointent sur le meme element

        BAL->dernier=malloc(sizeof(struct bal_i));
        BAL->dernier=file;
        file->premiere_lettre= NULL;
        file->bal_suivant=NULL;
        BAL->premier=BAL->dernier;
    }
    //sinon, faire le raccordement a l’interieur de la structure chainee
    //et modifier l−>dernier
    else
    {
        struct bal_i *nouveau;
        nouveau=BAL->dernier; // struct temporaire qui garde le dernier element de la liste
        BAL->dernier=malloc(sizeof(struct bal_i));
        file->premiere_lettre= NULL;
        file->bal_suivant=NULL;
        nouveau->bal_suivant=BAL->dernier;
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
        nouveau-> suivant=malloc(sizeof(struct lettre)); // allocation memoire
        nouveau-> suivant=file ; // on ajoute l'lement en fin de liste 
        LET-> dernier = nouveau-> suivant ; // on actualise le pointeur dernier 
    }
    return 0;
}


// on recupere toutes les lettres, on affiche leur message et on supprime l'espace memoire de la lettre
int pop_lettre (struct File_lettre *LET)
{
    LET-> courant = LET-> premier ;
    struct lettre *elmout ; 
    while (LET -> courant =! NULL) 
    {
        printf("%s", LET-> courant-> message) ;
        elmout = LET -> courant ;
        LET-> courant = LET-> courant-> suivant ;
        free(elmout) ; 
    }
}

// je pense que l'on a pas besoin de supprimer les bal 
// quand un recpeteur recupère ses lettre, on garde sa boite aux lettres
/*
int pop_bal(struct File_bal BAL, struct bal_i )  
{
    struct Element *ElmtOut;
    if (estVide(file))
    {
        printf("Impossible de supprimer, pas de boites aux lettre");
        return -1; //code erreur
    }

    if (file->courant == file->premier)// Cas ou pointeur courant est sur l'element premier
    {
        ElmtOut=file->premier; //On recupere le pointeur present dans premier
        file->premier=file->premier->suivant; //On designe la case suivante comme etant la premiere
        free(ElmtOut); //On libere l'espace memoire reservee precedemmment avec le malloc
        return 0;
    }

    if (file->courant == file->dernier)//Cas ou pointeur courant est sur l'element dernier
    {
        ElmtOut=file->premier;
        while(ElmtOut->suivant != file->courant)
        {
            ElmtOut=ElmtOut->suivant;
        }
        ElmtOut->suivant=ElmtOut->suivant->suivant;
        free(file->courant);
        file->dernier=ElmtOut;
        return 0;
    }

    else //Cas ou pointeur courant est sur n'importe quel element
    {
        ElmtOut=file->premier;
        while(ElmtOut->suivant != file->courant)
        {
            ElmtOut=ElmtOut->suivant;
        }
        ElmtOut->suivant=ElmtOut->suivant->suivant;
        free(file->courant);
        return 0;
    }
}

*/
/*
Fin de la definition des primitives de la structure de donnees file
*/

/////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////
///			Début gestion tsock						  ///
/////////////////////////////////////////////////////////

void main (int argc, char **argv)
{
	/*****************************************************************************************/
	/*							 Initialisaiton des variables							     */ 
	/*****************************************************************************************/
	int c;
	extern char *optarg;
	extern int optind;
    int lg_max=30;
	int nb_message = 10; // Nb de messages construire_message à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception
	int source = -1 ; // 0=puits, 1=source
    int reseau=1 ; // 0=UDP 1=TCP
	int sock;
	/*****************************************************************************************/
	/*																						 */
	/*****************************************************************************************/

	while ((c=getopt(argc,argv,"pn:l:su"))!=-1) 
	{
		switch (c) 
		{
		case 'p':
			if (source == 1) 
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0) 
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;

        case 'u' :
            reseau=0;
            break;

        case 'l' :
            lg_max=atoi(optarg);
            break;

		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}
	if (source==-1) 
	{
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1);
	}

	/*****************************************************************************************/
	/*							 		Gestion de TCP						        	     */ 
	/*****************************************************************************************/	

     if (reseau==1)
    {
        if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1) 
	    {
            perror("Echec de création du socket :");
            exit(1);
        }
        
	    if (source == 1) 
		{
		    //printf("on est dans la source\n");
            struct hostent *hp;
            struct sockaddr_in adr_distant; 

            // affectation domaine et numero de port
            adr_distant.sin_family=AF_INET;
            adr_distant.sin_port=htons(atoi(argv[argc -1])); // htons :pour convertir de la norme little endian a la norme big endian
            // affectation adresse IP
            if ((hp=gethostbyname(argv[argc -2]))==NULL) 
			{
                perror("Erreur du gethostbyname :");
                exit(1);
            }
		
            memcpy((char*)&(adr_distant.sin_addr.s_addr),(*hp).h_addr,(*hp).h_length);

		    char *m; //On peut declarer cette variable en amont mais il est preferable de laisser le malloc ici afin de ne pas faire une reservation inutile de memoire si on est en reception
            m=malloc(lg_max*sizeof(char));

            if (connect(sock,(struct sockaddr*) &adr_distant,sizeof(adr_distant))==-1) 
			{
            	perror("Erreur connection TCP"); 
			    exit(1);
            }

            printf("SOURCE : lg_mesg_lu=%d, port=%d, nb_envois=%d, TP=tcp, dest=%s\n",lg_max,adr_distant.sin_port,nb_message,argv[argc -2]);
            for (int i=0;i<nb_message;i++) 
		    {
		        construire_message(m,'a'+i,lg_max,i+1) ; // ATTENTION: ne pas mettre sizeof(m) 
			    if (write(sock,m,lg_max)==-1)
			    {
				    perror("Erreur ecriture du message :"); // affiche detail de l'erreur
				    exit(1);
			    } 
			    printf("SOURCE : Envoi n°%d (%d)[%s]\n",i+1,lg_max,m);
		    }
	    }

        // crée une autre adresse pour le bind
	    else 
        { // je suis recepteur
		    //printf("on est dans le puits\n");
            int sock_bis; //si probleme ici : on avait declare int sock,sock_bis; mais sock est 'normalement' deja declare precedemment
            struct sockaddr_in adr_client;
            int lg_adr_client=sizeof(adr_client);
            int lg_rec;
		    char *message; //Voir ce que l'on peut remonter

            memset((char*)& adr_client,0,sizeof(adr_client));
            adr_client.sin_family=AF_INET; 
            adr_client.sin_port=htons(atoi(argv[argc -1]));
            adr_client.sin_addr.s_addr=INADDR_ANY; 
            sock=socket(AF_INET,SOCK_STREAM,0);

            // association adresse du socket et representation interne
            if (bind(sock,(struct sockaddr*) &adr_client,lg_adr_client)==-1) 
			{
                perror("echec du bind"); 
                exit(1);
            }

            listen(sock,5);

            if ((sock_bis=accept(sock,(struct sockaddr*)&adr_client,&lg_adr_client))==-1)
            {
                perror("Echec du accept TCP :");
                exit(1);
            }

            message=malloc(lg_max*sizeof(char));
		    printf("PUITS : lg_mesg_lu=%d, port=%d, nb_reception=infini, TP=tcp\n",lg_max,adr_client.sin_port);
            for (int i=0; i<nb_message; i++) 
			{
                if ((lg_rec = read(sock_bis, message, lg_max))<0)
                {
                    perror("Echec lecture du message TCP :");
                    exit(1);
                }
			printf("PUITS : Reception n°%d (%d) [%s]\n",i+1,lg_max,message);
            }
        }
    }
	
	/*****************************************************************************************/
	/*							 	Fin Gestion de TCP						        	     */ 
	/*****************************************************************************************/


if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else
		printf("nb de tampons à envoyer = infini\n");
	}
}
