/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 09:34:45 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/18 13:19:12 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	lenght ;

	lenght = 0;
	while (s[lenght] != '\0')
	{
		lenght++;
	}
	return (lenght);
}
