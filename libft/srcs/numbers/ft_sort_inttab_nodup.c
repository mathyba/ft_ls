/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_inttab_nodup.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 11:20:37 by emuckens          #+#    #+#             */
/*   Updated: 2018/10/13 19:45:40 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_sort_inttab_nodup(int *tab, size_t size)
{
	size_t i;
	size_t j;

	if (!tab)
		return (1);
	i = 0;
	while (i < size)
	{
		j = i + 1;
		while (i < j && j < size)
		{
			if (tab[j] < tab[i])
				ft_swap(&tab[i], &tab[j]);
			else if (tab[j] == tab[i])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
