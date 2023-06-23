/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_first_sort.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 22:50:42 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/23 15:32:56 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_filename	*check_prefix_suffix(DIR *dir, char *prefix, char *suffix)
{
	char			*curr_filename;
	struct dirent	*dir_content;
	t_filename		*filenames;

	filenames = ft_calloc(sizeof(t_filename), 1);
	if (!filenames)
		return (NULL);
	dir_content = readdir(dir);
	while (dir_content)
	{
		if (prefix && *prefix != '.' && is_a_hidden_dir(dir_content))
			;
		curr_filename = search_from_start(dir_content->d_name, prefix);
		if (curr_filename)
		{
			curr_filename = search_from_end(dir_content->d_name, suffix);
			if (curr_filename)
				add_filename(&filenames, dir_content->d_name);
		}
		dir_content = readdir(dir);
	}
	return (filenames);
}

t_filename	*check_prefix(DIR *dir, char *prefix)
{
	char			*curr_filename;
	struct dirent	*dir_content;
	t_filename		*filenames;

	filenames = ft_calloc(sizeof(t_filename), 1);
	if (!filenames)
		return (NULL);
	dir_content = readdir(dir);
	while (dir_content)
	{
		curr_filename = search_from_start(dir_content->d_name, prefix);
		if (curr_filename)
			add_filename(&filenames, dir_content->d_name);
		dir_content = readdir(dir);
	}
	return (filenames);
}

t_filename	*check_suffix(DIR *dir, char *suffix)
{
	char			*curr_filename;
	struct dirent	*dir_content;
	t_filename		*filenames;

	filenames = ft_calloc(sizeof(t_filename), 1);
	if (!filenames)
		return (NULL);
	dir_content = readdir(dir);
	while (dir_content)
	{
		curr_filename = search_from_end(dir_content->d_name, suffix);
		if (curr_filename)
			add_filename(&filenames, dir_content->d_name);
		dir_content = readdir(dir);
	}
	return (filenames);
}

t_filename	*parse_current_dir(DIR *dir, char *prefix, char *suffix)
{
	size_t			prefix_len;
	size_t			suffix_len;
	char			*curr_filename;
	struct dirent	*dir_content;
	t_filename		*filenames;

	filenames = ft_calloc(sizeof(t_filename), 1);
	if (!filenames)
		return (NULL);
	prefix_len = ft_strlen(prefix);
	suffix_len = ft_strlen(suffix);
	dir_content = readdir(dir);
	if (prefix_len && suffix_len)
		return (check_prefix_suffix(dir, prefix, suffix));
	else if (prefix_len)
		return (check_prefix(dir, prefix));
	else
		return (check_suffix(dir, suffix));
}

/*
Retourne les filenames associes au wildcard,
	ou NULL lorsqu'aucun filename ne correspond 
IL S'AGIT DU PREMIER TRI
DIR *dir, char *
Note : readdir() renvoie NULL si il n'y a plus d'entité de directory à lire.
*/
t_filename	*first_sort(DIR *dir, char *prefix, char *suffix)
// prefix and suffix indexes in the big (aka merged, final) word
{
	size_t prefix_len;
	size_t suffix_len;
	struct dirent *dir_content;

	if (!prefix || !suffix)
		return (NULL);

	prefix_len = ft_strlen(prefix);
	suffix_len = ft_strlen(suffix);

	if (!prefix_len && !suffix_len)
		return (return_entire_dir_content(dir));

	if (prefix_len == 2 && prefix[0] == '.' && prefix[1] == '.')
		return (NULL);
	return (parse_current_dir(dir, prefix, suffix));
}