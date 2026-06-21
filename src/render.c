/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 09:59:25 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 23:49:26 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_raycasting/cub3d.h"

unsigned int	sample_texture(t_texture *tex, int tx, int ty)
{
	char	*pixel;

	if (!tex || !tex->addr)
		return (1);
	pixel = tex->addr + ty * tex->line_len + tx * (tex->bpp / 8);
	return (*(unsigned int *)pixel);
}

t_texture	*get_wall_texture(t_game *game, t_ray *ray,
		double *tex_pos, double *tex_step)
{
	t_texture	*tex;

	tex = &game->tex[ray->face];
	*tex_step = (double)TEX_H / (double)ray->line_h;
	*tex_pos = ((double)ray->draw_y0 - (double)SCREEN_H / 2.0
			+ (double)ray->line_h / 2.0) * (*tex_step);
	return (tex);
}

void	render_frame(t_game *game)
{
	t_ray	ray;
	int		col;

	col = 0;
	while (col < SCREEN_W)
	{
		cast_ray(game, col, &ray);
		draw_column(game, col, &ray);
		col++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->buf.img, 0, 0);
}

int	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->has_focus == 0)
	{
		ft_memset(&game->keys, 0, sizeof(t_keys));
		render_frame(game);
		return (0);
	}
	handle_input(game);
	render_frame(game);
	return (0);
}
