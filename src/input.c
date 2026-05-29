/* ************************************************************************** */
/*                                                                            */
/*   input.c                                       cub3D / 42                */
/*                                                                            */
/*   Gestion du clavier et mise à jour de l'état du joueur.                  */
/*                                                                            */
/*   Architecture événementielle de MLX :                                     */
/*     key_press()   → appelé par X11 dès qu'une touche EST enfoncée         */
/*     key_release() → appelé par X11 dès qu'une touche EST relâchée         */
/*     handle_input()→ appelé chaque frame pour lire les touches tenues       */
/*                                                                            */
/*   Pourquoi séparer press/release de handle_input ?                        */
/*   → key_press ne se déclenche qu'UNE FOIS par appui.                      */
/*     Si on déplaçait le joueur dans key_press, il avancerait par saccades  */
/*     (délai de répétition du système). En maintenant un état booléen et   */
/*     en lisant cet état CHAQUE FRAME, le mouvement est fluide.             */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"


/*
** key_press : appelé par X11 quand une touche est enfoncée.
** ──────────────────────────────────────────────────────────
**
** MLX enregistre ce hook avec :
**   mlx_hook(win, EVT_KEY_PRESS, 1L<<0, key_press, game)
**
** keycode : valeur X11 (KeySym) de la touche. Voir defines dans cub3d.h.
**
** ESC → sortie immédiate propre (pas d'attente de key_release).
** Autres → active le flag booléen correspondant dans game->keys.
**
** NOTE : La signature (int, void*) est imposée par MLX.
*/
int     key_press(int keycode, void *param)
{
    t_game  *game;

    game = (t_game *)param;
    if (keycode == KEY_ESC)
        cleanup_exit(game);
    else if (keycode == KEY_Z)
        game->keys.w = 1;
    else if (keycode == KEY_S)
        game->keys.s = 1;
    else if (keycode == KEY_Q)
        game->keys.a = 1;
    else if (keycode == KEY_D)
        game->keys.d = 1;
    else if (keycode == KEY_LEFT)
        game->keys.left = 1;
    else if (keycode == KEY_RIGHT)
        game->keys.right = 1;
    return (0);
}

/*
** key_release : appelé par X11 quand une touche est relâchée.
** ─────────────────────────────────────────────────────────────
**
** Remet le flag à 0 → le mouvement s'arrête immédiatement.
** Sans ce hook, le joueur continuerait à avancer après avoir relâché W.
*/
int     key_release(int keycode, void *param)
{
    t_game  *game;

    game = (t_game *)param;
    if (keycode == KEY_Z)
        game->keys.w = 0;
    else if (keycode == KEY_S)
        game->keys.s = 0;
    else if (keycode == KEY_Q)
        game->keys.a = 0;
    else if (keycode == KEY_D)
        game->keys.d = 0;
    else if (keycode == KEY_LEFT)
        game->keys.left = 0;
    else if (keycode == KEY_RIGHT)
        game->keys.right = 0;
    return (0);
}

/*
** handle_close : appelé quand l'utilisateur clique la croix rouge.
** ─────────────────────────────────────────────────────────────────
**
** MLX enregistre ce hook avec :
**   mlx_hook(win, EVT_CLOSE, 0, handle_close, game)
**
** L'événement 17 (ClientMessage) est envoyé par le gestionnaire de
** fenêtre (WM) quand l'utilisateur ferme la fenêtre.
** Sans ce hook, cliquer sur la croix planterait le programme.
*/
int     handle_close(void *param)
{
    t_game  *game;

    game = (t_game *)param;
    cleanup_exit(game);
    return (0);
}

/*
** rotate_player : applique une rotation d'angle θ au joueur.
** ────────────────────────────────────────────────────────────
**
** ┌───────────────────────────────────────────────────────────────┐
** │  MATRICE DE ROTATION 2D                                       │
** │                                                               │
** │  [ cos(θ)  -sin(θ) ] [ dir_x ]   [ dir_x' ]                 │
** │  [ sin(θ)   cos(θ) ] [ dir_y ] = [ dir_y' ]                 │
** │                                                               │
** │  Développé :                                                  │
** │    new_dir_x = dir_x * cos(θ) - dir_y * sin(θ)              │
** │    new_dir_y = dir_x * sin(θ) + dir_y * cos(θ)              │
** │                                                               │
** │  La MÊME matrice est appliquée au plan caméra pour maintenir  │
** │  la perpendiculaire entre dir et plane.                       │
** │                                                               │
** │  CONVENTION DE SIGNE (système Y vers le bas) :               │
** │    θ > 0 (positif) → rotation HORAIRE en coords écran        │
** │              → le joueur tourne à DROITE visuellement         │
** │    θ < 0 (négatif) → rotation ANTI-HORAIRE                   │
** │              → le joueur tourne à GAUCHE visuellement         │
** │                                                               │
** │  Vérification : joueur face Est (1,0), θ=+ROT_SPD small :    │
** │    new_dir_y = sin(small+) ≈ +small > 0 → face légèrement Sud│
** │    Est → légèrement Sud = tourner à DROITE ✓                  │
** └───────────────────────────────────────────────────────────────┘
**
** Astuce implémentation : stocker old_dir_x avant de le modifier
** car il est utilisé dans le calcul de new_dir_y.
** Sans cette précaution, new_dir_y utiliserait new_dir_x (déjà modifié).
*/
static void rotate_player(t_player *p, double angle)
{
    double  old_dir_x;
    double  old_plane_x;

    /* ── Rotation du vecteur direction ── */
    old_dir_x = p->dir_x;
    p->dir_x = p->dir_x * cos(angle) - p->dir_y * sin(angle);
    p->dir_y = old_dir_x * sin(angle) + p->dir_y * cos(angle);

    /* ── Rotation du plan caméra (même angle, garde ⊥ avec dir) ── */
    old_plane_x = p->plane_x;
    p->plane_x = p->plane_x * cos(angle) - p->plane_y * sin(angle);
    p->plane_y = old_plane_x * sin(angle) + p->plane_y * cos(angle);
}

/*
** is_wall : vérifie si une position (x, y) est dans un mur.
** ──────────────────────────────────────────────────────────
**
** Utilisé pour la détection de collision lors du déplacement.
** La position flottante est convertie en indices entiers de cellule.
** Retourne 1 si la cellule est un mur OU hors de la map.
**
** IMPORTANT : l'espace entre les guillemets compte aussi comme mur
** (lignes de longueurs différentes dans une map irrégulière).
*/
static int  is_wall(t_config *cfg, double x, double y)
{
    int mx;
    int my;

    mx = (int)x;
    my = (int)y;
    if (my < 0 || my >= cfg->map_h || mx < 0 || mx >= cfg->map_w)
        return (1);
    if (mx >= (int)strlen(cfg->map[my]))
        return (1);
    return (cfg->map[my][mx] == '1');
}

/*
** try_move : tente de déplacer le joueur de (delta_x, delta_y).
** ──────────────────────────────────────────────────────────────
**
** DÉTECTION DE COLLISION GLISSANTE :
**
** Au lieu de vérifier la position totale (x+dx, y+dy) d'un coup,
** on vérifie X et Y SÉPARÉMENT. Si X est bloqué mais pas Y, le
** joueur peut "glisser" le long du mur.
**
**   if (new_x, pos_y) est libre → mettre à jour pos_x
**   if (pos_x, new_y) est libre → mettre à jour pos_y
**
** Avantage : mouvement naturel contre les murs (wall sliding).
** Sans ça : le joueur s'arrête complètement dès qu'un mur est touché.
**
** Pas de rayon de collision (PLAYER_RADIUS) pour rester simple.
** Bonus : ajouter un offset de 0.2 ou 0.3 unité pour rester
** éloigné des coins de murs.
*/
static void try_move(t_game *game, double dx, double dy)
{
    double  new_x;
    double  new_y;

    new_x = game->player.pos_x + dx;
    new_y = game->player.pos_y + dy;

    /* Vérifier X indépendamment (permet le glissement le long des murs Y) */
    if (!is_wall(&game->config, new_x, game->player.pos_y))
        game->player.pos_x = new_x;

    /* Vérifier Y indépendamment (permet le glissement le long des murs X) */
    if (!is_wall(&game->config, game->player.pos_x, new_y))
        game->player.pos_y = new_y;
}

/*
** handle_input : traite l'état des touches à chaque frame.
** ──────────────────────────────────────────────────────────
**
** Appelée par game_loop() avant render_frame().
**
** ── ROTATION ──
**   LEFT  → angle négatif → tourne à gauche
**   RIGHT → angle positif → tourne à droite
**
** ── DÉPLACEMENT AVANT/ARRIÈRE ──
**   W → delta = (+dir_x, +dir_y) × MOV_SPD
**   S → delta = (-dir_x, -dir_y) × MOV_SPD
**
** ── STRAFE (déplacement latéral) ──
**   Le vecteur de strafe est PERPENDICULAIRE à dir.
**   Perpendiculaire à (dir_x, dir_y) = (-dir_y, dir_x) pour gauche
**                                     = (+dir_y, -dir_x) pour droite
**
**   Vérification avec dir = Est (1, 0) :
**     Strafe gauche : (-0, 1) = (0, 1) → vers le Sud... hmm
**
**   Attendez, recalculons avec Y vers le bas :
**     dir = Est (1, 0). Tourner à gauche = aller vers le Nord (y décroît).
**     Strafe gauche = direction perpendiculaire vers la gauche du joueur.
**     Si le joueur fait face à l'Est, sa gauche est le Nord (y décroît).
**     Strafe gauche = (dy, -dx) = (0, -1) → Nord ✓
**
**   Donc :
**     Strafe gauche  (A) : delta = (+dir_y, -dir_x) × MOV_SPD
**     Strafe droite  (D) : delta = (-dir_y, +dir_x) × MOV_SPD
*/
void    handle_input(t_game *game)
{
    t_player    *p;

    p = &game->player;

    /* ── Rotation ── */
    if (game->keys.left)
        rotate_player(p, -ROT_SPD);   /* θ négatif → tourne à gauche        */
    if (game->keys.right)
        rotate_player(p, ROT_SPD);    /* θ positif → tourne à droite         */

    /* ── Avancer / reculer ── */
    if (game->keys.w)
        try_move(game, p->dir_x * MOV_SPD, p->dir_y * MOV_SPD);
    if (game->keys.s)
        try_move(game, -p->dir_x * MOV_SPD, -p->dir_y * MOV_SPD);

    /* ── Strafe ── */
    if (game->keys.a)
        try_move(game, p->dir_y * MOV_SPD, -p->dir_x * MOV_SPD);
    if (game->keys.d)
        try_move(game, -p->dir_y * MOV_SPD, p->dir_x * MOV_SPD);
}