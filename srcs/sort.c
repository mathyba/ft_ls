/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 21:21:25 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 18:28:58 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Sort files by lexicographical order
** Take into acccount reverse option as needed
** Input: current = node to insert, content = node already in place
** Return: 1 or 0 for placement on tree branch (left or right)
*/

int		sort_lexi(ENV *e, void *current, void *content)
{
	char	reverse;

	reverse = e->options & OPT_REVERSE ? 1 : 0;
	if (ft_strcmp((const char *)((t_file *)current)->name,
				((const char *)((t_file *)content)->name)) < 0)
		return (!reverse);
	return (reverse);
}

/*
** Sort files by incoming order
** Take into acccount reverse option as needed
** Input: current = node to insert, content = node already in place
** Return: 1 or 0 for placement on tree branch (left or right)
*/

int		sort_incoming(ENV *e, void *current, void *content)
{
	char	reverse;

	(void)current;
	(void)content;
	return (reverse = e->options & OPT_REVERSE ? 1 : 0);
}

/*
** Sort files by time - if same second, sort by nanosecond
** Take into acccount reverse option as needed
** Input: current = node to insert, content = node already in place
** Return: 1 or 0 for placement on tree branch (left or right)
*/

int		sort_mtime(ENV *e, void *current, void *content)
{
	char	reverse;

	(void)e;
	reverse = e->options & OPT_REVERSE ? 1 : 0;
	if (((t_file *)current)->timestr[9] != ':'
			&& ((t_file *)content)->timestr[9] == ':')
		return (reverse);
	else if (((t_file *)content)->timestr[9] != ':'
			&& ((t_file *)current)->timestr[9] == ':')
		return (!reverse);
	else if (((t_file *)current)->time > ((t_file *)content)->time)
		return (!reverse);
	if (((t_file *)current)->time < ((t_file *)content)->time)
		return (reverse);
	if (((t_file *)current)->ntime > ((t_file *)content)->ntime)
		return (!reverse);
	if (((t_file *)current)->ntime < ((t_file *)content)->ntime)
		return (!reverse);
	return (sort_lexi(e, current, content));
}

/*
** Sort arguments (files first then directories, each in lexicographic order)
** retrieve name and time of file for subsequent lexi or time sort
** No reverse in case of reverse option
** Input: current = node to insert, content = node already in place
** Return: 1 or 0 for placement on tree branch (left or right)
*/

int		sort_arg(ENV *e, void *current, void *content)
{
	t_file		*con;
	t_file		*cur;
	char		buf[1024];

	if (!(e->nl & (1 << MULTIARG)))
	{
		ft_printf("sort arg 1st return\n");
		return (e->fsort[e->sort](e, current, content));
	}
	ft_bzero(buf, 1024);
	cur = get_fileinfo(e, ((t_file *)current)->name, "", 0);
	con = get_fileinfo(e, ((t_file *)content)->name, "", 0);
	if (!(cur->type == DIRECTORY))
	{
		if (!(con->type == DIRECTORY))
			return (e->fsort[e->sort](e, current, content));
		return (1);
	}
	if (!(con->type == DIRECTORY))
		return (0);
	return (e->fsort[e->sort](e, current, content));
}
