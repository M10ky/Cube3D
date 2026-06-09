/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 23:35:19 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/09 06:37:07 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

void    put_pixel(t_img *buf, int x, int y, unsigned int color)
{
    char    *dst;

    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H)
        return ;
    dst = buf->addr + y * buf->line_len + x * (buf->bpp / 8);
    *(unsigned int *)dst = color;
}


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


// static unsigned int get_brick_pixel(int x, int y, int r, int g, int b)
// {
//     int row;
//     int brick_h;
//     int brick_w;
//     int offset;
//     int col_pos;
//     int v;

//     brick_h = TEX_H / 4;        /* hauteur d'une rangée de briques : 16px   */
//     brick_w = TEX_W / 2;        /* largeur d'une brique : 32px              */
//     row = y / brick_h;
//     offset = (row % 2) * (brick_w / 2);  /* décalage zigzag                 */
//     col_pos = (x + offset) % brick_w;

//     /* Lignes de mortier (horizontales et verticales) */
//     if (y % brick_h == 0 || y % brick_h == 1)
//         return (0x00888888);             /* mortier horizontal               */
//     if (col_pos == 0 || col_pos == 1)
//         return (0x00888888);             /* mortier vertical                 */

//     /* Variation naturelle dans la brique : XOR crée un pattern pseudo-aléa */
//     v = ((x ^ (y * 7)) & 0x1F) - 16;    /* variation ∈ [-16, +15]          */
//     r = r + v / 3;
//     g = g + v / 4;
//     if (r < 0) r = 0; else if (r > 255) r = 255;
//     if (g < 0) g = 0; else if (g > 255) g = 255;
//     if (b < 0) b = 0; else if (b > 255) b = 255;
//     return ((unsigned int)((r << 16) | (g << 8) | b));
// }

// static void create_proc_texture(t_game *game, int idx, int r, int g, int b)
// {
//     t_texture   *tex;
//     int         x;
//     int         y;
//     char        *pixel;

//     tex = &game->tex[idx];
//     tex->width = TEX_W;
//     tex->height = TEX_H;
//     tex->img = mlx_new_image(game->mlx, TEX_W, TEX_H);
//     if (!tex->img)
//         return ;
//     tex->addr = mlx_get_data_addr(tex->img, &tex->bpp,
//                     &tex->line_len, &tex->endian);
//     if (!tex->addr)
//         return ;
//     y = 0;
//     while (y < TEX_H)
//     {
//         x = 0;
//         while (x < TEX_W)
//         {
//             pixel = tex->addr + y * tex->line_len + x * (tex->bpp / 8);
//             *(unsigned int *)pixel = get_brick_pixel(x, y, r, g, b);
//             x++;
//         }
//         y++;
//     }
// }
static void create_flat_texture(t_game *game, int idx, unsigned int color)
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
            *(unsigned int *)pixel = color;
            x++;
        }
        y++;
    }
}

// static void load_texture(t_game *game, int idx, int r, int g, int b)
// {
//     t_texture   *tex;

//     tex = &game->tex[idx];
//     tex->img = mlx_xpm_file_to_image(game->mlx, game->config.tex_path[idx],
//                     &tex->width, &tex->height);
//     if (tex->img)
//     {
//         /* Texture XPM chargée avec succès */
//         tex->addr = mlx_get_data_addr(tex->img, &tex->bpp,
//                         &tex->line_len, &tex->endian);
//     }
//     else
//     {
//         /* Fallback : texture procédurale en mock */
//         create_proc_texture(game, idx, r, g, b);
//     }
// }
static int  init_textures(t_game *game)
{
    create_flat_texture(game, NORTH, 0x005577AA);
    create_flat_texture(game, SOUTH, 0x003F8040);
    create_flat_texture(game, EAST,  0x00B42020);
    create_flat_texture(game, WEST,  0x00B46820);
    return (0);
}

// static int  init_textures(t_game *game)
// {
//     load_texture(game, NORTH, 60,  80,  180);  /* bleu  : murs nord          */
//     load_texture(game, SOUTH, 50,  130, 70);   /* vert  : murs sud           */
//     load_texture(game, EAST,  180, 50,  50);   /* rouge : murs est           */
//     load_texture(game, WEST,  180, 110, 40);   /* orange: murs ouest         */
//     return (0);
// }

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