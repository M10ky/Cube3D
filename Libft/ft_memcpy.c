/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 08:29:22 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/19 08:26:49 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*pt_dest;
	char	*pt_src;

	if ((dest == (void *)0) && (src == (void *)0))
		return (NULL);
	pt_dest = (char *)dest;
	pt_src = (char *)src;
	i = 0;
	while (i < n)
	{
		pt_dest[i] = pt_src[i];
		i++;
	}
	return (pt_dest);
}
