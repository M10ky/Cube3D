/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 11:51:57 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/21 08:38:40 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	ft_put(char *c, long nb)
{
	size_t	i;
	size_t	sign;

	i = 0;
	sign = 0;
	if (nb == 0)
	{
		c[i++] = '0';
		return (i);
	}
	if (nb < 0)
	{
		sign++;
		nb *= -1;
	}
	while (nb != 0)
	{
		c[i++] = nb % 10 + 48;
		nb = nb / 10;
	}
	if (sign == 1)
		c[i++] = '-';
	c[i] = '\0';
	return (i);
}

char	*ft_itoa(int n)
{
	char	c[12];
	size_t	j;
	char	*dest;
	size_t	len;

	len = ft_put(c, n);
	dest = malloc(sizeof(char) * (len + 1));
	if (dest == NULL)
		return (NULL);
	j = 0;
	while (len > 0)
	{
		dest[j++] = c[--len];
	}
	dest[j] = '\0';
	return (dest);
}
