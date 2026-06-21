/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 06:37:18 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 23:51:08 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_raycasting/cub3d.h"

static void	rotate_player(t_player *p, double angle)
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

static int	is_wall(t_config *cfg, double x, double y)
{
	int	mx;
	int	my;

	mx = (int)x;
	my = (int)y;
	if (my < 0 || my >= cfg->map_h || mx < 0 || mx >= cfg->map_w)
		return (1);
	if (!cfg->map[my])
		return (1);
	if (mx >= (int)ft_strlen(cfg->map[my]))
		return (1);
	return (cfg->map[my][mx] == '1');
}

static void	try_move(t_game *game, double dx, double dy)
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
