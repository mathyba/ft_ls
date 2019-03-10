/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/14 12:51:02 by emuckens          #+#    #+#             */
/*   Updated: 2018/05/20 17:04:32 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strcjoin(char const *s1, char const *s2, char c)
{
	char	*str;
	int		index;
	char	*ptr1;
	char	*ptr2;

	index = 0;
	ptr1 = (char *)(unsigned long)s1;
	ptr2 = (char *)(unsigned long)s2;
	if (!(str = (char *)ft_memalloc(ft_strlen(ptr1) + ft_strlen(ptr2) + 2)))
		return (NULL);
	while (s1 && *s1)
	{
		str[index] = *s1;
		++index;
		++s1;
	}
	str[index] = c;
	++index;
	while (s2 && *s2)
	{
		str[index] = *s2;
		++index;
		++s2;
	}
	return (str);
}
