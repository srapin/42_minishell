/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_first_sort_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 15:31:52 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
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
Renvoie NULL si le nom du directory stream donne
en argument ne commence pas par prefix,
et pointe vers le charactere suivant la fin du
prefix dans la string dans le cas contraire
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
	if (j >= 0)
		return (NULL);
	return (d_name);
}

t_filename	*return_entire_dir_content(char *current_dir, DIR *dir)
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
	if (closedir(dir) == -1)
		print_error_wildcard_opendir("minishell : closedir : ", current_dir);
	return (filenames);
}
