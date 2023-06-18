/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:38:56 by srapin            #+#    #+#             */
/*   Updated: 2023/06/18 23:35:33 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		g_exit_status;

void	handle_sigint(int sig)
{
	// dprintf(1, "sigint");
	g_exit_status = 130;
	ft_putstr_fd("\n", 1);

	// ln : a décommenter !!! (juste compilait pas bien avec sous mac os)
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigint2(int sig)
{
	// dprintf(1, "sigint2");
	g_exit_status = 130;
	ft_putstr_fd("\n", 1);
	// rl_replace_line("", 1);
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