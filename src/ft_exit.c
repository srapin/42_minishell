/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:57:49 by Helene            #+#    #+#             */
/*   Updated: 2023/06/28 13:12:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Returns the number in a long long type variable if the later
is in the interval [-LONG LONG MIN, LONG_LONG_MAX].
Returns -1 in case it exceeds LONG_LONG_MAX, or LONG LONG MIN. */
long long	is_numeric(char *str_nb)
{
	int					i;
	int					sign;
	unsigned long long	nb;

	i = 0;
	nb = 0;
	sign = 1;
	if (str_nb[i] && (str_nb[i] == '-' || str_nb[i] == '+'))
	{
		if (str_nb[i] == '-')
			sign = -1;
		i++;
	}
	while (str_nb[i] && nb <= LLONG_MAX)
	{
		nb = nb * 10 + (str_nb[i] - '0');
		i++;
	}
	if ((sign > 0 && nb > LLONG_MAX) || (sign < 0
			&& nb > (unsigned long long)LLONG_MAX + 1))
		return (-1);
	return (sign * nb);
}

void	check_if_numeric(t_cmd *cmd, t_cmd *first)
{
	int		i;
	char	*arg;

	i = 0;
	arg = cmd->val.args[1];
	if (arg[i] && (arg[i] == '-' || arg[i] == '+'))
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			if (cmd->pid == -1)
				write(STDERR_FILENO, "exit\n", 5);
			write(STDERR_FILENO, "minishell : exit : ",
				ft_strlen("minishell : exit : "));
			write(STDERR_FILENO, arg, ft_strlen(arg));
			write(STDERR_FILENO, ": numeric argument required\n",
				ft_strlen(" : numeric argument required\n"));
			free_cmds(&first, true);
			exit(NOT_A_NUM);
		}
		i++;
	}
}

void	invalid_first_arg(t_cmd *cmd, t_cmd *first, char *arg)
{
	if (cmd->pid == -1)
		write(STDERR_FILENO, "exit\n", 5);
	write(STDERR_FILENO, "minishell : exit : ",
		ft_strlen("minishell : exit : "));
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": numeric argument required\n",
		ft_strlen(" : numeric argument required\n"));
	free_cmds(&first, true);
	exit(NOT_A_NUM);
}

int	check_long_overflow(t_cmd *cmd, t_cmd *first)
{
	char			*arg;
	long long		initial_nb;
	unsigned char	exit_status;

	arg = cmd->val.args[1];
	initial_nb = is_numeric(arg);
	if (initial_nb == -1)
		invalid_first_arg(cmd, first, arg);
	if (cmd->val.args[2])
	{
		if (cmd->pid == -1)
			write(STDERR_FILENO, "exit\n", 5);
		write(STDERR_FILENO, "minishell : exit : too many arguments\n",
			ft_strlen("minishell : exit : too many arguments\n"));
		return (1);
	}
	exit_status = (unsigned char)initial_nb;
	if (cmd && cmd->pid == -1)
		write(STDERR_FILENO, "exit\n", 5);
	free_cmds(&first, true);
	exit(exit_status);
}

/* Exit retourne 0 lorsqu'aucun argument n'est donné.
Si le premier argument n'est pas numerique,
	il affiche une erreur et exit le shell en retournant 1.
Si non, il analyse le nomnre d'arguments.
Si trop d'arguments sont donnés (ie plus qu'un),
il affiche une erreur et retourne 1, sans exit le shell.
Sinon, il retourne l'argument donné, modulo 256.
*/
int	ft_exit(t_cmd *cmd, t_cmd *first)
{
	unsigned char	exit_status;
	char			*arg;

	arg = NULL;
	if (cmd)
		arg = cmd->val.args[1];
	if (!arg)
	{
		if (cmd && cmd->pid == -1)
			write(STDERR_FILENO, "exit\n", 5);
		free_cmds(&first, true);
		exit(g_exit_status);
	}
	check_if_numeric(cmd, first);
	exit_status = check_long_overflow(cmd, first);
	return (exit_status);
}
