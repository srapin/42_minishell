/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 01:36:40 by srapin            #+#    #+#             */
/*   Updated: 2022/11/22 03:26:02 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *format, ...)
{
	t_print	to_print;
	va_list	args;
	char	*str;

	str = (char *)format;
	if (!str || str[0] == '\0')
		return (0);
	va_start(args, format);
	ft_init_struct(&to_print);
	while (str && *str)
	{
		if (str[0] == '%')
			str = parse_format(str, &to_print, args);
		else if (str && *str)
			write_and_count(str, &to_print, 1);
		str++;
	}
	va_end(args);
	return (to_print.counter);
}
