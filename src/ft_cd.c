/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 01:12:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/27 17:29:24 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Obtient le path complet (chemin absolu) depuis la racine
Néanmoins, des './' ou '../' peuvent encore être présents
*/
char	*get_full_path(t_ht_hash_table *env, char *arg_path)
{
	char	*pwd;

	pwd = *get_pwd(env); // que faire si pwd = NULL ?
	return (ft_strjoin(pwd, arg_path));
}

void	set_path(char **path, char *before, char *after)
{
	free(*path);
	*path = ft_strjoin(before, after);
	free(before);
	free(after);
	before = NULL;
	after = NULL;
}

void	curr_dir(char **path, int i)
{
	char	*before;
	char	*after;

	before = ft_substr(*path, 0, i);
	after = ft_substr(*path, i + 2, ft_strlen(*path));
	set_path(path, before, after);
}

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
			// détermine l'index du précédent '/' (par ex le premier '/' de "cd ./dir1/..")
			j = *i - 2;
				// ie l'index de l'élément précédant le '/' précédant le premier '.'
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

int	go_to_home(t_cmd *cmd)
{
	char	*full_path;

	if (!ht_search(cmd->env, "HOME"))
	{
		write(STDERR_FILENO, "minishell : cd : HOME not set\n", ft_strlen("minishell : cd : HOME not set\n"));
		return (HOME_NOT_SET);
	}
	full_path = ft_strdup(ht_search(cmd->env, "HOME"));
	if (chdir(full_path) == -1)
	{
		perror("chdir ");
		free(full_path);
		return (CANNOT_ACCESS_DIR);
	}
	if (!ht_modify_value(cmd->env, "PWD", full_path))
		ht_insert_item(cmd->env, ft_strdup("PWD"), full_path);
	update_pwd(cmd->env, full_path);
	return (EXIT_OK);
}

void 	print_chdir_error(char *dir)
{
	char	*mess;
	
	mess = ft_strjoin("minishell : cd : ", dir);
	perror(mess);
	free(mess);
}

char	*cd_move_to(t_cmd *cmd)
{
	char	*full_path;
	char	*tmp;

	if (cmd->val.args[1][0] == '/')
		full_path = ft_strdup(cmd->val.args[1]);
	else
	{
		tmp = ft_strdup(cmd->val.args[1]);
		if (ft_strcmp(*get_pwd(cmd->env), "/")) // ie $PWD != "/"
		{
			free(tmp);
			tmp = ft_strjoin("/", cmd->val.args[1]);
		}
		full_path = get_full_path(cmd->env, tmp);
		free(tmp);
		tmp = NULL;
	}
	if (chdir(full_path) == -1)
	{
		print_chdir_error(cmd->val.args[1]);
		//perror("chdir ");
		free(full_path);
		full_path = NULL;
		return (NULL);
	}
	return (full_path);
}

void	set_pwd(t_cmd *cmd, char *full_path)
{
	char	*tmp;
	char 	*oldpwd;

	tmp = ft_strdup(full_path);
	free(full_path);
	full_path = replace_prev_or_actual_dir(tmp);
	if (ft_strlen(full_path) > 1 && full_path[ft_strlen(full_path) - 1] == '/'
		&& full_path[1] != '/')
	{
		tmp = ft_substr(full_path, 0, ft_strlen(full_path) - 1);
		free(full_path);
		full_path = tmp;
	}
	oldpwd = ft_strdup(*get_pwd(cmd->env));
	if (!ht_modify_value(cmd->env, "OLDPWD", oldpwd))
		ht_insert_item(cmd->env, ft_strdup("OLDPWD"), oldpwd);
	if (!ht_modify_value(cmd->env, "PWD", full_path))
		ht_insert_item(cmd->env, ft_strdup("PWD"), full_path);
	update_pwd(cmd->env, full_path);
}

/* 
Implements the cd builtin with only a relative or absolute path.
Returns 0 IF directory is successfully changed
*/
int	ft_cd(t_cmd *cmd, t_cmd *first)
{
	char	*full_path;

	(void) first;
	full_path = NULL;
	if (!cmd->val.args[1])
		return (go_to_home(cmd));
	if (cmd->val.args[2])
	{
		write(STDERR_FILENO, "minishell : cd : too many arguments\n", ft_strlen("minishell : cd : too many arguments\n"));
		return (CD_TOO_MANY_ARGS);
	}
	full_path = cd_move_to(cmd);
	if (!full_path)
		return (CANNOT_ACCESS_DIR);
	// si le dossier est accessible, et que s'y est bien déplacé :
	set_pwd(cmd, full_path);
	return (EXIT_OK);
}
