/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:39:02 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/04 10:00:53 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/cube3d.h"


/*
** MAP DE TEST
** ───────────
** Légende :
**   '1' = mur (wall)
**   '0' = sol vide (walkable)
**   Espace = vide hors map (traité comme mur pour la sécurité)
**
** Cette map a :
**   - Une bordure de murs fermée (obligatoire selon le sujet)
**   - Des murs intérieurs pour tester les 4 faces (N/S/E/W)
**   - Une "chambre" dans le coin bas-droit pour tester les coins
**   - Le joueur spawne en (7.5, 5.5) face au Nord
**
** Coordonnées :
**   map[y][x] — y=0 est en haut, y augmente vers le bas (écran)
**   x=0 est à gauche, x augmente vers la droite
**
** Visualisation (15 colonnes × 11 rangées) :
**
**   y=0  : 111111111111111  ← mur nord (borne)
**   y=1  : 100000001000001  ← couloir + pilier à x=8
**   y=2  : 100110001000001  ← bloc 2x2 à x=2-3, pilier
**   y=3  : 100110000000001  ← suite du bloc
**   y=4  : 100000000000001  ← espace ouvert
**   y=5  : 100000000111001  ← mur est interne à x=9-11
**   y=6  : 100000001000001  ← couloir entre les blocs
**   y=7  : 100000001000001  ← suite
**   y=8  : 100000001111001  ← fermeture de la "chambre" interne
**   y=9  : 100000000000001  ← espace ouvert bas
**   y=10 : 111111111111111  ← mur sud (borne)
*/
static char *g_mock_map[] = {
    "111111111111111",   /* y=0  */
    "100000001000001",   /* y=1  */
    "100110001000001",   /* y=2  */
    "100110000000001",   /* y=3  */
    "100000000000001",   /* y=4  */
    "100000000111001",   /* y=5  */
    "100000001000001",   /* y=6  */
    "100000001000001",   /* y=7  */
    "100000001111001",   /* y=8  */
    "100000000000001",   /* y=9  */
    "111111111111111",   /* y=10 */
    NULL                 /* terminateur NULL obligatoire                      */
};

/*
** setup_map : duplique la map statique dans le heap.
** ────────────────────────────────────────────────────
**
** Pourquoi dupliquer ?
** Le vrai parseur allouera la map dynamiquement (malloc).
** Pour que cleanup() puisse toujours appeler free() sans se demander
** si la mémoire est dans le heap ou non, on copie ici aussi.
**
** Calcule aussi map_h (nombre de lignes) et map_w (largeur max).
** map_w sert aux vérifications de bornes dans le DDA.
*/
static int  setup_map(t_config *cfg)
{
    int     h;
    int     w;
    int     len;

    h = 0;
    while (g_mock_map[h] != NULL)
        h++;
    cfg->map_h = h;
    cfg->map = (char **)malloc(sizeof(char *) * (h + 1));
    if (!cfg->map)
        return (1);
    w = 0;
    h = 0;
    while (g_mock_map[h] != NULL)
    {
        cfg->map[h] = strdup(g_mock_map[h]);
        if (!cfg->map[h])
            return (1);
        len = (int)strlen(g_mock_map[h]);
        if (len > w)
            w = len;
        h++;
    }
    cfg->map[h] = NULL;    /* terminateur NULL pour les boucles while */
    cfg->map_w = w;
    return (0);
}

/*
** setup_player : détermine la position et direction initiales du joueur.
** ────────────────────────────────────────────────────────────────────────
**
** Le vrai parseur scan la map pour trouver le caractère N/S/E/W,
** enregistre sa position, puis le remplace par '0' dans la map.
**
** Ici on hardcode directement le résultat de cette opération.
**
** Joueur placé en (7.5, 5.5) — centre de la cellule (7, 5)  ('0' confirmé)
** Orientation : face au NORD → dir = (0, -1), plane = (0.66, 0)
**
** Rappel des vecteurs par orientation :
**
**   NORTH: dir=(0,-1)  plane=( 0.66, 0  )  → regarde vers y décroissant
**   SOUTH: dir=(0, 1)  plane=(-0.66, 0  )  → regarde vers y croissant
**   EAST:  dir=(1, 0)  plane=( 0,    0.66) → regarde vers x croissant
**   WEST:  dir=(-1,0)  plane=( 0,   -0.66) → regarde vers x décroissant
**
** Le plan caméra est TOUJOURS perpendiculaire à dir.
** |plane| = 0.66 × |dir| = 0.66 → FOV = 2 × atan(0.66) ≈ 66°
*/
static void setup_player(t_config *cfg)
{
    cfg->spawn_x = 7.5;          /* centre de la cellule map[5][7]           */
    cfg->spawn_y = 5.5;
    cfg->spawn_dir_x = 0.0;      /* face au NORD : dir_x = 0                 */
    cfg->spawn_dir_y = -1.0;     /* face au NORD : dir_y = -1 (Y décroît)    */
    cfg->spawn_plane_x = 0.66;   /* plan caméra perpendiculaire, FOV ≈ 66°   */
    cfg->spawn_plane_y = 0.0;
}

/*
** setup_textures : chemins vers les fichiers XPM de textures.
** ─────────────────────────────────────────────────────────────
**
** Dans le mock, ces chemins pointent vers des fichiers qui n'existent
** pas forcément. init_textures() dans init.c détectera l'absence et
** créera des textures procédurales à la place.
**
** Dans le vrai parseur, ces chemins viennent des lignes :
**   NO ./textures/north.xpm
**   SO ./textures/south.xpm
**   WE ./textures/west.xpm
**   EA ./textures/east.xpm
**
** NOTE : strdup ici pour que cleanup() puisse toujours appeler free().
*/
static int  setup_textures(t_config *cfg)
{
    cfg->tex_path[NORTH] = strdup("./textures/north.xpm");
    cfg->tex_path[SOUTH] = strdup("./textures/south.xpm");
    cfg->tex_path[EAST]  = strdup("./textures/east.xpm");
    cfg->tex_path[WEST]  = strdup("./textures/west.xpm");
    if (!cfg->tex_path[NORTH] || !cfg->tex_path[SOUTH]
        || !cfg->tex_path[EAST] || !cfg->tex_path[WEST])
        return (1);
    return (0);
}

/*
** setup_colors : couleurs sol (F) et plafond (C) au format 0x00RRGGBB.
** ─────────────────────────────────────────────────────────────────────
**
** Ces couleurs correspondent aux lignes du .cub :
**   F 100, 80, 50   → sol brun
**   C  60,120,200   → ciel bleu
**
** Formule de packing : color = (R << 16) | (G << 8) | B
**   Sol brun  : (100<<16)|(80<<8)|50  = 0x00645032
**   Ciel bleu : (60<<16) |(120<<8)|200 = 0x003C78C8
**
** MLX interprète la couleur comme ARGB (ou BGRA selon endianness),
** mais en pratique 0x00RRGGBB fonctionne sur la plupart des configs.
*/
static void setup_colors(t_config *cfg)
{
    /* Sol : brun chaud */
    cfg->floor_color = (100 << 16) | (80 << 8) | 50;    /* 0x00645032       */

    /* Plafond : bleu ciel */
    cfg->ceil_color  = (60 << 16) | (120 << 8) | 200;   /* 0x003C78C8       */
}

/*
** init_mock_config : point d'entrée public du mock parseur.
** ──────────────────────────────────────────────────────────
**
** Remplace exactement parse_cub_file(cfg, filename) du vrai parseur.
** Retourne 0 en succès, 1 en erreur (malloc fail).
*/
int init_mock_config(t_config *cfg)
{
    setup_player(cfg);
    setup_colors(cfg);
    if (setup_textures(cfg) != 0)
        return (print_error("Mock: failed to duplicate texture paths"));
    if (setup_map(cfg) != 0)
        return (print_error("Mock: failed to duplicate map"));
    return (0);
}