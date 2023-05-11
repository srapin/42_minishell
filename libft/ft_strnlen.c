/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 01:45:55 by srapin            #+#    #+#             */
/*   Updated: 2022/11/22 03:39:53 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_strnlen(const char *s, size_t n)
{
	size_t	lenght;

	lenght = 0;
	while (s[lenght] && lenght < n)
		lenght++;
	return (lenght);
}
