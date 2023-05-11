/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:01:55 by srapin            #+#    #+#             */
/*   Updated: 2022/11/12 04:40:49 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*mem_zon;
	size_t			i;

	mem_zon = s;
	i = 0;
	while (i < n)
	{
		mem_zon[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
