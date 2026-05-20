/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 09:54:22 by jonandri          #+#    #+#             */
/*   Updated: 2026/04/26 09:54:38 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int main(void)
{
	t_data	img;

	img.mlx = mlx_init();
	img.win = mlx_new_window(img.mlx, 800, 300, "cube");
	img.img = mlx_new_image(img.mlx, 800, 300);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_lenght,
								&img.endian);
	my_mlx_pixel_put(&img, 400, 150, 0x00FF0000);
	mlx_put_image_to_window(img.mlx, img.win, img.img, 0, 0);
	mlx_loop(img.mlx);
	return (0);
}