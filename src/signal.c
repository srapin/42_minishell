/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:38:56 by srapin            #+#    #+#             */
/*   Updated: 2023/06/22 19:52:31 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		g_exit_status;

void sigint_next_prompt(int sig)
{
	// ////dprintf(1, "sigint");
	g_exit_status = 130;
	ft_putstr_fd("\n", 1);

	
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_during_cmd_exec(int sig)
{
	g_exit_status = 130;
	ft_putstr_fd("\n", 1);
}

void	handle_sigquit2(int sig)
{
}
void	handle_sigquit(int sig)
{
	if (sig == 2)
	{
		ft_putstr_fd("\n", 1);
		g_exit_status = 130;
	}
}