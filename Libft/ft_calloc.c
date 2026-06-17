/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:44:56 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/25 00:16:43 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void			*src;

	if (nmemb != 0 && size != 0 && nmemb > ((size_t) - 1) / size)
		return (NULL);
	src = malloc(nmemb * size);
	if (src == NULL)
		return (NULL);
	ft_bzero(src, (nmemb * size));
	return (src);
}
