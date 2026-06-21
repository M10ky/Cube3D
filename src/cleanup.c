/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 21:33:59 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 23:57:39 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_raycasting/cub3d.h"

static void	free_textures(t_game *game)
{
	int	i;

	if (!game || !game->mlx)
		return ;
	i = 0;
	while (i < 4)
	{
		if (game->tex[i].img)
		{
			mlx_destroy_image(game->mlx, game->tex[i].img);
			game->tex[i].img = NULL;
		}
		i++;
	}
}

static void	free_mlx_context(t_game *game)
{
	if (!game)
		return ;
	if (game->win)
	{
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
	if (game->mlx)
	{
		if (game->buf.img)
		{
			mlx_destroy_image(game->mlx, game->buf.img);
			game->buf.img = NULL;
		}
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}

void	cleanup(t_game *game)
{
	if (!game)
		return ;
	free_textures(game);
	cleanup_mock_config(&game->config);
	free_mlx_context(game);
	if (game->map)
	{
		free_all(game->map);
		free_map(game->map);
		game->map = NULL;
	}
	ft_memset(&game->config, 0, sizeof(t_config));
	ft_memset(&game->player, 0, sizeof(t_player));
}

void	cleanup_exit(t_game *game)
{
	cleanup(game);
	exit(0);
}
