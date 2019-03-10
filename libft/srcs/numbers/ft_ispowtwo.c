/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ispowtwo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 15:30:42 by emuckens          #+#    #+#             */
/*   Updated: 2018/10/23 15:31:11 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Returns false if number is not power to two | power otherwise
*/

int		ft_ispowtwo(int n)
{
	int pow;
	int factor;

	pow = 2;
	factor = 0;
	while (pow < n)
	{
		++factor;
		pow *= 2;
	}
	return (pow > n ? factor : 0);
}
