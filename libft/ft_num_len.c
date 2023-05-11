/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_num_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 01:30:45 by srapin            #+#    #+#             */
/*   Updated: 2023/02/12 16:57:54 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_count_nbr_len(unsigned long long ui, int base_size)
{
	int	count;

	count = 1;
	while (ui / base_size)
	{
		ui /= base_size;
		count++;
	}
	return (count);
}

void	set_int_len(unsigned long long ui, int base_size, t_print *to_print)
{
	int	count;

	count = ft_count_nbr_len(ui, base_size);
	if (ft_strstr(&to_print->type, "di", 1) >= 0 && (to_print->option[plus]
			|| to_print->option[space] || to_print->option[neg]))
		to_print->len[prefix_len] = 1;
	else if ((ft_strstr(&to_print->type, "xX", 1) && to_print->option[hashtag])
		|| (to_print->type == 'p' && ui != 0))
		to_print->len[prefix_len] = 2;
	to_print->len[num_len] = count;
	to_print->len[num_len] = ft_max(to_print->len[num_len],
			to_print->len[precision]);
	if (to_print->option[zero])
		to_print->len[num_len] = ft_max(to_print->len[num_len],
				to_print->len[width] - to_print->len[prefix_len]);
	if (to_print->type == 'p' && ui == 0)
	{
		if (!to_print->option[point] || to_print->len[precision] > 5)
			to_print->len[num_len] = 5;
		else
			to_print->len[num_len] = 0;
		to_print->len[prefix_len] = 0;
	}
}
