/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_slstadd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 11:23:59 by emuckens          #+#    #+#             */
/*   Updated: 2018/10/26 11:26:27 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_slstadd(t_slist **alst, t_slist *new)
{
	if (!new)
		return ;
	if (*alst)
	{
		new->prev = (*alst)->prev;
		(*alst)->prev = new;
	}
	new->next = *alst;
	*alst = new;
	(*alst)->prev = NULL;
}
