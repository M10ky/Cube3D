/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 09:54:22 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/21 23:51:48 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes_raycasting/cub3d.h"

static void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, EVT_KEY_PRESS, 1L << 0, key_press, game);
	mlx_hook(game->win, EVT_KEY_RELEASE, 1L << 1, key_release, game);
	mlx_hook(game->win, EVT_FOCUS_IN, 1L << 21, handle_focus_in, game);
	mlx_hook(game->win, EVT_FOCUS_OUT, 1L << 21, handle_focus_lost, game);
	mlx_hook(game->win, EVT_CLOSE, 0, handle_close, game);
	mlx_loop_hook(game->mlx, game_loop, game);
}

static int	run_game(t_game *game, t_map *map)
{
	ft_memset(game, 0, sizeof(t_game));
	if (init_game(game, map) != 0)
		return (1);
	setup_hooks(game);
	mlx_loop(game->mlx);
	return (0);
}

int	main(int argc, char **argv)
{
	t_map	map;
	t_game	game;

	if (argc != 2)
	{
		printf("Input : [cube3d] [file.cub]\n");
		return (1);
	}
	if (read_file(argv[1], &map) != 1 || run_game(&game, &map) != 0)
	{
		free_all(&map);
		free_map(&map);
		return (1);
	}
	free_all(&map);
	free_map(&map);
	return (0);
}
