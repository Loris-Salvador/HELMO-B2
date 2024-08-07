#ifndef Lecture_h
#define Lecture_h

/*--------------------------------------------------------------
Interface du module de lecture : lecture.h
---------------------------------------------------------------*/

/*--------------------------------------------------------------
Inviter à entrer une chaîne de caractères au clavier par le message
<invitation>, la lire et la ranger à partir de l'adresse <chaine> à
concurrence de <lgmax> caractères ('\0' exclu) .
--------------------------------------------------------------*/
void LireChaine(char invitation[], int lgmax, char chaine[]);

/*--------------------------------------------------------------
Inviter à entrer un seul caractère au clavier par le message
<invitation>, lire ce caractère et le ranger à l'adresse <carac>
--------------------------------------------------------------*/
void LireCaractere(char invitation[], char *carac);

/*--------------------------------------------------------------
Inviter à entrer un caractère en réponse à l'invitation <invitation>
Si le caractère lu, indépendamment de la casse, est égal à <affirmatif>,
la fonction retourne 1; s'il est égal à <negatif> la fonction
retourne 0 sinon l'invitation à entrer un caractère est émise à
nouveau.
----------------------------------------------------------------*/
int Encore(char invitation[], char affirmatif, char negatif);

/*-------------------------------------------------------------------
Inviter à entrer un entier compris entre <binf> et <bsup>, au clavier
par le message <invitation>, le lire et le ranger à l'adresse <entier>
En cas d'erreur, la fonction retourne 0. La valeur pointée par
<entier> n'est significative que si la fonction retourne 1.
--------------------------------------------------------------------*/
int LireEntierEntre(char invitation[], int *entier, int binf, int bsup);

/*-----------------------------------------------------------------
Inviter à entrer un entier au clavier par le message <invitation>,
le lire et le ranger à l'adresse <entier>. En cas d'erreur, la fonction
retourne 0. La valeur pointée par <entier> n'est significative
que si la fonction retourne 1.
--------------------------------------------------------------------*/
int LireEntier(char invitation[], int *entier);

/*-------------------------------------------------------------------
Inviter à entrer un réel compris entre <binf> et <bsup>, au clavier
par le message <invitation>, le lire et le ranger à l'adresse <entier>
En cas d'erreur, la fonction retourne 0. La valeur pointée par
<entier> n'est significative que si la fonction retourne 1.
--------------------------------------------------------------------*/
int LireReelEntre(char invitation[], float *reel, float binf, float bsup);

/*-------------------------------------------------------------------
Inviter à entrer un réel au clavier par le message <invitation>,
le lire et le ranger à l'adresse <entier>. En cas d'erreur, la fonction
retourne 0. La valeur pointée par <entier> n'est significative
que si la fonction retourne 1.
--------------------------------------------------------------------*/
int LireReel(char invitation[], float *reel);


/*--------------------------------------------------------------
Inviter à entrer une chaine de caracteres au clavier par le message
<invitation>, la lire et la ranger dans un emplacement alloué
dynamiquement dont l'adresse sera la valeur de retour de la fonction.
Si l'allocation échoue, la valeur de retour vaut NULL. L'appelant doit libérer
l'emplacement.
--------------------------------------------------------------*/
char* LireChaineDynamique(char invitation[]);

#endif