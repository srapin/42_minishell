/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 01:39:46 by srapin            #+#    #+#             */
/*   Updated: 2022/11/13 00:10:19 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	end;
	int	s1_len;

	if (!s1 || !set)
		return (NULL);
	s1_len = ft_strlen(s1);
	start = 0;
	end = s1_len - 1;
	while (start < s1_len && ft_strchr(set, s1[start]))
		start++;
	while (end > 0 && end > start && ft_strchr(set, s1[end]))
		end--;
	return (ft_substr(s1, start, end - start + 1));
}
