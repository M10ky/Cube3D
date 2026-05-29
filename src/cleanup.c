#include "../includes/cube3d.h"

void    cleanup(t_game *game)
{
    int i;

    /* Libérer les textures */
    i = 0;
    while (i < 4)
    {
        if (game->tex[i].img)
            mlx_destroy_image(game->mlx, game->tex[i].img);
        i++;
    }

    /* Libérer le framebuffer */
    if (game->buf.img)
        mlx_destroy_image(game->mlx, game->buf.img);

    /* Fermer la fenêtre et quitter MLX */
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    if (game->mlx)
        free(game->mlx);  /* MLX n'a pas de fonction de nettoyage, on libère juste le pointeur */
}

void    cleanup_exit(t_game *game)
{
    cleanup(game);
    exit(0);
}

int     print_error(const char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    return (1);
}