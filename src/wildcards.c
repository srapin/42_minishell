/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:13:13 by Helene            #+#    #+#             */
/*   Updated: 2023/06/24 11:33:50 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// retourne la string suivant le dernier '*' non contenu dans des quotes
char	*get_suffix(t_token_list *current)
{
	int		i;
	// t_word_data	*wd;

	// wd = current->merged_words;
	i = current->length - 1;
	while (i >= 0)
	{
		if (current->content[i] == '*' && !is_in_quotes(current, i))
			return (ft_substr(current->content, i + 1, current->length));
		i--;
	}
	return (NULL);
}

void	expand_wildcards_in_current(t_data *data, DIR *dir,
		t_token_list *current)
{
	char		*prefix;
	char		*suffix;
	char		*wildcard_start;
	size_t		wildcard_index;
	t_filename	*filenames;

	wildcard_start = ft_strchr(current->content, '*');
	if (wildcard_start)
	{
		wildcard_index = current->length - ft_strlen(wildcard_start);
		while (wildcard_start && is_in_quotes(current, wildcard_index))
		{
			wildcard_start = ft_strchr(wildcard_start, '*');
			wildcard_index = current->length - ft_strlen(wildcard_start);
		}
		if (wildcard_start)
		{
			prefix = ft_substr(current->content, 0, wildcard_index);
			suffix = get_suffix(current);
			filenames = first_sort(dir, prefix, suffix);
			second_sort(&filenames, current, prefix);
			free_and_null(prefix, suffix);
			insert_filenames(data->first, &current, &filenames);
		}
	}
}

void	parse_and_expand_wildcards(t_data *data, DIR *dir)
{
	t_token_list	*current;
	// t_filename		*filenames;

	current = *(data->first);
	while (current)
	{
		if (current->type == l_parenthesis)
		{
			while (current && current->type != r_parenthesis)
				current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		expand_wildcards_in_current(data, dir, current);
		current = current->next;
	}
}

/*
Une fois qu'a choppé les prefix et suffix,
	fait un premier tri dans la liste des d_name
pour trouver ceux ayant des prefixes et suffixes identiques.
Une fois qu'a fait ce premier tri, itère sur chaque d_name qui matchait, 
et essayant cette fois ci de trouver des matchs pour les wildcards a l'interieur du mot (s'il y en a)
Si on arrive au dernier index du current d_name sans etre arrivé à la fin du mot,
	alors supprime ce d_name
de la liste de d_name résultant du premier tri.
*/
void	perform_wildcard_exp(t_data *data)
{
	char *current_dir;
	DIR *dir;

	current_dir = *get_pwd(data->env);
	if (!current_dir)
		return ; // ?
	dir = opendir(current_dir);
	if (!dir)
	{
		perror("opendir ");
		return ;
	}
	parse_and_expand_wildcards(data, dir);
	if (closedir(dir) == -1)
		perror("closedir ");
}