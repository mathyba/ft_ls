/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtabdel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/25 15:51:19 by emuckens          #+#    #+#             */
/*   Updated: 2018/11/26 15:47:53 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_strtabdel(char ***tab)
{
	int i;

	if (!tab)
		return ;
	i = 0;
	while (tab && *tab && (*tab)[i])
	{
		ft_strdel(&(*tab)[i]);
		++i;
	}
	ft_memdel((void **)(*tab));
}
