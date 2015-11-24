//============================================================================
// Name        : Pilotage.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ObjBase.h>
#include "../Include/tinyxml.h"

using namespace std;

typedef struct {
	float coordonnees[3], coordDirection[3]; // coordonnées de la pupilles et d'un deuxième point de l'axe du champs de vision de la pupille
	int repereAssocie; // repère auquel les positions du point sont attribués
} s_Point, *s_PointPtr;

typedef struct {
	s_Point Origine; // Origine représente les coordonnées du vecteur de translation entre l'origine des 2 repères
	float G,S,R; // valeur de rotation du repère en x (G:gisement), en y (S:site) et en z (R:roulis).
	int nomRepere,ddl; // le nom du repère (entier) et les degrés de libertés du repère (entier allant de 0 a 63)
	float deplacementMax[6]; // valeurs maximales des translations et rotation (tx,ty,tz,rx (G),ry (S),rz (R))
} s_Repere, *s_ReperePtr;

void initRotS(float S , float ** rotS, float sens){
	// initialisation de la matrice de rotation en y (site)
	int i,j;
	float matRotS[3][3] = {{cos(S),0,sens*sin(S)},{0,1,0},{sens*-sin(S),0,cos(S)}}; // sens = +-1 et transpose en matrice inverse
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			rotS[i][j] = matRotS[i][j];
		}
	}
}

void initRotG(float G , float ** rotG, float sens){
	// initialisation de la matrice de rotation en x (gisement)
	int i,j;
	float matRotG[3][3] = {{1,0,0},{0,cos(G),sens*-sin(G)},{0,sens*sin(G),cos(G)}}; // sens = +-1 et transpose en matrice inverse
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			rotG[i][j] = matRotG[i][j];
		}
	}
}

void initRotR(float R , float ** rotR, float sens){
	// initialisation de la matrice de rotation en z (roulis)
	int i,j;
	float matRotR[3][3] = {{cos(R),sens*-sin(R),0},{sens*sin(R),cos(R),0},{0,0,1}}; // sens = +-1 et transpose en matrice inverse
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			rotR[i][j] = matRotR[i][j];
		}
	}
}

void produitMatriciel3x1(float * matResult3x1, float ** rot1, float coordonnees[3]){
	// produit matriciel entre une matrice 3x3 et une matrice 1x3
	int k;
	for(k=0;k<3;k++){
		matResult3x1[k]=rot1[0][k]*coordonnees[0]+rot1[1][k]*coordonnees[1]+rot1[2][k]*coordonnees[2];
	}
}

void additionMatriciel(float * matResult3x1, float coordonnees[3]){
	// addition matricielle entre 2 matrices 3x1
	float matCalcul[3];
	int k;
	for(k=0;k<3;k++){
		matCalcul[k]=matResult3x1[k];
	}
	for(k=0;k<3;k++){
		matResult3x1[k]=coordonnees[k]+matCalcul[k]; // matResult est un des 2 opérandes et contient le résultat
	}
}

void changementRepere(s_Repere r, s_PointPtr p1){
	/*
	 * Permet de changer les position d'un point passé en argument (s_Point * p1) pour le passer
	 * d'un repère a une autre. Cela fonctionne dans les 2 sens
	 */
	int n;
	float coordonneesPoint[3],coordonneesOrigine[3],sens=1;

	float ** rotS;
	rotS = (float **)malloc(sizeof(float*)*3);
	for(n=0;n<3;n++){
		rotS[n] = (float*)malloc(sizeof(float)*3);
		}

	float ** rotG;
	rotG = (float **)malloc(sizeof(float*)*3);
	for(n=0;n<3;n++){
		rotG[n] = (float*)malloc(sizeof(float)*3);
	}

	float ** rotR;
	rotR = (float **)malloc(sizeof(float*)*3);
	for(n=0;n<3;n++){
		rotR[n] = (float*)malloc(sizeof(float)*3);
	}

	float * matResult3x1;
	matResult3x1 = (float *)malloc(sizeof(float)*3);

	for(n=0;n<3;n++){
		coordonneesPoint[n]=p1->coordonnees[n]; // on enregistre les coordonnées du point passé en argument
	}

	float ** matCalc;
	matCalc = (float **)malloc(sizeof(float*)*3); // on initialiste notre matrice 3x3 pour les calculs
	for(n=0;n<3;n++){
		matCalc[n] = (float*)malloc(sizeof(float)*3);
	}

	if (r.nomRepere > p1->repereAssocie){ // on determine dans quel sens on souhaite faire le changement de repère
		sens = -1;
	}
	for(n=0;n<3;n++){
		coordonneesOrigine[n]=sens*r.Origine.coordonnees[n]; // coordonnées du repère passé en argument, pour le changement de repère
	}
	initRotS(r.S,rotS,sens);
	initRotG(r.G,rotG,sens);
	initRotR(r.R,rotR,sens);
	if (sens == -1){ // si l'on remonte dans les repère (nomRepere croissant)
		for(n=0;n<3;n++){
			matResult3x1[n]=coordonneesPoint[n];
		}
		additionMatriciel(matResult3x1,coordonneesOrigine);
		for(n=0;n<3;n++){
			coordonneesPoint[n]=matResult3x1[n];
		}
		produitMatriciel3x1(matResult3x1,rotG,coordonneesPoint);
		for(n=0;n<3;n++){
			coordonneesPoint[n]=matResult3x1[n];
		}
		produitMatriciel3x1(matResult3x1,rotS,coordonneesPoint);
		for(n=0;n<3;n++){
			coordonneesPoint[n]=matResult3x1[n];
		}
		produitMatriciel3x1(matResult3x1,rotR,coordonneesPoint);
	}
	else if (sens==1){ // si l'on descend dans les repères (nomRepere décroissant)
		produitMatriciel3x1(matResult3x1,rotR,coordonneesPoint);
		for(n=0;n<3;n++){
					coordonneesPoint[n]=matResult3x1[n];
				}
		produitMatriciel3x1(matResult3x1,rotS,coordonneesPoint);
		for(n=0;n<3;n++){
					coordonneesPoint[n]=matResult3x1[n];
				}
		produitMatriciel3x1(matResult3x1,rotG,coordonneesPoint);
		additionMatriciel(matResult3x1,coordonneesOrigine);
	}
	for(n=0;n<3;n++){
		p1->coordonnees[n]=matResult3x1[n]; // on attribue les nouvelles coordonnées au point
	}

	if(sens==1){
		p1->repereAssocie = r.nomRepere-1; // on attribue le nouveau repère associé
	}
	else if (sens == -1){
		p1->repereAssocie = r.nomRepere;
	}
}

/* fonction de rotation du repère passé en argument
 à utiliser lorsque les pupilles sont dans leur propre repère */
void rotation(s_ReperePtr r1, float G, float S, float R){
	if(fabsf(r1->G+G)<r1->deplacementMax[3] && fabsf(r1->S+S)<r1->deplacementMax[4] && fabsf(r1->R+R)<r1->deplacementMax[5]){ // on vérifie que la rotation ne dépasse pas les limites réelles possibles
		r1->G = r1->G + G;
		r1->S = r1->S + S;
		r1->R = r1->R + R;
	}
	else printf("Rotation impossible");
}

/* translate un repère des valeurs passées en argument
 à utiliser lorsque les pupilles sont dans leur propre repère*/
void translation(s_ReperePtr r1, float x, float y, float z,s_Repere posZero){
	s_Repere repereTest;
	repereTest.Origine.coordonnees[0]= r1->Origine.coordonnees[0]+x; // on place les coordonnées dans un repère de transition
	repereTest.Origine.coordonnees[1]= r1->Origine.coordonnees[1]+y;
	repereTest.Origine.coordonnees[2]= r1->Origine.coordonnees[2]+z;
	if (fabsf(repereTest.Origine.coordonnees[0]-posZero.Origine.coordonnees[0])<r1->deplacementMax[0] && fabsf(repereTest.Origine.coordonnees[1]-posZero.Origine.coordonnees[1])<r1->deplacementMax[1] && fabsf(repereTest.Origine.coordonnees[2]-posZero.Origine.coordonnees[2])<r1->deplacementMax[2]){
		// on test si les coordonnées du repère ne dépasse pas les limites réelles possibles
		r1->Origine.coordonnees[0]=repereTest.Origine.coordonnees[0];
		r1->Origine.coordonnees[1]=repereTest.Origine.coordonnees[1];
		r1->Origine.coordonnees[2]=repereTest.Origine.coordonnees[2];
	}
	else printf("Translation impossible");
}

/* fonction de rotation d'une pupille passée en argument (pchoisie)
 à utiliser lorsque les pupilles sont dans leur repère*/
void rotationPupille(s_ReperePtr recupConfigRep, s_PointPtr pchoisie, s_ReperePtr rchoisi, float G, float S, float R,s_Repere posZero,int nbReperes){
	int i;
	s_Point enregCoord;

	for(i=0;i<nbReperes;i++){ // on place la pupille dans le repère global
		changementRepere(recupConfigRep[i],pchoisie);
	}
	for(i=0;i<3;i++){ // on enregistre ces coordonnées globale
		enregCoord.coordonnees[i] = pchoisie->coordonnees[i];
	}
	for(i=nbReperes-1;i>=0;i--){ // on repasse la pupille dans son repère associé
		changementRepere(recupConfigRep[i],pchoisie);
	}

	rotation(rchoisi,G,S,R); // on fait notre rotation de valeurs demandées

	/*for(i=0;i<nbReperes;i++){ // on replace la pupille dans le repère globale
		changementRepere(recupConfigRep[i],pchoisie);
	}
	for(i=nbReperes-1;i>=0;i--){ // on replace la pupille dans son repère associé
		changementRepere(recupConfigRep[i],pchoisie);
	}*/

	for(i=0;i<nbReperes;i++){ // on replace la pupille dans le repère globale
		changementRepere(recupConfigRep[i],pchoisie);
	}
	for(i=0;i<nbReperes;i++){ // on mesure l'ecart entre la position enregistrée et la position apres rotation
		enregCoord.coordonnees[i] = enregCoord.coordonnees[i] - pchoisie->coordonnees[i];
	}
	for(i=nbReperes-1;i>=0;i--){ // on replace la pupille dans son repère associé
		changementRepere(recupConfigRep[i],pchoisie);
	}

	// on translate de l'ecart calculé entre l'ancienne et la nouvelle position de la pupille
	translation(rchoisi,enregCoord.coordonnees[0],enregCoord.coordonnees[1],enregCoord.coordonnees[2],posZero);

}

void alignementHexapode(s_PointPtr pchoisie,s_ReperePtr rchoisi, s_ReperePtr recupConfigRep,int nbReperes){
// met en paralèlle l'axe de vu de la pupille passée en argument et l'axe de la source collimatée
	int i,j;
	float coordCalcul[3],longueur1,longueur2,longueur3,angleR,angleS;
	s_PointPtr pCalcul;
	pCalcul = (s_PointPtr)malloc(sizeof(s_Point));

	for(i=0;i<3;i++){ // on place le vecteur direction dans un point pour le calcul
		pCalcul->coordonnees[i]=pchoisie->coordDirection[i]-pchoisie->coordonnees[i];
	}
	pCalcul->repereAssocie=pchoisie->repereAssocie; // on lui attribu le meme repère associé que la pupille

	for(i=0;i<nbReperes-1;i++){ // on change de repère en placant le vecteur direction a l'origine du repère dans lequel il vient d'être placé
		changementRepere(recupConfigRep[i],pCalcul);
		for(j=0;j<3;j++){
			pCalcul->coordonnees[j]=pCalcul->coordonnees[j]-recupConfigRep[i].Origine.coordonnees[j];
		}
	}
	for(i=0;i<2;i++){ // on prépare les différentes valeurs utiles au calculs
		coordCalcul[i]=recupConfigRep[nbReperes-1].Origine.coordonnees[i]-pCalcul->coordonnees[i];
	}
	coordCalcul[2]=-pCalcul->coordonnees[2];
	// Et enfin on applique Alkashi pour retrouver les 2 angles dont nous avons besoin
	// Pour l'angle en Z: le R
	longueur1=sqrt(pow(recupConfigRep[nbReperes-1].Origine.coordonnees[0],2)+pow(recupConfigRep[nbReperes-1].Origine.coordonnees[1],2));
	longueur2=sqrt(pow(pCalcul->coordonnees[0],2)+pow(pCalcul->coordonnees[1],2));
	longueur3=sqrt(pow(coordCalcul[0],2)+pow(coordCalcul[1],2));
	angleR=acos((pow(longueur3,2)-pow(longueur2,2)-pow(longueur1,2))/(-2*longueur2*longueur1));
	// Pour l'angle en Y: le S
	longueur1=sqrt(pow(recupConfigRep[nbReperes-1].Origine.coordonnees[0],2));
	longueur2=sqrt(pow(pCalcul->coordonnees[0],2)+pow(pCalcul->coordonnees[2],2));
	longueur3=sqrt(pow(coordCalcul[0],2)+pow(coordCalcul[2],2));
	angleS=acos((pow(longueur3,2)-pow(longueur2,2)-pow(longueur1,2))/(-2*longueur2*longueur1));

	rotation(rchoisi,0,-angleS,-angleR); // enfin on met en rotation le repère
}

void calibrageHexapode(s_PointPtr p1, s_ReperePtr r1, s_ReperePtr recupConfigRep,int nbReperes, s_Repere posZero){
	// Cette fonction va placer une pupille sur l'axe de la source collimatée
	int n;
	float coordCalc[3];
	for(n=0;n<nbReperes;n++){ // on se place dans le repère qui se déplace pour trouver l'offset en y et en z
	changementRepere(recupConfigRep[n],p1);
	}
	for(n=0;n<3;n++){ // on enregistre ces offset
		coordCalc[n]=p1->coordonnees[n];
	}
	for(n=nbReperes-1;n>=0;n--){ // on se replace dans le repère de la pupille (spécification de la fonction translation)
	changementRepere(recupConfigRep[n],p1);
	}
	translation(r1,0,-coordCalc[1],-coordCalc[2],posZero); // puis on translate de l'opposé de l'offset enregistré en y et en z
}

int * recupNbPupRep(char config1[], char config2[]){
// fonction de récupération du nombres de repères et de pupilles dans le fichier de configurations

	char Node1[]={"configurations"};
	char FullPath[512]={0};
	char FullPath1[512]={0};
	char Xmlfilename[] = {"CONFIG\\"};
	const char nb1[]="nbPupilles";
	const char nb2[]="nbReperes";

	int len = GetCurrentDirectory(0, NULL);
	GetCurrentDirectory(len+1, FullPath);
	strcat(FullPath,"\\");
	strcat(FullPath, Xmlfilename);
	strcat(FullPath1, FullPath);
	strcat(FullPath, config1);
	strcat(FullPath1, config2);

	int *recupNb;
	recupNb = (int*)malloc(sizeof(int)*2); // on créé notre pointeur d'entier pour 2 entiers
		if (recupNb == NULL){
		fprintf(stderr,"out of memory\n");
		}
	memset(recupNb,0,sizeof(int)*2);

	TiXmlDocument xmlFile(FullPath);
	if(!xmlFile.LoadFile()){
		printf("error0");
		recupNb=NULL;
		return(recupNb);
	}
	TiXmlHandle hdl(&xmlFile);

	TiXmlElement *TiNode = hdl.FirstChildElement(Node1).FirstChildElement(nb1).Element();
	recupNb[0]=atoi((char*)TiNode->Attribute((char*)"value"));

	TiXmlDocument xmlFile1(FullPath1);
	if(!xmlFile1.LoadFile()){
		printf("error1");
		recupNb=NULL;
		return(recupNb);
	}
	TiXmlHandle hdl1(&xmlFile1);

	TiXmlElement *TiNode1 = hdl1.FirstChildElement(Node1).FirstChildElement(nb2).Element();
	recupNb[1]=atoi((char*)TiNode1->Attribute((char*)"value"));

	return(recupNb); // on retourne le pointeur d'entier
}

s_Point recupConfigPupilles (int recupNb, char config[]){
	// On récupère les configurations des "recupNb" pupilles du fichier de configurations

	char Node1[]={"configurations"}; // Configuration du chemin vers les fichiers xml
	char FullPath[512]={0};
	char Xmlfilename[] = {"CONFIG\\"};

	int len = GetCurrentDirectory(0, NULL);
	GetCurrentDirectory(len+1, FullPath);
	strcat(FullPath,"\\");
	strcat(FullPath, Xmlfilename);
	strcat(FullPath, config);

	char **tab,buffer0[11],buffer1[11],buffer2[11],buffer3[11],buffer4[11],buffer5[11],buffer6[11];
	const char **tab1;
	int n,i;
	s_Point pupille,ptErreur;
	ptErreur.repereAssocie=-1;

	TiXmlDocument xmlFile(FullPath);
	if(!xmlFile.LoadFile()){
		printf("error2");
		return(ptErreur);
	}
	TiXmlHandle hdl(&xmlFile);

	tab=(char**)malloc(sizeof(char*)*7); // on créé un tableau contenant les chaines de caractères a reconnaitre
	tab[0]=(char*)"repereAsc";
	tab[1]=(char*)"pupillesx";
	tab[2]=(char*)"pupillesy";
	tab[3]=(char*)"pupillesz";
	tab[4]=(char*)"direcpupx";
	tab[5]=(char*)"direcpupy";
	tab[6]=(char*)"direcpupz";

	tab1=(const char**)malloc(sizeof(const char*)*9); // un deuxième tableau contenant les numéros
	tab1[0]="_1";tab1[1]="_2";tab1[2]="_3";tab1[3]="_4";tab1[4]="_5";
	tab1[5]="_6";tab1[6]="_7";tab1[7]="_8";tab1[8]="_9";

	for(i=recupNb;i<recupNb+1;i++){
			strcpy(buffer0,"000000000"); // on initialise les buffer (tableau de char) qui vont enregistrer les chaines de caractère
			strcpy(buffer1,"000000000");
			strcpy(buffer2,"000000000");
			strcpy(buffer3,"000000000");
			strcpy(buffer4,"000000000");
			strcpy(buffer5,"000000000");
			strcpy(buffer6,"000000000");
			for(n=0;n<9;n++){
				buffer0[n]=tab[0][n]; // on attribu a chaque buffer une chaine de caractère
				buffer1[n]=tab[1][n];
				buffer2[n]=tab[2][n];
				buffer3[n]=tab[3][n];
				buffer4[n]=tab[4][n];
				buffer5[n]=tab[5][n];
				buffer6[n]=tab[6][n];
				}
			strncat(buffer0,tab1[i],2); // a laquel on ajoute le suffixe du tableau des numéros
			TiXmlElement *TiNode = hdl.FirstChildElement(Node1).FirstChildElement(buffer0).Element();
			pupille.repereAssocie=atoi((char*)TiNode->Attribute((char*)"value"));

			strncat(buffer1,tab1[i],2);
			TiXmlElement *TiNode1 = hdl.FirstChildElement(Node1).FirstChildElement(buffer1).Element();
			pupille.coordonnees[0]=atof((char*)TiNode1->Attribute((char*)"value"));

			strncat(buffer2,tab1[i],2);
			TiXmlElement *TiNode2 = hdl.FirstChildElement(Node1).FirstChildElement(buffer2).Element();
			pupille.coordonnees[1]=atof((char*)TiNode2->Attribute((char*)"value"));

			strncat(buffer3,tab1[i],2);
			TiXmlElement *TiNode3 = hdl.FirstChildElement(Node1).FirstChildElement(buffer3).Element();
			pupille.coordonnees[2]=atof((char*)TiNode3->Attribute((char*)"value"));

			strncat(buffer4,tab1[i],2);
			TiXmlElement *TiNode4 = hdl.FirstChildElement(Node1).FirstChildElement(buffer4).Element();
			pupille.coordDirection[0]=atof((char*)TiNode4->Attribute((char*)"value"));

			strncat(buffer5,tab1[i],2);
			TiXmlElement *TiNode5 = hdl.FirstChildElement(Node1).FirstChildElement(buffer5).Element();
			pupille.coordDirection[1]=atof((char*)TiNode5->Attribute((char*)"value"));

			strncat(buffer6,tab1[i],2);
			TiXmlElement *TiNode6 = hdl.FirstChildElement(Node1).FirstChildElement(buffer6).Element();
			pupille.coordDirection[2]=atof((char*)TiNode6->Attribute((char*)"value"));
	}
	return(pupille); // On retourne la pupille
}

s_Repere recupConfigReperes(int recupNb, char* config){
	// On récupère les configurations des "recupNb" repères du fichier de configurations

	char Node1[]={"configurations"}; // Configuration du chemin vers les fichiers xml
	char FullPath[512]={0};
	char Xmlfilename[] = {"CONFIG\\"};

	int len = GetCurrentDirectory(0, NULL);
	GetCurrentDirectory(len+1, FullPath);
	strcat(FullPath,"\\");
	strcat(FullPath, Xmlfilename);
	strcat(FullPath, config);

	char **tab,buffer0[11],buffer1[11],buffer2[11],buffer3[11], buffer4[11],buffer5[11],buffer6[11],buffer7[11],buffer8[11],buffer9[11],buffer10[11],buffer11[11],buffer12[11],buffer13[11];
	const char **tab1;
	int n,i;
	s_Repere repere,repErreur;
	repErreur.nomRepere=-1;

	TiXmlDocument xmlFile(FullPath);
	if(!xmlFile.LoadFile()){
		printf("error3");
		return(repErreur);
	}
	TiXmlHandle hdl(&xmlFile);

	tab=(char**)malloc(sizeof(char*)*14); // on créé un tableau contenant les chaines de caractères a reconnaitre
	tab[0]=(char*)"nomRepere";
	tab[1]=(char*)"repcoordx";
	tab[2]=(char*)"repcoordy";
	tab[3]=(char*)"repcoordz";
	tab[4]=(char*)"reprotatx";
	tab[5]=(char*)"reprotaty";
	tab[6]=(char*)"reprotatz";
	tab[7]=(char*)"repdeplac";
	tab[8]=(char*)"depmaxtrx";
	tab[9]=(char*)"depmaxtry";
	tab[10]=(char*)"depmaxtrz";
	tab[11]=(char*)"depmaxrtx";
	tab[12]=(char*)"depmaxrty";
	tab[13]=(char*)"depmaxrtz";

	tab1=(const char**)malloc(sizeof(const char*)*9); // un deuxième tableau contenant les numéros
	tab1[0]="_1";tab1[1]="_2";tab1[2]="_3";tab1[3]="_4";tab1[4]="_5";
	tab1[5]="_6";tab1[6]="_7";tab1[7]="_8";tab1[8]="_9";

	for(i=recupNb;i<recupNb+1;i++){ // on initialise les buffer (tableau de char) qui vont enregistrer les chaines de caractère
		strcpy(buffer0,"000000000");
		strcpy(buffer1,"000000000");
		strcpy(buffer2,"000000000");
		strcpy(buffer3,"000000000");
		strcpy(buffer4,"000000000");
		strcpy(buffer5,"000000000");
		strcpy(buffer6,"000000000");
		strcpy(buffer7,"000000000");
		strcpy(buffer8,"000000000");
		strcpy(buffer9,"000000000");
		strcpy(buffer10,"000000000");
		strcpy(buffer11,"000000000");
		strcpy(buffer12,"000000000");
		strcpy(buffer13,"000000000");
		for(n=0;n<9;n++){
			buffer0[n]=tab[0][n]; // on attribu a chaque buffer une chaine de caractère
			buffer1[n]=tab[1][n];
			buffer2[n]=tab[2][n];
			buffer3[n]=tab[3][n];
			buffer4[n]=tab[4][n];
			buffer5[n]=tab[5][n];
			buffer6[n]=tab[6][n];
			buffer7[n]=tab[7][n];
			buffer8[n]=tab[8][n];
			buffer9[n]=tab[9][n];
			buffer10[n]=tab[10][n];
			buffer11[n]=tab[11][n];
			buffer12[n]=tab[12][n];
			buffer13[n]=tab[13][n];
			}
		strncat(buffer0,tab1[i],2); // a laquel on ajoute le suffixe du tableau des numéros
		TiXmlElement *TiNode = hdl.FirstChildElement(Node1).FirstChildElement(buffer0).Element();
		repere.nomRepere=atoi((char*)TiNode->Attribute((char*)"value"));

		strncat(buffer1,tab1[i],2);
		TiXmlElement *TiNode1 = hdl.FirstChildElement(Node1).FirstChildElement(buffer1).Element();
		repere.Origine.coordonnees[0]=atof((char*)TiNode1->Attribute((char*)"value"));

		strncat(buffer2,tab1[i],2);
		TiXmlElement *TiNode2 = hdl.FirstChildElement(Node1).FirstChildElement(buffer2).Element();
		repere.Origine.coordonnees[1]=atof((char*)TiNode2->Attribute((char*)"value"));

		strncat(buffer3,tab1[i],2);
		TiXmlElement *TiNode3 = hdl.FirstChildElement(Node1).FirstChildElement(buffer3).Element();
		repere.Origine.coordonnees[2]=atof((char*)TiNode3->Attribute((char*)"value"));

		strncat(buffer4,tab1[i],2);
		TiXmlElement *TiNode4 = hdl.FirstChildElement(Node1).FirstChildElement(buffer4).Element();
		repere.G=atof((char*)TiNode4->Attribute((char*)"value"));

		strncat(buffer5,tab1[i],2);
		TiXmlElement *TiNode5 = hdl.FirstChildElement(Node1).FirstChildElement(buffer5).Element();
		repere.S=atof((char*)TiNode5->Attribute((char*)"value"));

		strncat(buffer6,tab1[i],2);
		TiXmlElement *TiNode6 = hdl.FirstChildElement(Node1).FirstChildElement(buffer6).Element();
		repere.R=atof((char*)TiNode6->Attribute((char*)"value"));

		strncat(buffer7,tab1[i],2);
		TiXmlElement *TiNode7 = hdl.FirstChildElement(Node1).FirstChildElement(buffer7).Element();
		repere.ddl=atoi((char*)TiNode7->Attribute((char*)"value"));

		strncat(buffer8,tab1[i],2);
		TiXmlElement *TiNode8 = hdl.FirstChildElement(Node1).FirstChildElement(buffer8).Element();
		repere.deplacementMax[0]=atof((char*)TiNode8->Attribute((char*)"value"));

		strncat(buffer9,tab1[i],2);
		TiXmlElement *TiNode9 = hdl.FirstChildElement(Node1).FirstChildElement(buffer9).Element();
		repere.deplacementMax[1]=atof((char*)TiNode9->Attribute((char*)"value"));

		strncat(buffer10,tab1[i],2);
		TiXmlElement *TiNode10 = hdl.FirstChildElement(Node1).FirstChildElement(buffer10).Element();
		repere.deplacementMax[2]=atof((char*)TiNode10->Attribute((char*)"value"));

		strncat(buffer11,tab1[i],2);
		TiXmlElement *TiNode11 = hdl.FirstChildElement(Node1).FirstChildElement(buffer11).Element();
		repere.deplacementMax[3]=atof((char*)TiNode11->Attribute((char*)"value"));

		strncat(buffer12,tab1[i],2);
		TiXmlElement *TiNode12 = hdl.FirstChildElement(Node1).FirstChildElement(buffer12).Element();
		repere.deplacementMax[4]=atof((char*)TiNode12->Attribute((char*)"value"));

		strncat(buffer13,tab1[i],2);
		TiXmlElement *TiNode13 = hdl.FirstChildElement(Node1).FirstChildElement(buffer13).Element();
		repere.deplacementMax[5]=atof((char*)TiNode13->Attribute((char*)"value"));

	}
	return(repere); // On retourne le repère
}

int choixPupille (int nbPupilles){
	// fonction de choix d'une pupille parmis les "nbPupilles" disponibles
	int choix,valide=0,i;
	FILE *cra;
	while(valide==0){
		printf("Choisissez la pupille à étudier dans les %d pupilles disponibles: \n",nbPupilles); fflush(stdout);
		for(i=0;i<nbPupilles;i++){
			printf("- %d \n",i+1); fflush(stdout);
		}
		scanf("%d",&choix);
		for(i=0;i<nbPupilles;i++){
			if(i+1 == choix){
				valide=1;
			}
		}
	}
	cra = fopen("CRA.txt","a");
	fprintf(cra,"Choix de la pupille n°%d.\n\n",choix);
	fclose(cra);
	return(choix-1);
}

s_Repere enregPos(s_ReperePtr rchoisi){
	// enregistre la position zéro du repère passé en argument
	s_Repere posZero;
	int i;
	for(i=0;i<3;i++){
		posZero.Origine.coordonnees[i]=rchoisi->Origine.coordonnees[i];
	}
	posZero.G=rchoisi->G;
	posZero.S=rchoisi->S;
	posZero.R=rchoisi->R;
	return(posZero);
}

s_ReperePtr choixRepere(s_ReperePtr recupConfigRep, int nbReperes){
	s_ReperePtr choix;
	choix = (s_ReperePtr)malloc(sizeof(s_Repere));
	choix = &recupConfigRep[nbReperes-1];
	int i;
	for(i=0;i<nbReperes;i++){
		if(recupConfigRep[i].ddl==63){
			choix=&recupConfigRep[i];
		}
	}
	return(choix);
}

void initHexapode(float *tabEnvoi){
	int i=0;
	for(i=0;i<12;i++){
		tabEnvoi[i]=-1;
	}
	FILE *cra;
	cra = fopen("CRA.txt","w");
	fprintf(cra,"Initialisation.\n\n");
	fclose(cra);
}

void moveHexapode(s_ReperePtr r, s_Repere posZero, s_Repere posAvtMvt, float *tabEnvoi){
	int i=0;
	for(i=0;i<3;i++){
		tabEnvoi[i]=r->Origine.coordonnees[i]-posZero.Origine.coordonnees[i];
	}
	tabEnvoi[3]=r->G-posZero.G;
	tabEnvoi[4]=r->S-posZero.S;
	tabEnvoi[5]=r->R-posZero.R;
	for(i=0;i<3;i++){
		tabEnvoi[i+6]=posAvtMvt.Origine.coordonnees[i]-posZero.Origine.coordonnees[i];
	}
	tabEnvoi[9]=posAvtMvt.G-posZero.G;
	tabEnvoi[10]=posAvtMvt.S-posZero.S;
	tabEnvoi[11]=posAvtMvt.R-posZero.R;
	FILE *cra;
	cra = fopen("CRA.txt","a");
	fprintf(cra,"Mouvements vers:x=%f, y=%f, z=%f, rx=%f, ry=%f, rz=%f\n\n",(r->Origine.coordonnees[0]-posZero.Origine.coordonnees[0])/1000,(r->Origine.coordonnees[1]-posZero.Origine.coordonnees[1])/1000,(r->Origine.coordonnees[2]-posZero.Origine.coordonnees[2])/1000,r->G,r->S,r->R);
	fclose(cra);
}

void finHexapode(float *tabEnvoi){
	int i=0;
	for(i=0;i<12;i++){
		tabEnvoi[i]=1;
	}
	FILE *cra;
	cra = fopen("CRA.txt","a");
	fprintf(cra,"Arrêt du mouvement.\n\n");
	fclose(cra);
}

void majCoordRep(s_ReperePtr r, s_Repere posZero, float *tabRetour){
	int i;
	FILE *cra;
	for(i=0;i<3;i++){
	r->Origine.coordonnees[i]=tabRetour[i]+posZero.Origine.coordonnees[i];
	}
	r->G=tabRetour[3]+posZero.G;
	r->S=tabRetour[4]+posZero.S;
	r->R=tabRetour[5]+posZero.R;

	cra = fopen("CRA.txt","a");
	fprintf(cra,"Repositionnement en: x=%f, y=%f, z=%f, rx=%f, ry=%f, rz=%f\n\n",(r->Origine.coordonnees[0]-posZero.Origine.coordonnees[0])/1000,(r->Origine.coordonnees[1]-posZero.Origine.coordonnees[1])/1000,(r->Origine.coordonnees[2]-posZero.Origine.coordonnees[2])/1000,r->G,r->S,r->R);
	fclose(cra);
}
int main() {
	int i, fin=0, *recupNb; // fin: utilisé pour le menu, recupNb: utilisé pour récup le nombre de pupilles et de repères
	float *tabEnvoi, *tabRetour; // Envoi des ressources vers le driver
	tabEnvoi = (float*)malloc(sizeof(float)*12);
	tabRetour = (float*)malloc(sizeof(float)*6);
	recupNb = (int*)malloc(sizeof(int)*2);
	s_PointPtr  pchoisie; // pupille choisie pour les deplacements
	pchoisie = (s_PointPtr)malloc(sizeof(s_Point));
	s_ReperePtr rchoisi; // repere choisi pour les deplacements
	rchoisi = (s_ReperePtr)malloc(sizeof(s_Repere));
	recupNb = recupNbPupRep((char*)"ConfigPup.xml",(char*)"ConfigRep.xml"); // On récupère le nb de pupilles et de repères
	if(recupNb == NULL){
		return 0;
	}
	int nbPupilles=recupNb[0],nbReperes=recupNb[1]; // on initialise le nombre de pupilles et de repères grace a recupNb

	s_PointPtr recupConfigPup;
	recupConfigPup = (s_PointPtr)malloc(sizeof(s_Point)*nbPupilles);
		if (recupConfigPup == NULL){ // on alloue dynamiquement la mémoire de notre tableau de pupilles
			fprintf(stderr,"out of memory\n");
		}
	memset(recupConfigPup,0,sizeof(s_Point)*nbPupilles);

	s_ReperePtr recupConfigRep;
	recupConfigRep = (s_ReperePtr)malloc(sizeof(s_Repere)*nbReperes);
		if (recupConfigRep == NULL){ // on alloue dynamiquement la mémoire de notre tableau de repères
			fprintf(stderr,"out of memory\n");
		}
	memset(recupConfigRep,0,sizeof(s_Repere)*nbReperes);

	for(i=0;i<nbPupilles;i++){ // on initialise notre tableau de pupilles avec les configurations du fichier XML
			recupConfigPup[i]=recupConfigPupilles(i,(char*)"ConfigPup.xml");
			if(recupConfigPup[i].repereAssocie == -1){
				return 0;
			}
		}
	for(i=0;i<nbReperes;i++){ // on initialise notre tableau de repères avec les configurations du fichier XML
		if(i==0){
			recupConfigRep[i]=recupConfigReperes(i,(char*)"ConfigPup.xml");
		}
		else recupConfigRep[i]=recupConfigReperes(i,(char*)"ConfigRep.xml");
		//printf("%d;%f;%f;%f;%f;%f;%f;%d;%f;%f;%f;%f;%f;%f\n\n",recupConfigRep[i].nomRepere,recupConfigRep[i].Origine.coordonnees[0],recupConfigRep[i].Origine.coordonnees[1],recupConfigRep[i].Origine.coordonnees[2],recupConfigRep[i].G,recupConfigRep[i].R,recupConfigRep[i].S,recupConfigRep[i].ddl,recupConfigRep[i].deplacementMax[0],recupConfigRep[i].deplacementMax[1],recupConfigRep[i].deplacementMax[2],recupConfigRep[i].deplacementMax[3],recupConfigRep[i].deplacementMax[4],recupConfigRep[i].deplacementMax[5]);
		if(recupConfigRep[i].nomRepere == -1){
			return 0;
		}
	}

	initHexapode(tabEnvoi); // On envois les commandes d'initialisation a l'API
	pchoisie=&recupConfigPup[choixPupille(nbPupilles)]; // faire une selection dans les nbPupilles au choix
	rchoisi=choixRepere(recupConfigRep,nbReperes); // choisi le repère de l'hexapod
	s_Repere posZero = enregPos(rchoisi); // on enregistre la position zéro de notre hexapod
	s_Repere posAvtMvt;

	/*
	 * Enfin on créé un menu nous permettant d'utiliser toutes nos fonctions de déplacements.
	 */
	while(fin==0){
		int menu=0,menu1=0;
		float x=0,y=0,z=0;
		printf("\n\nChoisissez une action:\n-1:Translation\n-2:Rotation\n-3:Rotation de la pupille\n-4:Alignement de la pupille\n-5:Choix de la pupille\n-6:Quitter\n-Autre: Afficher les positions"); fflush(stdout);
		scanf("%d",&menu);
			switch(menu){
			case 1:
				printf("Deplacement:\n-1:Relatif\n-2:Absolu"); fflush(stdout);
				scanf("%d",&menu1);
				switch (menu1){
				case 1:
					printf("Entrer le vecteur translation:\n- x:"); fflush(stdout);
					scanf("%f",&x);
					printf("- y:"); fflush(stdout);
					scanf("%f",&y);
					printf("- z:"); fflush(stdout);
					scanf("%f",&z);
					posAvtMvt=enregPos(rchoisi);
					translation(rchoisi,x,y,z,posZero);
					moveHexapode(rchoisi,posZero,posAvtMvt,tabEnvoi);
					break;
				case 2:
					posAvtMvt=enregPos(rchoisi);
					translation(rchoisi,-(rchoisi->Origine.coordonnees[0]-posZero.Origine.coordonnees[0]),-(rchoisi->Origine.coordonnees[1]-posZero.Origine.coordonnees[1]),-(rchoisi->Origine.coordonnees[2]-posZero.Origine.coordonnees[2]),posZero);
					printf("Entrer les coordonnées:\n- x:"); fflush(stdout);
					scanf("%f",&x);
					printf("- y:"); fflush(stdout);
					scanf("%f",&y);
					printf("- z:"); fflush(stdout);
					scanf("%f",&z);
					translation(rchoisi,x,y,z,posZero);
					moveHexapode(rchoisi,posZero,posAvtMvt,tabEnvoi);
					break;
				default:
					break;
				}
				break;
			case 2:
				printf("Rotation:\n-1:Relatif\n-2:Absolu"); fflush(stdout);
				scanf("%d",&menu1);
				switch (menu1){
				case 1:
					printf("Entrer les valeurs de rotation:\n- G:"); fflush(stdout);
					scanf("%f",&x);
					printf("- S:"); fflush(stdout);
					scanf("%f",&y);
					printf("- R:"); fflush(stdout);
					scanf("%f",&z);
					posAvtMvt=enregPos(rchoisi);
					rotation(rchoisi,x,y,z);
					moveHexapode(rchoisi,posZero,posAvtMvt,tabEnvoi);
					break;
				case 2:
					posAvtMvt=enregPos(rchoisi);
					rotation(rchoisi,-rchoisi->G,-rchoisi->S,-rchoisi->R);
					printf("Entrer les valeurs de rotations:\n- G:"); fflush(stdout);
					scanf("%f",&x);
					printf("- S:"); fflush(stdout);
					scanf("%f",&y);
					printf("- R:"); fflush(stdout);
					scanf("%f",&z);
					rotation(rchoisi,x,y,z);
					moveHexapode(rchoisi,posZero,posAvtMvt,tabEnvoi);
					break;
				default: break;
				}
				break;
			case 3:
				printf("Entrer les valeurs de rotation:\n- G:"); fflush(stdout);
				scanf("%f",&x);
				printf("- S:"); fflush(stdout);
				scanf("%f",&y);
				printf("- R:"); fflush(stdout);
				scanf("%f",&z);
				posAvtMvt=enregPos(rchoisi);
				rotationPupille(recupConfigRep,pchoisie,rchoisi,x,y,z,posZero,nbReperes);
				moveHexapode(rchoisi,posZero,posAvtMvt,tabEnvoi);
				break;
			case 4:
				posAvtMvt=enregPos(rchoisi);
				rotation(rchoisi,-rchoisi->G,-rchoisi->S,-rchoisi->R);
				for(i=0;i<nbReperes;i++){
						changementRepere(recupConfigRep[i],pchoisie);
					}
				for(i=nbReperes-1;i>=0;i--){
						changementRepere(recupConfigRep[i],pchoisie);
					}
				alignementHexapode(pchoisie,rchoisi,recupConfigRep,nbReperes);
				calibrageHexapode(pchoisie,rchoisi,recupConfigRep,nbReperes,posZero);
				moveHexapode(rchoisi,posZero,posAvtMvt,tabEnvoi);
				break;
			case 5:
				pchoisie=&recupConfigPup[choixPupille(nbPupilles)];
				break;
			case 6:
				printf("Quitter ?\n-1:Oui\n-2:Non"); fflush(stdout);
				scanf("%d",&menu1);
				if (menu1==1){
					finHexapode(tabEnvoi);
					fin++;
					break;
				}
				else break;
			case 7:
				printf("\nTest: MAJ coordonnées Hexapode\n");
				tabRetour[0]=0.12345;
				tabRetour[1]=1.23456;
				tabRetour[2]=2.34567;
				tabRetour[3]=3.45678;
				tabRetour[4]=4.56789;
				tabRetour[5]=5.67890;
				majCoordRep(rchoisi,posZero,tabRetour);
			default: break;
			}
		printf("\nHexapod: %f ; %f ; %f\n      -> %f ; %f ; %f\n\nPupille:",rchoisi->Origine.coordonnees[0]-posZero.Origine.coordonnees[0],rchoisi->Origine.coordonnees[1]-posZero.Origine.coordonnees[1],rchoisi->Origine.coordonnees[2]-posZero.Origine.coordonnees[2],rchoisi->G,rchoisi->S,rchoisi->R);

		for(i=0;i<3;i++){
				printf("%f ; ",pchoisie->coordonnees[i]);
			}
		printf("Repère n°: %d\n      -> ",pchoisie->repereAssocie);
		for(i=0;i<nbReperes;i++){
			changementRepere(recupConfigRep[i],pchoisie);
		}
		for(i=0;i<3;i++){
				printf("%f ; ",pchoisie->coordonnees[i]);
			}
		printf("Repère n°: %d\n\n",pchoisie->repereAssocie);
		for(i=nbReperes-1;i>=0;i--){
				changementRepere(recupConfigRep[i],pchoisie);
			}
	}

	return EXIT_SUCCESS;
}
