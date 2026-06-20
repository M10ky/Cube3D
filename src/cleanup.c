/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 21:33:59 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/20 11:13:59 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	print_error(const char *msg)
{
	write(2, "Error\n", 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (1);
}

void	cleanup_mock_config(t_config *cfg)
{
	int	i;

	if (!cfg)
		return ;

	i = 0;
	while (i < 4)
	{
		if (cfg->tex_path[i])
		{
			free(cfg->tex_path[i]);
			cfg->tex_path[i] = NULL;
		}
		i++;
	}

	if (cfg->map)
	{
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

	cfg->map_h = 0;
	cfg->map_w = 0;
}

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
		/* tex_path déjà libéré par cleanup_mock_config */
		i++;
	}
}

// static void	free_map(t_config *cfg)
// {
// 	int	i;

// 	if (!cfg || !cfg->map)
// 		return ;

// 	i = 0;
// 	while (cfg->map[i])
// 	{
// 		free(cfg->map[i]);
// 		cfg->map[i] = NULL;
// 		i++;
// 	}
// 	free(cfg->map);
// 	cfg->map = NULL;
// }

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
	// free_map(&game->config);
	cleanup_mock_config(&game->config);   /* sécurité supplémentaire */
	free_mlx_context(game);

	/* Remettre à zéro les structures principales */
	ft_memset(&game->config, 0, sizeof(t_config));
	ft_memset(&game->player, 0, sizeof(t_player));
}


void	cleanup_exit(t_game *game)
{
	cleanup(game);
	exit(0);
}