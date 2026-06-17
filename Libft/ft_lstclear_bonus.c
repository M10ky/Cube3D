/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jonandri <jonandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:13:32 by jonandri          #+#    #+#             */
/*   Updated: 2025/03/25 05:48:06 by jonandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*first;

	if ((*lst) == NULL)
		return ;
	while ((*lst) != NULL)
	{
		first = (*lst)-> next;
		del ((*lst)->content);
		free(*lst);
		(*lst) = first;
	}
}
