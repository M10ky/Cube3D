/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 20:37:10 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 20:41:06 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	put_pixel(t_img *buf, int x, int y, unsigned int color)
{
	char	*dst;

	if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H)
		return ;
	dst = buf->addr + y * buf->line_len + x * (buf->bpp / 8);
	*(unsigned int *)dst = color;
}

int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (print_error("MLX: failed to initialize"));
	game->win = mlx_new_window(game->mlx, SCREEN_W, SCREEN_H, "Cub3D");
	if (!game->win)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
		return (print_error("MLX: failed to create window"));
	}
	return (0);
}

int	init_framebuffer(t_game *game)
{
	game->buf.img = mlx_new_image(game->mlx, SCREEN_W, SCREEN_H);
	if (!game->buf.img)
		return (print_error("MLX: failed to create image buffer"));
	game->buf.addr = mlx_get_data_addr(game->buf.img, &game->buf.bpp,
			&game->buf.line_len, &game->buf.endian);
	if (!game->buf.addr)
		return (print_error("MLX: failed to get image address"));
	return (0);
}
