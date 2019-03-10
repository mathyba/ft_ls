/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 13:33:38 by emuckens          #+#    #+#             */
/*   Updated: 2018/11/20 15:57:26 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strndup(const char *src, int n)
{
	int		i;
	char	*dup;
	char	*ptr;
	size_t	len;

	ptr = (char *)(unsigned long)src;
	len = n;
	if (!(dup = (char *)ft_memalloc((len + 1))))
		return (NULL);
	i = 0;
	while (len--)
	{
		dup[i] = src[i];
		i++;
	}
	return (dup);
}
