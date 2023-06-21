/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_first_sort.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 22:50:42 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/21 04:29:03 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_a_hidden_dir(struct dirent *dir_content)
{
	if (ft_strlen(dir_content->d_name) && *(dir_content->d_name) == '.')
		return (1);
	return (0);
}

void	add_filename(t_filename **filenames, char *new)
{
	t_filename	*current;
	t_filename	*new_path;

	if (!new || !(*filenames))
		return ;
	new_path = ft_calloc(sizeof(t_filename), 1);
	if (!new_path)
		return ;
	new_path->filename = ft_strdup(new);
	if (!(*filenames)->filename)
	{
		free(*filenames);
		(*filenames) = new_path;
	}
	else
	{
		current = (*filenames);
		while (current->next)
			current = current->next;
		current->next = new_path;
	}
}

/*
Renvoie NULL si le nom du directory stream donne en argument ne commence pas par prefix,
et pointe vers le charactere suivant la fin du prefix dans la string dans le cas contraire
*/
char	*search_from_start(char *d_name, char *prefix)
{
	int	i;

	i = 0;
	while (d_name[i] && prefix[i])
	{
		if (d_name[i] != prefix[i])
			return (NULL);
		i++;
	}
	if (prefix[i])
		return (NULL);
	return (d_name);
}

char	*search_from_end(char *d_name, char *suffix)
{
	int	i;
	int	j;

	i = ft_strlen(d_name) - 1;
	j = ft_strlen(suffix) - 1;
	while (i >= 0 && j >= 0)
	{
		if (d_name[i] != suffix[j])
			return (NULL);
		i--;
		j--;
	}
	if (j >= 0) // ie n'a pas parcouru tout le suffix
		return (NULL);
	return (d_name);
}

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

t_filename	*return_entire_dir_content(DIR *dir)
{
	t_filename		*filenames;
	struct dirent	*dir_content;

	filenames = ft_calloc(sizeof(t_filename), 1);
	if (!filenames)
		return (NULL);
	dir_content = readdir(dir);
	while (dir_content)
	{
		if (!is_a_hidden_dir(dir_content))
			add_filename(&filenames, dir_content->d_name);
		dir_content = readdir(dir);
	}
	return (filenames);
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