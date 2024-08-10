#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include "lecture.h"

#define DELIMITEUR ';' //delimiteur que l'on peut changer si jms on a des , on change
#define LGMAXLIGNE 512 // nbr max lignes fichier

// en tete fonction
char* RecupererToken(char chaine[], char delimiteur);//fonction qui renvoie un pointeur vers une chaine de caractere,prend une chaine jusqu'au premiere delimiteur 
//si on veut appeler toute la ligne on doit l'appeler autant de fois qu'on en a besoin.
int ExtraireInformations(char ligne[], char** question, int* nbPropositions, char*** propositions, float** points); //char*** est la pour ressortir un tableau a double entrée 
char** ExtraireFichierEnTableau(char nomFichier[], int* nbLigne);
int TirageAleatoire(int maximum, int tab[], int nbElements);
void FreeTabQcm(char** tabQcm, int nbLignes);
float RechercherMaximum(float tab[], int nbElements);
void PermutationNEntier(int *tab, int n);
//Fonction 5
void CalculCoeficients(float tab[], int x, float* a, float* b, float* pearson);
//Fonction 6
float* AjoutResultat(float* tab, int n, float resultat);


int main() {

    srand(time(NULL));// chaque execution avoir tirage aleatoire

    int nbQuestions, nbLignes, numLigne, reponse, nbPropositions, retour, i;
    int questionsPosees = 0;
    int* tabQuestionsPosees;
    char* question;
    char* nomFichier;
    char** propositions; //pointeur de pointeur d'une chaine de caractere
    float* points; //ini d'un pointeur de float une fonction plus loin 
    float total = 0;
    float maxPoints = 0;
    char rejouer;
    float* tabResultat = NULL;
    int nbPartiesJouee = 0;
    float a;
    float b;
    float pearson;
    float pourcentage;

    nomFichier = LireChaineDynamique("Entrez le nom du fichier : "); //nom fichier il y aura ce qu'on a envoyer dans terminal et donc le chemin vers le fichier

    char** tabQcm = ExtraireFichierEnTableau(nomFichier, &nbLignes); //tableau a deux dimension, lis tout le fichier et le mets dans tabqcm.Nous avons tout le fichier dedans
    free(nomFichier); //vu que tout est dans tab qcm on n'a plus besoin du fihcier, meme si on rejoue on garde tabqcm
    if (tabQcm == NULL)
    {
        return 1;//erreur dans extraire fichier tableau. 
    }

    do //nous permet d'avoir acces a toute les infos dans l'excel du prof
    {
        do
        {
            LireEntier("\nCombien de questions voulez-vous : ", &nbQuestions); //demander le nbr de question qu'on veut 
            if (nbQuestions > nbLignes)
            {
                printf("Il n'y a pas assez de questions dans le fichier !"); 
            }
        } while (nbQuestions > nbLignes); //tant que le nbr de question n'est pas bon 10 ca nous renvoie pas assez de question

        tabQuestionsPosees = (int*)malloc(nbQuestions * sizeof(int)); //tableau de int qui sert à ne pas poser une question deux fois. et savoir ce qu'on a deja posé
        if (tabQuestionsPosees == NULL) {
            FreeTabQcm(tabQcm, nbLignes); //liberer la memoire en cas de problemes 
            printf("Erreur d'allocation Tableau questions posees.\n");
            return 1;
        }

        do
        {
            numLigne = TirageAleatoire(nbLignes, tabQuestionsPosees, nbQuestions); //numligne == nouvelle question tirée

            retour = ExtraireInformations(tabQcm[numLigne], &question, &nbPropositions, &propositions, &points); //  1er entre et 4autres sorties --> une fois passer dedans toutes variables sont initialise 
            if (retour == 1) //si probleme on libere 
            {
                free(tabQuestionsPosees);
                FreeTabQcm(tabQcm, nbLignes);
                return 1;
            }

            tabQuestionsPosees[questionsPosees] = numLigne; //on ajoute le numero de la ligne au tableau de questions deja posées car on veut plus retombé dessus

            printf("\nQuestion: %s\n", question); 

            int* tabOrdreProposition = (int*)malloc(nbPropositions * sizeof(int));

            PermutationNEntier(tabOrdreProposition, nbPropositions);

            printf("\nPropositions :\n");
            for (i = 0; i < nbPropositions; i++) {
                printf("%d : %s\n", i + 1, propositions[tabOrdreProposition[i]]); //%s imprime les chaines de caracter == les propositions
            }

            do {
                LireEntier("\nVotre Reponse : ", &reponse);
                if (reponse > nbPropositions || reponse < 1) // || ==OU
                {
                    printf("Reponse invalide\n");
                }
            } while (reponse > nbPropositions || reponse < 1);

            total = total + points[reponse - 1]; //-1 car tableau commence a 0 
            maxPoints = maxPoints + RechercherMaximum(points, nbPropositions); 

            free(question); //on libere tout pour la prochaine question 
            for (i = 0; i < nbPropositions; i++) { // boucle car plrs propo dans le tableau 
                free(propositions[i]); //liberer les elements du tableau
            }
            free(propositions); // libere le tableau 
            free(points);
            free(tabOrdreProposition);

            questionsPosees++;
        } while (questionsPosees < nbQuestions);

        pourcentage = total/maxPoints * 100;
        if(pourcentage < 0)
        {
            pourcentage = 0;
        }

        tabResultat = AjoutResultat(tabResultat, nbPartiesJouee, pourcentage);
        nbPartiesJouee++;


        printf("\nVotre Total : %.2f/%.2f", total, maxPoints); 

        LireCaractere("\n\nVoulez vous rejouer? (O/N)", &rejouer);

        if (rejouer == 'O')
        {
            total = 0;
            questionsPosees = 0;
            maxPoints = 0;
            system("cls"); //clear le terminal 
        }

        free(tabQuestionsPosees); // on peut reutilise les questions qu'on a utiliser dans le 1er jeu pour le prochian

    } while (rejouer == 'O');

    if(nbPartiesJouee > 1) // pas de sens de calculer la linéarité si 1 seule partie
    {
        system("cls"); //clear le terminal 
        printf("Voici l'ensemble de vos résultat sous forme de pourcentage :\n\n");
        for(int i = 0 ; i < nbPartiesJouee; i++)
        {
            printf("%d : %.2f %%\n", i+1, tabResultat[i]);
        }
        CalculCoeficients(tabResultat, nbPartiesJouee, &a, &b, &pearson);
        printf("\nOrdonnee a l'origine (a) : %.2f\n", a);
        printf("Pente (b) : %.2f\n", b);
        printf("Coefficient de Pearson : %.2f\n", pearson);
        if (pearson > 0.8 || pearson < -0.8) 
        {
            if (b > 0) 
            {
                printf("\nLa progression est fortement lineaire et positive.\n");
            } 
            else if (b < 0) 
            {
                printf("La progression est fortement lineaire et negative.\n");
            } 
            else 
            {
                printf("La progression est lineaire mais sans changement significatif.\n");
            }
        } 
        else 
        {
            printf("\nLa relation lineaire n'est pas tres forte, ce qui suggere une variabilité importante dans la progression.\n");
        }
    }

    free(tabResultat);
    FreeTabQcm(tabQcm, nbLignes); 

    return 0;
}

char* RecupererToken(char chaine[], char delimiteur) { //deux entrees chaine et delimiteur, une chaine et le delimiteur et la sortie est debut chaine jusqu'au delimeur 
    int i = 0;
    while (chaine[i] != delimiteur && i < strlen(chaine)) {
        i++; //compte le nbr de caractere jusqu'au 1er delimiteur. && est la pour s'arreter a la fin de la chaine
    }

    char* token = (char*)malloc((i + 1) * sizeof(char));//allocation dynamique car apres etre passer dans la chaine on sait le nbr de caractere, malloc== fonction
    if (token == NULL) { //si probleme avec malloc ca nous renvoie rien, token c'est ce qu'on va renvoyer(token== chaine jusqu'au delimiteur) 
        return NULL;
    }

    strncpy(token, chaine, i);//token== destination, chaine==source(on met chaine dans token), on met que les i premiers dans token
    token[i] = '\0'; //tu finis ta chaine par un \0
    return token;
}

int ExtraireInformations(char ligne[], char** question, int* nbPropositions, char*** propositions, float** points) { //extraire ce qu'il y a dans ligne dans chauqe variable
    char* ptrLigne = ligne; //pointe vers le premier element de la ligne

    *question = RecupererToken(ptrLigne, DELIMITEUR); //on lui passe toute la ligne, et dans question on met tout ce qui a avant le premier; 
    if (question == NULL) {
        printf("Erreur d'allocation Token question.\n");
        return 1;
    }

    ptrLigne = ptrLigne + strlen(*question) + 1;//on avance le pointeur jusqu'au premier ; et on va au 2eme ;

    char* nbPropStr = RecupererToken(ptrLigne, DELIMITEUR); //recup token renvoie des char*
    sscanf(nbPropStr, "%d", nbPropositions); //on veut pas des char mais des int. On recupere le nbr de propositon en char et on les transforme avec sscanf pour avoir int
   

    ptrLigne = ptrLigne + strlen(nbPropStr) + 1; //on avance le pointeur jusqu'au 2eme ; et on va au 3eme ;
    free(nbPropStr); //libere une fois que l'on a converti le char en int

    *propositions = (char**)malloc(*nbPropositions * sizeof(char*));//on alloue pour le propositon,pointeur vers tableau de pointeur et malloc va donc alloue pour le char de nbr de proposition
    if (question == NULL) {
        free(*question); //si double pointeur on asse une *
        printf("Erreur d'allocation propositions.\n");
        return 1;
    }

    for (int i = 0; i < *nbPropositions; i++) {//on a trouvé les proposition dans la ligne et on va remplir le tableau de propostions
        (*propositions)[i] = RecupererToken(ptrLigne, DELIMITEUR); //determiner ordre prio pour les () du *propo, ici on veut l'element i de la valeur de propositions si on met pas on a la valeur de l'element i de proposionts
        ptrLigne = ptrLigne + strlen((*propositions)[i]) + 1;  //on met chaque element a sa place //on veut la valeur et non pas l'adresse(positions exacte dans pc) alord que valeur== ce qu'il y a dans l'adresse
    }

    *points = (float*)malloc(*nbPropositions * sizeof(float)); //alloue nbpropositon x la taille d'un float 
    if (question == NULL) {
        free(*propositions);
        free(*question);
        printf("Erreur d'allocation points.\n");
        return 1;
    }

    for (int i = 0; i < *nbPropositions; i++) { //rempli dans un tableau les points de la ligne

        char* tmp = RecupererToken(ptrLigne, DELIMITEUR); //recuperer dans tampon les points dans chaque tour de boucle
        sscanf(tmp, "%f", &(*points)[i]); //recuperer dans un char et le transformer en float et le mettre dans tableau de points 
        ptrLigne = ptrLigne + strlen(tmp) + 1;//avancer le pointeur
        free(tmp);
    }

    return 0;
}

char** ExtraireFichierEnTableau(char nomFichier[], int* nbLigne) //nom fichier ce qu'on a rentrer dans terminal, main envoie adresse de nblignes comme ca la fonction peut modif nbr de ligne
{
    FILE* fp; //pointeur vers un fichier (fichier excel), se deplace dedans
    char** tab; //ce qu'on renvoie a la fin de la fonction, tout le tableau
    char ligne[LGMAXLIGNE];//ou on stocke une ligne dans le fihcier;lgmax car on sait pas nbr de caractere dans une ligne du fichier 

    fp = fopen(nomFichier, "r"); //ouvre le fichier en mode lecture, fp va se mettre dans le premiere caractere du fichier
    if (fp == NULL) // probleme car pas fp au debur du fichier
    {
        printf("Impossible d'ouvrir le fichier !\n");
        return NULL;
    }

    *nbLigne = 0; //modifier valeur de nb lignes et pas son adresse car*
    while (fgets(ligne, LGMAXLIGNE + 1, fp) != NULL)//sert a compter le nombre de lignes, il se mettra a la ligne suivante grace au lignemax +1, fp va parcourir le fichier
    {                                                  // copie ligne du fichier dans la variable ligne. 
        (*nbLigne)++;                                  
    }

    tab = malloc((*nbLigne) * sizeof(char*)); //allouer autant de places que l'on a de ligne, allouer nb ligne *de la place pour des pointeurs de char. 
    if (tab == NULL)//probleme                // pointeur de chaine de caractere
    {
        printf("Erreur d'allocation tableau\n");
        return NULL;
    }

    rewind(fp);//une fois tout lu,la fct rewind remets fp au debut pour le relire et ne pas avoir de tabqcm vide. 
    int i = 0; //adresse du premier element, i parcours le tableau de pointeur. 

    while (fgets(ligne, LGMAXLIGNE + 1, fp) != NULL) //chaque element du tab va etre allouer a une zone de memoire
    {
        tab[i] = malloc((strlen(ligne) + 1) * sizeof(char)); // on alloue une chaine de caractere == la ligne du tab, 
        if (tab[i] == NULL)
        {
            free(tab); // liberer le tab si on a une erreur de malloc
            printf("Erreur d'allocation ligne\n");
            return NULL;
        }
        strcpy(tab[i], ligne);//dans tab i on a alloué de la place donc on peut copier la ligne dans tab i.
        tab[i][strlen(ligne)] = '\0'; //mettre le \0 a la fin de la ligne
        i++;
    }

    fclose(fp); //femeture du fichier Excel 
    return tab; // tout ce qu'on a fait dans la fonction on le renvoie dans tab. 
}

int TirageAleatoire(int maximum, int tab[], int nbElements) { //maximum== nbr de ligne max ici 10, tab== tableau de question deja posées, nbElement == cbm element dans tab, (1;3;4;...)
    int retry, nb; //si on tombe sur question deja poser

    do
    {
        retry = 0;
        nb = rand() % maximum; // je ne veux pas un nbr plus gd que le max==10, si max==10 nbr peut pas etre plus grand que 9

        for (int i = 0; i < nbElements; i++) //parcours chaque element du tableau
        {
            if (nb == tab[i]) //si le nb est egale a un element du tableau qu'on a deja tire alors retry==1
            {
                retry = 1;
                break; //sort du for car c'est sur qu'on va recommencer
            }
        }
    } while (retry == 1); // des qu'on rentre dans le if retry ==1 donc refais le do while tant que retry n'est pas different de 1. Des qu'il est ==0 on sort de la boucle et nouvelle question est posé

    return nb;
}

void FreeTabQcm(char** tabQcm, int nbLignes)
{
    for (int i = 0; i < nbLignes; i++) {
        free(tabQcm[i]); //liberer element tableau 
    }
    free(tabQcm);//liberer le tableau 
}

float RechercherMaximum(float tab[], int nbElements)  // parcours le tableau pour trouver le max et donc trouver le deno des points /...
{
    float max = 0;

    for (int i = 0; i < nbElements; i++)
    {
        if (tab[i] > 0)
        {
            max = max + tab[i];
        }
    }

    return max;
}

void PermutationNEntier(int *tab, int n) {
    for (int i = 0; i < n; i++) {
        tab[i] = i;
    }

    int tmp;

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        tmp = tab[i];
        tab[i] = tab[j];
        tab[j] = tmp;
    }
}



//AOUT
//Fonction 5

void CalculCoeficients(float tab[], int x, float* a, float* b, float* pearson)
{
    int sommeX = 0;
    float sommeY = 0;
    int sommeCarreX = 0;
    float sommeProduitXY = 0;
    float moyenneX = 0;
    float moyenneY = 0;
    float sommeProduitEcart = 0;
    float sommeCarreEcartX = 0;
    float sommeCarreEcartY = 0;

    for(int i = x; i>0; i--)
    {
        sommeX = sommeX + i;
    }

    for(int i = 0; i<x; i++)
    {
        sommeY = sommeY + tab[i];
    }

    for(int i = x; i>0; i--)
    {
        sommeCarreX = sommeCarreX + (i*i);
    }

    for(int i = 0; i<x; i++)
    {
        sommeProduitXY = sommeProduitXY + ((i+1) * tab[i]);
    }

    moyenneX = (float)sommeX/(float)x;
    moyenneY = sommeY/x;

    *b = (sommeProduitXY - x * moyenneX * moyenneY)/(sommeCarreX - x * moyenneX);
    *a = moyenneY - *b * moyenneX;

    for(int i = 0; i<x; i++)
    {
        sommeProduitEcart = sommeProduitEcart + ((i+1 - moyenneX) * (tab[i] - moyenneY));
    }

    for(int i = 0; i<x; i++)
    {
        sommeCarreEcartX = sommeCarreEcartX + ((i + 1 - moyenneX) * (i + 1 - moyenneX));
    }

    for(int i = 0; i<x; i++)
    {
        sommeCarreEcartY = sommeCarreEcartY + ((tab[i] - moyenneY) * (tab[i] - moyenneY));
    }

    *pearson = sommeProduitEcart / sqrtf(sommeCarreEcartX * sommeCarreEcartY);
}

//Fonction 6

float* AjoutResultat(float* tab, int n, float resultat)
{
    float* nouveauTab = (float*)malloc((n + 1) * sizeof(float));
    if(nouveauTab == NULL)
    {
        printf("Erreur d'allocation nouveau tableau\n");
        return NULL;
    }

    if(tab == NULL)
    {
        nouveauTab[0] = resultat;
        return nouveauTab;
    }

    if (tab != NULL) {
        memcpy(nouveauTab, tab, n * sizeof(float));
        free(tab);
    }

    nouveauTab[n] = resultat;
    return nouveauTab;
}
