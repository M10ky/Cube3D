/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:21:31 by miokrako          #+#    #+#             */
/*   Updated: 2026/05/29 21:53:53 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"


int main(int argc, char **argv)
{
	(void)argv;  /* éviter l'avertissement de paramètre non utilisé */
    t_game  game;
    if (argc != 2)
        return (print_error("Usage: ./cub3D <map.cub>"));
    /* Zero-init CRUCIAL : évite les valeurs garbage dans les structures */
    memset(&game, 0, sizeof(t_game));

    if (init_game(&game) != 0)
        return (1);

    // Ajouter dans init_game() ou juste avant mlx_loop() dans main.c :
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, EVT_KEY_PRESS,   1L<<0, key_press,    &game);
	mlx_hook(game.win, EVT_KEY_RELEASE, 1L<<1, key_release,  &game);
	mlx_hook(game.win, EVT_CLOSE,       0,     handle_close, &game);
	mlx_loop(game.mlx);
    /* Lancement de la boucle principale de MLX (game_loop appelé à chaque frame) */
    mlx_loop(game.mlx);

    /* Cleanup à la sortie (jamais atteint ici car mlx_loop est infini) */
    cleanup(&game);
    return (0);
}