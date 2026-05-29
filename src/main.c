/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miokrako <miokrako@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 20:21:31 by miokrako          #+#    #+#             */
/*   Updated: 2026/05/29 20:53:45 by miokrako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cube3d.h"

int main(void)
{
    t_game  game;

    /* Zero-init CRUCIAL : évite les valeurs garbage dans les structures */
    memset(&game, 0, sizeof(t_game));

    if (init_game(&game) != 0)
        return (1);

    /* Lancement de la boucle principale de MLX (game_loop appelé à chaque frame) */
    mlx_loop(game.mlx);

    /* Cleanup à la sortie (jamais atteint ici car mlx_loop est infini) */
    cleanup(&game);
    return (0);
}