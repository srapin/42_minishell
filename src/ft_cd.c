/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 01:12:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 14:20:46 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	go_to_home(t_cmd *cmd)
{
	char	*full_path;

	if (!ht_search(cmd->env, "HOME"))
	{
		write(STDERR_FILENO, "minishell : cd : HOME not set\n",
			ft_strlen("minishell : cd : HOME not set\n"));
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

void	print_chdir_error(char *dir)
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
		full_path = get_full_path(cmd->env, tmp);
		free(tmp);
		tmp = NULL;
	}
	if (chdir(full_path) == -1)
	{
		print_chdir_error(cmd->val.args[1]);
		free(full_path);
		full_path = NULL;
		return (NULL);
	}
	return (full_path);
}

void	set_pwd(t_cmd *cmd, char *full_path)
{
	char	*tmp;
	char	*oldpwd;

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
		write(STDERR_FILENO, "minishell : cd : too many arguments\n",
			ft_strlen("minishell : cd : too many arguments\n"));
		return (CD_TOO_MANY_ARGS);
	}
	full_path = cd_move_to(cmd);
	if (!full_path)
		return (CANNOT_ACCESS_DIR);
	set_pwd(cmd, full_path);
	return (EXIT_OK);
}
