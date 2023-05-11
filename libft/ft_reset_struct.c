/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reset_struct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 01:40:02 by srapin            #+#    #+#             */
/*   Updated: 2022/11/22 03:39:11 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_reset_struct(t_print *to_print)
{
	int	i;

	i = -1;
	to_print->type = 0;
	while (++i < 8)
		to_print->option[i] = 0;
	i = -1;
	while (++i < 4)
		to_print->len[i] = 0;
}

void	ft_init_struct(t_print *to_print)
{
	ft_bzero(to_print, sizeof(t_print));
}
