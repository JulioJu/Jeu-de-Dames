/*********************************************************************/
/* ééééBibliotheque graphique graphlib pour l'enseignement universitaire */
/* D'apres les idees de Francoise et Jean-Paul Bertrandias sur Atari */
/* Implementation sous X Windows: Michel Bonin et Xavier Girod       */
/* UFR IMA - Universite Joseph Fourier (Grenoble)                    */
/* Centre Joseph Fourier Drome-Ardeche (Valence)                     */
/* email: Michel.Bonin@ujf-grenoble.fr                               */
/*                                                                   */
/* 11/05/2005 (P. Habraken) :                                        */
/* Remise a plat et simplification...                                */
/*                                                                   */
/* 17/10/2005 (P. Habraken) :                                        */
/* Ajout des primitives creer_image_bitmap(),                        */
/* charger_fichier_bitmap() et afficher_image()                      */
/*                                                                   */
/* 15/11/2005 (P. Habraken) :                                        */
/* Ajout des primitives creer_image_xpm() et charger_fichier_xpm()   */
/*                                                                   */
/* 03/04/2006 (P. Habraken) :                                        */
/* Ajout de la primitive delai()                                     */
/*                                                                   */
/* 14/05/2007 (P. Habraken) :                                        */
/* - Ajout d'un parametre permettant de recuperer le numero de       */
/*   bouton (cliquer_xy, cliquer_xy_dans, lacher_xy, lacher_xy_dans) */
/*   Les variables designees par les parametres x, y et b ne sont    */
/*   modifiees que si x, y et b sont non NULL                        */
/*                                                                   */
/* 23/03/2009 (P. Habraken) :                                        */
/* Nouvelles primitives toucheOuClic() et toucheOuClicDans()         */
/* (pour machine trace)                                              */
/*********************************************************************/

#ifndef GRAPHLIB_W2_H
#define GRAPHLIB_W2_H

#define XSize  999
#define YSize  749

#define NB_MAX_FENETRES 256
#define NB_MAX_IMAGES 256

typedef void *Fenetre;
typedef void *Image;

typedef enum {
   BLANC = 0, ROUGE, BLEU_FONCE, SAUMON_FONCE, JAUNE, BEIGE, BLEU_CLAIR,
   ORANGE, MAGENTA, CYAN_FONCE, BLEU_VERT, NOIR, BLEU, VERT
} Couleur_glw2;

/**********************************************************************/
/* gr_inits : initialisation de l'e'cran graphique                    */
void gr_inits(Fenetre *f, int xcorner, int ycorner);

/**********************************************************************/
/* gr_inits_2 : initialisation de l'e'cran graphique                  */
/* cree une fenetre X-Window de taille larg X haut                    */

void gr_inits_2(Fenetre *f, int xcorner, int ycorner,
                            int larg, int haut);

/**********************************************************************/
/* gr_inits_w : initialisation de l'e'cran graphique                  */
/* cree une fenetre X-Window de taille larg x haut avec un titre      */

void gr_inits_w(Fenetre *f, int xcorner, int ycorner,
                            int larg, int haut,
                            char titre[]);

/**********************************************************************/
/* gr_close : fermeture de l'e'cran graphique                         */

void gr_close(Fenetre *f);

/**********************************************************************/
/* creer_image_bitmap : creation d'une image a partir de donnees au   */
/* format bitmap                                                      */

void creer_image_bitmap(Image *image, unsigned char *donnees,
                        unsigned int largeur, unsigned int hauteur);

/**********************************************************************/
/* charger_fichier_bitmap : chargement des donnees d'une image a      */
/* partir d'un fichier au format bitmap                               */ 

void charger_fichier_bitmap(char *nomFichier,
                            unsigned char **donnees,
                            unsigned int *largeur,
                            unsigned int *hauteur);

/**********************************************************************/
/* creer_image_xpm : creation d'une image a partir de donnees au      */
/* format xpm                                                         */

#ifdef XPM
void creer_image_xpm(Image *image, unsigned char *donnees[]);
#endif /* XPM */

/**********************************************************************/
/* charger_fichier_xpm : chargement des donnees d'une image a         */
/* partir d'un fichier au format xpm                                  */ 

#ifdef XPM
void charger_fichier_xpm(char *nomFichier, unsigned char **donnees[]);
#endif /* XPM */

/**********************************************************************/
/* set_blue : changement de couleur                                   */

void set_blue(Fenetre f);

/**********************************************************************/
/* set_red : changement de couleur                                    */

void set_red(Fenetre f);

/**********************************************************************/
/* set_green : changement de couleur                                  */

void set_green(Fenetre f);

/**********************************************************************/
/* set_yellow : changement de couleur                                 */

void set_yellow(Fenetre f);

/**********************************************************************/
/* set_black : changement de couleur                                  */

void set_black(Fenetre f);

/**********************************************************************/
/* set_white : changement de couleur                                  */

void set_white(Fenetre f);

/**********************************************************************/
/* set_color : changement de couleur                                  */

void set_color(Fenetre f, Couleur_glw2 c);

/**********************************************************************/
/* ecriture de texte en blanc sur fond noir                           */

void video_inv(Fenetre f);

/**********************************************************************/
/* dessin en blanc sur fond noir                                      */

void graph_inv(Fenetre f);

/**********************************************************************/
/* ecriture de texte en noir sur fond blanc                           */

void video_nor(Fenetre f);

/**********************************************************************/
/* dessin en noir sur fond blanc                                      */

void graph_nor(Fenetre f);

/**********************************************************************/
/* clear_screen : effacement de l'e'cran                              */

void clear_screen(Fenetre f);

/**********************************************************************/
/* afficher_image : affiche une image prealablement creee             */

void afficher_image(Fenetre f, Image i, int x, int y);

/**********************************************************************/
/* effacer_image : efface une image prealablement affichee            */

void effacer_image(Fenetre f, Image i, int x, int y);

/**********************************************************************/
/* line : trace une ligne                                             */

void line(Fenetre f, int x1, int y1, int x2, int y2);

/**********************************************************************/
/* line_off : efface une ligne                                        */

void line_off(Fenetre f, int x1, int y1, int x2, int y2);

/**********************************************************************/
/* point :  affichage du point (x,y)                                  */

void point(Fenetre f, int x, int y);

/**********************************************************************/
/* point_off : effacement du point (x,y)                              */

void point_off(Fenetre f, int x, int y);

/**********************************************************************/
/* fill_triangle: remplit un triangle                                 */

void fill_triangle(Fenetre f, int x1, int y1, int x2, int y2,
                              int x3, int y3);

/**********************************************************************/
/* circle : trace un cercle                                           */

void circle(Fenetre f, int x, int y, int R);

/**********************************************************************/
/* fill_circle : remplit un cercle                                    */

void fill_circle(Fenetre f, int x, int y, int R);

/**********************************************************************/
/* circle_off : efface un cercle                                      */

void circle_off(Fenetre f, int x, int y, int R);

/**********************************************************************/
/* trace d'un rectangle dont les extremites de la diagonale sont      */
/* (x1, y1) et (x2, y2)                                               */

void rectangle(Fenetre f, int x1, int y1, int x2, int y2);

/**********************************************************************/
/* remplit un rectangle dont les extremites de la diagonale sont      */
/* (x1, y1) et (x2, y2)                                               */

void fill_rectangle(Fenetre f, int x1, int y1, int x2, int y2);

/**********************************************************************/
/* effacement d'un rectangle dont les extremites de la diagonale sont */
/* (x1, y1) et (x2, y2)                                               */

void rectangle_off(Fenetre f, int x1, int y1, int x2, int y2);

/**********************************************************************/
/* ellipse : trace une ellipse de centre (x,y) et de rayons rx et ry  */

void ellipse(Fenetre f, int x, int y, int rx, int ry);

/**********************************************************************/
/* fill_ellipse : remplit une ellipse de centre (x,y) et de rayons rx */
/* et ry                                                              */

void fill_ellipse(Fenetre f, int x, int y, int rx, int ry);

/**********************************************************************/
/* ellipse_off : efface l'ellipse de centre (x,y) et de rayons rx et  */
/* ry                                                                 */

void ellipse_off(Fenetre f, int x, int y, int rx, int ry);

/**********************************************************************/
/* write_gr : ecriture sur l'ecran graphique en superposition         */

void write_gr(Fenetre f, int x, int y, char str[]);

/**********************************************************************/
/* overwrite_gr : e'criture sur l'e'cran graphique avec effacement    */

void overwrite_gr(Fenetre f, int x, int y, char str[]);

/**********************************************************************/
/* draw_begin : debut d'une sequence de traces graphiques             */

void draw_begin(Fenetre f);

/**********************************************************************/
/* draw_end : fin d'une sequence de traces graphiques                 */

void draw_end(Fenetre f);

/**********************************************************************/
/* cliquer : Suspend l'execution du programme jusqu'a un clic de la   */
/* souris                                                             */
void cliquer(Fenetre f);

/**********************************************************************/
/* cliquer_dans : Suspend l'execution du programme jusqu'a un clic de */
/* la souris                                                          */

void cliquer_dans(Fenetre *f);

/**********************************************************************/
/* lacher : Suspend l'execution du programme jusqu'a un relachement   */
/* de la souris                                                       */

void lacher(Fenetre f);

/**********************************************************************/
/* lacher_dans : Suspend l'execution du programme jusqu'a un          */
/* relachement de la souris                                           */

void lacher_dans(Fenetre *f);

/**********************************************************************/
/* cliquer_xy : Met a jour x, y et b avec les coordonnees du dernier  */
/* clic souris et le numero du bouton clique                          */
/* Chacune des variables designees par les parametre x, y et b n'est  */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void cliquer_xy(Fenetre f, int *x, int *y, int *b);

/**********************************************************************/
/* cliquer_xy_dans : Met a jour x, y, b et f avec les coordonnees du  */
/* dernier clic souris, le numero du bouton clique et                 */
/* l'identificateur de la fenetre concernee                           */
/* Chacune des variables designees par les parametre x, y et b n'est  */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void cliquer_xy_dans(Fenetre *f, int *x, int *y, int *b);

/**********************************************************************/
/* lacher_xy : Met a jour x, y et b avec les coordonnees du dernier   */
/* relache de bouton de souris et le numero du bouton relache         */
/* Chacune des variables designees par les parametre x, y et b n'est  */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void lacher_xy(Fenetre f, int *x, int *y, int *b);

/**********************************************************************/
/* lacher_xy_dans : Met a jour x, y, b et f avec les coordonnees du   */
/* relache de bouton de souris, le numero du bouton clique et         */
/* l'identificateur de la fenetre concernee                           */
/* Chacune des variables designees par les parametre x, y et b n'est  */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void lacher_xy_dans(Fenetre *f, int *x, int *y, int *b);

/**********************************************************************/
/* glisser_xy : Met a jour les coordonnees x et y avec les dernieres  */
/* coordonnees de la souris                                           */
/* Chacune des variables designees par les parametre x et y n'est     */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void glisser_xy(Fenetre f, int *x, int *y);

/**********************************************************************/
/* glisser_xy_dans : Met a jour les coordonnees x et y avec les       */
/* dernieres coordonnees de la souris                                 */
/* Chacune des variables designees par les parametre x et y n'est     */
/* mise a jour que si le parametre correspondant n'est pas NULL       */

void glisser_xy_dans(Fenetre *f, int *x, int *y);

/**********************************************************************/
/* pour la compatibilite avec Atari                                   */

void wait_kbd(Fenetre f);

/**********************************************************************/
/* pour la compatibilite avec Atari                                   */

void wait_kbd_dans(Fenetre *f);

/**********************************************************************/
/* cstat teste l'enfoncement d'une touche du clavier                  */

int cstat(Fenetre f);

/**********************************************************************/
/* cstat_dans teste l'enfoncement d'une touche du clavier             */

int cstat_dans(Fenetre *f);

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

char consilent(Fenetre f);

/**********************************************************************/
/* consilent_dans fournit le caractere present s'il y en a un, le     */
/* caractere nul sinon                                                */
/* (voir consilent)                                                   */

char consilent_dans(Fenetre *f);

/**********************************************************************/	
/* bouton fournit le numero de bouton si un bouton de la souris a ete */
/* enfonce, 0 sinon                                                   */

int bouton(Fenetre f);

/**********************************************************************/	
/* bouton_dans fournit le numero de bouton si un bouton de la souris  */
/* a ete enfonce, 0 sinon                                             */

int bouton_dans(Fenetre *f);

/**********************************************************************/	
/* toucheOuClic(f) = 1 si une touche du clavier ou un bouton de la    */
/* souris a ete appuye, 0 sinon                                       */

int toucheOuClic(Fenetre f);

/**********************************************************************/	
/* toucheOuClic_dans() designe la fenetre dans laquelle une touche du */
/* clavier ou un bouton de la souris a ete appuye ; si aucun de ces   */
/* deux evenements n'a eu lieu la valeur produite est NULL            */

Fenetre toucheOuClic_dans(void);

/**********************************************************************/
/* bouton_relache fournit le numero de bouton si un bouton de la      */
/* souris a ete relache, 0 sinon                                      */

int bouton_relache(Fenetre f);

/**********************************************************************/
/* bouton_relache_dans fournit le numero de bouton si un bouton de la */
/* souris a ete relache, 0 sinon                                      */

int bouton_relache_dans(Fenetre *f);

/**********************************************************************/
/* bouton_glisse retourne un numero de bouton (1, 2 ou 3) si la       */
/* souris a ete deplacee l'un des boutons etant appuye, 0 sinon       */

int bouton_glisse(Fenetre f);

/**********************************************************************/
/* bouton_glisse_dans retourne un numero de bouton (1, 2 ou 3) si la  */
/* souris a ete deplacee l'un des boutons etant appuye, 0 sinon       */

int bouton_glisse_dans(Fenetre *f);

/**********************************************************************/
/* delai bloque l'execution pendant millis millisecondes              */

void delai(long millis);

#endif /* GRAPHLIB_W2_H */
