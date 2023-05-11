/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 01:36:41 by srapin            #+#    #+#             */
/*   Updated: 2022/11/26 20:07:38 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	write_and_count(char *str, t_print *to_print, int len)
{
	int	i;

	i = write(1, str, len);
	if (i < 0 || to_print->counter < 0)
		to_print->counter = -1;
	else
		to_print->counter += i;
}
