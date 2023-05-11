/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 22:24:50 by srapin            #+#    #+#             */
/*   Updated: 2022/11/19 00:07:19 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_tab(char ***s, int i)
{
	while (i-- > 0)
		free((*s)[i]);
	free(*s);
	*s = NULL;
}

static char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

static int	count_words(const char *str, char charset)
{
	int	i;
	int	cpt;

	i = 0;
	cpt = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == charset))
			i++;
		if (str[i] && !(str[i] == charset))
		{
			cpt++;
			while (str[i] && !(str[i] == charset))
				i++;
		}
	}
	return (cpt);
}

static void	fill_words(char **split, const char *str, char charset, int words)
{
	int	j;
	int	k;
	int	i;

	i = 0;
	j = 0;
	k = 0;
	while (i < words)
	{
		while (str[j] && charset == str[j])
			j++;
		k = 0;
		while (str[j + k] && !(str[j + k] == charset))
			k++;
		split[i] = ft_calloc((k + 1), sizeof(char));
		if (!split[i])
		{
			free_tab(&split, i);
		}
		if (split[i] != 0)
			ft_strncpy(split[i], &str[j], k);
		j += k;
		i++;
	}
	split[i] = 0;
}

char	**ft_split(const char *str, char charset)
{
	char	**split;
	int		words;

	if (str == NULL)
		return (NULL);
	words = (count_words(str, charset));
	split = ft_calloc((words + 1), sizeof(char *));
	if (!split)
		return (NULL);
	fill_words(split, str, charset, words);
	return (split);
}
