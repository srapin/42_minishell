/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 21:56:53 by srapin            #+#    #+#             */
/*   Updated: 2023/02/12 16:58:08 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strcat(char *dst, const char *src)
{
	size_t	d_len;
	size_t	s_len;
	size_t	i;

	i = 0;
	d_len = ft_strlen(dst);
	s_len = ft_strlen(src);
	while (dst[d_len])
		d_len++;
	while (src && src[i] && src[i] != '\n')
	{
		dst[d_len + i] = src[i];
		i++;
	}
	if (src && src[i] == '\n')
	{
		dst[d_len + i] = '\n';
		i++;
	}
	dst[d_len + i] = '\0';
	return (d_len + s_len);
}
