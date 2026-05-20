/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:22:53 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/19 08:28:21 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*pt_dest;
	char	*pt_src;

	if ((dest == (void *)0) && (src == (void *)0))
		return (NULL);
	pt_dest = (char *)dest;
	pt_src = (char *)src;
	if (dest < src)
		ft_memcpy(dest, src, n);
	else
	{
		while (n > 0)
		{
			*(pt_dest + n - 1) = *(pt_src + n - 1);
			n--;
		}
	}
	return (pt_dest);
}
