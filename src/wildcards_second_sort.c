/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_second_sort.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 01:36:57 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/22 16:47:30 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ft_search_str_in_str() renvoie NULL en cas d'échec
char	*ft_search_str_in_str(char *s, char *to_find)
{
	int	i;
	int	j;

	if (!s || ft_strlen(to_find) > ft_strlen(s))
		return (NULL);
	if (!ft_strlen(to_find))
		return (s);
	i = -1;
	j = 0;
	while (s[++i])
	{
		if (s[i] == to_find[j])
		{
			while (s[i] == to_find[j])
			{
				i++;
				j++;
			}
			if (s[i] || (!s[i] && !to_find[j]))
				return (&s[i - j + 1]);
			i -= j;
			j = 0;
		}
	}
	return (NULL);
}

int	get_next_wcard_index(t_token_list *current, size_t index)
{
	int	i;

	i = index;
	while (current->content[i])
	{
		if (current->content[i] == '*' && !is_in_quotes(current, i))
			return (i);
		i++;
	}
	return (-1);
}

void	del_filename(t_filename **first, t_filename *to_del)
{
	t_filename	*current;

	current = (*first);
	while (current->next && current->next != to_del)
		current = current->next;
	if (!current->next)
		return ;
	current->next = to_del->next;
	free(to_del->filename);
	free(to_del);
	to_del = NULL;
}

void	parse_current_filename(t_filename **filenames, t_filename *current_f,
		t_token_list *current, size_t prefix_len)
{
	int		i;
	int		next_wcard_index;
	char	*filename_pos;

	i = prefix_len + 1;
	filename_pos = current_f->filename + prefix_len;
	while (i < current->length)
	{
		next_wcard_index = get_next_wcard_index(current, i);
		if (next_wcard_index >= 0)
		{
			filename_pos = ft_search_str_in_str(filename_pos,
					ft_substr(current->content, i, next_wcard_index - i));
			if (!filename_pos)
			{
				del_filename(filenames, current_f);
				break ;
			}
			i = next_wcard_index + 1;
			while (current->content[i] && current->content[i] == '*'
				&& !is_in_quotes(current, i))
				i++;
		}
		else
			break ;
	}
}

/*
Passe le prefix. 
Tombe ainsi sur un '*'. Itère sur le d_name jusqu'a ce que 
l'on trouve dans le d_name la meme string que
celle suivant le '*' trouvé dans le mot originel,
	ou que l'on ait parcouru l'entièreté du d_name. 
Si ce qui suit le '*' est la chaine vide (ie on a "*\0"),
	arrête et recommence l'opération sur le d_name suivant.
Sinon : en continuant dans le mot originel,
	si tombe sur un autre '*' : si ce qui suit est non vide :
    Si a entièrement parcouru le d_name précédemment, supprime le d_name 
    Sinon,
	itère à nouveau sur le d_name jusqu'a trouver la première apparition de la string recherchée,
    ou jusqu'à ce que la fin du d_name soit reached.
Etc...    int i;

prefix
Sous chaine entre deux '*' : 
    taille de la sous chaine : next_wcard_index - (curr_wcard_index + 1)
    index du debut de la sous chaine : curr_wcard_index + 1
*/
void	second_sort(t_filename **filenames, t_token_list *current, char *prefix)
{
	size_t		prefix_len;
	t_filename	*current_f;

	if (!filenames)
		return ;
	if (!(*filenames))
		return ;
	prefix_len = ft_strlen(prefix);
	current_f = (*filenames);
	while (current_f)
	{
		parse_current_filename(filenames, current_f, current, prefix_len);
		current_f = current_f->next;
	}
}
