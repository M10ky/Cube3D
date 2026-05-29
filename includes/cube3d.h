/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 19:47:24 by miokrako          #+#    #+#             */
/*   Updated: 2026/05/29 20:32:41 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*   cub3d.h                                       cub3D / 42                */
/*                                                                            */
/*   LE "CONTRAT" ENTRE LE PARSEUR ET LE RAYCASTER.                          */
/*   Ce fichier définit TOUTES les structures partagées entre les deux        */
/*   parties du projet binôme. Changer un champ ici impacte les deux.        */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/*
** ─── INCLUDES SYSTÈME ────────────────────────────────────────────────────────
*/
# include <math.h>          /* cos, sin, fabs, floor                          */
# include <stdio.h>         /* fprintf, perror                                 */
# include <stdlib.h>        /* malloc, free, exit                              */
# include <string.h>        /* memset, strlen                                  */
# include <mlx.h>           /* toute l'API MiniLibX                            */

/*
** ─── DIMENSIONS ÉCRAN ────────────────────────────────────────────────────────
**
**  SCREEN_W : largeur en pixels. Aussi le nombre de rayons castés par frame.
**  SCREEN_H : hauteur en pixels. Sert dans toutes les formules de projection.
**
**  Rapport 2:1 recommandé pour le raycasting (plus large que haut donne un
**  bon rendu FPS). Réduire si les performances sont mauvaises.
*/
# define SCREEN_W    1024
# define SCREEN_H    512
# define WIN_TITLE   "cub3D — DDA Raycaster"

/*
** ─── TEXTURES ─────────────────────────────────────────────────────────────────
**
**  Dimensions en pixels. DOIVENT être des puissances de 2.
**  Pourquoi puissance de 2 ?
**  → L'opération  tex_y & (TEX_H - 1)  est un modulo ultra-rapide.
**    Si TEX_H=64 (0b01000000), alors TEX_H-1=63 (0b00111111).
**    Le AND masque juste les 6 bits bas : résultat ∈ [0,63] instantanément.
**  → 64×64 = standard classique depuis Wolfenstein 3D.
*/
# define TEX_W       64
# define TEX_H       64

/*
** ─── PHYSIQUE JOUEUR ─────────────────────────────────────────────────────────
**
**  MOV_SPD  : mètres par frame (unité = 1 cellule de la map).
**  ROT_SPD  : radians par frame. 0.03 rad ≈ 1.7°/frame = rotation fluide.
**
**  Ces valeurs sont indépendantes du frame rate (pas de delta time ici,
**  mais tu peux l'ajouter en divisant par le temps écoulé).
*/
# define MOV_SPD     0.05
# define ROT_SPD     0.030

/*
** ─── FACES DES MURS ──────────────────────────────────────────────────────────
**
**  Convention : indices dans le tableau tex[4] de t_game.
**  NORTH=0, SOUTH=1, EAST=2, WEST=3.
**
**  Règle de détermination de la face (après DDA) :
**    side==0 (X-side touchée), step_x > 0 → rayon allait vers l'EST  → EAST
**    side==0 (X-side touchée), step_x < 0 → rayon allait vers l'OUEST → WEST
**    side==1 (Y-side touchée), step_y > 0 → rayon allait vers le SUD  → SOUTH
**    side==1 (Y-side touchée), step_y < 0 → rayon allait vers le NORD → NORTH
*/
# define NORTH       0
# define SOUTH       1
# define EAST        2
# define WEST        3

/*
** ─── CODES TOUCHES (Linux/X11) ───────────────────────────────────────────────
**
**  Ces valeurs sont les keysyms X11 retournés par mlx_hook.
**  Sur macOS avec mlx d'Elève : ESC=53, W=13, A=0, S=1, D=2,
**                               LEFT=123, RIGHT=124.
*/
# define KEY_ESC     65307
# define KEY_W       119
# define KEY_A       97
# define KEY_S       115
# define KEY_D       100
# define KEY_LEFT    65361
# define KEY_RIGHT   65363

/*
** ─── ÉVÉNEMENTS MLX ──────────────────────────────────────────────────────────
**
**  IDs des événements X11 passés à mlx_hook :
**    2  = KeyPress   → touche enfoncée
**    3  = KeyRelease → touche relâchée
**    17 = ClientMessage → clic sur la croix rouge de la fenêtre
*/
# define EVT_KEY_PRESS    2
# define EVT_KEY_RELEASE  3
# define EVT_CLOSE        17

/* ============================================================================
**
**  STRUCTURES DE DONNÉES
**  ─────────────────────
**  Ordre important : lire de bas en haut (t_config est rempli en premier,
**  t_game contient tout).
**
** ============================================================================ */

/*
** t_config : Données brutes issues du parseur (ou du mock).
** ─────────────────────────────────────────────────────────
**
** C'est le "paquet de données" que ton binôme te livre après parsing.
** MERGER AVEC LE VRAI PARSEUR = remplacer init_mock_config() par
** parse_cub_file() qui remplit cette même structure, champ par champ.
**
** Champs map : la map est stockée comme tableau de chaînes (char **).
**   map[y]    = une rangée complète (ex: "111001101")
**   map[y][x] = le caractère à la cellule (x, y) : '0', '1', ou espace
**
** Champs spawn : position et orientation initiales du joueur, extraites
** du caractère N/S/E/W trouvé dans la map lors du parsing.
*/
typedef struct s_config
{
    /* Chemins vers les textures (remplis par les lignes NO/SO/WE/EA) */
    char        *tex_path[4];   /* [NORTH, SOUTH, EAST, WEST]                 */

    /* Couleurs sol et plafond (lignes F et C du .cub) : 0x00RRGGBB         */
    int         floor_color;    /* ex: "F 220,100,0" → 0x00DC6400           */
    int         ceil_color;     /* ex: "C 225,30,0"  → 0x00E11E00           */

    /* Map 2D sous forme de tableau de chaînes, terminé par NULL             */
    char        **map;          /* map[y] = ligne y de la map                 */
    int         map_h;          /* nombre de lignes (hauteur)                 */
    int         map_w;          /* longueur maximale d'une ligne (largeur)    */

    /*
    ** Spawn joueur : le parseur extrait le caractère N/S/E/W de la map,
    ** calcule pos (centre de la cellule) et les vecteurs dir + plane.
    **
    ** Vecteurs initiaux selon orientation :
    **   N → dir=(0,-1),  plane=(0.66, 0)
    **   S → dir=(0, 1),  plane=(-0.66,0)
    **   E → dir=(1, 0),  plane=(0, 0.66)
    **   W → dir=(-1,0),  plane=(0,-0.66)
    */
    double      spawn_x;        /* position X initiale (centre de cellule)    */
    double      spawn_y;        /* position Y initiale                        */
    double      spawn_dir_x;    /* vecteur direction X (normalisé)            */
    double      spawn_dir_y;    /* vecteur direction Y                        */
    double      spawn_plane_x;  /* plan caméra X (⊥ à dir, |plane|=0.66)    */
    double      spawn_plane_y;  /* plan caméra Y                              */
}   t_config;

/*
** t_texture : Une texture chargée depuis un fichier XPM (ou générée).
** ──────────────────────────────────────────────────────────────────────
**
** mlx_get_data_addr() remplit bpp, line_len et endian automatiquement.
** Pour accéder au pixel (x,y) :
**   ptr = addr + y * line_len + x * (bpp / 8)
**   color = *(unsigned int *)ptr
*/
typedef struct s_texture
{
    void        *img;       /* handle MLX (pour mlx_destroy_image)            */
    char        *addr;      /* pointeur vers les données pixels               */
    int         bpp;        /* bits par pixel (typiquement 32)                */
    int         line_len;   /* octets par ligne d'image                       */
    int         endian;     /* 0=little-endian, 1=big-endian                  */
    int         width;      /* largeur texture en pixels                      */
    int         height;     /* hauteur texture en pixels                      */
}   t_texture;

/*
** t_img : Le framebuffer (image de rendu qui accumule les pixels).
** ───────────────────────────────────────────────────────────────────
**
** Stratégie double-buffer (similaire à OpenGL) :
**   1. On écrit tous les pixels dans t_img en RAM (rapide)
**   2. On envoie l'image finie à la fenêtre avec mlx_put_image_to_window
**   Avantage : évite le scintillement (tearing) qu'on aurait en dessinant
**   pixel par pixel directement sur la fenêtre.
*/
typedef struct s_img
{
    void        *img;       /* handle MLX                                     */
    char        *addr;      /* pointeur données pixels SCREEN_W×SCREEN_H     */
    int         bpp;        /* bits par pixel                                 */
    int         line_len;   /* octets par ligne                               */
    int         endian;     /* ordre des octets                               */
}   t_img;

/*
** t_player : État du joueur (mis à jour chaque frame par l'input).
** ──────────────────────────────────────────────────────────────────
**
** Le modèle caméra "vectoriel" (selon Lode Vandevenne / lodev.org) :
**
**   pos  = point de l'espace 2D où se trouve le joueur
**   dir  = vecteur UNITAIRE pointant dans la direction de vue
**   plane= vecteur plan caméra, PERPENDICULAIRE à dir
**
** Le FOV (champ de vision) est déterminé par le rapport |plane|/|dir| :
**   |plane| = 0.66 et |dir| = 1.0  →  FOV = 2×atan(0.66) ≈ 66°
**
** Lors d'une rotation d'angle θ, dir ET plane tournent ensemble :
**   new_dir_x   = dir_x  * cos(θ) - dir_y  * sin(θ)
**   new_dir_y   = dir_x  * sin(θ) + dir_y  * cos(θ)
**   (même formule pour plane_x et plane_y)
*/
typedef struct s_player
{
    double      pos_x;      /* position X dans le monde (en cellules)         */
    double      pos_y;      /* position Y dans le monde                       */
    double      dir_x;      /* direction X (vecteur unitaire, ex: (1,0)=Est)  */
    double      dir_y;      /* direction Y                                    */
    double      plane_x;    /* plan caméra X (perpendiculaire à dir)          */
    double      plane_y;    /* plan caméra Y                                  */
}   t_player;

/*
** t_ray : Toutes les variables d'UN rayon pour UNE colonne d'écran.
** ──────────────────────────────────────────────────────────────────
**
** Cette structure est réinitialisée à chaque colonne.
** Cycle de vie par colonne :
**   1. init_ray()   → calcule cam, dir_x/y, map_x/y
**   2. init_dda()   → calcule step, side_dist, delta_dist
**   3. do_dda()     → trouve le mur (hit=1, side, map_x/y finaux)
**   4. calc_wall()  → calcule wall_dist, line_h, draw_y0/y1, face, tex_x
**   5. draw_col()   → utilise tous ces résultats pour dessiner
*/
typedef struct s_ray
{
    /* ── Étape 1 : init_ray ─────────────────────────────────────────────── */
    double      cam;        /* coordonnée caméra : [-1.0 (gauche), +1.0 (droite)] */
    double      dir_x;      /* direction X du rayon = dir_x + plane_x * cam  */
    double      dir_y;      /* direction Y du rayon = dir_y + plane_y * cam  */
    int         map_x;      /* cellule map courante X = (int)pos_x           */
    int         map_y;      /* cellule map courante Y = (int)pos_y           */

    /* ── Étape 2 : init_dda ─────────────────────────────────────────────── */
    int         step_x;     /* sens du pas DDA en X : +1 ou -1              */
    int         step_y;     /* sens du pas DDA en Y : +1 ou -1              */
    double      side_x;     /* distance jusqu'à la prochaine X-side         */
    double      side_y;     /* distance jusqu'à la prochaine Y-side         */
    double      delta_x;    /* distance entre deux X-sides = |1/dir_x|      */
    double      delta_y;    /* distance entre deux Y-sides = |1/dir_y|      */

    /* ── Étape 3 : do_dda ───────────────────────────────────────────────── */
    int         hit;        /* 1 quand un mur est trouvé                     */
    int         side;       /* 0 = X-side touchée, 1 = Y-side touchée       */

    /* ── Étape 4 : calc_wall ────────────────────────────────────────────── */
    double      wall_dist;  /* distance perp. au mur (sans effet fisheye)    */
    int         line_h;     /* hauteur du mur en pixels = SCREEN_H/wall_dist */
    int         draw_y0;    /* pixel Y de début du mur (clampé à 0)         */
    int         draw_y1;    /* pixel Y de fin du mur (clampé à SCREEN_H-1)  */
    int         face;       /* NORTH/SOUTH/EAST/WEST (index dans tex[4])    */
    int         tex_x;      /* coordonnée X dans la texture : [0, TEX_W)    */
}   t_ray;

/*
** t_keys : État booléen de chaque touche surveillée.
** ────────────────────────────────────────────────────
**
** Pourquoi un struct plutôt qu'un tableau ?
** Les keycodes X11 sont grands (ex: ESC=65307), donc un tableau
** indexé par keycode serait énorme et inutile.
** Ici on mappe chaque touche utile vers un int 0/1.
**
** Mis à jour par key_press() et key_release().
** Consommé par handle_input() à chaque frame.
*/
typedef struct s_keys
{
    int         w;          /* avancer                                        */
    int         s;          /* reculer                                        */
    int         a;          /* strafer gauche                                 */
    int         d;          /* strafer droite                                 */
    int         left;       /* rotation gauche                                */
    int         right;      /* rotation droite                                */
}   t_keys;

/*
** t_game : Structure principale, contient TOUT l'état du jeu.
** ─────────────────────────────────────────────────────────────
**
** Passée par pointeur à quasiment toutes les fonctions.
** Initialisée une fois dans main(), puis mise à jour chaque frame.
**
** Cycle de vie :
**   main()
**     → memset(&game, 0, sizeof) ← zero-init CRUCIAL (pas de valeurs garbage)
**     → init_game(&game)         ← remplit toutes les sous-structures
**     → mlx_loop()              ← game_loop() appelé en boucle par MLX
**     → cleanup(&game)           ← libération à la sortie
*/
typedef struct s_game
{
    t_config    config;     /* données du parseur (map, couleurs, paths tex)  */
    t_player    player;     /* état courant du joueur (pos, dir, plane)       */
    t_texture   tex[4];    /* textures chargées [NORTH][SOUTH][EAST][WEST]   */
    t_img       buf;        /* framebuffer : image où on dessine avant affichage */
    t_keys      keys;       /* état des touches (0=relâchée, 1=enfoncée)     */
    void        *mlx;       /* pointeur de contexte MLX (opaque)              */
    void        *win;       /* pointeur de fenêtre MLX                        */
}   t_game;

/* ============================================================================
**
**  PROTOTYPES DES FONCTIONS
**  Organisés par fichier source pour une navigation facile.
**
** ============================================================================ */

/* ── mock_parser.c ─── simulation du parseur ──────────────────────────────── */
int     init_mock_config(t_config *config);

/* ── init.c ─── initialisation MLX et ressources ─────────────────────────── */
int     init_game(t_game *game);
void    init_player(t_game *game);
void    put_pixel(t_img *buf, int x, int y, unsigned int color);

/* ── raycasting.c ─── algorithme DDA ─────────────────────────────────────── */
void    cast_ray(t_game *game, int col, t_ray *ray);

/* ── render.c ─── rendu d'une frame complète ─────────────────────────────── */
int     game_loop(void *param);
void    render_frame(t_game *game);

/* ── input.c ─── gestion clavier et déplacement ──────────────────────────── */
int     key_press(int key, void *param);
int     key_release(int key, void *param);
int     handle_close(void *param);
void    handle_input(t_game *game);

/* ── cleanup.c ─── libération mémoire et sortie ──────────────────────────── */
void    cleanup(t_game *game);
void    cleanup_exit(t_game *game);
int     print_error(const char *msg);

#endif