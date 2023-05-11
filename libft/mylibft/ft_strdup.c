/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 18:35:40 by srapin            #+#    #+#             */
/*   Updated: 2022/11/12 04:52:57 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	size_t	len_src;
	char	*cpy;

	len_src = 0;
	while (src[len_src])
		len_src++;
	cpy = (char *)ft_calloc(len_src + 1, sizeof(char));
	if (!cpy)
		return (NULL);
	ft_memcpy(cpy, src, len_src);
	return (cpy);
}
