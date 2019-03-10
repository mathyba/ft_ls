/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 13:56:49 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 17:54:31 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <errno.h>

/*
** Display error for files passed as arguments only
*/

static void		display_err(t_lbtree *node)
{
	char		*err;
	t_file		*file;
	static int	fd = STDERR;

	file = (t_file *)node->content;
	err = strerror(file->type);
	ft_printf("{FD!}%s: %s: %s\n{EOO}", &fd, NAME, file->name, err);
}

/*
** Handle -- argument case: read current directory or treat as error
** Input: argc for special case where -- is only argument
*/

static void		read_dash(ENV *e, int argc, int *stop)
{
	t_file *file;

	if (e->nl & (1 << MULTIARG) && argc == 3)
		e->nl ^= (1 << MULTIARG);
	if (argc == 2)
	{
		file = get_fileinfo(e, ".", "", 1);
		ft_treeadd_cust(e, &e->filelist, file, ARG);
		return ;
	}
	*stop = 1;
}

/*
** Dispatch argument to error list or valid filelist
** Set bits en e->nl for later ability to detect new line presence
** Handle -- exception
*/

static void		arg_to_filelist(ENV *e, char **argv, int endopt, int argc)
{
	static t_file			*file;
	static int				rev;
	int						stop_dash;

	stop_dash = 0;
	e->nl |= (argc - endopt - 2 > 0 ? (1 << MULTIARG) : e->nl);
	e->nl |= (argc - endopt == 1 ? (1 << NOARG) : e->nl);
	while (++endopt < argc && (e->argerr = 1))
	{
		if (!ft_strequ(argv[endopt], "--") || stop_dash)
		{
			file = get_fileinfo(e, argv[endopt], "", 0);
			if (e->argerr - 1 == T_ERR && (stop_dash = 1))
			{
				if (e->options & OPT_REVERSE && (rev = 1))
					e->options ^= OPT_REVERSE;
				ft_treeadd_cust(e, &e->errlist, file, LEXI);
				e->options |= (rev == 1 && !(rev = 0)) ? OPT_REVERSE : 0;
			}
			else
				ft_treeadd_cust(e, &e->filelist, file, ARG);
		}
		else
			read_dash(e, argc, &stop_dash);
	}
}

/*
** Read input from user through arguments, scan filesystem and display files
** as required
** Input: options and files to display
*/

int				main(int argc, char **argv, char **environment)
{
	ENV				e;
	int				endopt;

	init_env(&e, environment);
	if ((endopt = read_options(&e, argv, argc) - 1) == -2)
		return (free_env(&e, FAIL));
	handle_environment_var(&e);
	if (e.options & OPT_MTIME)
		e.sort = TIME;
	else if (e.options & OPT_INCOMING)
		e.sort = INCOMING;
	else
		e.sort = LEXI;
	arg_to_filelist(&e, argv, endopt, argc);
	if (e.errlist)
		ft_treedo_order(e.errlist, &display_err);
	reset_colors(&e, 0);
	list_all(&e, e.filelist, NULL);
	free_env(&e, e.exit);
	return (e.exit);
}
