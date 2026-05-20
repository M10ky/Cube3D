/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 10:19:40 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/25 00:15:49 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*pt;

	i = 0;
	pt = (char *)s;
	while (i < n)
	{
		pt[i] = c;
		i++;
	}
	return (pt);
}
