/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deal_with_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 22:38:37 by srapin            #+#    #+#             */
/*   Updated: 2023/06/27 22:45:07 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err(t_cmd *cmd)
{
	char	*mess;

	mess = ft_strjoin("minishell: ", cmd->val.value);
	perror(mess);
	free(mess);
}

void	not_found(t_cmd *cmd, t_cmd *first)
{
	print_err(cmd);
	free_cmds(&first, true);
	exit(CMD_NOT_FOUND);
}

void	failed_to_open_files(t_cmd *first)
{
	free_cmds(&first, true);
	exit(CMD_NOT_FOUND);
}

void	cmd_not_found(t_cmd *cmd, t_cmd *first)
{
	char	*mess;
	char	*tmp;

	tmp = ft_strjoin("minishell: ", cmd->val.value);
	mess = ft_strjoin(tmp, ": command not found\n");
	write(STDERR_FILENO, mess, ft_strlen(mess));
	free(tmp);
	free(mess);
	free_cmds(&first, true);
	exit(CMD_NOT_FOUND);
}
