/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 01:35:10 by srapin            #+#    #+#             */
/*   Updated: 2022/11/22 03:44:42 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*set_prec(t_print *to_print, char *format)
{
	if (format && *format == '.')
	{
		format++;
		to_print->option[point] = 1;
		to_print->option[zero] = 0;
		while (format[0] == '0')
			format++;
		if (ft_strstr("0123456789", format, 1) >= 0)
		{
			to_print->len[precision] = ft_atoi(format);
			format += ft_count_nbr_len(to_print->len[precision], 10);
		}
	}
	return (format);
}

char	*set_option(t_print *to_print, char *format)
{
	int	next;

	next = ft_strstr(OPTIONS_SYMBOL, format, 1);
	while (format && *format && next >= 0)
	{
		to_print->option[next] = 1;
		format++;
		next = ft_strstr(OPTIONS_SYMBOL, format, 1);
	}
	if (format && *format && ft_strstr(DIGIT_SYMBOL, format, 1) >= 0)
	{
		to_print->len[width] = ft_atoi(format);
		format += ft_count_nbr_len(to_print->len[width], 10);
	}
	format = set_prec(to_print, format);
	return (format);
}

char	*parse_format(char *format, t_print *to_print, va_list test)
{
	int	next_char;

	format++;
	next_char = ft_strstr(FORMAT_SYMBOL, format, 1);
	if (next_char < 0)
		return (format);
	format = set_option(to_print, format);
	next_char = ft_strstr(TYPES_SYMBOL, format, 1);
	if (next_char < 9)
	{
		to_print->type = TYPES_SYMBOL[next_char];
		ft_print_arg(to_print, test);
	}
	return (format);
}
