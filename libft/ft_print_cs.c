/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_cs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 01:38:09 by srapin            #+#    #+#             */
/*   Updated: 2022/11/22 03:22:28 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_str(t_print *to_print, char *str)
{
	int	i;

	i = 0;
	if (to_print->option[point] && ((!str && to_print->len[precision] < 6)
			|| to_print->len[precision] == 0))
		str = (char *){""};
	if (!str)
		str = (char *){"(null)"};
	if (to_print->option[point])
		to_print->len[str_len] = ft_strnlen(str, to_print->len[precision]);
	else if (to_print->type != 'c' && str)
		to_print->len[str_len] = ft_strlen(str);
	if (to_print->len[str_len] < to_print->len[width]
		&& !to_print->option[minus])
		while (i++ < to_print->len[width] - to_print->len[str_len])
			write_and_count((char *){" "}, to_print, 1);
	write_and_count(str, to_print, to_print->len[str_len]);
	if (to_print->len[str_len] < to_print->len[width]
		&& to_print->option[minus])
		while (i++ < to_print->len[width] - to_print->len[str_len])
			write_and_count((char *){" "}, to_print, 1);
}

void	ft_print_char(t_print *to_print, char c)
{
	to_print->len[str_len] = 1;
	to_print->option[point] = 0;
	ft_print_str(to_print, &c);
}
