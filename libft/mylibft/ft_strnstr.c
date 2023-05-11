/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 22:20:45 by srapin            #+#    #+#             */
/*   Updated: 2023/03/02 00:20:59 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	len_to_find;

	i = -1;
	if (!*to_find)
		return ((char *)(str));
	len_to_find = ft_strlen(to_find);
	while (++i < len && str[i])
		if (ft_strncmp((char *)str + i, (char *)to_find, len_to_find) == 0 && i
			+ len_to_find <= len)
			return ((char *)(str + i));
	return (NULL);
}
