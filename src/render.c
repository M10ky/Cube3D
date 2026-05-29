/* ************************************************************************** */
/*                                                                            */
/*   render.c                                      cub3D / 42                */
/*                                                                            */
/*   Dessin de chaque frame dans le framebuffer.                              */
/*                                                                            */
/*   Pipeline par frame :                                                     */
/*   game_loop() → render_frame() → pour chaque colonne :                     */
/*     draw_ceiling_strip() + draw_wall_strip() + draw_floor_strip()          */
/*   puis : mlx_put_image_to_window()                                         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

/*
** sample_texture : lit la couleur d'un pixel dans une texture.
** ─────────────────────────────────────────────────────────────
**
** Accès mémoire à la texture MLX :
**   La texture est stockée en mémoire linéaire (comme le framebuffer).
**   Formule d'adressage : addr + row × line_len + col × (bpp/8)
**
** Paramètres :
**   tex  : pointeur vers la texture (chargée ou procédurale)
**   tx   : coordonnée X dans la texture [0, TEX_W)
**   ty   : coordonnée Y dans la texture [0, TEX_H)
**
** Retourne 0xFF00FF (magenta) si la texture est invalide — signal visuel
** immédiat d'un bug de chargement.
**
** NOTE : on utilise unsigned int pour éviter la sign extension lors
** des opérations de couleur (assombrissement avec >> et &).
*/
static unsigned int sample_texture(t_texture *tex, int tx, int ty)
{
    char *pixel;

    if (!tex || !tex->addr)
        return (0x00FF00FF);  /* magenta = texture manquante               */
    pixel = tex->addr + ty * tex->line_len + tx * (tex->bpp / 8);
    return (*(unsigned int *)pixel);
}

/*
** draw_ceiling_strip : dessine la bande de plafond d'une colonne.
** ─────────────────────────────────────────────────────────────────
**
** Le plafond occupe les pixels de y=0 à y=draw_y0-1.
** C'est une couleur unie (pas de texture dans la partie obligatoire).
**
** Optimisation possible (non implémentée ici) : floor casting —
** calculer la texture du sol/plafond pixel par pixel en utilisant
** l'inverse de la projection. Plus complexe, c'est un bonus.
*/
static void draw_ceiling_strip(t_game *game, int col, int end_y)
{
    int y;

    y = 0;
    while (y < end_y)
    {
        put_pixel(&game->buf, col, y, (unsigned int)game->config.ceil_color);
        y++;
    }
}

/*
** draw_floor_strip : dessine la bande de sol d'une colonne.
** ──────────────────────────────────────────────────────────
**
** Le sol occupe les pixels de draw_y1+1 à SCREEN_H-1.
** Couleur unie (partie obligatoire du sujet).
*/
static void draw_floor_strip(t_game *game, int col, int start_y)
{
    int y;

    y = start_y;
    while (y < SCREEN_H)
    {
        put_pixel(&game->buf, col, y, (unsigned int)game->config.floor_color);
        y++;
    }
}

/*
** draw_wall_strip : dessine la bande de mur texturée d'une colonne.
** ──────────────────────────────────────────────────────────────────
**
** C'est la partie la plus complexe du rendu.
**
** ┌───────────────────────────────────────────────────────────────────┐
** │  MAPPAGE DE TEXTURE (texture mapping)                             │
** │                                                                    │
** │  On doit mapper les TEX_H lignes de la texture sur line_h pixels  │
** │  de l'écran. Si line_h > TEX_H (mur proche), la texture est       │
** │  agrandie. Si line_h < TEX_H (mur loin), elle est compressée.    │
** │                                                                    │
** │  Paramètre tex_step :                                             │
** │    tex_step = TEX_H / line_h                                      │
** │    = combien de lignes de texture par pixel d'écran               │
** │    Exemples :                                                      │
** │      line_h = 512 (mur à 1 unité)  → tex_step = 64/512 = 0.125   │
** │        → 1 ligne de texture couvre 8 pixels écran (agrandissement)│
** │      line_h = 32  (mur à 16 unités) → tex_step = 64/32 = 2.0     │
** │        → 2 lignes de texture par pixel écran (compression)        │
** │                                                                    │
** │  Position initiale tex_pos :                                       │
** │    Le mur commence IDÉALEMENT à y = SCREEN_H/2 - line_h/2.       │
** │    Si ce point est au-dessus de l'écran (y < 0), le mur est       │
** │    clampé à draw_y0 = 0.                                          │
** │    Mais on a "raté" (draw_y0 - idéal) pixels en haut de la       │
** │    texture ! Il faut compenser en commençant tex_pos plus bas :   │
** │                                                                    │
** │    tex_pos = (draw_y0 - SCREEN_H/2 + line_h/2) × tex_step        │
** │                                                                    │
** │    Si draw_y0 = SCREEN_H/2 - line_h/2 (pas de clamping) :        │
** │      tex_pos = 0 → on commence au sommet de la texture ✓          │
** │    Si draw_y0 = 0 (mur très proche, clampé) :                    │
** │      tex_pos > 0 → on commence plus bas dans la texture ✓         │
** └───────────────────────────────────────────────────────────────────┘
**
** ── ASSOMBRISSEMENT DES Y-SIDES ──
**
**   Les faces touchées via une Y-side (Nord/Sud, murs horizontaux)
**   sont rendues plus sombres que les X-sides (Est/Ouest).
**   Effet : donne l'illusion d'un éclairage latéral — les murs face
**   à la lumière (Est/Ouest) paraissent plus clairs.
**
**   Technique : division par 2 de chaque composante couleur.
**   (color >> 1) décale tous les bits de 1 → divise par 2.
**   & 0x7F7F7F masque le bit de carry entre les composantes :
**     Sans mask : si R=0x80 (1000 0000), >> 1 = 0x40 (0100 0000) ✓
**     Mais si R=0xFF (1111 1111) et G=0x00 dans un int 32-bit,
**     le shift pourrait faire "couler" des bits de R dans G sans mask.
**   → 0x7F7F7F = 0111 1111 0111 1111 0111 1111 : isole chaque canal.
**
** ── BOUCLE PIXEL PAR PIXEL ──
**
**   Pour chaque pixel y de draw_y0 à draw_y1 :
**     1. tex_y = (int)tex_pos & (TEX_H - 1)
**        Le AND avec (64-1=63) est un modulo rapide (fonctionne car
**        TEX_H est une puissance de 2). Garde tex_y ∈ [0, 63].
**     2. Lire la couleur : sample_texture(tex, tex_x, tex_y)
**     3. Assombrir si Y-side
**     4. Écrire le pixel dans le framebuffer
**     5. tex_pos += tex_step  (avancer dans la texture)
*/
static void draw_wall_strip(t_game *game, int col, t_ray *ray)
{
    t_texture   *tex;
    double      tex_step;   /* lignes de texture par pixel d'écran           */
    double      tex_pos;    /* position courante dans la texture (flottant)   */
    unsigned int color;
    int         tex_y;      /* ligne courante dans la texture [0, TEX_H)     */
    int         y;

    tex = &game->tex[ray->face];

    /* Nombre de lignes de texture à sauter par pixel d'écran */
    tex_step = (double)TEX_H / (double)ray->line_h;

    /*
    ** Position de départ dans la texture.
    ** Compense le clamping si le mur est plus grand que l'écran.
    ** (draw_y0 - SCREEN_H/2 + line_h/2) = pixels clampés en haut
    */
    tex_pos = ((double)ray->draw_y0 - (double)SCREEN_H / 2.0
                + (double)ray->line_h / 2.0) * tex_step;

    y = ray->draw_y0;
    while (y < ray->draw_y1)
    {
        /* Coordonnée Y dans la texture, bornée via AND (modulo puissance 2) */
        tex_y = (int)tex_pos & (TEX_H - 1);
        tex_pos += tex_step;

        /* Lire la couleur dans la texture */
        color = sample_texture(tex, ray->tex_x, tex_y);

        /* Assombrir les Y-sides (Nord/Sud) pour simuler un éclairage       */
        if (ray->side == 1)
            color = (color >> 1) & 0x7F7F7F;

        put_pixel(&game->buf, col, y, color);
        y++;
    }
}

/*
** draw_column : dessine UNE colonne complète de l'écran.
** ───────────────────────────────────────────────────────
**
** Une colonne = 3 zones verticales :
**
**   y=0          ┐
**                │  Plafond (ceil_color)
**   y=draw_y0   ┘
**   y=draw_y0   ┐
**                │  Mur texturé (texture NORTH/SOUTH/EAST/WEST)
**   y=draw_y1   ┘
**   y=draw_y1+1 ┐
**                │  Sol (floor_color)
**   y=SCREEN_H-1┘
**
** L'appel est ordonné pour assurer la cohérence (pas de pixel manquant
** entre les zones, pas de recouvrement).
*/
static void draw_column(t_game *game, int col, t_ray *ray)
{
    draw_ceiling_strip(game, col, ray->draw_y0);       /* 0 → draw_y0-1     */
    draw_wall_strip(game, col, ray);                   /* draw_y0 → draw_y1  */
    draw_floor_strip(game, col, ray->draw_y1 + 1);    /* draw_y1+1 → fin    */
}

/*
** render_frame : rend une frame complète dans le framebuffer.
** ─────────────────────────────────────────────────────────────
**
** Appelée par game_loop() à chaque frame.
**
** Pour chaque colonne :
**   1. cast_ray() → remplit t_ray avec toutes les infos DDA
**   2. draw_column() → dessine les 3 zones (plafond, mur, sol)
**
** Après la boucle, envoyer le framebuffer à la fenêtre en UNE opération.
** mlx_put_image_to_window : envoie l'image complète au serveur X11/Cocoa.
**   - Position (0, 0) = coin supérieur gauche de la fenêtre
**   - Beaucoup plus rapide que SCREEN_W × SCREEN_H appels à mlx_pixel_put
*/
void    render_frame(t_game *game)
{
    t_ray   ray;
    int     col;

    col = 0;
    while (col < SCREEN_W)
    {
        cast_ray(game, col, &ray);
        draw_column(game, col, &ray);
        col++;
    }
    /* Envoyer le framebuffer complet à la fenêtre en une seule fois */
    mlx_put_image_to_window(game->mlx, game->win, game->buf.img, 0, 0);
}

/*
** game_loop : hook appelé par mlx_loop() à chaque itération.
** ────────────────────────────────────────────────────────────
**
** Signature imposée par MLX : int func(void *param), retourne toujours 0.
**
** Ordre d'appel :
**   1. handle_input() → lire les touches enfoncées, mettre à jour pos/dir
**   2. render_frame() → dessiner la scène avec la nouvelle position
**
** L'ordre est important : si on rendait d'abord, la position serait
** mise à jour APRÈS l'affichage → décalage visuel d'une frame.
*/
int     game_loop(void *param)
{
    t_game  *game;

    game = (t_game *)param;
    handle_input(game);
    render_frame(game);
    return (0);
}