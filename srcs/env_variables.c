/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 17:08:58 by emuckens          #+#    #+#             */
/*   Updated: 2018/11/29 20:50:04 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <sys/ioctl.h>

/*
** Set coeficient for adjusting blocksize relatively to 512KB default
** Retrieve value in environment of set to 1;
*/

static void		set_blocksize_coef(ENV *e)
{
	int index;
	int	fd;

	fd = STDERR;
	if (e->options & OPT_KILOBYTES)
		e->blocksize_coef = 0.5;
	else if (e->options & OPT_LONG
			&& (index = get_envvar_index(e, BLOCKSIZE)) >= 0)
	{
		if ((e->blocksize_coef = ft_atoi(e->env_var[index][1])) < 512)
		{
			ft_printf("{FD!}%s: %s %d\n{EOO}", &fd, NAME,
					ERR_BLOCKSIZE, DEFAULT_BLOCKSIZE);
			e->blocksize_coef = 1;
		}
		else
			e->blocksize_coef = 512 / e->blocksize_coef;
		return ;
	}
	e->blocksize_coef = 1;
}

/*
** Set terminal window width
** Retrieve value in environment, else retrieve from system
*/

static void		get_window_w(ENV *e)
{
	int				index;
	struct winsize	w;

	if ((index = get_envvar_index(e, COLUMNS)) != -1)
		e->col.win_w = ft_atoi(e->env_var[index][1]);
	else
	{
		ioctl(0, TIOCGWINSZ, &w);
		e->col.win_w = w.ws_col;
	}
}

/*
** Scan environment tab for a variable
** Input: variable as string
** Return : corresponding index if found, -1 otherwise
*/

int				get_envvar_index(ENV *e, char *var)
{
	int		index;

	index = 0;
	while (e->env_var[index] && !ft_strequ(e->env_var[index][0], var))
		++index;
	return (index == e->nb_envvar ? -1 : index);
}

/*
** Split environment, store in char **tab by variable name
** Input: environment
** Return: Failure if malloc faile, success otherwise
*/

int				get_envvar(ENV *e, char **env)
{
	int	index;

	index = -1;
	e->env_var = (char ***)ft_memalloc(sizeof(char **) * (e->nb_envvar + 1));
	if (!e->env_var)
		return (FAIL);
	while (++index < e->nb_envvar)
		e->env_var[index] = ft_strsplit(env[index], '=');
	get_window_w(e);
	return (SUCCESS);
}

/*
** Handle environment impact
*/

void			handle_environment_var(ENV *e)
{
	set_color_ref(e);
	set_blocksize_coef(e);
	get_window_w(e);
}
