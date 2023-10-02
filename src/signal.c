/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:38:56 by srapin            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status;

void	sigint_next_prompt(int sig)
{
	(void)sig;
	g_exit_status = 130;
	ft_putstr_fd("\n", STDERR_FILENO);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_during_cmd_exec(int sig)
{
	(void)sig;
	g_exit_status = 130;
	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	hd_sigint(int i)
{
	(void)i;
	g_exit_status = 130;
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	reset_defaults_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
