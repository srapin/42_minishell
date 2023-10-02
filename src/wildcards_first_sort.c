/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_first_sort.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 22:50:42 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_filename	*check_prefix_suffix(char *current_dir, DIR *dir,
			char *prefix, char *suffix)
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
		{
			curr_filename = search_from_end(dir_content->d_name, suffix);
			if (curr_filename)
				add_filename(&filenames, dir_content->d_name);
		}
		dir_content = readdir(dir);
	}
	if (closedir(dir) == -1)
		print_error_wildcard_opendir("minishell : closedir : ", current_dir);
	return (filenames);
}

t_filename	*check_prefix(char *current_dir, DIR *dir, char *prefix)
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
	if (closedir(dir) == -1)
		print_error_wildcard_opendir("minishell : closedir : ", current_dir);
	return (filenames);
}

t_filename	*check_suffix(char *current_dir, DIR *dir, char *suffix)
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
	if (closedir(dir) == -1)
		print_error_wildcard_opendir("minishell : closedir : ", current_dir);
	return (filenames);
}

t_filename	*parse_current_dir(char *current_dir, DIR *dir,
			char *prefix, char *suffix)
{
	size_t	prefix_len;
	size_t	suffix_len;

	prefix_len = ft_strlen(prefix);
	suffix_len = ft_strlen(suffix);
	if (prefix_len && suffix_len)
		return (check_prefix_suffix(current_dir, dir, prefix, suffix));
	else if (prefix_len)
		return (check_prefix(current_dir, dir, prefix));
	else
		return (check_suffix(current_dir, dir, suffix));
}

/*
Retourne les filenames associes au wildcard,
	ou NULL lorsqu'aucun filename ne correspond 
IL S'AGIT DU PREMIER TRI
DIR *dir, char *
Note : readdir() renvoie NULL si il n'y a plus d'entité de directory à lire.
*/
t_filename	*first_sort(t_data *data, char *prefix, char *suffix)
{
	char	*current_dir;
	DIR		*dir;
	size_t	prefix_len;
	size_t	suffix_len;

	current_dir = *get_pwd(data->env);
	if (!current_dir)
		return (NULL);
	dir = opendir(current_dir);
	if (!dir)
	{
		print_error_wildcard_opendir("minishell : opendir : ", current_dir);
		return (NULL);
	}
	if (!prefix || !suffix)
		return (NULL);
	prefix_len = ft_strlen(prefix);
	suffix_len = ft_strlen(suffix);
	if (!prefix_len && !suffix_len)
		return (return_entire_dir_content(current_dir, dir));
	return (parse_current_dir(current_dir, dir, prefix, suffix));
}
