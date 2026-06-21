/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 11:12:42 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/21 20:38:50 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	load_texture(t_game *game, int index, char *path)
{
	game->tex[index].img = mlx_xpm_file_to_image(game->mlx, path,
			&game->tex[index].width, &game->tex[index].height);
	if (!game->tex[index].img)
		return (print_error("Texture: failed to load XPM"));
	game->tex[index].addr = mlx_get_data_addr(game->tex[index].img,
			&game->tex[index].bpp, &game->tex[index].line_len,
			&game->tex[index].endian);
	if (!game->tex[index].addr)
		return (print_error("Texture: failed to get data address"));
	return (0);
}

static int	init_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (load_texture(game, i, game->config.tex_path[i]) != 0)
		{
			while (--i >= 0)
			{
				if (game->tex[i].img)
					mlx_destroy_image(game->mlx, game->tex[i].img);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

void	init_player(t_game *game)
{
	game->player.pos_x = game->config.spawn_x;
	game->player.pos_y = game->config.spawn_y;
	game->player.dir_x = game->config.spawn_dir_x;
	game->player.dir_y = game->config.spawn_dir_y;
	game->player.plane_x = game->config.spawn_plane_x;
	game->player.plane_y = game->config.spawn_plane_y;
	game->player.move_speed = 0.1;
	game->player.rot_speed = 0.05;
}

int	init_game(t_game *game, t_map *map)
{
	ft_memset(game, 0, sizeof(t_game));
	game->map = map;
	if (init_mock_config(&game->config, map) != 0)
	{
		cleanup_mock_config(&game->config);
		return (1);
	}
	if (init_mlx(game) != 0)
	{
		cleanup_mock_config(&game->config);
		return (1);
	}
	if (init_framebuffer(game) != 0)
	{
		cleanup(game);
		return (1);
	}
	if (init_textures(game) != 0)
	{
		cleanup(game);
		return (1);
	}
	game->has_focus = 1;
	init_player(game);
	return (0);
}
