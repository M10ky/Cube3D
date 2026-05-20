/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 08:47:55 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/21 09:05:29 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	i;
	long	nbr;
	char	c[100];

	i = 0;
	nbr = n;
	if (nbr == 0)
	{
		ft_putchar_fd('0', fd);
	}
	if (nbr < 0)
	{
		nbr *= -1;
		ft_putchar_fd('-', fd);
	}
	while (nbr > 0)
	{
		c[i++] = (nbr % 10) + 48;
		nbr = nbr / 10;
	}
	while (i > 0)
	{
		ft_putchar_fd(c[--i], fd);
	}
}
