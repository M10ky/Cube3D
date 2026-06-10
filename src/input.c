/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 06:37:18 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/10 21:41:28 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

int	key_press(int keycode, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keycode == KEY_ESC)
		cleanup_exit(game);
	else if (keycode == KEY_W)
		game->keys.w = 1;
	else if (keycode == KEY_S)
		game->keys.s = 1;
	else if (keycode == KEY_A)
		game->keys.a = 1;
	else if (keycode == KEY_D)
		game->keys.d = 1;
	else if (keycode == KEY_LEFT)
		game->keys.left = 1;
	else if (keycode == KEY_RIGHT)
		game->keys.right = 1;
	return (0);
}

int	key_release(int keycode, void *param)
{
	t_game  *game;

	game = (t_game *)param;
	if (keycode == KEY_W)
		game->keys.w = 0;
	else if (keycode == KEY_S)
		game->keys.s = 0;
	else if (keycode == KEY_A)
		game->keys.a = 0;
	else if (keycode == KEY_D)
		game->keys.d = 0;
	else if (keycode == KEY_LEFT)
		game->keys.left = 0;
	else if (keycode == KEY_RIGHT)
		game->keys.right = 0;
	return (0);
}

int	handle_close(void *param)
{
	t_game  *game;

	game = (t_game *)param;
	cleanup_exit(game);
	return (0);
}


static	void	rotate_player(t_player *p, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = p->dir_x;
	p->dir_x = p->dir_x * cos(angle) - p->dir_y * sin(angle);
	p->dir_y = old_dir_x * sin(angle) + p->dir_y * cos(angle);
	old_plane_x = p->plane_x;
	p->plane_x = p->plane_x * cos(angle) - p->plane_y * sin(angle);
	p->plane_y = old_plane_x * sin(angle) + p->plane_y * cos(angle);
}

static	int	is_wall(t_config *cfg, double x, double y)
{
	int	mx;
	int	my;

	mx = (int)x;
	my = (int)y;
	if (my < 0 || my >= cfg->map_h || mx < 0 || mx >= cfg->map_w)
		return (1);
	if (mx >= (int)ft_strlen(cfg->map[my]))
		return (1);
	return (cfg->map[my][mx] == '1');
}

static	void	try_move(t_game *game, double dx, double dy)
{
	double	new_x;
	double	new_y;

	new_x = game->player.pos_x + dx;
	new_y = game->player.pos_y + dy;
	if (!is_wall(&game->config, new_x, game->player.pos_y))
		game->player.pos_x = new_x;
	if (!is_wall(&game->config, game->player.pos_x, new_y))
		game->player.pos_y = new_y;
}

void	handle_input(t_game *game)
{
	t_player	*p;

	p = &game->player;
	if (game->keys.left)
		rotate_player(p, -ROT_SPD);
	if (game->keys.right)
		rotate_player(p, ROT_SPD);
	if (game->keys.w)
		try_move(game, p->dir_x * MOV_SPD, p->dir_y * MOV_SPD);
	if (game->keys.s)
		try_move(game, -p->dir_x * MOV_SPD, -p->dir_y * MOV_SPD);
	if (game->keys.a)
		try_move(game, p->dir_y * MOV_SPD, -p->dir_x * MOV_SPD);
	if (game->keys.d)
		try_move(game, -p->dir_y * MOV_SPD, p->dir_x * MOV_SPD);
}
