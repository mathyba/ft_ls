/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myenv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 11:49:16 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 18:03:21 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <stdlib.h>

/*
** Free directory files stored for column display
*/

void		free_files(ENV *e)
{
	int col;

	if (!e->col.files)
		return ;
	col = -1;
	while (++col < e->col.nbcol)
		ft_memdel((void **)&e->col.files[col]);
	ft_memdel((void **)&e->col.files);
}

/*
** Allocate tab to store all directory files, for later column display
** Return fail if failed allocation, success otherwise
*/

int			allocate_files(ENV *e)
{
	int		index;
	int		size;

	index = -1;
	size = e->col.nbcol * sizeof(t_colfile *);
	if (!(e->col.files = (t_colfile **)ft_memalloc(size)))
		return (FAIL);
	size = e->col.nbrow * sizeof(t_colfile);
	while (++index < e->col.nbcol)
		if (!(e->col.files[index] = (t_colfile *)ft_memalloc(size)))
		{
			ft_memdel((void **)&e->col.files);
			return (FAIL);
		}
	return (SUCCESS);
}

/*
** Free program environment
** Input: exit code to take place in return statement, full list tree to free
** Return: exit code
*/

int			free_env(ENV *e, int exit_code)
{
	int			index;
	int			i;

	index = 0;
	while (index < e->nb_envvar)
	{
		i = 0;
		while (e->env_var[index][i])
		{
			ft_strdel(&e->env_var[index][i]);
			++i;
		}
		ft_memdel((void **)&e->env_var[index]);
		++index;
	}
	ft_memdel((void **)&e->env_var);
	ft_destroy_tree(e->filelist);
	ft_destroy_tree(e->errlist);
	return (exit_code);
}

/*
** Initialize program environment
** Input: computer environment
** Return: success or failure (in case of malloc fail)
*/

int			init_env(ENV *e, char **env)
{
	struct stat		filestat;

	ft_bzero(e, sizeof(*e));
	e->nb_envvar = ft_strarrlen(env);
	e->fsort[0] = &sort_arg;
	e->fsort[1] = &sort_lexi;
	e->fsort[2] = &sort_mtime;
	e->fsort[3] = &sort_incoming;
	get_stat(e, &filestat, "/dev/fd", 0);
	e->fdinode = filestat.st_ino;
	e->max.nlen = MIN_COLW;
	return (get_envvar(e, env));
}
