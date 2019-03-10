/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emuckens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/31 12:25:03 by emuckens          #+#    #+#             */
/*   Updated: 2018/12/02 18:13:07 by emuckens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		get_padding_info(ENV *e, t_file *file)
{
	float	tmpbs;

	tmpbs = file->blocksize * e->blocksize_coef;
	if ((file->len.ulen = (unsigned char)ft_strlen(file->uid)) > e->max.ulen)
		e->max.ulen = file->len.ulen;
	if ((file->len.nlen = (unsigned char)ft_strlen(file->name)) > e->max.nlen)
		e->max.nlen = file->len.nlen;
	if ((file->len.glen = (unsigned char)ft_strlen(file->gid)) > e->max.glen)
		e->max.glen = file->len.glen;
	if ((file->len.llen = (char)ft_nblen(file->links, 10)) > e->max.llen)
		e->max.llen = file->len.llen;
	if ((file->len.slen = (char)ft_nblen(file->size, 10)) > e->max.slen)
		e->max.slen = file->len.slen;
	if ((file->len.majlen = (char)ft_strlen(file->major)) > e->max.majlen)
		e->max.majlen = file->len.majlen;
	if ((file->len.minlen = (char)ft_strlen(file->minor)) > e->max.minlen)
		e->max.minlen = file->len.minlen;
	if ((file->len.ilen = (char)ft_nblen(file->inode, 10)) > e->max.ilen)
		e->max.ilen = file->len.ilen;
	if ((file->len.blen = (char)ft_nblen(tmpbs, 10)) > e->max.blen)
		e->max.blen = file->len.blen;
	if (file->type == CHAR || file->type == BLOCK)
		e->max.slen = e->max.majlen + e->max.minlen + 4;
}

void		reset_padding(ENV *e)
{
	ft_bzero(&e->max, sizeof(e->max));
	e->max.nlen = MIN_COLW;
}
