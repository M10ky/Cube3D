#include "../includes/cube3d.h"

void cleanup(t_game *game)
{
    int i;

    // Libérer les textures
    i = 0;
    while (i < 4)
    {
        if (game->tex[i].img)
            mlx_destroy_image(game->mlx, game->tex[i].img);
        // Libérer le chemin
        free(game->config.tex_path[i]);
        i++;
    }
    // Libérer le framebuffer
    if (game->buf.img)
        mlx_destroy_image(game->mlx, game->buf.img);
    // Libérer la map
    if (game->config.map)
    {
        i = 0;
        while (game->config.map[i])
            free(game->config.map[i++]);
        free(game->config.map);
    }
    // Fermer fenêtre/MLX
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    if (game->mlx)
        mlx_destroy_display(game->mlx);  // Linux uniquement
    free(game->mlx);
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