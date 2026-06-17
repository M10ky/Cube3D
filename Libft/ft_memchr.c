/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:05:28 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/19 13:26:39 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	unsigned char		*pt_s;

	pt_s = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (pt_s[i] == (unsigned char)c)
		{
			return ((void *)pt_s + i);
		}
		i++;
	}
	return (NULL);
}
