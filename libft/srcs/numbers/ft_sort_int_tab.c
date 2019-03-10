/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_int_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 11:20:37 by emuckens          #+#    #+#             */
/*   Updated: 2018/10/12 12:57:13 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_sort_int_tab(int *tab, size_t size)
{
	size_t i;
	size_t j;

	if (!tab)
		return ;
	i = 0;
	while (i < size)
	{
		j = i + 1;
		while (i < j && j < size)
		{
			if (tab[j] < tab[i])
				ft_swap(&tab[i], &tab[j]);
			j++;
		}
		i++;
	}
}
