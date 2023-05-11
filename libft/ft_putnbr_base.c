/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 03:29:37 by srapin            #+#    #+#             */
/*   Updated: 2022/11/22 03:38:25 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_pnb_rec(unsigned long long nbr, char *base, t_print *to_print,
		int min_len)
{
	unsigned long long	base_size;

	base_size = ft_strlen(base);
	if (nbr >= base_size || min_len)
		ft_pnb_rec(nbr / base_size, base, to_print, --min_len);
	write_and_count(&base[nbr % base_size], to_print, 1);
}

void	print_prefix(t_print *to_print)
{
	if ((to_print->type == 'x' && to_print->option[hashtag])
		|| to_print->type == 'p')
		write_and_count((char *){"0x"}, to_print, 2);
	else if (to_print->type == 'X' && to_print->option[hashtag])
		write_and_count((char *){"0X"}, to_print, 2);
	else if (ft_strstr("di", &to_print->type, 1) >= 0)
	{
		if (to_print->option[neg])
			write_and_count((char *){"-"}, to_print, 1);
		else if (to_print->option[plus])
			write_and_count((char *){"+"}, to_print, 1);
		else if (to_print->option[space])
			write_and_count((char *){" "}, to_print, 1);
	}
}

void	fill_with_blank(t_print *to_print)
{
	int	i;

	i = 0;
	while (i++ < to_print->len[width] - to_print->len[num_len]
		- to_print->len[prefix_len])
		write_and_count(&(char){' '}, to_print, 1);
}

void	ft_putnbr_base(unsigned long long nbr, char *base, t_print *to_print)
{
	if (to_print->option[point] && to_print->len[precision] == 0 && nbr == 0)
		to_print->len[num_len] = 0;
	if (!to_print->option[minus])
		fill_with_blank(to_print);
	if (to_print->type == 'p' && nbr == 0)
	{
		if (!to_print->option[point] || to_print->len[precision] > 5)
			write_and_count((char *){"(nil)"}, to_print, 5);
	}
	else
	{
		print_prefix(to_print);
		if (!(to_print->option[point] && to_print->len[precision] == 0
				&& nbr == 0))
			ft_pnb_rec(nbr, base, to_print, to_print->len[num_len] - 1);
	}
	if (to_print->option[minus])
		fill_with_blank(to_print);
}
