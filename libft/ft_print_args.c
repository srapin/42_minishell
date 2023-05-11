/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 01:39:27 by srapin            #+#    #+#             */
/*   Updated: 2022/11/22 03:22:08 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_arg(t_print *to_print, va_list args)
{
	if (to_print->type == '%')
		ft_print_mod(to_print);
	else if (to_print->type == 'c')
		ft_print_char(to_print, va_arg(args, int));
	else if (to_print->type == 's')
		ft_print_str(to_print, va_arg(args, char *));
	else if (to_print->type == 'd' || to_print->type == 'i')
		ft_print_int(to_print, va_arg(args, int));
	else if (to_print->type == 'x')
		ft_print_hex(to_print, va_arg(args, unsigned int), 0);
	else if (to_print->type == 'X')
		ft_print_hex(to_print, va_arg(args, unsigned int), 1);
	else if (to_print->type == 'u')
		ft_print_unsigned(to_print, va_arg(args, unsigned int));
	else if (to_print->type == 'p')
		ft_print_ptr(to_print, (unsigned long long)va_arg(args, void *));
	ft_reset_struct(to_print);
}
