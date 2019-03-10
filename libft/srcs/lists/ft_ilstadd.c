/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ilstadd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 11:23:52 by emuckens          #+#    #+#             */
/*   Updated: 2018/10/26 11:55:34 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void		ft_ilstadd(t_ilist **alst, t_ilist *new)
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
