/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 18:32:40 by srapin            #+#    #+#             */
/*   Updated: 2022/11/12 03:39:22 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*pos;

	pos = 0;
	i = 0;
	if (c == '\0')
		return ((char *)s + ft_strlen(s));
	while (s[i])
	{
		if ((char)s[i] == (char)c)
			pos = (char *)s + i;
		i++;
	}
	return (pos);
}
