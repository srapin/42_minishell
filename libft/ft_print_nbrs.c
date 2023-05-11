/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nbrs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 01:38:55 by srapin            #+#    #+#             */
/*   Updated: 2022/11/22 03:24:01 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_int(t_print *to_print, long i)
{
	if (i < 0)
	{
		to_print->option[neg] = 1;
		to_print->len[prefix_len] = 1;
		i *= -1;
	}
	set_int_len(i, 10, to_print);
	ft_putnbr_base(i, "0123456789", to_print);
}

void	ft_print_hex(t_print *to_print, unsigned int ui, int maj)
{
	if (ui == 0)
		to_print->option[hashtag] = 0;
	if (to_print->option[hashtag])
		to_print->len[prefix_len] = 2;
	set_int_len(ui, 16, to_print);
	if (maj)
		ft_putnbr_base(ui, "0123456789ABCDEF", to_print);
	else
		ft_putnbr_base(ui, "0123456789abcdef", to_print);
}

void	ft_print_unsigned(t_print *to_print, unsigned int i)
{
	set_int_len(i, 10, to_print);
	ft_putnbr_base(i, "0123456789", to_print);
}

void	ft_print_ptr(t_print *to_print, unsigned long long i)
{
	to_print->len[prefix_len] = 2;
	set_int_len(i, 16, to_print);
	ft_putnbr_base(i, "0123456789abcdef", to_print);
}
