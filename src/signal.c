/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:38:56 by srapin            #+#    #+#             */
/*   Updated: 2023/06/27 17:39:10 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		g_exit_status;

void sigint_next_prompt(int sig)
{
	(void) sig;
	g_exit_status = 130;
	ft_putstr_fd("\n", STDERR_FILENO);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_during_cmd_exec(int sig)
{
	(void) sig;
	g_exit_status = 130;
	ft_putstr_fd("\n", STDOUT_FILENO);
}

// void	handle_sigquit(int sig)
// {
// 	if (sig == 2)
// 	{
// 		g_exit_status = 130;
// 		ft_putstr_fd("\n", 1);
// 	}
// }