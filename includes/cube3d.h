/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:47:24 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/09 21:00:09 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <math.h>                                
# include <stdio.h>  
# include <unistd.h>                                      
# include <stdlib.h>        
// # include <string.h>     //soloina libft   
# include <mlx.h>    
# include "../libft/libft.h"


# define SCREEN_W    1024
# define SCREEN_H    512
# define WIN_TITLE   "cub3D"

# define TEX_W       64
# define TEX_H       64


# define MOV_SPD     0.05
# define ROT_SPD     0.030


# define NORTH       0
# define SOUTH       1
# define EAST        2
# define WEST        3


# define KEY_ESC     65307
# define KEY_W       119
# define KEY_A       97
# define KEY_S       115
# define KEY_D       100
# define KEY_LEFT    65361
# define KEY_RIGHT   65363


# define EVT_KEY_PRESS    2
# define EVT_KEY_RELEASE  3
# define EVT_CLOSE        17


typedef struct s_config
{
    char        *tex_path[4];   /* [NORTH, SOUTH, EAST, WEST]                 */

    int         floor_color;    /* "F 220,100,0" → 0x00DC6400           */
    int         ceil_color;     /* "C 225,30,0"  → 0x00E11E00           */

    char        **map;
    int         map_h;     
    int         map_w;         

    double      spawn_x;        /* position X voloany (centre de cellule)    */
    double      spawn_y;        /* position Y voloany                        */
    double      spawn_dir_x;    /* vecteur direction X           */
    double      spawn_dir_y;    /* vecteur direction Y                        */
    double      spawn_plane_x;  /* plan caméra X     */
    double      spawn_plane_y;  /* plan caméra Y                              */
}   t_config;


typedef struct s_texture
{
    void        *img;
    char        *addr;
    int         bpp;
    int         line_len;
    int         endian;
    int         width;
    int         height;    
}   t_texture;


typedef struct s_img
{
    void        *img;       /* handle MLX                                     */
    char        *addr;      /* pointeur données pixels SCREEN_W×SCREEN_H     */
    int         bpp;        /* bits par pixel                                 */
    int         line_len;   /* octets par ligne                               */
    int         endian;     /* ordre des octets                               */
}   t_img;

typedef struct s_player
{
    double      pos_x;      /* position X dans le monde (en cellules)         */
    double      pos_y;      /* position Y dans le monde                       */
    double      dir_x;      /* direction X (vecteur unitaire, ex: (1,0)=Est)  */
    double      dir_y;      /* direction Y                                    */
    double      plane_x;    /* plan caméra X (perpendiculaire à dir)          */
    double      plane_y;    /* plan caméra Y                                  */
}   t_player;


typedef struct s_ray
{
    double      cam;
    double      dir_x;
    double      dir_y;
    int         map_x;
    int         map_y;

    int         step_x;
    int         step_y;
    double      side_x;
    double      side_y;
    double      delta_x;
    double      delta_y;

    int         hit;
    int         side;

    double      wall_dist;
    int         line_h;
    int         draw_y0;
    int         draw_y1;
    int         face;
    int         tex_x;      
}   t_ray;

typedef struct s_keys
{
    int         w;          /* avancer                                        */
    int         s;          /* reculer                                        */
    int         a;          /* strafer gauche                                 */
    int         d;          /* strafer droite                                 */
    int         left;       /* rotation gauche                                */
    int         right;      /* rotation droite                                */
}   t_keys;

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


int     init_mock_config(t_config *config);

int     init_game(t_game *game);
void    init_player(t_game *game);
void    put_pixel(t_img *buf, int x, int y, unsigned int color);

void    cast_ray(t_game *game, int col, t_ray *ray);

int     game_loop(void *param);
void    render_frame(t_game *game);

int     key_press(int key, void *param);
int     key_release(int key, void *param);
int     handle_close(void *param);
void    handle_input(t_game *game);

void    cleanup(t_game *game);
void    cleanup_exit(t_game *game);
int     print_error(const char *msg);

#endif