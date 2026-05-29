/* ************************************************************************** */
/*                                                                            */
/*   raycasting.c                                  cub3D / 42                */
/*                                                                            */
/*   ALGORITHME DDA (Digital Differential Analysis) — CŒUR DU PROJET.        */
/*                                                                            */
/*   Référence : https://lodev.org/cgtutor/raycasting.html                    */
/*   Auteur théorie : Lode Vandevenne                                          */
/*                                                                            */
/*   Ce fichier est 100% mathématique — pas de MLX, pas de dessin.           */
/*   Il calcule pour CHAQUE colonne de l'écran :                              */
/*     → quelle cellule de la map le rayon frappe                             */
/*     → à quelle distance                                                    */
/*     → sur quelle face (N/S/E/W)                                           */
/*     → à quel endroit de la texture                                         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"


/*
** map_safe : accès sécurisé à la map.
** ─────────────────────────────────────
**
** Retourne '1' (mur) pour tout accès hors limites.
** CRITIQUE : sans ce garde-fou, un rayon qui sort de la map planterait
** avec un segfault (accès à map[-1] ou map[100]).
**
** Cas traités :
**   - y hors [0, map_h)  → '1'
**   - x < 0              → '1'
**   - x >= longueur ligne → '1' (lignes de longueurs différentes !)
**   - cellule normale     → caractère réel ('0' ou '1')
*/
static char map_safe(t_config *cfg, int x, int y)
{
    if (y < 0 || y >= cfg->map_h || x < 0 || x >= cfg->map_w)
        return ('1');
    if (x >= (int)strlen(cfg->map[y]))
        return ('1');
    return (cfg->map[y][x]);
}

/* ─────────────────────────────────────────────────────────────────────────────
**
** ÉTAPE 1 : init_ray()
** ─────────────────────
**
** Calcule la DIRECTION du rayon pour la colonne col (0 à SCREEN_W-1).
**
** ┌─────────────────────────────────────────────────────────┐
** │  MODÈLE CAMÉRA VECTORIEL                                 │
** │                                                         │
** │  cam = 2 × col / SCREEN_W - 1                           │
** │      ↕                                                   │
** │  cam = -1.0  →  bord gauche de l'écran                  │
** │  cam =  0.0  →  centre de l'écran (rayon = dir)         │
** │  cam = +1.0  →  bord droit de l'écran                   │
** │                                                         │
** │  rayDir = dir + plane × cam                             │
** │                                                         │
** │  Le rayon passe par le point (dir + plane×cam) du plan  │
** │  caméra, vu depuis la position du joueur.               │
** └─────────────────────────────────────────────────────────┘
**
** Exemple concret (joueur face Nord, col=0, SCREEN_W=1024) :
**   cam    = 2×0/1024 - 1 = -1.0         (bord gauche)
**   dir    = (0, -1)                     (nord)
**   plane  = (0.66, 0)
**   rayDir = (0+0.66×-1, -1+0×-1) = (-0.66, -1) → NordOuest ✓
*/
static void init_ray(t_game *game, int col, t_ray *ray)
{
    t_player    *p;

    p = &game->player;
    memset(ray, 0, sizeof(t_ray));

    /* Coordonnée sur le plan caméra : [-1.0, +1.0] */
    ray->cam = 2.0 * col / (double)SCREEN_W - 1.0;

    /* Direction du rayon = vecteur direction + fraction du plan caméra */
    ray->dir_x = p->dir_x + p->plane_x * ray->cam;
    ray->dir_y = p->dir_y + p->plane_y * ray->cam;

    /* Cellule de départ = cellule contenant le joueur */
    ray->map_x = (int)p->pos_x;
    ray->map_y = (int)p->pos_y;
}

/* ─────────────────────────────────────────────────────────────────────────────
**
** ÉTAPE 2 : init_dda()
** ─────────────────────
**
** Prépare les paramètres de la boucle DDA :
**   - delta_dist : longueur d'un "pas" en X ou Y
**   - step       : sens du déplacement (+1 ou -1) par axe
**   - side_dist  : distance jusqu'à la PREMIÈRE frontière en X et Y
**
** ┌─────────────────────────────────────────────────────────────────┐
** │  DELTA DISTANCES                                                 │
** │                                                                  │
** │  Considérons le triangle rectangle formé par le rayon :          │
** │  Pour traverser 1 cellule en X, l'hypoténuse vaut :             │
** │    sqrt(1² + (dir_y/dir_x)²) = sqrt(1 + dir_y²/dir_x²)         │
** │  Simplification (on s'intéresse au RATIO, pas à la valeur abs.) │
** │    delta_x = |1 / dir_x|                                        │
** │    delta_y = |1 / dir_y|                                        │
** │  Si dir_x=0 → rayon vertical → ne croise jamais une X-side      │
** │    → delta_x = 1e30 (pratiquement infini)                       │
** └─────────────────────────────────────────────────────────────────┘
**
** ┌─────────────────────────────────────────────────────────────────┐
** │  DISTANCES INITIALES (side_dist initial)                        │
** │                                                                  │
** │  Depuis la position du joueur (pos_x=7.5, map_x=7 p.ex.) :     │
** │                                                                  │
** │  Si dir_x > 0 (rayon vers la droite) :                          │
** │    La première X-side est à x = map_x + 1.0                     │
** │    Distance = (map_x + 1.0 - pos_x) × delta_x                  │
** │             = (7 + 1 - 7.5) × delta_x = 0.5 × delta_x          │
** │                                                                  │
** │  Si dir_x < 0 (rayon vers la gauche) :                          │
** │    La première X-side est à x = map_x (côté gauche)             │
** │    Distance = (pos_x - map_x) × delta_x                         │
** │             = (7.5 - 7) × delta_x = 0.5 × delta_x              │
** │  (même résultat mais symétrique)                                 │
** │                                                                  │
** │  Même logique pour la direction Y.                               │
** └─────────────────────────────────────────────────────────────────┘
*/
static void init_dda(t_game *game, t_ray *ray)
{
    t_player    *p;

    p = &game->player;

    /* ── Delta distances (constantes pour ce rayon) ── */
    ray->delta_x = (ray->dir_x == 0.0) ? 1e30 : fabs(1.0 / ray->dir_x);
    ray->delta_y = (ray->dir_y == 0.0) ? 1e30 : fabs(1.0 / ray->dir_y);

    /* ── Axe X : sens et distance initiale ── */
    if (ray->dir_x < 0.0)
    {
        ray->step_x = -1;                                    /* va vers la gauche (Ouest) */
        ray->side_x = (p->pos_x - ray->map_x) * ray->delta_x; /* dist jusqu'à x=map_x */
    }
    else
    {
        ray->step_x = 1;                                     /* va vers la droite (Est) */
        ray->side_x = (ray->map_x + 1.0 - p->pos_x) * ray->delta_x; /* dist jusqu'à x=map_x+1 */
    }

    /* ── Axe Y : sens et distance initiale ── */
    if (ray->dir_y < 0.0)
    {
        ray->step_y = -1;                                    /* va vers le haut (Nord) */
        ray->side_y = (p->pos_y - ray->map_y) * ray->delta_y; /* dist jusqu'à y=map_y */
    }
    else
    {
        ray->step_y = 1;                                     /* va vers le bas (Sud) */
        ray->side_y = (ray->map_y + 1.0 - p->pos_y) * ray->delta_y; /* dist jusqu'à y=map_y+1 */
    }
    (void)game;
}

/* ─────────────────────────────────────────────────────────────────────────────
**
** ÉTAPE 3 : do_dda()
** ────────────────────
**
** LA BOUCLE DDA — l'algorithme central.
**
** ┌─────────────────────────────────────────────────────────────────┐
** │  PRINCIPE FONDAMENTAL                                           │
** │                                                                  │
** │  À chaque itération, le rayon avance vers la frontière de grille │
** │  la plus PROCHE — soit en X, soit en Y.                         │
** │                                                                  │
** │  Comparer side_x et side_y pour décider :                        │
** │    side_x < side_y → la prochaine frontière est une X-side      │
** │      → sauter la frontière verticale suivante                   │
** │      → side_x += delta_x   (préparer le saut suivant)          │
** │      → map_x += step_x     (entrer dans la cellule suivante)    │
** │      → side = 0            (se souvenir que c'est une X-side)   │
** │    sinon → la prochaine frontière est une Y-side                 │
** │      → traitement symétrique                                    │
** │                                                                  │
** │  Après le saut, vérifier si la nouvelle cellule est un mur.      │
** │  Si oui → STOP (on a trouvé le mur !)                           │
** │  Si non → continuer                                              │
** └─────────────────────────────────────────────────────────────────┘
**
** ┌─────────────────────────────────────────────────────────────────┐
** │  POURQUOI LE DDA NE RATE JAMAIS UN MUR ?                        │
** │                                                                  │
** │  Contrairement à un rayon à pas constant (qui peut "passer à    │
** │  travers" un mur fin si le pas est trop grand), le DDA vérifie  │
** │  EXACTEMENT chaque frontière de grille dans l'ordre croissant   │
** │  de distance. Si un mur existe, sa frontière sera forcément      │
** │  vérifiée avant toute frontière plus lointaine.                  │
** └─────────────────────────────────────────────────────────────────┘
**
** VARIABLE 'side' :
**   0 = une X-side a été franchie en dernier → mur vertical (Est/Ouest)
**   1 = une Y-side a été franchie en dernier → mur horizontal (Nord/Sud)
*/
static void do_dda(t_game *game, t_ray *ray)
{
    ray->hit = 0;
    while (ray->hit == 0)
    {
        /* Avancer vers la frontière la plus proche */
        if (ray->side_x < ray->side_y)
        {
            ray->side_x += ray->delta_x;   /* next X-side après celle-ci    */
            ray->map_x  += ray->step_x;    /* entrer dans la cellule Est/Ouest */
            ray->side = 0;                 /* mémoriser : X-side franchie    */
        }
        else
        {
            ray->side_y += ray->delta_y;   /* next Y-side après celle-ci    */
            ray->map_y  += ray->step_y;    /* entrer dans la cellule Nord/Sud */
            ray->side = 1;                 /* mémoriser : Y-side franchie    */
        }
        /* Vérifier si on vient d'entrer dans un mur */
        if (map_safe(&game->config, ray->map_x, ray->map_y) == '1')
            ray->hit = 1;
    }
}

/* ─────────────────────────────────────────────────────────────────────────────
**
** ÉTAPE 4 : calc_wall()
** ─────────────────────
**
** Après le DDA, calculer tout ce qu'il faut pour dessiner le mur.
**
** ── 4a. Distance perpendiculaire (anti-fisheye) ──
**
** ┌─────────────────────────────────────────────────────────────────┐
** │  POURQUOI PAS LA DISTANCE EUCLIDIENNE ?                         │
** │                                                                  │
** │  La distance euclidienne = distance réelle entre joueur et mur.  │
** │  Si on l'utilisait, les rayons aux bords de l'écran (plus        │
** │  obliques) donneraient une distance plus grande que le rayon     │
** │  central, même si le mur est à la même distance réelle.         │
** │  Résultat : les murs semblent "courbés" vers les bords → fisheye │
** │                                                                  │
** │  Solution : distance PERPENDICULAIRE au plan caméra.             │
** │                                                                  │
** │  Formule (suite directe du DDA, sans calcul supplémentaire) :   │
** │    Si la dernière frontière franchie était une X-side :          │
** │      side_x a été incrémenté APRÈS avoir franchi la frontière.  │
** │      side_x - delta_x = distance exacte à cette X-side.         │
** │      wall_dist = side_x - delta_x                               │
** │    De même pour Y-side : wall_dist = side_y - delta_y           │
** │                                                                  │
** │  Remarque : c'est la distance au PLAN CAMÉRA (projection de la  │
** │  distance sur l'axe du regard), pas au JOUEUR. C'est ça qui     │
** │  évite le fisheye.                                               │
** └─────────────────────────────────────────────────────────────────┘
**
** ── 4b. Hauteur du mur ──
**
**   line_h = SCREEN_H / wall_dist
**
**   Loi de perspective : un objet 2× plus loin paraît 2× moins grand.
**   wall_dist = 1.0 → mur tout proche → hauteur = SCREEN_H (plein écran)
**   wall_dist = 2.0 → 2 cellules → hauteur = SCREEN_H/2
**
** ── 4c. Bornes de dessin ──
**
**   Le mur est centré verticalement à SCREEN_H/2.
**   draw_y0 = SCREEN_H/2 - line_h/2  (bord haut du mur)
**   draw_y1 = SCREEN_H/2 + line_h/2  (bord bas du mur)
**   Clampé à [0, SCREEN_H-1] pour les murs proches (ligne_h > SCREEN_H).
**
** ── 4d. Face du mur (NORTH/SOUTH/EAST/WEST) ──
**
**   side=0 (X-side franchie) :
**     step_x > 0 → rayon allait vers l'Est → touche face WEST du mur suivant
**     Hmm... Attendons, si le rayon va à l'Est et touche un mur, la face
**     visible est la face OUEST de ce mur (celle qui fait face Ouest, vers le joueur).
**     Convention retenue (cohérente avec l'orientation lodev) :
**       step_x > 0 → EAST  (mur à l'Est du joueur)
**       step_x < 0 → WEST
**     side=1 (Y-side) :
**       step_y > 0 → SOUTH (mur au Sud, rayon va vers le bas)
**       step_y < 0 → NORTH
**
** ── 4e. Coordonnée X dans la texture ──
**
**   On trouve l'endroit EXACT où le rayon frappe le mur (de 0.0 à 1.0
**   sur la largeur de la face).
**
**   Pour une X-side (mur vertical) :
**     Le rayon a parcouru wall_dist dans la direction du rayon.
**     Coordonnée Y du point d'impact = pos_y + wall_dist × dir_y
**     La partie fractionnaire donne la position [0,1) sur la face.
**
**   Pour une Y-side (mur horizontal) :
**     Coordonnée X du point d'impact = pos_x + wall_dist × dir_x
**
**   Correction miroir :
**     Selon le sens du rayon, l'image peut être miroir-ée.
**     On corrige pour que les textures s'affichent dans le bon sens.
*/
static void calc_wall(t_game *game, t_ray *ray)
{
    double  wall_hit;
    t_player *p;

    p = &game->player;

    /* ── 4a. Distance perpendiculaire (évite le fisheye) ── */
    if (ray->side == 0)
        ray->wall_dist = ray->side_x - ray->delta_x;
    else
        ray->wall_dist = ray->side_y - ray->delta_y;

    /* Sécurité : wall_dist ne doit jamais être 0 (division par zéro) */
    if (ray->wall_dist < 0.001)
        ray->wall_dist = 0.001;

    /* ── 4b. Hauteur de la bande de mur en pixels ── */
    ray->line_h = (int)(SCREEN_H / ray->wall_dist);

    /* ── 4c. Bornes de dessin (clampées à l'écran) ── */
    ray->draw_y0 = SCREEN_H / 2 - ray->line_h / 2;
    if (ray->draw_y0 < 0)
        ray->draw_y0 = 0;
    ray->draw_y1 = SCREEN_H / 2 + ray->line_h / 2;
    if (ray->draw_y1 >= SCREEN_H)
        ray->draw_y1 = SCREEN_H - 1;

    /* ── 4d. Détermination de la face touchée ── */
    if (ray->side == 0)
        ray->face = (ray->step_x > 0) ? EAST : WEST;
    else
        ray->face = (ray->step_y > 0) ? SOUTH : NORTH;

    /* ── 4e. Coordonnée X dans la texture ── */
    if (ray->side == 0)
        wall_hit = p->pos_y + ray->wall_dist * ray->dir_y;
    else
        wall_hit = p->pos_x + ray->wall_dist * ray->dir_x;
    wall_hit -= floor(wall_hit);            /* partie fractionnaire [0, 1) */
    ray->tex_x = (int)(wall_hit * TEX_W);

    /* Correction d'orientation (évite l'image miroir sur certaines faces) */
    if (ray->side == 0 && ray->dir_x > 0.0)
        ray->tex_x = TEX_W - ray->tex_x - 1;
    if (ray->side == 1 && ray->dir_y < 0.0)
        ray->tex_x = TEX_W - ray->tex_x - 1;

    /* Borner tex_x par sécurité */
    if (ray->tex_x < 0)
        ray->tex_x = 0;
    if (ray->tex_x >= TEX_W)
        ray->tex_x = TEX_W - 1;
}

/* ─────────────────────────────────────────────────────────────────────────────
**
** cast_ray() : PIPELINE COMPLET pour une colonne d'écran.
** ─────────────────────────────────────────────────────────
**
** Appelé pour chaque colonne col ∈ [0, SCREEN_W).
** Remplit intégralement la structure t_ray que render.c utilisera.
**
** Ordre d'appel OBLIGATOIRE (chaque étape utilise les résultats de la précédente) :
**   init_ray  → init_dda  → do_dda  → calc_wall
*/
void    cast_ray(t_game *game, int col, t_ray *ray)
{
    init_ray(game, col, ray);   /* ① direction du rayon (cam, dir_x/y, map)  */
    init_dda(game, ray);        /* ② paramètres DDA (delta, step, side_init)  */
    do_dda(game, ray);          /* ③ boucle DDA → trouve le mur              */
    calc_wall(game, ray);       /* ④ hauteur, bornes, face, tex_x             */
}