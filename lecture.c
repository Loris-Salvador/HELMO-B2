#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lecture.h"
#define LGMAXCHAINE 512

void LireChaine(char invitation[], int lgmax, char chaine[])
{
	char tampon [LGMAXCHAINE];

	printf("%s", invitation);
	gets(tampon);	// version originale de la fonction, qui n'est plus acceptee par visual studio
	//gets_s(tampon,LGMAXCHAINE);	// version "securisee" de visual studio, pas acceptee par d'autres compilateurs
	tampon[lgmax]='\0';
	strcpy(chaine,tampon);
}


void LireCaractere(char invitation[], char *carac)
{
	char tampon[2];
	LireChaine(invitation,1,tampon);
	*carac=tampon[0];
}


int Encore(char invitation[], char affirmatif, char negatif)
{
	char temp[] = " (x/y) : ";
	char reponse;
	char invitcomplet[LGMAXCHAINE] ;

	/* construire l'invitation complete */
	temp[2]= affirmatif; temp[4]=negatif;
	strcpy(invitcomplet, invitation);
	strcat(invitcomplet, temp);

	do
		LireCaractere(invitcomplet, &reponse);
	while (toupper(reponse)!=toupper(affirmatif)
		   && toupper(reponse)!=toupper(negatif));

	return toupper(reponse)==toupper(affirmatif);
}

int LireEntierEntre(char invitation[], int *entier, int binf, int bsup)
{
	if(!LireEntier(invitation, entier)) return 0;

	if (*entier <= bsup && *entier >= binf)
		return 1;
	else
		return 0;
}

int LireEntier(char invitation[], int *entier)
{
	int temoin=0;
	float reel;

	if(!LireReel(invitation,&reel)) return 0;

	if (reel==(int)reel)
	{
		*entier=(int)reel;
		return 1;
	}

	return 0;
}
int LireReel(char invitation[], float *reel)
{
	char tampon[50+1],fin[50+1];
	int temoin=0;

	LireChaine(invitation,50,tampon);

	temoin=sscanf(tampon,"%f%s",reel,fin);
	if (temoin!=1) return 0;

	return 1;
}

int LireReelEntre(char invitation[], float *reel, float binf, float bsup)
{
	char invitcomplet[LGMAXCHAINE];
	int temoin=0;

	sprintf(invitcomplet,"%s (%f<=   <= %f) ",invitation,binf,bsup);
	if(!LireReel(invitcomplet, reel)) return 0;

	if (*reel<=bsup && *reel>=binf)
		return 1;
	else
		return 0;
}

char* LireChaineDynamique(char invitation[])
{
	char tampon [LGMAXCHAINE];
	char *chaine;

	printf("%s",invitation);
	gets(tampon);	// version originale de la fonction, qui n'est plus acceptee par visual studio
	//gets_s(tampon, LGMAXCHAINE);	// version "securisee" de visual studio, pas acceptee par d'autres compilateurs
	
	chaine=(char*)malloc((strlen(tampon)+1)*sizeof(char));
    if (chaine!=NULL) 
		strcpy(chaine,tampon);
    return chaine;
}