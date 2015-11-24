	//Jeu de dames version graphique, p 78, 79 du fascicule 
#include <stdio.h>
#include <stdlib.h>
#include <graphsimple.h>
#define TAILLECOTEDAMIER 10
#define UNITEPIXEL 40 //Dans le fascicule, ils demandaient de mettre 50 pixels, mais mon écran est trop petit pour que le damier rentre. 
#define NBRCASES 10 //Un damier de Dames fait 10 * 10. Il existe des varianets. //Dans la fonction « affiche_damier_graphique », vu qu'on a définit cara[3], on ne peut pas dépasser 99, car il ne faut pas que sprintf mette de valeur dans cara[2] sinon ça bug avec la fonction Ecrire, il faut que « cara[2]=='\0' » 

/*
pion des Bleus sont notés 1
pion des Rouges sont notés 2
dame des bleus sont notés 3
dame des rouges sont notés 4 
*/


/*
 *## Overview
 *
 *It's a small Draughts video game (call « jeu de Dames » in French), implemented for the class of [« Méthodes informatiques et techniques de programmation » (Université Grenoble 1)](https://dlst.ujf-grenoble.fr/?module=ue&idue=123).
 *
 ****This video game it's not totally finished.***
 *
 *
 *## Features
 *
 *This game wants respect [WCDF Rules of Draughts](http://www.usacheckers.com/downloads/WCDF_Revised_Rules.doc ). 
 *
 *This program valid every move of man according to the rules :
 ** « 1.13 The first move in each game is made by the player with the Whight men (…) »
 ** « 1.15 An ordinary move of a man is its transfer diagonally forward left or right from one square to an immediately neighbouring vacant square.  »
 ** « 1.16 When a man reaches the farthest row forward (known as the “king-row” or “crown-head”) it becomes a king, »
 ** « 1.18 A capturing move of a man is its transfer from one square over a diagonally adjacent and forward square occupied by an opponent\`s piece (man or king) and on to a vacant square immediately beyond it. (A capturing move is called a "jump"). On completion of the jump the captured piece is removed from the board. »
 ** « 1.19 If a jump creates an immediate further capturing opportunity, then the capturing move of the piece (man or king) is continued until all the jumps are completed. (…) At the end of the capturing sequence, all captured pieces are removed from the board. »
 ** « 1.20 All capturing moves are compulsory, whether offered actively or passively. If there are two or more ways to jump, a player may select any one that they wish, not necessarily that which gains the most pieces. Once started, a multiple jump must be carried through to completion. A man can only be jumped once during a multiple jumping sequence. »
 ** « 1.23 If a player on their turn to move touches a piece they must play that piece (…) »
 *
 *
 *## @TODO
 *
 ** Modular programming. 
 ** Import a clock. 
 ** Change libraries, use the cross-platform development library [SDL](https://www.libsdl.org/).
 ** King is not totally finished. He can't move and capture an opponent's piece. 
 ** Display result of the game.
 ** Make a mode single player (play against the computer).
 *
 *
 *## Installation
 *
 *1. Compile graphsimple.c: `gcc -c graphsimple.c`
 *2. Compile graphlib_w2.c: `gcc -c graphlib_w2.c`
 *3. Links: `ar -r libgraph.a raphsimple.o graphlib_w2.o`
 *4. Make libary: `ranlib libgraph.a`
 *5. Step 5
 *
 ** For root user :
 *        1. Put include files `*.h` in `/usr/include`, and put the libary in `/usr/lib`.
 *        2. Compile: `gcc Demineur.c -lgraphe -lX11`
 *
 ** For not root user :
 *        1. Put include files `*.h` in custom direcotry, for example `/home/toto/include`. Put libary in a custom directory lib, for example `/home/toto/lib`.
 *        2. Compile: `gcc monprog.c -I/home/toto/include -L/home/toto/lib -lgraphe -lX11`
 *
 *## Licenses
 *
 ** The file Jeu_de_Dames.c, by juanes10 in under MIT license.  Libaries Graphsimple and Graphlib_w2 are under copyright, see this files for credits.  ## Version 
 *
 *Development started in december 2014. Last modified : january 2015. First realase : november 2015.
 */

void initialiserdamier (int damier[][TAILLECOTEDAMIER + 3]);
int lignePaireOuImpaire (int variable);
void afficher_damier_graphique(int damier[][TAILLECOTEDAMIER + 3]);
void afficher_pions_graphique(int damier[][TAILLECOTEDAMIER + 3],int i, int j, char prise);
void effacer_pions_graphique( int i, int j );
void afficher_dames_graphique (int damier[][TAILLECOTEDAMIER + 3], int i, int j);
// Met dans la case damier[0][NBRCASES+1] le numéro de la personne qui doit jouer. Ensuite, affiche la couleur de la personne à jouer dans la case du graphique à jouer. 
void aquidejouer (int damier[TAILLECOTEDAMIER + 3][TAILLECOTEDAMIER + 3], int progressiondelapartie, int *numerojoueur, int *numeroAdversaire );//Quand on clique dans le graphique, cette fonction récupère le numéro i, j de la case cliquée.
void recuperernumerocasecilquee (int *i, int *j);
void DeplacerPionSimple (int damier[][TAILLECOTEDAMIER + 3], int numerojoueur);
int ScanToutDammierSiJoueurDoitPrendre (int damier[][TAILLECOTEDAMIER + 3], int numerojoueur, int numeroAdversaire);
int caseintermediaire (int min, int max);
int ScanCasesContigueSiPionAdverseAPrendre ( int damier[][TAILLECOTEDAMIER + 3],int i, int j, int numerojoueur , int numeroAdversaire );
void DeplacerPionMangerBouclePrincipale (int damier[][TAILLECOTEDAMIER + 3], int numerojoueur, int numeroAdversaire);
void DeplacerPionMangerSelectionCaseDEPART (int damier[][TAILLECOTEDAMIER + 3], int *i, int *j,int numeroAdversaire );
void DeplacerPionMangerSelectionCaseARRIVEE (int damier[][TAILLECOTEDAMIER + 3], int i, int j, int *iplus2, int *jplus2, int numeroAdversaire);
void DeplacementPionMangeurAffichage (int damier [][TAILLECOTEDAMIER + 3], int piecesAdversesPrises[][TAILLECOTEDAMIER + 3], int i, int j, int iplus2, int jplus2, int numerojoueur, int numeroAdversaire );

int main (void){

	//DECLARATION DES VARIABLES 
	int i, j;
	int damier[TAILLECOTEDAMIER +3][TAILLECOTEDAMIER+3]; //L'état actuel du damier du jeu. 
	int progressiondelapartie;
	int numerojoueur, numeroAdversaire;
	int obligationPrendrePion;

	printf ("\n\n***************\033[4mJeu de dames version graphique\033[0m***************\n\n");


	// INITIALISATION ET AFFICHAGE.  
	//DéfinCaseDEPART initiale des pions.
	initialiserdamier (damier);
	afficher_damier_graphique(damier);
	//Initialise l'affichage de tous les pions.
	for (j=1;j<=TAILLECOTEDAMIER;j++)
		for (i=1;i<=TAILLECOTEDAMIER;i++)
			if (damier[j][i]==1 || damier[j][i]==2 )
				afficher_pions_graphique(damier, i, j, 'n');


	//LA BOUCLE PRINCIPALE. 
	for (progressiondelapartie=1;progressiondelapartie<=5000;progressiondelapartie++){
		// Met dans la case damier[0][NBRCASES+1] le numéro de la de la personne qui doit jouer. Ensuite, affiche la couleur de la personne à jouer dans la case du graphique à jouer.
		aquidejouer (damier, progressiondelapartie, &numerojoueur, &numeroAdversaire);
		obligationPrendrePion = ScanToutDammierSiJoueurDoitPrendre (damier, numerojoueur, numeroAdversaire);
		printf (" obligationPrendrePion == %d ", obligationPrendrePion);
		if (obligationPrendrePion == 0)
			DeplacerPionSimple (damier, numerojoueur );

			//Lorsqu’au cours d’une prise par un pion, celui-ci se trouve à nouveau en présence, diagonalement, d’une pièce adverse derrière laquelle se trouve une case libre, il doit obligatoirement sauter par-dessus cette seconde pièce, voire d’une troisième et ainsi de suite, et occuper la case libre se trouvant derrière la dernière pièce capturée. Les pièces adverses ainsi capturées sont ensuite enlevées du damier dans l’ordre de la prise. Cette opération complète est une rafle par un pion (source ffjd)
		else if (obligationPrendrePion == 1)
			DeplacerPionMangerBouclePrincipale (damier, numerojoueur,  numeroAdversaire);
	}

	AttendreClic();
	Clore();

printf ("\n\n************ Fin du jeu ************ \n\n");
return 0;
}


//Définit la position initiale des pions.
void initialiserdamier (int damier[][TAILLECOTEDAMIER + 3]){
	
	int i, j, k;
	
	//initialiser le damier à - 1. ON NE PEUT PAS JOUER SUR LES CASES À -1. 
	for (j=1;j<TAILLECOTEDAMIER + 3;j++)
		for (i=1;i<TAILLECOTEDAMIER + 3;i++)
			damier [j][i]=-1;

 	// Pour les lignes impaires, le premier pion est placé en colonne 2, pour les lignes paires le premier pion est placé en colonne 1. 
	//Les cases qui ont la valeur 2 ont un pion rouge. Les pions qui ont la valeur 1 ont un pion rouge. Les cases avec une valeur 0 n'ont pas de pions. 

	// initialisation des cases noires à 0 (c'est QUE sur ELLES qu'on va jouer).
	for (j=1; j<=10; j++)
		k = lignePaireOuImpaire (j);
		for (i=k;i<=TAILLECOTEDAMIER;i = i + 2)
			damier[j][i]=0; // sans pions.

	for (j=1; j<=4;j++){
		k = lignePaireOuImpaire (j);
		for (i=k;i<=TAILLECOTEDAMIER;i = i+ 2)
			damier[j][i]=2; //Rouge
	}

	//Les cases qui ont la valeur 1 ont un pion bleu.
	for (j=7; j<=NBRCASES;j++){
		k = lignePaireOuImpaire (j);
		for (i=k;i<=TAILLECOTEDAMIER;i = i+ 2)
			damier[j][i]=1; // blanc
	}



}

void afficher_damier_graphique(int damier[][TAILLECOTEDAMIER + 3]){
	
	int i,l, k;
	char cara[3]="";
	
	Initialiser (UNITEPIXEL * (NBRCASES + 2), UNITEPIXEL * (NBRCASES + 2));
	// Affiche le damier
 	for (l = 1; l<=NBRCASES;l++){
 		k = lignePaireOuImpaire (l); 	// Pour les lignes impaires, le premier pion est placé en colonne 2, pour les lignes paires le premier pion est placé en colonne 1. 
		for (i=UNITEPIXEL * k; i<=UNITEPIXEL * NBRCASES; i = i + (2 * UNITEPIXEL)){
			RectanglePlein (i, UNITEPIXEL * l, i + UNITEPIXEL, (UNITEPIXEL * l) + UNITEPIXEL);
		}
	}
	//Affiche les lettres en tête de colonne. 
	for (i=1; i <= NBRCASES; i++){
		sprintf (cara, "%d",i);
		Ecrire (i * UNITEPIXEL + UNITEPIXEL / 2, UNITEPIXEL / 2, cara);
	}
	//Affiche les numéros en tête de ligne
	for (i=1;i<=NBRCASES;i++){
		sprintf (cara,"%d",i); //On a donc cara [0]== %c et Cara [1] == %c ou '\0'.. 
		Ecrire (UNITEPIXEL / 2, i * UNITEPIXEL + UNITEPIXEL/2, cara);
	}
	//Titre 
	Ecrire ( NBRCASES*UNITEPIXEL /2 + 8,UNITEPIXEL*NBRCASES+(2*UNITEPIXEL) - 20,"Jeu de dame");
}

void afficher_pions_graphique(int damier[][TAILLECOTEDAMIER + 3], int i, int j, char prise ){
	
	int x, y, couleur;

	//Conversion des coordonnées des cases (i, j), en coordonnées pixel (x, y).
	x= i * UNITEPIXEL + (UNITEPIXEL /2);
	y= j * UNITEPIXEL + (UNITEPIXEL /2);

	// Affichage des pions 
	switch (damier[j][i]){
		case 1:
			couleur = 3;//Bleu
		break;
		case 2:
			couleur = 2;//Rouge
		break;
	}
	ChangerCouleur(couleur);//Change la couleur utilisee pour les traces avec la convention suivante : rouge   2   bleu  3. 
	switch (prise){
		case 'n' :
			CerclePlein(x, y , 15); // Affiche le pion du numerojouer dans sa case  d'arrivée. 
		break;
		case 'p' :
			CerclePlein(x + 3, y + 3 , 10); //Pour la fonction «  DeplacementPionMangeurAffichage ». Quand on passe sur une case, on affiche la pièce du joueur qui prend pendant un petit temps, en décalé. 
		break;
		case 'r' :
			CerclePlein(x + 1, y + 1 , 5);//  //Pour la fonction «  DeplacementPionMangeurAffichage ». Quand on passe sur une case, on affiche la pièce du joueur en tout petit à la fin. 
		break;
	}
}

void effacer_pions_graphique(int i, int j ){
	
	int x, y;
	
	//Conversion des coordonnées des cases (i, j), en coordonnées pixel (x, y).
	x= i * UNITEPIXEL + (UNITEPIXEL /2);
	y= j * UNITEPIXEL + (UNITEPIXEL /2);
	ChangerCouleur(0);//Change la couleur en noir
	CerclePlein(x,y,15);
}

void afficher_dames_graphique (int damier[][TAILLECOTEDAMIER + 3], int i, int j){

	int x, y;

	printf("\nmarqueur dbt fonction afficher_damier_console");
	//Conversion des coordonnées des cases (i, j), en coordonnées pixel (x, y).
	x= i * UNITEPIXEL + (UNITEPIXEL /2);
	y= j * UNITEPIXEL + (UNITEPIXEL /2);

	switch (damier[j][i]){
		case 3:
			ChangerCouleur(3); //bleu
			CerclePlein(x, y , 15);
			ChangerCouleur(1); //blanc
			CerclePlein(x + 1, y + 1, 10);
		break;
		case 4:
			ChangerCouleur(2);// rouge
			CerclePlein(x + 1, y +1 , 10);
			ChangerCouleur(1); //blanc
			CerclePlein(x+1, y+1, 10);
		break;
	}
}

int lignePaireOuImpaire (int variable){
	
	int k;
 	
 		if (variable%2!=0)
 			k=2;
 		if (variable%2==0)
 			k=1;
 	return k;
}

// Met dans la case damier[0][NBRCASES+1] le numéro de la personne qui doit jouer. Ensuite, affiche la couleur de la personne à jouer dans la case du graphique à jouer. 
void aquidejouer (int damier[TAILLECOTEDAMIER + 3][TAILLECOTEDAMIER + 3], int progressiondelapartie, int *numerojoueur, int *numeroAdversaire ){
	
	if (progressiondelapartie % 2 != 0){// Si c'est le coup numéro 1, 3, 5, etc, c'est au bleu de jouer
		damier[0][NBRCASES +1] =1; // On met un pion bleu dans la case du coin superieur droit. 
		*numerojoueur = 1;
		*numeroAdversaire = 2;
	}
	if (progressiondelapartie % 2 == 0){// Si c'est le coup numéro 2, 4, 6, etc, c'est au rouge de jouer
		damier[0][NBRCASES +1] = 2; // On met un pion rouge dans la case du coin superieur droit.
		*numerojoueur = 2;
		*numeroAdversaire = 1;
	}
	//Afficher le la couleur du joueur dans la case du coin supérieur droit. 
	Ecrire (UNITEPIXEL * (NBRCASES + 1 )+ (UNITEPIXEL / 2) - 15, UNITEPIXEL / 5, "Joueur");
	afficher_pions_graphique (damier, NBRCASES + 1, 0, 'n' );
}

int caseintermediaire (int min, int max){
	
	int tmp;
	
	if (min > max){
		tmp = min;
		min = max;
		tmp = min;
	}//Tri par sélection

	return min + 1;
}

//Quand on clique dans le graphique, cette fonction récupère le numéro i, j de la case cliquée.
void recuperernumerocasecilquee (int *i, int *j){
	
	int x,y;
	
	AttendreClicXY (&x,&y,NULL);
	*i = x / UNITEPIXEL;
	*j = y / UNITEPIXEL;
}

int ScanToutDammierSiJoueurDoitPrendre (int damier[][TAILLECOTEDAMIER + 3], int numerojoueur, int numeroAdversaire){

	int i, j, k;
	
	for (j=1; j<=NBRCASES ; j++)
		for (i= 1; i<=NBRCASES && k !=1 ;i++) 
			 k = ScanCasesContigueSiPionAdverseAPrendre (damier,i, j,numerojoueur,numeroAdversaire); // dès qu'on trouve que le joueur peut prendre un pion, on arrête la recherche avec k == 1. Si le joueur ne peut pas prendre aucun pion, la boucle se termine avec k == 0
	return k;
}

void DeplacerPionSimple ( int damier[][TAILLECOTEDAMIER + 3], int numerojoueur){

	int i, j, iplus1, jplus1, k;

	if (numerojoueur == 1)
		k = -1;
	if (numerojoueur == 2)
		k = 1; 

		//Il faut cliquer sur la bonne case où une pièce de sa couleur est présente. 
		do{
			recuperernumerocasecilquee ( &i,&j);
		}while (damier[j][i]!=numerojoueur);// Cette boucle permet de vérifier que la personne qui doit jouer clique bien sur son propre pion. 

		do {
			recuperernumerocasecilquee (&iplus1,&jplus1);
		}while ( (jplus1!=j+k || iplus1!=i-1) /*case contigüe nord (B) ou sud (R) ouest*/ && (jplus1!=j+k || iplus1!=i+1)/*case contigüe nord (B) ou sud (R) EST*/); //Cette boucle permet de s'assurer qu'on clique bien dans la bonne case CONTIGÜE de la DIAGONALE. Pour qu'on sorte de la boucle, il faut qu'une des deux propositions soient vraie. 
		printf("- %d = iplus1, %d = jplus1 - ",iplus1, jplus1);
		//Met un pion là où le joueur veut déplacer son pion
		printf("- %d = iplus1, %d = jplus1 - ",iplus1, jplus1);
		damier[jplus1][iplus1] = numerojoueur; 
		afficher_pions_graphique(damier,iplus1,jplus1, 'n');
		
		//Supprime le pion où il était avant de bouger. 
		damier[j][i]=0;
		effacer_pions_graphique (i, j);

	// FABRICATION DE DAMES 
 	// Une dame doit attendre que l’adversaire ait joué au moins une fois avant d’entrer en action. (source ffjd).
 	// En France, rois et reines ne sont pas couronnées à leur avènement. De la même manière, j'ai décrété qu'au jeu de dames, les dames étaient couronnées à leur « entrée en action », (clin d'œuil aux historiens).
	printf("- %d = iplus1, %d = jplus1 - ",iplus1, jplus1);
 	if (numerojoueur == 1 && jplus1 == 1 ){
 			printf("\nmarqueur 1");
			damier[jplus1][iplus1] = 3; // 3 c'est une dame bleu
			afficher_dames_graphique(damier,iplus1,jplus1);
			printf("- %d = iplus1, %d = jplus1 - ",iplus1, jplus1);
	}
 	else if ( numerojoueur == 2 && jplus1 == 10){
 			printf("\nmarqueur 2");
	 		damier[jplus1][iplus1] = 4; // 4 c'est une dame rouge. 
	 		afficher_dames_graphique(damier,iplus1,jplus1);
	 		printf("- %d = iplus1, %d = jplus1 - ",iplus1, jplus1);
	 }
}

void DeplacerPionMangerSelectionCaseDEPART (int damier[][TAILLECOTEDAMIER + 3], int *i, int *j,int numeroAdversaire ){

		do{
			recuperernumerocasecilquee ( &*i,&*j);
		}while (damier[*j][*i]!=damier[0][NBRCASES+1]/* 
		*/||  (/*
				*/   (damier[*j-1][*i-1] != numeroAdversaire || damier[*j-2][*i-2] !=0)/*
				*/&& (damier[*j-1][*i+1] != numeroAdversaire || damier[*j-2][*i+2] !=0) /*
				*/&& (damier[*j+1][*i-1] != numeroAdversaire || damier[*j+2][*i-2] !=0) /*
            	*/&& (damier[*j+1][*i+1] != numeroAdversaire || damier[*j+2][*i+2] !=0) /*
			*/)/*
		*/);/*Il faut cliquer sur la bonne case où une pièce de sa couleur est présente 
      ET que ce pion ait la possibilité de prendre un pion adverse. Cette boucle vérifie :
		1° / Que la personne qui doit jouer clique bien sur son propre pion. Grâce à la fonction «aquidejouer » dans « damier[0][NBRCASES+1] » y'a le numéro de la personne qui doit jouer.
		2° / ET que le pion sélectionné posséde bien un pion du joueur adverse dans une des QUATRE cases contigües AINSI QUE la case sur-contigüe correspondante est bien vide 
    */
}


void DeplacerPionMangerSelectionCaseARRIVEE (int damier[][TAILLECOTEDAMIER + 3], int i, int j, int *iplus2, int *jplus2, int numeroAdversaire){

		do {
			recuperernumerocasecilquee (&*iplus2,&*jplus2);
		}while ( /*
			*/(  damier[*jplus2][*iplus2]!=0 || damier[j-1][i-1]!= numeroAdversaire || (*jplus2 != j-2 || *iplus2 != i-2 ))/*
			*/&&(damier[*jplus2][*iplus2]!=0 || damier[j-1][i+1]!= numeroAdversaire|| (*jplus2 != j-2 || *iplus2 != i+2 ))/*
			*/&&(damier[*jplus2][*iplus2]!=0 || damier[j+1][i+1]!= numeroAdversaire|| (*jplus2 != j+2 || *iplus2 != i+2 ))/*
			*/&&(damier[*jplus2][*iplus2]!=0 || damier[j+1][i-1]!= numeroAdversaire|| (*jplus2 != j+2 || *iplus2 != i-2 ))/*
			*/); /*DANS LES QUATRES DIRECTIONS, cette boucle permet de s'assurer :
		1° / qu'on clique bien dans UNE CASE VIDE
		2° / ET que la case située entre la case de départ et la case d'arrivée du pion comporte bien un pion adverse
		3° / ET que l'on déplace bien le pion dans la case sur-contigüe. */
}

void DeplacementPionMangeurAffichage (int damier [][TAILLECOTEDAMIER + 3], int piecesAdversesPrises [][TAILLECOTEDAMIER + 3], int i, int j, int iplus2, int jplus2, int numerojoueur, int numeroAdversaire) { 

	int l, m;
 	//Une rafle doit être clairement indiquée, pièce par pièce, en posant la pièce preneuse sur la case de saut, le temps d’y passer, et en déposant cette pièce sur la case terminale.
	//Les pièces prises ne peuvent être enlevées du damier qu’à l’issue de l’exécution complète de la rafle. (source ffjd)


	//efface le pion de départ du joueur.
	damier[j][i]=0;
	effacer_pions_graphique (i,j);
	AttendreDelai (250);


	//ANIMATION. 
	//Affiche le pion sur la case à prendre
	
	l = caseintermediaire (i, iplus2);
	m = caseintermediaire (j, jplus2);
	
	damier[m][l]=numerojoueur;
	afficher_pions_graphique(damier,l,m, 'p'); // affiche la pièce du joueur sur la pièce prise 
	AttendreDelai (1500);

	damier[m][l]=numeroAdversaire;
	afficher_pions_graphique(damier,l,m, 'n');//réaffiche la pièce de l'adversaire 


	damier[m][l]=numerojoueur;
	afficher_pions_graphique(damier,l,m, 'r');//Met un peit point de la couleur du joueur, pour se rappeler que cette pièce a été prise. 

	damier[m][l]=0; //supprime le pion du damier, mais la pièce reste affichée. 
	piecesAdversesPrises[m][l]=numeroAdversaire; //Retient que la case a été prise. Elle sera supprimée une fois la raffle terminée, comme les règles du jeu l'ennoncent.

	AttendreDelai (1000);
	//Met un pion là où le joueur veut déplacer son pion
	damier[jplus2][iplus2] = numerojoueur;
	afficher_pions_graphique(damier,iplus2,jplus2, 'n');

}

void DeplacerPionMangerBouclePrincipale (int damier[][TAILLECOTEDAMIER + 3], int numerojoueur, int numeroAdversaire){
	
	int  piecesAdversesPrises[TAILLECOTEDAMIER + 3][TAILLECOTEDAMIER + 3], i, j /*Attention, les indices i et j servent à définir où on a cliqué, ce ne sont pas des indices de boucles*/, iplus2, jplus2, JouerConinuePrendre, iScanne, jScanne/*iScanne, jScanne sont les indices de boucles */;

	//initialisation du tableau « piecesAdversesPrises »
	for (iScanne = 1; iScanne<= TAILLECOTEDAMIER;iScanne++)
		for (jScanne=1; jScanne<=TAILLECOTEDAMIER; jScanne++)
			piecesAdversesPrises [jScanne][iScanne] = 0;


	//Ci-après, les fonctions pour sélectionner le pion mangeur, sélectionner sa case d'arrivée, et enfin afficher.
	DeplacerPionMangerSelectionCaseDEPART ( damier,  &i,  &j, numeroAdversaire );
	DeplacerPionMangerSelectionCaseARRIVEE (damier, i, j, &iplus2, &jplus2, numeroAdversaire);
	DeplacementPionMangeurAffichage (damier,piecesAdversesPrises, i, j,  iplus2, jplus2, numerojoueur, numeroAdversaire );

	//CI APRÈS, « Lorsqu’au cours d’une prise par un pion, celui-ci se trouve à nouveau en présence, diagonalement, d’une pièce adverse derrière laquelle se trouve une case libre, il doit obligatoirement sauter par-dessus cette seconde pièce, voire d’une troisième et ainsi de suite, et occuper la case libre se trouvant derrière la dernière pièce capturée. Les pièces adverses ainsi capturées sont ensuite enlevées du damier dans l’ordre de la prise. Cette opération complète est une rafle par un pion. » (source ffjd). 
	i = iplus2;
 	j = jplus2;
 	printf ("- i= %d, j =%d,  -", i, j);
	JouerConinuePrendre = ScanCasesContigueSiPionAdverseAPrendre (damier, i, j, numerojoueur, numeroAdversaire);
	printf ("- i= %d, j =%d, -", i, j);
	while  ( JouerConinuePrendre == 1){
 		i = iplus2;
 		j = jplus2;
 		DeplacerPionMangerSelectionCaseARRIVEE (damier, i, j, &iplus2, &jplus2, numeroAdversaire);
 		DeplacementPionMangeurAffichage (damier,piecesAdversesPrises, i, j,  iplus2, jplus2, numerojoueur, numeroAdversaire );
 		JouerConinuePrendre = ScanCasesContigueSiPionAdverseAPrendre (damier, i, j, numerojoueur, numeroAdversaire);
 	}
 	//Les pièces prises ne peuvent être enlevées du damier qu’à l’issue de l’exécution complète de la rafle. (source ffjd)
 	for (jScanne=1;jScanne<=TAILLECOTEDAMIER;jScanne++)
 		for (iScanne=1;iScanne<=TAILLECOTEDAMIER;iScanne++)
 			if (piecesAdversesPrises[jScanne][iScanne] > 0){//(sélectionne les pions et les dames des 2 couleurs)
 				effacer_pions_graphique (iScanne,jScanne);
 			}

 	// FABRICATION DE DAMES 
 	// Une dame doit attendre que l’adversaire ait joué au moins une fois avant d’entrer en action. (source ffjd).
 	// Un pion qui au cours d’une rafle, ne fait que passer sur une des 5 cases de la rangée de base adverse reste un pion lorsque la rafle est terminée.
 	//Fabrication de dames (source ffjd)
	//Lorsqu'il atteint la dernière rangée, le pion devient dame. Pour cela, on couronne le pion en plaçant dessus un deuxième pion de la même couleur. (source ffjd). [pour nous, on la couronnera de blanc, mon plateau n'est qu'en 2D.)]

 	printf (" i= %d, j = %d", i, j);
	if (numerojoueur == 1){
		if (j == 10){
			damier [j][i] = 3; // 3 c'est une dame bleu
			afficher_dames_graphique(damier,i,j);
		}
	}
	if (numerojoueur == 2){
		if (j == 1){
	 		damier [j][i] = 4; // 4 c'est une dame rouge. 
	 		afficher_dames_graphique(damier,i,j);
		}
	}
}

int ScanCasesContigueSiPionAdverseAPrendre ( int damier[][TAILLECOTEDAMIER + 3],int i, int j, int numerojoueur , int numeroAdversaire ){

	int k ; //iScanne et jScanne sont des indices de boucle.

	//Rapper. Pion blanc == 1; pion rouge == 2, cases vides == 0, cases sur lesquels on ne peut jouer == -1.



	//RAPPEL, les cases autour du damier valent -1, du coup on ne les scanne pas dans les algo ci-après.
	//On ne peut pas utiliser l'astuce d'utiliser marge == -1, car sinon on déborde du tableau.  
	if (damier[j][i]==numerojoueur ){// Pour ne faire les recherches que pour les cases où y'a un PION du joueur. 
		if (damier [j+1][i+1] == numeroAdversaire &&  damier [j+2][i+2]==0 && j!=1 && j != 2 && i != 9 && i != 10)//Pour savoir si sur la diagonale sud-est y'a un pion de l'autre joueur. ET si sur cette même diagonale, la case suivante est vide, et donc si le joueur peut prendre la pièce. Attention, il ne faut tester cette direction si le pion est situé sur la colonne 9 ou 10 ou la ligne 9 et 10 sinon on sort du damier.  
			k=1;
		else if (damier [j-1][i-1] == numeroAdversaire && damier [j-2][i-2]==0 && j!= 1 && j!=  2 && i != 1 && i != 2)//Idem, diagonale nord-ouest. Attention, il ne faut tester cette direction si le pion est situé sur la colonne 1 ou 2, ou la ligne 1 et 2, sinon on sort du damier,  
			k=1;
		else if (damier[j+1][i-1] == numeroAdversaire && damier [j+2][i-2]==0 && j != 9 && j!= 10 && i != 1 && i!= 2)//Idem diagonale sud-ouest. Attention, il ne faut tester cette direction si le pion est situé sur la colonne 9 ou 10, ou la ligne 1 et 2, sinon on sort du damier,  
			k=1;
		else if (damier[j-1][i+1] == numeroAdversaire && damier [j-2][i+2]==0 && j != 1 && j!= 2 && i != 9 && i != 10)//Idem diagonale nord-est. Attention, il ne faut tester cette direction si le pion est situé sur la colonne 9 ou 10, ou la ligne 1 et 2, sinon on sort du damier,  
			k=1;
		else 
			k=0;	
	}
	return k;
}


 
