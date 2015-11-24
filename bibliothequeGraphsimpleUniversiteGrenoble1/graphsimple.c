/***********************************************************************
 * ééégraphsimple.c
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
 *
 * modifie par P.Habraken le 19/11/2009 :
 * Ajout du switch _POSIX_C_SOURCE pour la compilation de nanosleep
 * en mode ansi
 **********************************************************************/

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphsimple.h"
#include "graphlib_w2.h"

Fenetre fenetreCourante = NULL;

int XFen, YFen; 		/* dimensions de la fenetre */
int XCorner, YCorner;

/* Change la couleur utilisee pour les traces */
void ChangerCouleur(int couleur) {
   switch (couleur) {
      case Rouge :
         set_red(fenetreCourante); 
         break;
      case Bleu :
         set_blue(fenetreCourante); 
         break;
      case Vert:
         set_green(fenetreCourante); 
         break;
      case Jaune:
         set_yellow(fenetreCourante); 
         break;
      case Blanc:
         set_white(fenetreCourante);
         break;
      default:
         set_black(fenetreCourante); 
         break;      
   }
}

/* Initialise la fenetre graphique */
void Initialiser(int x, int y) {

   if (fenetreCourante != NULL) {
      fprintf(stderr, "Initialiser : erreur fenetre deja ouverte\n");
      exit(1);
   }

   if (x < 0 || x > XSize)
    	XFen = XSize;
   else
    	XFen = x;
   if (y < 0 || y > XSize)
    	YFen = YSize;
   else
    	YFen = y;
   XCorner = 10;
   YCorner = 10;
   gr_inits_2(&fenetreCourante, XCorner, YCorner, XFen, YFen);
}

/* Clot la fenetre graphique */
void Clore() {
	gr_close(&fenetreCourante);
}

/* Efface la fenetre graphique */
void Effacer() {
	clear_screen(fenetreCourante);
}

/* Suspend l'execution du programme pendant un temps tps donne en ms */
void AttendreDelai(long tps) {
   struct timespec t1;
   struct timespec t2;

   t1.tv_sec = tps / 1000;
   t1.tv_nsec = (tps % 1000) * 1000000;
   nanosleep(&t1, &t2);
}

/* Suspend l'execution du programme jusqu'a la frappe d'une touche sur
   le clavier */
void AttendreTaper(char *c) {	
   printf("-- Taper sur une touche (pointeur dans la fenetre) --");
   fflush(stdout);
   if (c == NULL) {
      wait_kbd(fenetreCourante);
   }
   else {
      char touche;

      touche = consilent(fenetreCourante);
      while (touche == '\0') {
         AttendreDelai(10);
         touche = consilent(fenetreCourante);
      }
      *c = touche;
   }
   printf("\n");
}

/* Si une touche du clavier vient d'etre enfoncee, retourne le caractere
   correspondant, sinon retourne le caractere null (\0)  */ 
char Touche() {
   char c;
  
   fflush(stdout);
   c = consilent(fenetreCourante);
   return c;
}

/* Retourne la valeur 1 si une touche du clavier a ete enfonce,
   0 sinon */
int TestClavier() {
   return cstat(fenetreCourante);
}

/* Suspend l'execution du programme jusqu'a un appui du bouton de la
   souris */
void AttendreClic() {
   printf("-- Cliquer --");
   fflush(stdout);
   cliquer(fenetreCourante);
   printf("\n");
}

/* Suspend l'exe'cution du programme jusqu'a un relachement du bouton de
   la souris */
void AttendreLacher() {
   printf("-- Lacher --");
   fflush(stdout);
   lacher(fenetreCourante);
   printf("\n");
}

/* Attend un clic de la souris et recupere ses cordonnees en mettant a
   jour les valeurs designees par x et y */
void AttendreClicXY(int *x, int *y, int *b) {	
   printf("-- Cliquer --");
   fflush(stdout);
   cliquer_xy(fenetreCourante, x, y, b);
   printf("\n");
}

/* Attend un relachement de la souris et recupere ses coordonnees en
   mettant a jour les valeurs designees par x et y */
void AttendreLacherXY(int *x, int *y, int *b) {	
   printf("-- Lacher --");
   fflush(stdout);
   lacher_xy(fenetreCourante, x, y, b);
   printf("\n");
}

/* Attend un deplacement de la souris et recupere ses coordonnees en
   mettant a jour les valeurs designees par x et y */
void AttendreDeplacementXY(int *x, int *y) {	
   glisser_xy(fenetreCourante, x, y);
}

/* Retourne le numero de bouton si un bouton de la souris a ete enfonce,
   0 sinon */
int BoutonAppuye() {
   return bouton(fenetreCourante);
}

/* Retourne le numero de bouton si un bouton de la souris a ete relache,
   0 sinon */
int BoutonRelache() {
   return bouton_relache(fenetreCourante);
}

/* Retourne un numero de bouton (1, 2 ou 3) si la souris vient d'etre
   deplacee l'un des boutons etant appuye, 0 sinon */
int BoutonGlisse() {
   return bouton_glisse(fenetreCourante);
}

/* Trace une ligne entre les points (x1, y1) et (x2, y2) */
void Ligne(int x1, int y1, int x2, int y2) {
   line(fenetreCourante, x1, y1, x2, y2);
}

/* Efface une ligne entre les points (x1, y1) et (x2, y2) */
void SuppLigne(int x1, int y1, int x2, int y2) {
   line_off(fenetreCourante, x1, y1, x2, y2);
}

/* Trace un cercle de centre (x, y) et de rayon r */
void Cercle(int x, int y, int r) {
   circle(fenetreCourante, x, y, r);
}

/* Efface un cercle de centre (x, y) et de rayon r */
void SuppCercle(int x, int y, int r) {
   circle_off(fenetreCourante, x, y, r);
}

/* Remplit un cercle de centre (x, y) et de rayon r */
void CerclePlein(int x, int y, int r) {
   fill_circle(fenetreCourante, x, y, r);
}

/* Trace un rectangle dont les extremites d'une de ses diagonales sont
   (x1, y1) et (x2, y2) */
void Rectangle(int x1, int y1, int x2, int y2) {
   rectangle(fenetreCourante, x1, y1, x2, y2);
}

/* Efface un rectangle dont les extremites d'une de ses diagonales sont
   (x1, y1) et (x2, y2) */
void SuppRectangle(int x1, int y1, int x2, int y2) {
   rectangle_off(fenetreCourante, x1, y1, x2, y2);
}

/* Remplit un rectangle dont les extremites d'une de ses diagonales sont
   (x1, y1) et (x2, y2) */
void RectanglePlein(int x1, int y1, int x2, int y2) {
   fill_rectangle(fenetreCourante, x1, y1, x2, y2);
}

/* Trace une ellipse de centre (x, y) et de rayons rx et ry */
void Ellipse(int x, int y, int rx, int ry) {
   ellipse(fenetreCourante, x, y, rx, ry);
}

/* Efface une ellipse de centre (x, y) et de rayons rx et ry */
void SuppEllipse(int x, int y, int rx, int ry) {
   ellipse_off(fenetreCourante, x, y, rx, ry);
}

/* Remplit une ellipse de centre (x, y) et de rayons rx et ry */
void EllipsePleine(int x, int y, int rx, int ry) {
   fill_ellipse(fenetreCourante, x, y, rx, ry);
}

/* Trace un triangle dont les sommets sont les points (x1, y1),
   (x2, y2) et (x3, y3) */
void Triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
   line(fenetreCourante, x1, y1, x2, y2);
   line(fenetreCourante, x2, y2, x3, y3);
   line(fenetreCourante, x3, y3, x1, y1);
}

/* Efface un triangle dont les sommets sont les points (x1, y1),
   (x2, y2) et (x3, y3) */
void SuppTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
   line_off(fenetreCourante, x1, y1, x2, y2);
   line_off(fenetreCourante, x2, y2, x3, y3);
   line_off(fenetreCourante, x3, y3, x1, y1);
}

/* Remplit un triangle dont les sommets sont les points (x1, y1),
   (x2, y2) et (x3, y3) */
void TrianglePlein(int x1, int y1, int x2, int y2, int x3, int y3) {
   fill_triangle(fenetreCourante, x1, y1, x2, y2, x3, y3);
}

/* Affiche un point de coordonnees (x, y) */
void Point(int x, int y) {
   point(fenetreCourante, x, y);
}

/* Efface un point de coordonnees (x, y) */
void SuppPoint(int x, int y) {
   point_off(fenetreCourante, x, y);
}

/* Initialise la couleur pour les traces et les textes a blanc et la
   couleur du fond des textes a noir */
void TextInverse() {
   video_inv(fenetreCourante);
}

/* Initialise la couleur pour les traces et les textes a noir et la
   couleur du fond des textes a blanc */
void TextNormal() {
   video_nor(fenetreCourante);
}

/* Initialise la couleur pour les traces a blanc, la couleur du fond a
   noir et colorie le fond de la fenetre en noir */
void GraphInverse() {
   graph_inv(fenetreCourante);
}

/* Initialise la couleur pour les traces a noir, la couleur du fond a
   blanc et colorie le fond de la fenetre en blanc */
void GraphNormal() {
   graph_nor(fenetreCourante);
}

/* Affiche le texte donne a partir du point de cordonnees (x, y) */
void Ecrire(int x, int y, char texte[]) {
   write_gr(fenetreCourante, x, y, texte);
}

/* Affiche le texte donne a partir du point de cordonnees (x, y) en
   effacant ce qu'il y a dessous */
void EcrireDessus(int x, int y, char texte[]) {
   overwrite_gr(fenetreCourante, x, y, texte);
}
 
/* Debute une sequence de traces. Le dessin effectue sera affiche lors
   de l'appel de FinDessin() */
void DebutDessin() {
   draw_begin(fenetreCourante);
}

/* Termine une sequence de traces. Le dessin effectue depuis l'appel de
   DebutDessin() est affiche */
void FinDessin() {
   draw_end(fenetreCourante);
}
