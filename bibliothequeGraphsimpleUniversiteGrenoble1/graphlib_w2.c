/*********************************************************************/
/*ééééééBibliotheque graphique graphlib pour l'enseignement universitaire */
/* D'apres les idees de Francoise et Jean-Paul Bertrandias sur Atari */
/* Implementation sous X Windows: Michel Bonin et Xavier Girod       */
/* UFR IMA - Universite Joseph Fourier (Grenoble)                    */
/* Centre Joseph Fourier Drome-Ardeche (Valence)                     */
/* email: Michel.Bonin@ujf-grenoble.fr                               */
/*                                                                   */
/* 11/05/2005 (P. Habraken) :                                        */
/* Remise a plat et simplification...                                */
/* Ajout buffer de rafraichissement                                  */
/*                                                                   */
/* 17/10/2005 (P. Habraken) :                                        */
/* Ajout des primitives creer_image_bitmap() et afficher_image()     */
/*                                                                   */
/* 15/11/2005 (P. Habraken) :                                        */
/* Ajout des primitives creer_image_xpm() et charger_fichier_xpm()   */
/*                                                                   */
/* 03/04/2006 (P. Habraken) :                                        */
/* Ajout de la primitive delai()                                     */
/*                                                                   */
/* 14/05/2007 (P. Habraken) :                                        */
/* Ajout d'un parametre permettant de recuperer le numero de bouton  */
/* (cliquer_xy, cliquer_xy_dans, lacher_xy, lacher_xy_dans)          */
/* Les variables designees par les parametres x, y et b ne sont      */
/* modifiees que si x, y et b sont non NULL                          */
/*                                                                   */
/* 23/03/2009 (P. Habraken) :                                        */
/* Nouvelles primitives toucheOuClic() et toucheOuClicDans()         */
/* (pour machine trace)                                              */
/*                                                                   */
/* 19/11/2009 (P. Habraken) :                                        */
/* Ajout du switch _POSIX_C_SOURCE pour la compilation de nanosleep  */
/* en mode ansi                                                      */
/*********************************************************************/

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#ifdef XPM
#include <X11/xpm.h>
#endif /* XPM */

#include "graphlib_w2.h"

typedef struct {
   Window win;
   Pixmap buffer;
   unsigned int largeur, hauteur;
   GC gc;
   Colormap colormap;
   unsigned int drawing;
   unsigned long background, foreground;
} StructFenetre;

typedef struct {
   Pixmap pixmap;
   Pixmap masque;
   unsigned int largeur, hauteur;
   unsigned int profondeur;
} StructImage;

Display *Moniteur;
int Ecran;

StructFenetre *fenetres[NB_MAX_FENETRES];
unsigned int derniereFenetre = -1;

StructImage *images[NB_MAX_IMAGES];
unsigned int derniereImage;

char *nomsCouleurs[] = {
   "white", "red", "DarkSlateBlue", "DarkSalmon", "yellow", "beige",
   "LightSlateBlue", "orange", "magenta", "DarkCyan", "CadetBlue",
   "black", "blue", "green"
};

static StructFenetre *quelleFenetre(Window win);
static void setColor(char *nom, StructFenetre *fenetre);
static void rafraichirFenetre(StructFenetre *fenetre);
static void open_w(Fenetre *f, int abs, int ord, int larg, int haut,
                   char titre[]);
static void close_w(Fenetre *f);
static char traduction(XKeyEvent *xkey);

/**********************************************************************/
/* gr_inits : initialisation de l'e'cran graphique                    */

void gr_inits(Fenetre *f, int xcorner, int ycorner) {

   gr_inits_2(f, xcorner, ycorner, XSize, YSize);
}

/**********************************************************************/
/* gr_inits_2 : initialisation de l'e'cran graphique                  */
/* cree une fenetre X-Window de taille larg X haut                    */

void gr_inits_2(Fenetre *f, int xcorner, int ycorner,
                            int larg, int haut) {
   char message[] = "Fenetre graphique";

   gr_inits_w(f, xcorner, ycorner, larg, haut, message);
}

/**********************************************************************/
/* gr_inits_w(larg,haut,titre) : initialisation de l'ecran graphique  */
/* cree une fenetre par defaut X-Window de taille larg X haut avec un */
/* titre                                                              */

void gr_inits_w(Fenetre *f, int xcorner, int ycorner,
                            int larg, int haut,
                            char titre[]) {

   open_w(f, xcorner, ycorner, larg, haut, titre);
}

/**********************************************************************/
/* gr_close : fermeture de l'e'cran graphique                         */

void gr_close(Fenetre *f) {

   close_w(f);
}

/**********************************************************************/
/* creer_image_bitmap : creation d'une image a partir de donnees au   */
/* format bitmap                                                      */

void creer_image_bitmap(Image *image, unsigned char *donnees,
                        unsigned int largeur, unsigned int hauteur) {
   StructImage *im;
   Pixmap pixmap;

   if (Moniteur == NULL) {
      fprintf(stderr,
              "creer_image_bitmap : erreur connexion non creee\n");
      exit(1);
   }

   if (derniereImage == NB_MAX_IMAGES - 1) {
      fprintf(stderr,
              "creer_image_bitmap : erreur trop d'images creees\n");
      exit(1);
   }

   im = (StructImage *)malloc(sizeof(StructImage));
   if (im == NULL) {
      fprintf(stderr, "creer_image_bitmap : erreur malloc\n");
      exit(1);
   }

   pixmap = XCreateBitmapFromData(Moniteur, RootWindow(Moniteur, Ecran),
                                  (char *)donnees, largeur, hauteur);
   if (pixmap == None) {
      fprintf(stderr,
              "creer_image_bitmap : memoire insuffisante\n");
      exit(1);
   }

   im->pixmap = pixmap;
   im->largeur = largeur;
   im->hauteur = hauteur;
   im->profondeur = 1;

   derniereImage += 1;
   images[derniereImage] = im;

   *image = (Image)im;
}

/**********************************************************************/
/* charger_fichier_bitmap : chargement des donnees d'une image a      */
/* partir d'un fichier au format bitmap                               */ 

void charger_fichier_bitmap(char *nomFichier,
                            unsigned char **donnees,
                            unsigned int *largeur,
                            unsigned int *hauteur) {
   int hotspotx, hotspoty;
   int resultat;

   resultat = XReadBitmapFileData(nomFichier, largeur, hauteur,
                                  donnees, &hotspotx, &hotspoty);
   
   if (resultat != BitmapSuccess) {
      fprintf(stderr,
              "charger_image_bitmap : erreur chargement\n");
      exit(1);
   }
}

/**********************************************************************/
/* creer_image_xpm : creation d'une image a partir de donnees au      */
/* format xpm                                                         */

#ifdef XPM
void creer_image_xpm(Image *image, unsigned char *donnees[]) {
   StructImage *im;
   Pixmap pixmap, masque;
   XpmAttributes attributs;
   int resultat;

   if (Moniteur == NULL) {
      fprintf(stderr,
              "creer_image_xpm : erreur connexion non creee\n");
      exit(1);
   }

   if (derniereImage == NB_MAX_IMAGES - 1) {
      fprintf(stderr,
              "creer_image_xpm : erreur trop d'images creees\n");
      exit(1);
   }

   im = (StructImage *)malloc(sizeof(StructImage));
   if (im == NULL) {
      fprintf(stderr, "creer_image_xpm : erreur malloc\n");
      exit(1);
   }

   attributs.valuemask = XpmSize;
   resultat = XpmCreatePixmapFromData(Moniteur,
                                      RootWindow(Moniteur, Ecran),
                                      (char **)donnees,
                                      &pixmap, &masque, &attributs);
   if (resultat != XpmSuccess) {
      fprintf(stderr,
              "creer_image_xpm : %s\n", XpmGetErrorString(resultat));
      exit(1);
   }

   im->pixmap = pixmap;
   im->masque = masque;
   im->largeur = attributs.width;
   im->hauteur = attributs.height;
   im->profondeur = attributs.depth;

   derniereImage += 1;
   images[derniereImage] = im;

   *image = (Image)im;
}
#endif /* XPM */

/**********************************************************************/
/* charger_fichier_xpm : chargement des donnees d'une image a         */
/* partir d'un fichier au format xpm                                  */ 

#ifdef XPM
void charger_fichier_xpm(char *nomFichier, unsigned char **donnees[]) {
   int resultat;

   resultat = XpmReadFileToData(nomFichier, (char ***)donnees);
   
   if (resultat != XpmSuccess) {
      fprintf(stderr,
              "charger_fichier_xpm : erreur chargement\n");
      exit(1);
   }
}
#endif /* XPM */

/**********************************************************************/
/* set_blue : changement de couleur                                   */

void set_blue(Fenetre f) {

   setColor("blue", (StructFenetre *)f);
}

/**********************************************************************/
/* set_red : changement de couleur                                    */

void set_red(Fenetre f) {

   setColor("red", (StructFenetre *)f);
}

/**********************************************************************/
/* set_green : changement de couleur                                  */

void set_green(Fenetre f) {

   setColor("green", (StructFenetre *)f);
}

/**********************************************************************/
/* set_yellow : changement de couleur                                 */

void set_yellow(Fenetre f) {

   setColor("yellow", (StructFenetre *)f);
}

/**********************************************************************/
/* set_black : changement de couleur                                  */

void set_black(Fenetre f) {

   setColor("black", (StructFenetre *)f);
}

/**********************************************************************/
/* set_white : changement de couleur                                  */

void set_white(Fenetre f) {

   setColor("white", (StructFenetre *)f);
}

/**********************************************************************/
/* set_color : changement de couleur                                  */

void set_color(Fenetre f, Couleur_glw2 c) {
   
   setColor(nomsCouleurs[c], (StructFenetre *)f);
}

/**********************************************************************/
/* ecriture de texte en blanc sur fond noir                           */

void video_inv(Fenetre f) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XSetBackground(Moniteur, fenetre->gc, BlackPixel(Moniteur, Ecran));
   XSetForeground(Moniteur, fenetre->gc, WhitePixel(Moniteur, Ecran));
}

/**********************************************************************/
/* dessin en blanc sur fond noir                                      */

void graph_inv(Fenetre f) {

   StructFenetre *fenetre = (StructFenetre *)f;

   video_inv(f);
   fenetre->background = BlackPixel(Moniteur, Ecran);
}

/**********************************************************************/
/* ecriture de texte en noir sur fond blanc                           */

void video_nor(Fenetre f) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XSetBackground(Moniteur, fenetre->gc, WhitePixel(Moniteur, Ecran));
   XSetForeground(Moniteur, fenetre->gc, BlackPixel(Moniteur, Ecran));
}

/**********************************************************************/
/* dessin en noir sur fond blanc                                      */

void graph_nor(Fenetre f) {

   StructFenetre *fenetre = (StructFenetre *)f;

   video_nor(f);
   fenetre->background = BlackPixel(Moniteur, Ecran);
}

/**********************************************************************/
/* clear_screen : effacement de l'e'cran                              */

void clear_screen(Fenetre f) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XSetForeground(Moniteur, fenetre->gc, fenetre->background);
   XFillRectangle(Moniteur, (Drawable)(fenetre->buffer), fenetre->gc,
                  0, 0, fenetre->largeur, fenetre->hauteur);
   XSetForeground(Moniteur, fenetre->gc, fenetre->foreground);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* afficher_image : affiche une image prealablement creee             */

void afficher_image(Fenetre f, Image i, int x, int y) {

   StructFenetre *fenetre = (StructFenetre *)f;
   StructImage *image = (StructImage *)i;

   if (image->profondeur == 1) {
      /* il s'agit d'un bitmap */
      XCopyPlane(Moniteur, (Drawable)image->pixmap,
                           (Drawable)fenetre->buffer, fenetre->gc,
                           0, 0, image->largeur, image->hauteur, x, y,
                           1);
   }
   else {
      /* la prof. de l'image est identique a celle de la fenetre */
      if (image->masque == None) {
         XCopyArea(Moniteur, (Drawable)image->pixmap,
                             (Drawable)fenetre->buffer, fenetre->gc,
                             0, 0, image->largeur, image->hauteur,
                             x, y);
      }
      else {
         XSetClipMask(Moniteur, fenetre->gc, image->masque);
         XSetClipOrigin(Moniteur, fenetre->gc, x, y);
         XCopyArea(Moniteur, (Drawable)image->pixmap,
                             (Drawable)fenetre->buffer, fenetre->gc,
                             0, 0, image->largeur, image->hauteur,
                             x, y);
         XSetClipMask(Moniteur, fenetre->gc, None);
      }
   }
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* effacer_image : efface une image prealablement affichee            */

void effacer_image(Fenetre f, Image i, int x, int y) {

   StructFenetre *fenetre = (StructFenetre *)f;
   StructImage *image = (StructImage *)i;

   XSetForeground(Moniteur, fenetre->gc, fenetre->background);
   XFillRectangle(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
                  x, y, image->largeur, image->hauteur);
   XSetForeground(Moniteur, fenetre->gc, fenetre->foreground);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* line : trace une ligne                                             */

void line(Fenetre f, int x1, int y1, int x2, int y2) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XDrawLine(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
             x1, y1, x2, y2);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* line_off : efface une ligne                                        */

void line_off(Fenetre f, int x1, int y1, int x2, int y2) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XSetForeground(Moniteur, fenetre->gc, fenetre->background);
   XDrawLine(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
             x1, y1, x2, y2);
   XSetForeground(Moniteur, fenetre->gc, fenetre->foreground);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* point :  affichage du point (x,y)                                  */

void point(Fenetre f, int x, int y) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XDrawPoint(Moniteur, (Drawable)fenetre->buffer, fenetre->gc, x, y);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* point_off : effacement du point (x,y)                              */

void point_off(Fenetre f, int x, int y) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XSetForeground(Moniteur, fenetre->gc, fenetre->background);
   XDrawPoint(Moniteur, (Drawable)fenetre->buffer, fenetre->gc, x, y);
   XSetForeground(Moniteur, fenetre->gc, fenetre->foreground);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* fill_triangle: remplit un triangle                                 */

void fill_triangle(Fenetre f, int x1, int y1, int x2, int y2,
                              int x3, int y3) {

   StructFenetre *fenetre = (StructFenetre *)f;
   XPoint triangle[3];

   triangle[0].x = x1;
   triangle[0].y = y1;
   triangle[1].x = x2;
   triangle[1].y = y2;
   triangle[2].x = x3;
   triangle[2].y = y3;
   XFillPolygon(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
                triangle, 3, Convex, CoordModeOrigin);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* circle : trace un cercle                                           */

void circle(Fenetre f, int x, int y, int R) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XDrawArc(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
            x - R, y - R, 2 * R, 2 * R, 0, 23040);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* fill_circle : remplit un cercle                                    */

void fill_circle(Fenetre f, int x, int y, int R) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XFillArc(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
            x - R, y - R, 2 * R, 2 * R, 0, 23040);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* circle_off : efface un cercle                                      */

void circle_off(Fenetre f, int x, int y, int R) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XSetForeground(Moniteur, fenetre->gc, fenetre->background);
   XDrawArc(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
            x - R, y - R, 2 * R, 2 * R, 0, 23040);
   XSetForeground(Moniteur, fenetre->gc, fenetre->foreground);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* trace d'un rectangle dont les extremites de la diagonale sont      */
/* (x1, y1) et (x2, y2)                                               */

void rectangle(Fenetre f, int x1, int y1, int x2, int y2) {

   StructFenetre *fenetre = (StructFenetre *)f;
   int x0, y0, dx, dy;

   if (x2 >= x1 && y2 >= y1) {
      x0 = x1; y0 = y1; dx = x2 - x1 + 1; dy = y2 - y1 + 1;
   }
   else if (x2 >= x1 && y2 < y1) {
      x0 = x1; y0 = y2; dx = x2 - x1 + 1; dy = y1 - y2 + 1;
   }
   else if (x2 < x1 && y2 < y1) {
      x0 = x2; y0 = y2; dx = x1 - x2 + 1; dy = y1 - y2 + 1;
   }
   else {
      x0 = x2; y0 = y1; dx = x1 - x2 + 1; dy = y2 - y1 + 1;
   }

   XDrawRectangle(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
                  x0, y0, dx, dy);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* remplit un rectangle dont les extremites de la diagonale sont      */
/* (x1, y1) et (x2, y2)                                               */

void fill_rectangle(Fenetre f, int x1, int y1, int x2, int y2) {

   StructFenetre *fenetre = (StructFenetre *)f;
   int x0, y0, dx, dy;

   if (x2 >= x1 && y2 >= y1) {
      x0 = x1; y0 = y1; dx = x2 - x1 + 1; dy = y2 - y1 + 1;
   }
   else if (x2 >= x1 && y2 < y1) {
      x0 = x1; y0 = y2; dx = x2 - x1 + 1; dy = y1 - y2 + 1;
   }
   else if (x2 < x1 && y2 < y1) {
      x0 = x2; y0 = y2; dx = x1 - x2 + 1; dy = y1 - y2 + 1;
   }
   else {
      x0 = x2; y0 = y1; dx = x1 - x2 + 1; dy = y2 - y1 + 1;
   }

   XFillRectangle(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
                  x0, y0, dx, dy);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* effacement d'un rectangle dont les extremites de la diagonale sont */
/* (x1, y1) et (x2, y2)                                               */

void rectangle_off(Fenetre f, int x1, int y1, int x2, int y2) {

   StructFenetre *fenetre = (StructFenetre *)f;
   int x0, y0, dx, dy;

   if (x2 >= x1 && y2 >= y1) {
      x0 = x1; y0 = y1; dx = x2 - x1 + 1; dy = y2 - y1 + 1;
   }
   else if (x2 >= x1 && y2 < y1) {
      x0 = x1; y0 = y2; dx = x2 - x1 + 1; dy = y1 - y2 + 1;
   }
   else if (x2 < x1 && y2 < y1) {
      x0 = x2; y0 = y2; dx = x1 - x2 + 1; dy = y1 - y2 + 1;
   }
   else {
      x0 = x2; y0 = y1; dx = x1 - x2 + 1; dy = y2 - y1 + 1;
   }

   XSetForeground(Moniteur, fenetre->gc, fenetre->background);
   XDrawRectangle(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
                  x0, y0, dx, dy);
   XSetForeground(Moniteur, fenetre->gc, fenetre->foreground);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* ellipse : trace une ellipse de centre (x,y) et de rayons rx et ry  */

void ellipse(Fenetre f, int x, int y, int rx, int ry) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XDrawArc(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
            x - rx, y - ry, 2 * rx, 2 * ry, 0, 23040);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* fill_ellipse : remplit une ellipse de centre (x,y) et de rayons rx */
/* et ry                                                              */

void fill_ellipse(Fenetre f, int x, int y, int rx, int ry) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XFillArc(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
            x - rx, y - ry, 2 * rx, 2 * ry, 0, 23040);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* ellipse_off : efface l'ellipse de centre (x,y) et de rayons rx et  */
/* ry                                                                 */

void ellipse_off(Fenetre f, int x, int y, int rx, int ry) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XSetForeground(Moniteur, fenetre->gc, fenetre->background);
   XDrawArc(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
            x - rx, y - ry, 2 * rx, 2 * ry, 0, 23040);
   XSetForeground(Moniteur, fenetre->gc, fenetre->foreground);
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* write_gr : ecriture sur l'ecran graphique en superposition         */

void write_gr(Fenetre f, int x, int y, char str[]) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XDrawString(Moniteur, (Drawable)fenetre->buffer, fenetre->gc, x, y,
               str, strlen(str));
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* overwrite_gr : e'criture sur l'e'cran graphique avec effacement    */

void overwrite_gr(Fenetre f, int x, int y, char str[]) {

   StructFenetre *fenetre = (StructFenetre *)f;

   XDrawImageString(Moniteur, (Drawable)fenetre->buffer, fenetre->gc,
                    x, y, str, strlen(str));
   rafraichirFenetre(fenetre);
}

/**********************************************************************/
/* draw_begin : debut d'une sequence de traces graphiques             */

void draw_begin(Fenetre f) {

   StructFenetre *fenetre = (StructFenetre *)f;

   fenetre->drawing++;
}

/**********************************************************************/
/* draw_end : debut d'une sequence de traces graphiques               */

void draw_end(Fenetre f) {

   StructFenetre *fenetre = (StructFenetre *)f;

   fenetre->drawing--;
   if (fenetre->drawing == 0) {
      rafraichirFenetre(fenetre);
   }
}

/**********************************************************************/
/* cliquer : Suspend l'execution du programme jusqu'a un clic de la   */
/* souris                                                             */

void cliquer(Fenetre f) {

   cliquer_xy(f, 0, 0, 0);
}

/**********************************************************************/
/* cliquer_dans : Suspend l'execution du programme jusqu'a un clic de */
/* la souris                                                          */

void cliquer_dans(Fenetre *f) {

   cliquer_xy_dans(f, 0, 0, 0);
}

/**********************************************************************/
/* lacher : Suspend l'execution du programme jusqu'a un relachement   */
/* de la souris                                                       */

void lacher(Fenetre f) {

   lacher_xy(f, 0, 0, 0);
}

/**********************************************************************/
/* lacher_dans : Suspend l'execution du programme jusqu'a un          */
/* relachement de la souris                                           */

void lacher_dans(Fenetre *f) {

   lacher_xy_dans(f, 0, 0, 0);
}

/**********************************************************************/
/* cliquer_xy : Met a jour x, y et b avec les coordonnees du dernier  */
/* clic souris et le numero du bouton clique                          */
/* Chacune des variables designees par les parametre x, y et b n'est  */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void cliquer_xy(Fenetre f, int *x, int *y, int *b) {

   XEvent evt;

   do {
      XNextEvent(Moniteur, &evt);
      if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   } while (evt.type != ButtonPress
            || f != (Fenetre)quelleFenetre(evt.xbutton.window));

   if (x != NULL) *x = evt.xbutton.x ;
   if (y != NULL) *y = evt.xbutton.y;
   if (b != NULL) *b = evt.xbutton.button;
}

/**********************************************************************/
/* cliquer_xy_dans : Met a jour x, y, b et f avec les coordonnees du  */
/* dernier clic souris, le numero du bouton clique et                 */
/* l'identificateur de la fenetre concernee                           */
/* Chacune des variables designees par les parametre x, y et b n'est  */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void cliquer_xy_dans(Fenetre *f, int *x, int *y, int *b) {

   XEvent evt;

   do {
      XNextEvent(Moniteur, &evt);
      if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   } while (evt.type != ButtonPress);

   *f = (Fenetre)quelleFenetre(evt.xbutton.window);
   if (x != NULL) *x = evt.xbutton.x ;
   if (y != NULL) *y = evt.xbutton.y;
   if (b != NULL) *b = evt.xbutton.button;
}

/**********************************************************************/
/* lacher_xy : Met a jour x, y et b avec les coordonnees du dernier   */
/* relache de bouton de souris et le numero du bouton relache         */
/* Chacune des variables designees par les parametre x, y et b n'est  */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void lacher_xy(Fenetre f, int *x, int *y, int *b) {

   XEvent evt;

   do {
      XNextEvent(Moniteur, &evt);
      if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   } while (evt.type != ButtonRelease
            || f != (Fenetre)quelleFenetre(evt.xbutton.window));

   if (x != NULL) *x = evt.xbutton.x ;
   if (y != NULL) *y = evt.xbutton.y;
   if (b != NULL) *b = evt.xbutton.button;
}

/**********************************************************************/
/* lacher_xy_dans : Met a jour x, y, b et f avec les coordonnees du   */
/* relache de bouton de souris, le numero du bouton clique et         */
/* l'identificateur de la fenetre concernee                           */
/* Chacune des variables designees par les parametre x, y et b n'est  */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void lacher_xy_dans(Fenetre *f, int *x, int *y, int *b) {

   XEvent evt;

   do {
      XNextEvent(Moniteur, &evt);
      if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   } while (evt.type != ButtonRelease);

   *f = (Fenetre)quelleFenetre(evt.xbutton.window);
   if (x != NULL) *x = evt.xbutton.x ;
   if (y != NULL) *y = evt.xbutton.y;
   if (b != NULL) *b = evt.xbutton.button;
}

/**********************************************************************/
/* glisser_xy : Met a jour les coordonnees x et y avec les dernieres  */
/* coordonnees de la souris                                           */
/* Chacune des variables designees par les parametre x et y n'est     */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void glisser_xy(Fenetre f, int *x, int *y) {

   XEvent evt;

   do {
      XNextEvent(Moniteur, &evt);
      if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   } while (evt.type != MotionNotify
            || f != (Fenetre)quelleFenetre(evt.xmotion.window));

   if (x != NULL) *x = evt.xbutton.x ;
   if (y != NULL) *y = evt.xbutton.y;
}

/**********************************************************************/
/* glisser_xy_dans : Met a jour les coordonnees x et y avec les       */
/* dernieres coordonnees de la souris                                 */
/* Chacune des variables designees par les parametre x et y n'est     */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void glisser_xy_dans(Fenetre *f, int *x, int *y) {

   XEvent evt;

   do {
      XNextEvent(Moniteur, &evt);
      if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   } while (evt.type != MotionNotify);

   *f = (Fenetre)quelleFenetre(evt.xmotion.window);
   if (x != NULL) *x = evt.xbutton.x ;
   if (y != NULL) *y = evt.xbutton.y;
}

/**********************************************************************/
/* pour la compatibilite avec Atari                                   */

void wait_kbd(Fenetre f) {

   XEvent evt;

   do {
      XNextEvent(Moniteur, &evt);
      if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   } while (evt.type != KeyPress
            || f != (Fenetre)quelleFenetre(evt.xkey.window));
}

/**********************************************************************/
/* pour la compatibilite avec Atari                                   */

void wait_kbd_dans(Fenetre *f) {

   XEvent evt;

   do {
      XNextEvent(Moniteur, &evt);
      if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   } while (evt.type != KeyPress);

   *f = (Fenetre)quelleFenetre(evt.xkey.window);
}

/**********************************************************************/
/* cstat teste l'enfoncement d'une touche du clavier                  */

int cstat(Fenetre f) {

   return consilent(f) != '\0';
}

/**********************************************************************/
/* cstat_dans teste l'enfoncement d'une touche du clavier             */

int cstat_dans(Fenetre *f) {

   return consilent_dans(f) != '\0';
}

/**********************************************************************/
/* consilent fournit le caractere present s'il y en a un, le          */
/* caractere nul sinon                                                */
/* return | entree -> ascii CR  (0x0D # '\r')                         */
/* backspace       -> ascii BS  (0x08 # '\b')                         */
/* tab             -> ascii HT  (0x09 # '\t')                         */
/* echap           -> ascii ESC (0x1B)                                */
/* suppr/del       -> ascii DEL (0x7F)                                */
/* defil. gauche   -> ascii SOH (0x01)                                */
/* defil. droite   -> ascii STX (0x02)                                */
/* defil. haut     -> ascii ETX (0x03)                                */
/* defil. bas      -> ascii EOT (0x04)                                */

char consilent(Fenetre f) {

   XEvent evt;
		
   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      if (evt.type == KeyPress 
          && f == (Fenetre)quelleFenetre(evt.xkey.window)) {
         return traduction(&(evt.xkey));
      }
      else if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   }
   return '\0';
}

/**********************************************************************/
/* consilent_dans fournit le caractere present s'il y en a un, le     */
/* caractere nul sinon                                                */
/* (voir consilent)                                                   */

char consilent_dans(Fenetre *f) {

   XEvent evt;
		
   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      if (evt.type == KeyPress) {
         *f = (Fenetre)quelleFenetre(evt.xkey.window);
         return traduction(&(evt.xkey));
      }
      else if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   }
   return '\0';
}

/**********************************************************************/	
/* bouton fournit le numero de bouton si un bouton de la souris a ete */
/* enfonce, 0 sinon                                                   */

int bouton(Fenetre f) {

   XEvent evt;

   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      if (evt.type == ButtonPress
          && f == (Fenetre)quelleFenetre(evt.xbutton.window)) {
         return evt.xbutton.button;
      }
      else if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   }
   return 0;
}

/**********************************************************************/	
/* bouton_dans fournit le numero de bouton si un bouton de la souris  */
/* a ete enfonce, 0 sinon                                             */

int bouton_dans(Fenetre *f) {

   XEvent evt;

   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      if (evt.type == ButtonPress) {
         *f = (Fenetre)quelleFenetre(evt.xbutton.window);
         return evt.xbutton.button;
      }
      else if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   }
   return 0;
}

/**********************************************************************/	
/* toucheOuClic(f) = 1 si une touche du clavier ou un bouton de la    */
/* souris a ete appuye, 0 sinon                                       */

int toucheOuClic(Fenetre f) {
   XEvent evt;
   StructFenetre *fenetre;

   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      switch (evt.type) {
         case KeyPress:
            return f == (Fenetre)quelleFenetre(evt.xkey.window);
         case ButtonPress:
            return f == (Fenetre)quelleFenetre(evt.xbutton.window);
         case Expose:
            fenetre = (Fenetre)quelleFenetre(evt.xexpose.window);
            rafraichirFenetre(fenetre);
            break;
         default:
            break;
      }
   }
   return 0;
}

/**********************************************************************/	
/* toucheOuClic_dans() designe la fenetre dans laquelle une touche du */
/* clavier ou un bouton de la souris a ete appuye ; si aucun de ces   */
/* deux evenements n'a eu lieu la valeur produite est NULL            */

Fenetre toucheOuClic_dans(void) {
   XEvent evt;
   StructFenetre *fenetre;

   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      switch (evt.type) {
         case KeyPress:
            return (Fenetre)quelleFenetre(evt.xkey.window);
         case ButtonPress:
            return (Fenetre)quelleFenetre(evt.xbutton.window);
         case Expose:
            fenetre = quelleFenetre(evt.xexpose.window);
            rafraichirFenetre(fenetre);
            break;
         default:
            break;
      }
   }
   return NULL;
}

/**********************************************************************/
/* bouton_relache fournit le numero de bouton si un bouton de la      */
/* souris a ete relache, 0 sinon                                      */

int bouton_relache(Fenetre f) {

   XEvent evt;

   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      if (evt.type == ButtonRelease
          && f == quelleFenetre(evt.xbutton.window)) {
         return evt.xbutton.button;
      }
      else if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   }
   return 0;
}

/**********************************************************************/
/* bouton_relache_dans fournit le numero de bouton si un bouton de la */
/* souris a ete relache, 0 sinon                                      */

int bouton_relache_dans(Fenetre *f) {

   XEvent evt;

   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      if (evt.type == ButtonRelease) {
         *f = (Fenetre)quelleFenetre(evt.xbutton.window);
         return evt.xbutton.button;
      }
      else if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   }
   return 0;
}

/**********************************************************************/
/* bouton_glisse retourne un numero de bouton (1, 2 ou 3) si la       */
/* souris a ete deplacee l'un des boutons etant appuye, 0 sinon       */

int bouton_glisse(Fenetre f) {
	
   XEvent evt;

   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      if (evt.type == MotionNotify
          && f == quelleFenetre(evt.xmotion.window)) {
         if (evt.xmotion.state & Button1Mask) return 1;
         else if (evt.xmotion.state & Button2Mask) return 2;
         else if (evt.xmotion.state & Button3Mask) return 3;
         else if (evt.xmotion.state & Button4Mask) return 4;
         else if (evt.xmotion.state & Button5Mask) return 5;
         else ;
      }
      else if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   }
   return 0;
}

/**********************************************************************/
/* bouton_glisse_dans retourne un numero de bouton (1, 2 ou 3) si la  */
/* souris a ete deplacee l'un des boutons etant appuye, 0 sinon       */

int bouton_glisse_dans(Fenetre *f) {
	
   XEvent evt;

   while (XEventsQueued(Moniteur, QueuedAfterFlush) != 0) {
      XNextEvent(Moniteur, &evt);
      if (evt.type == MotionNotify) {
         switch (evt.xmotion.state) {
            case Button1Mask:
               *f = (Fenetre)quelleFenetre(evt.xbutton.window);
               return 1;
            case Button2Mask:
               *f = (Fenetre)quelleFenetre(evt.xbutton.window);
               return 2;
            case Button3Mask: 
               *f = (Fenetre)quelleFenetre(evt.xbutton.window);
               return 3;
            case Button4Mask: 
               *f = (Fenetre)quelleFenetre(evt.xbutton.window);
               return 4;
            case Button5Mask: 
               *f = (Fenetre)quelleFenetre(evt.xbutton.window);
               return 5;
            default:
               break;
         }
      }
      else if (evt.type == Expose) {
         StructFenetre *fenetre = quelleFenetre(evt.xexpose.window);
         rafraichirFenetre(fenetre);
      }
   }
   return 0;
}

/**********************************************************************/
/* delai bloque l'execution pendant millis millisecondes              */

void delai(long millis) {
   struct timespec t1;
   struct timespec t2;

   t1.tv_sec = millis / 1000;
   t1.tv_nsec = (millis % 1000) * 1000000;
   nanosleep(&t1, &t2);
}

/**********************************************************************/
/* quelleFenetre : renvoie un pointeur sur la structure decrivant la  */
/*                 fenetre passee en argument (NULL si la fenetre     */
/*                 n'existe pas)                                      */

static StructFenetre *quelleFenetre(Window win) {
   unsigned int i;

   for (i = 0 ; i != derniereFenetre + 1 ; i++) {
      if (fenetres[i]->win == win) return fenetres[i];
   }
   return NULL;
}

/**********************************************************************/
/* rafraichirFenetre : recopie le contenu du buffer dans la fenetre   */

static void rafraichirFenetre(StructFenetre *fenetre) {

   if (fenetre->drawing == 0) {
      XCopyArea(Moniteur, (Drawable)fenetre->buffer,
                (Drawable)fenetre->win, fenetre->gc,
                0, 0, fenetre->largeur, fenetre->hauteur, 0, 0);
      XFlush(Moniteur);
   }
}

/**********************************************************************/
/*  pour allouer des couleurs                                         */

static void setColor(char *nom, StructFenetre *fenetre) {
   
   XColor xc1, xc2;

   XAllocNamedColor(Moniteur, fenetre->colormap, nom, &xc1, &xc2);
   XSetForeground(Moniteur, fenetre->gc, xc1.pixel);
}

/**********************************************************************/
/* Ouverture d'une fenetre                                            */

static void open_w(Fenetre *f, int abs, int ord, int larg, int haut,
                   char titre[]) {

   StructFenetre *fenetre;
   Window win;
   Pixmap buffer;
   GC gc;
   Colormap colormap;
   unsigned long background, foreground;
   unsigned int noFenetre;

   XSizeHints myhint;
   XEvent evt;

   fenetre = (StructFenetre *)(*f);
   noFenetre = 0;
   while (noFenetre != derniereFenetre + 1
          && fenetres[noFenetre] != fenetre) {
      noFenetre++;
   }
   if (noFenetre != derniereFenetre + 1) {
      fprintf(stderr, "open_w : erreur fenetre deja ouverte\n");
      exit(1);
   }

   if (derniereFenetre == NB_MAX_FENETRES - 1) {
      fprintf(stderr, "open_w : erreur trop de fenetres ouvertes\n");
      exit(1);
   }

   if (derniereFenetre == -1) {
      Moniteur = XOpenDisplay(NULL);
      if (Moniteur == NULL) {
         fprintf(stderr, "open_w : erreur XOpenDisplay\n");
         exit(1);
      }
      Ecran = DefaultScreen(Moniteur);
   }

   fenetre = (StructFenetre *)malloc(sizeof(StructFenetre));
   if (fenetre == NULL) {
      fprintf(stderr, "open_w : erreur malloc\n");
      exit(1);
   }

   win = XCreateSimpleWindow(Moniteur, RootWindow(Moniteur, Ecran),
	                          abs, ord, larg, haut, 5,
                             BlackPixel(Moniteur, Ecran),
                             WhitePixel(Moniteur, Ecran));

   colormap = DefaultColormap(Moniteur, Ecran);

   myhint.x = abs; myhint.y = ord;
   myhint.width = larg; myhint.height = haut;
   myhint.flags = PPosition | PSize;
   XSetStandardProperties(Moniteur, win, titre, titre, None, NULL, 0,
                          &myhint);

   /* Solicitation des e've'nements en entre'e. Indispensable */
   XSelectInput(Moniteur, win,
                ButtonPressMask | ButtonReleaseMask | KeyPressMask
                | ExposureMask | ButtonMotionMask | PointerMotionMask);

   /* Affichage sur l'e'cran */
   XMapWindow(Moniteur, win);

   do {
      XNextEvent(Moniteur, &evt);
   } while (evt.type != Expose || evt.xexpose.window != win);

   buffer = XCreatePixmap(Moniteur, (Drawable)win, larg, haut,
                          DefaultDepth(Moniteur, Ecran));

   gc = XCreateGC(Moniteur, RootWindow(Moniteur, Ecran), 0, NULL);

   background = WhitePixel(Moniteur, Ecran);
   foreground = BlackPixel(Moniteur, Ecran);

   XSetBackground(Moniteur, gc, background);

   XSetForeground(Moniteur, gc, background);
   XFillRectangle(Moniteur, (Drawable)buffer, gc, 0, 0, larg, haut);
   XSetForeground(Moniteur, gc, foreground);

   XCopyArea(Moniteur, (Drawable)buffer, (Drawable)win, gc, 0, 0,
                        larg, haut, 0, 0);
   XFlush(Moniteur);

   fenetre->win = win;
   fenetre->buffer = buffer;
   fenetre->largeur = larg;
   fenetre->hauteur = haut;
   fenetre->gc = gc;
   fenetre->colormap = colormap;
   fenetre->drawing = 0;
   fenetre->background = background;
   fenetre->foreground = foreground;

   derniereFenetre = derniereFenetre + 1;
   fenetres[derniereFenetre] = fenetre;
   *f = (void *)fenetre;
}

/**********************************************************************/
/* Fermeture d'une fenetre                                            */

static void close_w(Fenetre *f) {

   StructFenetre *fenetre = (StructFenetre *)(*f);
   unsigned int noFenetre = 0;

   while (noFenetre != derniereFenetre + 1
          && fenetres[noFenetre] != fenetre) {
      noFenetre++;
   }

   if (noFenetre == derniereFenetre + 1) {
      fprintf(stderr, "close_w : erreur fenetre inexistante\n");
      exit(1);
   }

   XDestroyWindow(Moniteur, fenetre->win);
   XFreePixmap(Moniteur, fenetre->buffer);
   XFreeGC(Moniteur, fenetre->gc);
   XFreeColormap(Moniteur, fenetre->colormap);
   XFlush(Moniteur);

   *f = NULL;
   free((void *)fenetre);

   fenetres[noFenetre] = fenetres[derniereFenetre];
   derniereFenetre = derniereFenetre - 1;

   if (derniereFenetre == -1) {
      XCloseDisplay(Moniteur);
   }
}

/**********************************************************************/
/* Recuperation caractere correspondant a touche frappee              */
/* return | entree -> ascii CR  (0x0D # '\r')                         */
/* backspace       -> ascii BS  (0x08 # '\b')                         */
/* tab             -> ascii HT  (0x09 # '\t')                         */
/* echap           -> ascii ESC (0x1B)                                */
/* suppr/del       -> ascii DEL (0x7F)                                */
/* defil. gauche   -> ascii SOH (0x01)                                */
/* defil. droite   -> ascii STX (0x02)                                */
/* defil. haut     -> ascii ETX (0x03)                                */
/* defil. bas      -> ascii EOT (0x04)                                */

static char traduction(XKeyEvent *xkey) {
   char buffer[1];
   unsigned int nbTraduits;
   KeySym keysym;
   XComposeStatus status;
         
   nbTraduits = XLookupString(xkey, buffer, 1, &keysym, &status);
   switch (keysym) {
      case XK_Left: return 0x01;
      case XK_Right: return 0x02;
      case XK_Up: return 0x03;
      case XK_Down: return 0x04;
      default:
         if (nbTraduits != 0)
            return buffer[0];
         else
            return '\0';
   }
}
