/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deal_with_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 22:38:37 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err(t_cmd *cmd)
{
	char	*mess;

	if (errno == ENOENT)
		g_exit_status = CMD_NOT_FOUND;
	else if (errno == 13)
		g_exit_status = NO_FILE_OR_DIR;
	else if (errno == EISDIR)
		g_exit_status = CMD_NOT_EXECUTABLE;
	else
		g_exit_status = NO_FILE_OR_DIR;
	mess = ft_strjoin("minishell: ", cmd->val.value);
	perror(mess);
	free(mess);
}

void	not_found(t_cmd *cmd, t_cmd *first)
{
	print_err(cmd);
	free_cmds(&first, true);
	exit(g_exit_status);
}

void	failed_to_open_files(t_cmd *first)
{
	if (errno == 13)
		g_exit_status = NO_FILE_OR_DIR;
	else if (errno == EISDIR)
		g_exit_status = CMD_NOT_EXECUTABLE;
	else
		g_exit_status = NO_FILE_OR_DIR;
	free_cmds(&first, true);
	exit(g_exit_status);
}

void	cmd_not_found(t_cmd *cmd, t_cmd *first)
{
	char	*mess;
	char	*tmp;

	if (errno == ENOENT)
		g_exit_status = CMD_NOT_FOUND;
	else if (errno == 13)
		g_exit_status = NO_FILE_OR_DIR;
	else if (errno == EISDIR)
		g_exit_status = CMD_NOT_EXECUTABLE;
	else
		g_exit_status = NO_FILE_OR_DIR;
	tmp = ft_strjoin("minishell: ", cmd->val.value);
	mess = ft_strjoin(tmp, ": command not found\n");
	write(STDERR_FILENO, mess, ft_strlen(mess));
	free(tmp);
	free(mess);
	free_cmds(&first, true);
	exit(g_exit_status);
}
