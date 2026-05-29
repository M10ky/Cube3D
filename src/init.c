/* ************************************************************************** */
/*                                                                            */
/*   init.c                                        cub3D / 42                */
/*                                                                            */
/*   Initialisation de toutes les ressources graphiques.                      */
/*   Ordre : MLX → fenêtre → framebuffer → textures → joueur.               */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/cube3d.h"


/*
** put_pixel : écrit un pixel dans le framebuffer.
** ─────────────────────────────────────────────────
**
** THÉORIE DU FRAMEBUFFER :
** L'image MLX est un tableau 1D de pixels en mémoire, organisé en
** rangées contiguës (scanlines). La formule d'adressage est :
**
**   adresse = base + y × line_len + x × (bpp / 8)
**
**   - base     : adresse de début de l'image (cfg->addr)
**   - y        : rangée (0 = haut, SCREEN_H-1 = bas)
**   - line_len : taille d'une rangée EN OCTETS (≥ SCREEN_W × bpp/8)
**                peut avoir du padding, donc utiliser line_len et pas SCREEN_W
**   - bpp/8    : octets par pixel (bpp=32 → 4 octets/pixel)
**
** On caste le pointeur char* en unsigned int* pour écrire 4 octets
** d'un coup (l'entier 32-bit représentant la couleur ARGB ou BGRA).
**
** IMPORTANT : la vérification des bornes est OBLIGATOIRE car les calculs
** de draw_y0 et draw_y1 peuvent déborder de ±1 à cause des arrondis
** entiers. Un seul pixel hors limites = crash ou corruption mémoire.
*/
void    put_pixel(t_img *buf, int x, int y, unsigned int color)
{
    char    *dst;

    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H)
        return ;
    dst = buf->addr + y * buf->line_len + x * (buf->bpp / 8);
    *(unsigned int *)dst = color;
}

/*
** init_mlx : ouvre la connexion MLX et crée la fenêtre.
** ──────────────────────────────────────────────────────
**
** mlx_init() :
**   Linux : ouvre une connexion X11 et retourne un pointeur opaque.
**   macOS : crée une connexion Cocoa. Retourne NULL en cas d'échec.
**
** mlx_new_window() :
**   Crée une fenêtre native du système avec les dimensions données.
**   L'affichage n'est PAS immédiat — il faut envoyer des images avec
**   mlx_put_image_to_window() ou dessiner avec mlx_pixel_put().
*/
static int  init_mlx(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
        return (print_error("mlx_init() failed"));
    game->win = mlx_new_window(game->mlx, SCREEN_W, SCREEN_H, WIN_TITLE);
    if (!game->win)
        return (print_error("mlx_new_window() failed"));
    return (0);
}

/*
** init_framebuffer : crée l'image tampon pour le rendu.
** ───────────────────────────────────────────────────────
**
** STRATÉGIE DOUBLE-BUFFER :
**
**   Naïf (à éviter) : dessiner pixel par pixel avec mlx_pixel_put()
**     → chaque appel envoie un paquet X11 → très lent pour 1024×512 pixels
**
**   Correct (notre approche) :
**     1. Créer une image MLX en RAM (mlx_new_image)
**     2. Accéder directement aux pixels (mlx_get_data_addr → pointeur RAM)
**     3. Écrire TOUS les pixels en RAM (rapide, cache-friendly)
**     4. Envoyer l'image complète une seule fois (mlx_put_image_to_window)
**     → 1 seule communication GPU par frame = fluidité maximale
**
** mlx_get_data_addr remplit :
**   bpp      : bits par pixel (32 sur la plupart des systèmes)
**   line_len : octets par ligne (peut inclure du padding)
**   endian   : ordre des octets (0=little, 1=big)
*/
static int  init_framebuffer(t_game *game)
{
    game->buf.img = mlx_new_image(game->mlx, SCREEN_W, SCREEN_H);
    if (!game->buf.img)
        return (print_error("mlx_new_image (framebuffer) failed"));
    game->buf.addr = mlx_get_data_addr(game->buf.img, &game->buf.bpp,
                        &game->buf.line_len, &game->buf.endian);
    if (!game->buf.addr)
        return (print_error("mlx_get_data_addr (framebuffer) failed"));
    return (0);
}

/*
** get_brick_pixel : génère un pixel de texture "brique" procédurale.
** ───────────────────────────────────────────────────────────────────
**
** Crée un motif réaliste sans fichier externe :
**
**   Structure d'une brique (TEX_H=64, 4 rangées de briques) :
**     - Hauteur par rangée : 16 pixels
**     - Largeur par brique : 32 pixels
**     - Décalage alterné  : les rangées impaires sont décalées de 16px
**       (comme de vraies briques)
**
**   Couleurs :
**     - Mortier (jointure) : gris 0x888888
**     - Brique : couleur de base (r,g,b) + variation aléatoire légère
**
**   La variation (XOR de x et y) donne un aspect "granuleux" naturel
**   sans calculs coûteux.
**
**  Paramètres r,g,b : teinte de base de la face (ex: bleu pour NORTH)
*/
static unsigned int get_brick_pixel(int x, int y, int r, int g, int b)
{
    int row;
    int brick_h;
    int brick_w;
    int offset;
    int col_pos;
    int v;

    brick_h = TEX_H / 4;        /* hauteur d'une rangée de briques : 16px   */
    brick_w = TEX_W / 2;        /* largeur d'une brique : 32px              */
    row = y / brick_h;
    offset = (row % 2) * (brick_w / 2);  /* décalage zigzag                 */
    col_pos = (x + offset) % brick_w;

    /* Lignes de mortier (horizontales et verticales) */
    if (y % brick_h == 0 || y % brick_h == 1)
        return (0x00888888);             /* mortier horizontal               */
    if (col_pos == 0 || col_pos == 1)
        return (0x00888888);             /* mortier vertical                 */

    /* Variation naturelle dans la brique : XOR crée un pattern pseudo-aléa */
    v = ((x ^ (y * 7)) & 0x1F) - 16;    /* variation ∈ [-16, +15]          */
    r = r + v / 3;
    g = g + v / 4;
    if (r < 0) r = 0; else if (r > 255) r = 255;
    if (g < 0) g = 0; else if (g > 255) g = 255;
    if (b < 0) b = 0; else if (b > 255) b = 255;
    return ((unsigned int)((r << 16) | (g << 8) | b));
}

/*
** create_proc_texture : remplit une texture MLX avec un motif brique.
** ─────────────────────────────────────────────────────────────────────
**
** Appelée quand le fichier XPM n'existe pas (mode mock).
** Chaque face reçoit une teinte distincte pour différencier N/S/E/W :
**   NORTH (0) : bleu   (r=60,  g=80,  b=180)
**   SOUTH (1) : vert   (r=50,  g=130, b=70 )
**   EAST  (2) : rouge  (r=180, g=50,  b=50 )
**   WEST  (3) : orange (r=180, g=110, b=40 )
**
** Accès aux pixels de la texture créée :
**   adresse = addr + y * line_len + x * (bpp/8)
*/
static void create_proc_texture(t_game *game, int idx, int r, int g, int b)
{
    t_texture   *tex;
    int         x;
    int         y;
    char        *pixel;

    tex = &game->tex[idx];
    tex->width = TEX_W;
    tex->height = TEX_H;
    tex->img = mlx_new_image(game->mlx, TEX_W, TEX_H);
    if (!tex->img)
        return ;
    tex->addr = mlx_get_data_addr(tex->img, &tex->bpp,
                    &tex->line_len, &tex->endian);
    if (!tex->addr)
        return ;
    y = 0;
    while (y < TEX_H)
    {
        x = 0;
        while (x < TEX_W)
        {
            pixel = tex->addr + y * tex->line_len + x * (tex->bpp / 8);
            *(unsigned int *)pixel = get_brick_pixel(x, y, r, g, b);
            x++;
        }
        y++;
    }
}

/*
** load_texture : tente de charger un XPM, sinon crée une texture proc.
** ─────────────────────────────────────────────────────────────────────
**
** mlx_xpm_file_to_image :
**   - Charge un fichier .xpm et retourne un handle MLX image.
**   - Remplit width/height par référence.
**   - Retourne NULL si le fichier n'existe pas ou est invalide.
**
** Stratégie de fallback :
**   XPM trouvé → charge et utilise la vraie texture
**   XPM absent → génère une texture procédurale colorée
**   (Le fallback garantit que le rendu fonctionne même sans textures)
*/
static void load_texture(t_game *game, int idx, int r, int g, int b)
{
    t_texture   *tex;

    tex = &game->tex[idx];
    tex->img = mlx_xpm_file_to_image(game->mlx, game->config.tex_path[idx],
                    &tex->width, &tex->height);
    if (tex->img)
    {
        /* Texture XPM chargée avec succès */
        tex->addr = mlx_get_data_addr(tex->img, &tex->bpp,
                        &tex->line_len, &tex->endian);
    }
    else
    {
        /* Fallback : texture procédurale en mock */
        create_proc_texture(game, idx, r, g, b);
    }
}

/*
** init_textures : charge les 4 textures de murs.
** ─────────────────────────────────────────────────
**
** Chaque face reçoit une couleur de base distinctive.
** Ces couleurs permettent de vérifier visuellement que la détection
** de face (NORTH/SOUTH/EAST/WEST) fonctionne correctement.
*/
static int  init_textures(t_game *game)
{
    load_texture(game, NORTH, 60,  80,  180);  /* bleu  : murs nord          */
    load_texture(game, SOUTH, 50,  130, 70);   /* vert  : murs sud           */
    load_texture(game, EAST,  180, 50,  50);   /* rouge : murs est           */
    load_texture(game, WEST,  180, 110, 40);   /* orange: murs ouest         */
    return (0);
}

/*
** init_player : copie les données spawn du config vers t_player.
** ───────────────────────────────────────────────────────────────
**
** Séparation config/player :
**   config.spawn_* = données statiques du parseur (ne changent pas)
**   player.*       = état dynamique (mis à jour chaque frame par input)
**
** À la réinitialisation du jeu (restart), on peut re-copier depuis
** config.spawn_* pour remettre le joueur à sa position initiale.
*/
void    init_player(t_game *game)
{
    t_config    *cfg;
    t_player    *p;

    cfg = &game->config;
    p = &game->player;
    p->pos_x   = cfg->spawn_x;
    p->pos_y   = cfg->spawn_y;
    p->dir_x   = cfg->spawn_dir_x;
    p->dir_y   = cfg->spawn_dir_y;
    p->plane_x = cfg->spawn_plane_x;
    p->plane_y = cfg->spawn_plane_y;
}

/*
** init_game : orchestre toute l'initialisation.
** ───────────────────────────────────────────────
**
** Ordre IMPÉRATIF (chaque étape dépend de la précédente) :
**   1. init_mock_config : rempli t_config (map, couleurs, paths tex, spawn)
**   2. init_mlx         : ouvre MLX (nécessaire pour les images)
**   3. init_framebuffer : crée l'image de rendu (nécessite mlx)
**   4. init_textures    : charge/crée les textures (nécessite mlx)
**   5. init_player      : copie spawn → player (nécessite config)
**
** Retourne 0 en succès, 1 en erreur (affiche le message et retourne).
*/
int init_game(t_game *game)
{
    if (init_mock_config(&game->config) != 0)
        return (1);
    if (init_mlx(game) != 0)
        return (1);
    if (init_framebuffer(game) != 0)
        return (1);
    if (init_textures(game) != 0)
        return (1);
    init_player(game);
    return (0);
}