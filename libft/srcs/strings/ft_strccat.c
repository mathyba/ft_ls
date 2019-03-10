/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strccat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 21:33:30 by emuckens          #+#    #+#             */
/*   Updated: 2018/05/20 17:06:11 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strccat(char *s1, const char *s2, char c)
{
	char *cat;

	cat = s1;
	while (*cat)
		++cat;
	*cat = c;
	++cat;
	while (*s2)
	{
		*cat = *s2;
		++cat;
		++s2;
	}
	*cat = 0;
	return (s1);
}
