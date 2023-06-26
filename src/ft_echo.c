/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:31:24 by srapin            #+#    #+#             */
/*   Updated: 2023/06/26 19:03:23 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	write_error(t_cmd *cmd)
{
	char	*err_mess;

	err_mess = ft_strjoin("minishell: ", cmd->val.value);
	perror(err_mess);
	free(err_mess);
	return (EXIT_FAILURE);
}

int	ft_echo(t_cmd *cmd, t_cmd *first)
{
	char	end[2];
	int		i;
	int		ret;

	(void) first;
	end[0] = '\n';
	end[1] = '\0';
	i = 1;
	ret = 0;
	if (ft_strisequal(cmd->val.args[i], "-n"))
	{
		i++;
		end[0] = '\0';
	}
	while (cmd->val.args[i] && ret >= 0)
	{
        ret = write(1, cmd->val.args[i], ft_strlen(cmd->val.args[i]));
		i++;
		if (ret >= 0 && cmd->val.args[i])
            ret = write(1, " ", 1);
	}
    if (ret >= 0)
	{
		ret = write(1, end, ft_strlen(end));
	}
	if (ret < 0)
	{
		return (write_error(cmd));
	}
	return (EXIT_OK);
}