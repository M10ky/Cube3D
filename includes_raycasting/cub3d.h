/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:47:24 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/22 00:02:36 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <math.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "../Libft/libft.h"
# include "../includes_parsing/cube.h"
# include "../minilibx-linux/mlx.h"

# define SCREEN_W    1024
# define SCREEN_H    512
# define WIN_TITLE   "cub3D"

# define TEX_W       64
# define TEX_H       64

# define MOV_SPD     0.02
# define ROT_SPD     0.01

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
# define EVT_FOCUS_OUT    10
# define EVT_FOCUS_IN     9
# define EVT_LEAVE_NOTIFY 8
# define EVT_CLOSE        17

typedef struct s_config
{
	char	*tex_path[4];
	int		floor_color;
	int		ceil_color;
	char	**map;
	int		map_h;
	int		map_w;

	double	spawn_x;
	double	spawn_y;
	double	spawn_dir_x;
	double	spawn_dir_y;
	double	spawn_plane_x;
	double	spawn_plane_y;
}	t_config;

typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_texture;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	move_speed;
	double	rot_speed;
}	t_player;

typedef struct s_ray
{
	double	cam;
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	double	side_x;
	double	side_y;
	double	delta_x;
	double	delta_y;
	int		hit;
	int		side;
	double	wall_dist;
	int		line_h;
	int		draw_y0;
	int		draw_y1;
	int		face;
	int		tex_x;
}	t_ray;

typedef struct s_keys
{
	int	w;
	int	s;
	int	a;
	int	d;
	int	left;
	int	right;
}	t_keys;

typedef struct s_game
{
	t_config	config;
	t_player	player;
	t_texture	tex[4];
	t_img		buf;
	t_keys		keys;
	void		*mlx;
	void		*win;
	int			has_focus;
	t_map		*map;
}	t_game;

int				init_mock_config(t_config *config, t_map *map);
void			cleanup_mock_config(t_config *cfg);
void			free_map(t_map *map);
void			free_all(t_map *map);

int				init_game(t_game *game, t_map *map);
void			init_player(t_game *game);
void			put_pixel(t_img *buf, int x, int y, unsigned int color);
void			cast_ray(t_game *game, int col, t_ray *ray);

int				game_loop(void *param);
void			render_frame(t_game *game);

int				key_press(int key, void *param);
int				key_release(int key, void *param);
int				handle_close(void *param);
void			handle_input(t_game *game);

void			cleanup(t_game *game);
void			cleanup_exit(t_game *game);
int				print_error(const char *msg);

int				handle_focus_lost(void *param);
int				handle_focus_in(void *param);

int				init_framebuffer(t_game *game);
int				init_mlx(t_game *game);
void			put_pixel(t_img *buf, int x, int y, unsigned int color);

void			free_cfg_map(t_config *cfg);
void			free_cfg_textures(t_config *cfg);

void			set_dir_north(t_config *cfg);
void			set_dir_south(t_config *cfg);
void			set_dir_east(t_config *cfg);
void			set_dir_west(t_config *cfg);

int				setup_map(t_config *cfg, t_map *map);
void			setup_direction(t_config *cfg, char dir);
int				setup_textures(t_config *cfg, t_map *map);
int				setup_player(t_config *cfg, t_map *map);
void			setup_colors(t_config *cfg, t_map *map);
void			free_mock_textures(t_config *cfg);
int				duplicate_map_rows(t_config *cfg, t_map *map, int h);
void			free_partial_map(char **map, int i);
int				count_map_height(t_map *map);
void			calc_delta(t_ray *ray);
void			calc_step_x(t_game *game, t_ray *ray);
void			calc_step_y(t_game *game, t_ray *ray);
void			init_dda(t_game *game, t_ray *ray);

unsigned int	sample_texture(t_texture *tex, int tx, int ty);
void			calc_wall_dist(t_ray *ray);
void			calc_wall_face(t_ray *ray);
void			calc_draw_bounds(t_ray *ray);
void			calc_tex_x(t_player *p, t_ray *ray);
void			calc_wall(t_game *game, t_ray *ray);
void			draw_ceiling_strip(t_game *game, int col, int end_y);
void			draw_floor_strip(t_game *game, int col, int start_y);
void			draw_wall_strip(t_game *game, int col, t_ray *ray);
void			draw_column(t_game *game, int col, t_ray *ray);
t_texture		*get_wall_texture(t_game *game, t_ray *ray,
					double *tex_pos, double *tex_step);
void			free_paths(t_map *map);
void			free_grid(t_map *map);

#endif
