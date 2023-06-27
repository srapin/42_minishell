/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:00:28 by hlesny            #+#    #+#             */
/*   Updated: 2023/06/27 19:12:21 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Deletes the '..' or '../', along with the <dir_name>/ that 
comes previously, ONLY if there's something before the 
previous '/' */
void	prev_dir(char **path, int *i)
{
	int		j;
	char	*before;
	char	*after;

	if ((!(*path)[*i + 2] || (*path)[*i + 2] == '/')) // '..' ou '../'
	{
		if (*i - 2 >= 0) // ie n'est pas à la racine
		{
			/* détermine l'index du précédent '/' 
            (par ex le premier '/' de "cd ./dir1/..") */
			j = *i - 2;
				/* ie l'index de l'élément précédant le '/' 
                précédant le premier '.' */
			while (j >= 0 && (*path)[j] != '/')
				j--;
			before = ft_substr(*path, 0, j + 1);
		}
		else // ie est à la racine -> supprime alors juste '..' (ou '../')
			before = ft_strdup("/");
		after = ft_substr(*path, *i + 3, ft_strlen(*path));
		if (*i - 2 >= 0)
			*i = j + 1;
		else
			*i = 0;
		set_path(path, before, after);
	}
}

void	del_slashes(char **path)
{
	int		i;
	int		j;
	char	*before;
	char	*after;

	i = 0;
	j = 0;
	while ((*path)[i])
	{
		if (((*path))[i] == '/')
		{
			j = i;
			while ((*path)[j] && (*path)[j] == '/')
				j++;
			before = ft_substr(*path, 0, i + 1);
			after = ft_substr(*path, j, ft_strlen(*path));
			set_path(path, before, after);
		}
		i++;
	}
}

char	*replace_prev_or_actual_dir(char *path)
{
	int		i;

	i = 0;
	if (path[0] && path[0] == '/' && path[1] && path[1] == '/' && !path[2])
		return (path);
	while (path[i])
	{
		if (path[i] == '.' && path[i - 1] == '/') // '.' ou './'
		{
			// simply deletes the '.' or './'
			if ((!path[i + 1] || path[i + 1] == '/'))
				curr_dir(&path, i);
			else if (path[i + 1] && path[i + 1] == '.')
				prev_dir(&path, &i);
		}
		else
			i++;
	}
	del_slashes(&path);
	return (path);
}