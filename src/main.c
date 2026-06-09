/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:21:31 by miokrako          #+#    #+#             */
/*   Updated: 2026/06/09 06:39:26 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

int main(int argc, char **argv)
{
    t_game  game;

    if (argc != 2)
        return (print_error("Usage: ./cub3D <fichier.cub>"));
    (void)argv;   

    memset(&game, 0, sizeof(t_game));

    if (init_game(&game) != 0)
        return (1);


    mlx_hook(game.win, EVT_KEY_PRESS,   1L << 0, key_press,    &game);
    mlx_hook(game.win, EVT_KEY_RELEASE, 1L << 1, key_release,  &game);
    mlx_hook(game.win, EVT_CLOSE,       0,       handle_close, &game);
    mlx_loop_hook(game.mlx,                      game_loop,    &game);

   
    mlx_loop(game.mlx);


    cleanup(&game);
    return (0);
}
