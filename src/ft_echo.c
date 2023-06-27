/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:31:24 by srapin            #+#    #+#             */
/*   Updated: 2023/06/27 19:19:45 by hlesny           ###   ########.fr       */
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

int	no_newline(char *arg)
{
	int	i;

	i = 1;
	if (!(*arg))
		return (0);
	if (*arg && *arg != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	write_on_std(t_cmd *cmd, int i)
{
	int	j;
	int	ret;

	j = i;
	ret = 0;
	while (cmd->val.args[j] && ret >= 0)
	{
		ret = write(1, cmd->val.args[j], ft_strlen(cmd->val.args[j]));
		if (ret >= 0 && cmd->val.args[j + 1])
			ret = write(1, " ", 1);
		j++;
	}
	return (ret);
}

int	ft_echo(t_cmd *cmd, t_cmd *first)
{
	char	end[2];
	int		i;
	int		ret;

	(void)first;
	end[0] = '\n';
	end[1] = '\0';
	i = 1;
	ret = 0;
	if (cmd->val.args[i] && no_newline(cmd->val.args[i]))
	{
		end[0] = '\0';
		while (cmd->val.args[i] && no_newline(cmd->val.args[i]))
			i++;
	}
	ret = write_on_std(cmd, i);
	if (ret >= 0)
		ret = write(1, end, ft_strlen(end));
	if (ret < 0)
		return (write_error(cmd));
	return (EXIT_OK);
}
