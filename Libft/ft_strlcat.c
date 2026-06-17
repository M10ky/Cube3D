/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 07:13:57 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/25 00:08:03 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	lenght_dest;
	size_t	lenght_src;
	size_t	i;

	lenght_dest = ft_strlen(dst);
	lenght_src = ft_strlen(src);
	i = 0;
	if (size == 0 || size <= lenght_dest)
		return (lenght_src + size);
	while (src[i] != '\0' && i < size - lenght_dest - 1)
	{
		dst[lenght_dest + i] = src[i];
		i++;
	}
	dst[lenght_dest + i] = '\0';
	return (lenght_dest + lenght_src);
}
