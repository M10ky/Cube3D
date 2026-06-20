/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 09:54:22 by jonandri          #+#    #+#             */
/*   Updated: 2026/06/18 23:49:05 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

void free_map(t_map *map)
{
    t_line	*tmp;
	// t_line	*tmp2;

    while (map->first_line)
    {
        tmp = map->first_line->next;
        free(map->first_line->content);
        free(map->first_line);
        map->first_line = tmp;
    }

    // while (map->last_line)
    // {
    //     tmp2 = map->last_line->next;
    //     free(map->last_line->content);
    //     free(map->last_line);
    //     map->last_line = tmp2;
    // }
}

void    free_all(t_map *map)
{
	int i;

    if (map->no_path)
	{
        free(map->no_path);
		map->no_path = NULL;
	}
    if (map->so_path)
	{
        free(map->so_path);
		map->so_path = NULL;
	}
    if (map->ea_path)
	{
        free(map->ea_path);
		map->ea_path = NULL;
	}
    if (map->we_path)
	{
        free(map->we_path);
		map->we_path = NULL;
	}

	if (map->grid)
	{
		i = 0;
		while (map->grid[i] != NULL)
		{
			if (map->true_grid[i])
				free(map->grid[i]);
			i++;
		}
		free(map->grid);
	}
}

int main(int argc, char **argv)
{
    t_map map;
    t_game game;

    if (argc != 2)
    {
        printf("Input : [cube3d] [file.cub]\n");
        return (1);
    }
    if (read_file(argv[1], &map) == 1)
    {
        (void)argv;
        ft_memset(&game, 0, sizeof(t_game));
        if (init_game(&game, &map) != 0)
            return (1);
        mlx_hook(game.win, EVT_KEY_PRESS, 1L << 0, key_press, &game);
        mlx_hook(game.win, EVT_KEY_RELEASE, 1L << 1, key_release, &game);
        // mlx_hook(game.win, EVT_FOCUS_OUT, 1L<<5, handle_focus_lost, &game);
        mlx_hook(game.win, EVT_FOCUS_IN,  1L<<5, handle_focus_in,  &game);
        mlx_hook(game.win, EVT_LEAVE_NOTIFY, 1L<<5, handle_focus_lost, &game);
        mlx_hook(game.win, EVT_CLOSE, 0, handle_close, &game);
        mlx_loop_hook(game.mlx, game_loop, &game);
        mlx_loop(game.mlx);
        // cleanup(&game);
		free_all(&map);
    	free_map(&map);
    }
    else
    {
		free_all(&map);
        free_map(&map);
        return (1);
    }

    return (0);
}
