/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 21:33:59 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/10 21:34:24 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

int	print_error(const char *msg)
{
	write(2, "Error\n", 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (1);
}

static	void	free_textures(t_game *game)
{
	int	i;

	if (!game->mlx)
		return ;
	i = 0;
	while (i < 4)
	{
		if (game->tex[i].img)
			mlx_destroy_image(game->mlx, game->tex[i].img);
		if (game->config.tex_path[i])
		{
			free(game->config.tex_path[i]);
			game->config.tex_path[i] = NULL;
		}
		i++;
	}
}

static	void	free_map(t_config *cfg)
{
	int	i;

	if (!cfg->map)
		return ;
	i = 0;
	while (cfg->map[i])
	{
		free(cfg->map[i]);
		cfg->map[i] = NULL;
		i++;
	}
	free(cfg->map);
	cfg->map = NULL;
}

static	void free_mlx_context(t_game *game)
{
	if (game->win)
	{
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}

void	cleanup(t_game *game)
{
	free_textures(game);
	if (game->mlx && game->buf.img)
	{
		mlx_destroy_image(game->mlx, game->buf.img);
		game->buf.img = NULL;
	}
	free_map(&game->config);
	free_mlx_context(game);
}

void	cleanup_exit(t_game *game)
{
	cleanup(game);
	exit(0);
}
