/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_slistnew.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 11:18:02 by emuckens          #+#    #+#             */
/*   Updated: 2018/10/26 15:27:53 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_slist		*ft_slstnew(char *str)
{
	t_slist *new;

	if (!(new = (t_slist *)ft_memalloc(sizeof(t_slist))))
		return (NULL);
	new->str = ft_strdup(str);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
