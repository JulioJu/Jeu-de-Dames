/***********************************************************************
 * ééégraphsimple.h
 *
 * modifie par P.Habraken le 25/05/2005 :
 * - mise en forme cosmetique
 * - ajout des operations DebutDessin() et FinDessin()
 * - modification des noms de certaines operations
 * - interface et semantique des operations pre-existantes non modifiees
 *
 * modifie par P.Habraken le 26/01/2006 :
 * - graphdsu devient graphsimple
 *
 * modifie par P.Habraken le 14/05/2007 :
 * - Ajout d'un parametre permettant de recuperer le numero de bouton
 *   (AttendreClicXY, AttendreLacherXY)
 *   Les variables designees par les parametres x, y (et eventuellement
 *   b) ne sont modifiees que si x, y et b sont non NULL
 * - Ajout a la procedure AttendreTaper d'un parametre permettant de
 *   recuperer le caractere correspondant a la touche tapee
 *   La variable designee par le parametre c n'est modifiee que si c est
 *   initialement non NULL
 **********************************************************************/

#ifndef GRAPHSIMPLE_H
#define GRAPHSIMPLE_H

/* anciens noms de certaines operations */
#define Attendre(d)        AttendreDelai(d)
#define Taper()            AttendreTaper()
#define Cliquer()          AttendreClic()
#define Lacher()           AttendreLacher()
#define CliquerXY(x, y, b) AttendreClicXY(x, y, b)
#define LacherXY(x, y, b)  AttendreLacherXY(x, y, b)
#define GlisserXY(x, y)    AttendreDeplacementXY(x, y)
#define Clic()             BoutonAppuye()
#define Relache()          BoutonRelache()
#define Glisse()           BoutonGlisse()
#define ChangeCouleur(c)   ChangerCouleur(c)

/* Constante definissant les couleurs */
#define Noir   0
#define Blanc  1
#define Rouge  2
#define Bleu   3
#define Vert   4
#define Jaune  5

/* Initialise la fenetre graphique */
void Initialiser(int x, int y);
 
/* Clot la fenetre graphique */
void Clore();
 
/* Efface la fenetre graphique */
void Effacer();
  
/* Affiche un point de coordonnees (x, y) */
void Point(int x, int y);
 
/* Efface un point de coordonnees (x, y) */
void SuppPoint(int x, int y);
 
/* Trace une ligne entre les points (x1, y1) et (x2, y2) */
void Ligne(int x1, int y1, int x2, int y2);
 
/* Efface une ligne entre les points (x1, y1) et (x2, y2) */
void SuppLigne(int x1, int y1, int x2, int y2);
 
/* Trace un cercle de centre (x, y) et de rayon r */
void Cercle(int x, int y, int r);
 
/* Efface un cercle de centre (x, y) et de rayon r */
void SuppCercle(int x, int y, int r);
 
/* Remplit un cercle de centre (x, y) et de rayon r */
void CerclePlein(int x, int y, int r);
 
/* Trace un rectangle dont les extremites d'une de ses diagonales sont
   (x1, y1) et (x2, y2) */
void Rectangle(int x1, int y1, int x2, int y2);
 
/* Efface un rectangle dont les extremites d'une de ses diagonales sont
   (x1, y1) et (x2, y2) */
void SuppRectangle(int x1, int y1, int x2, int y2);
 
/* Remplit un rectangle dont les extremites d'une de ses diagonales sont
   (x1, y1) et (x2, y2) */
void RectanglePlein(int x1, int y1, int x2, int y2);
 
/* Trace une ellipse de centre (x, y) et de rayons rx et ry */
void Ellipse(int x, int y, int rx, int ry);
 
/* Efface une ellipse de centre (x, y) et de rayons rx et ry */
void SuppEllipse(int x, int y, int rx, int ry);
 
/* Remplit une ellipse de centre (x, y) et de rayons rx et ry */
void EllipsePleine(int x, int y, int rx, int ry);
 
/* Trace un triangle dont les sommets sont les points (x1, y1),
   (x2, y2) et (x3, y3) */
void Triangle(int x1, int y1, int x2, int y2, int x3, int y3);
 
/* Efface un triangle dont les sommets sont les points (x1, y1),
   (x2, y2) et (x3, y3) */
void SuppTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
 
/* Remplit un triangle dont les sommets sont les points (x1, y1),
   (x2, y2) et (x3, y3) */
void TrianglePlein(int x1, int y1, int x2, int y2, int x3, int y3);

/* Debute une sequence de traces. Le dessin effectue sera affiche lors
   de l'appel de FinDessin() */
void DebutDessin();

/* Termine une sequence de traces. Le dessin effectue depuis l'appel de
   DebutDessin() est affiche */
void FinDessin();
 
/* Suspend l'execution du programme pendant un temps tps donne en ms */
void AttendreDelai(long tps);
 
/* Suspend l'execution du programme jusqu'a la frappe d'une touche sur
   le clavier et recupere le caractere correspondant */
void AttendreTaper(char *c);
 
/* Si une touche du clavier vient d'etre enfoncee, retourne le caractere
   correspondant, sinon retourne le caractere nul (\0)  */ 
char Touche();
 
/* Retourne la valeur 1 si une touche du clavier a ete enfonce,
   0 sinon */
int TestClavier();
 
/* Suspend l'execution du programme jusqu'a un appui du bouton de la
   souris */
void AttendreClic();

/* Suspend l'exe'cution du programme jusqu'a un relachement du bouton de
   la souris */
void AttendreLacher();
 
/* Attend un clic de la souris et recupere ses cordonnees ainsi que le
   numero du bouton clique en mettant a jour les valeurs designees par
   x, y et b.
   La mise a jour n'est effectuee que si les valeurs initiales des
   parametres sont non NULL */
void AttendreClicXY(int *x, int *y, int *b);

/* Attend un relachement de la souris et recupere ses coordonnees ainsi
   que le numero du bouton relache en mettant a jour les valeurs
   designees par x, y et b.
   La mise a jour n'est effectuee que si les valeurs initiales des
   parametres sont non NULL */
void AttendreLacherXY(int *x, int *y, int *b);
 
/* Attend un deplacement de la souris et recupere ses coordonnees en
   mettant a jour les valeurs designees par x et y.
   La mise a jour n'est effectuee que si les valeurs initiales des
   parametres sont non NULL */
void AttendreDeplacementXY(int *x, int *y);

/* Retourne le numero de bouton si un bouton de la souris a ete enfonce,
   0 sinon */
int BoutonAppuye();

/* Retourne le numero de bouton si un bouton de la souris a ete relache,
   0 sinon */
int BoutonRelache();
 
/* Retourne un numero de bouton (1, 2 ou 3) si la souris vient d'etre
   deplacee l'un des boutons etant appuye, 0 sinon */
int BoutonGlisse();

/* Initialise la couleur pour les traces et les textes a blanc et la
   couleur du fond des textes a noir */
void TextInverse();
 
/* Initialise la couleur pour les traces et les textes a noir et la
   couleur du fond des textes a blanc */
void TextNormal();
 
/* Initialise la couleur pour les traces a blanc, la couleur du fond a
   noir et colorie le fond de la fenetre en noir */
void GraphInverse();
 
/* Initialise la couleur pour les traces a noir, la couleur du fond a
   blanc et colorie le fond de la fenetre en blanc */
void GraphNormal();
 
/* Affiche le texte donne a partir du point de cordonnees (x, y) */
void Ecrire(int x, int y, char texte[]);
 
/* Affiche le texte donne a partir du point de cordonnees (x, y) en
   effacant ce qu'il y a dessous */
void EcrireDessus(int x, int y, char texte[]);
 
/* Change la couleur utilisee pour les traces avec la convention
   suivante :
   Noir   0
   Blanc  1
   Rouge  2
   Bleu   3
   Vert   4
   Jaune  5
 */
void ChangerCouleur(int couleur);

#endif /* GRAPHSIMPLE_H */
