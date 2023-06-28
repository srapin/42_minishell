/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 01:38:19 by srapin            #+#    #+#             */
/*   Updated: 2023/06/27 22:31:56 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcat(char *dest, char const *src)
{
	int	i;
	int	j;

	if (!dest)
		return (NULL);
	if (!src)
		return (dest);
	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j])
	{
		dest[i] = src[j];
		j++;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*join;

	join = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!join)
		return (NULL);
	ft_strcat(join, s1);
	ft_strcat(join, s2);
	return (join);
}
