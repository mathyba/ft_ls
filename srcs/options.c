/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 14:47:59 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 17:53:32 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Check option validity (= valid printable character)
** Input: single option char
** Return true or false
*/

static int			is_option(int c)
{
	if (!ft_isprint(c) || (!ft_strchr(OPTIONS, c) && c != ' ') || !c)
		return (FALSE);
	return (TRUE);
}

/*
** Set on bit corresponding to option (cf ft_ls.h)
*/

static void			add_option(ENV *e, int ch)
{
	if (ch == 'R')
		e->options |= OPT_RECUR;
	else if (ch == 'G')
		e->options |= OPT_COLOR;
	else if (ch == 'C')
	{
		e->options |= OPT_COLUMNS;
		if (e->options & OPT_LONG)
			e->options ^= OPT_LONG;
	}
	else if (ch == '1')
	{
		if (e->options & OPT_LONG)
			e->options ^= OPT_LONG;
		if (e->options & OPT_COLUMNS)
			e->options ^= OPT_COLUMNS;
	}
	else
	{
		if (ch == 'l' && e->options & OPT_COLUMNS)
			e->options ^= OPT_COLUMNS;
		e->options |= (1 << (ch - (ch >= 'a' ? 'a' : 'A')));
	}
}

/*
** add linked options or removed incompatible ones
** Input: option code
*/

static void			handle_option_compatibility(ENV *e, int *options)
{
	if (*options & OPT_CTIME && !(*options & OPT_MTIME))
		*options ^= OPT_CTIME;
	if (*options & OPT_KILOBYTES && !(*options & OPT_BLOCKSIZE))
		*options ^= OPT_KILOBYTES;
	if (*options & OPT_INCOMING)
	{
		if (e->options & OPT_REVERSE)
			*options ^= OPT_REVERSE;
		*options |= OPT_HIDDEN;
	}
	if (*options & OPT_NONAME || *options & OPT_NOGID)
		add_option(e, 'l');
}

/*
** Display illegal options error and usage on STDERR
** Input: single option char
*/

static int			display_usage(char option)
{
	ft_printf("%s: %s %c\n", NAME, ERR_OPTION, option);
	ft_printf("usage: %s [%s] [file ...]\n", NAME, OPTIONS);
	return (-1);
}

/*
** Read arguments starting with -, modify binary tracker
** Input: argv, argc
** Returns -1 if option error, otherwise index of first non option arg.
** Options value is stored in env
*/

int					read_options(ENV *e, char **argv, int argc)
{
	int		arg;
	int		i;
	int		ch;

	arg = 0;
	e->options |= OPT_COLUMNS;
	while (++arg < argc && argv[arg][0] == '-' && (i = 1))
	{
		if (!argv[arg][1])
			break ;
		if (argv[arg][1] == '-')
			return (argv[arg][2] ? display_usage('-') : arg);
		while ((ch = argv[arg][i]) && is_option(ch) && ++i)
			add_option(e, ch);
		if (i > 0 && ch && !is_option(ch))
			return (display_usage(ch));
	}
	if (get_envvar_index(e, CLICOLOR) >= 0)
		e->options |= OPT_COLOR;
	handle_option_compatibility(e, &e->options);
	return (arg);
}
