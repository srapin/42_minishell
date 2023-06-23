/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:57:49 by Helene            #+#    #+#             */
/*   Updated: 2023/06/23 02:27:59 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Returns the number in a long long if the later is in the interval [0,
	LONG_LONG_MAX].
Returns -1 in case it exceeds LONG_LONG_MAX.
The number sent as argument has to be a positive integer*/
long long	is_numeric(char *str_nb)
{
	int					i;
	unsigned long long	nb;

	i = 0;
	nb = 0;
	while (str_nb[i] && nb <= LLONG_MAX)
	{
		nb = nb * 10 + (str_nb[i] - '0');
		i++;
	}
	if (nb > LLONG_MAX)
		return (-1);
	return (nb);
}

void	check_if_numeric(t_cmd *cmd, t_cmd *first)
{
	int		i;
	char	*arg;

	i = 0;
	arg = cmd->val.args[1];
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			write(STDERR_FILENO, "exit\n", 5);
			printf("minishell : exit : %s : numeric argument required\n", arg);
			// ecrire sur STDERR
			free_cmds(&first, true);
			exit(NOT_A_NUM);
		}
		i++;
	}
}

int	check_long_overflow(t_cmd *cmd, t_cmd *first)
{
	char			*arg;
	long long		initial_nb;
	unsigned char	exit_status;

	arg = cmd->val.args[1];
	initial_nb = is_numeric(arg);
	if (initial_nb == -1)
	{
		write(STDERR_FILENO, "exit\n", 5);
		printf("minishell : exit : %s : numeric argument required\n", arg);
		// ecrire sur STDERR
		free_cmds(&first, true);
		exit(NOT_A_NUM);
	}
	if (cmd->val.args[2])
	{
		write(STDERR_FILENO, "exit\n", 5);
		printf("minishell : exit : too many arguments\n"); // ecrire sur STDERR
		return (1);
	}
	exit_status = (unsigned char)initial_nb;
	free_cmds(&first, true);
	write(STDERR_FILENO, "exit\n", 5);
	exit(exit_status);
}

/* Exit retourne 0 lorsqu'aucun argument n'est donné.
Si le premier argument n'est pas numerique,
	il affiche une erreur et exit le shell en retournant 1.
Si non, il analyse le nomnre d'arguments.
Si trop d'arguments sont donnés (ie plus qu'un),
il affiche une erreur et retourne 1, sans exit le shell.
Sinon, il retourne l'argument donné, modulo 255.
*/
int	ft_exit(t_cmd *cmd, t_cmd *first)
{
	unsigned char exit_status;
	char *arg;

	arg = NULL;
	if (cmd)
		arg = cmd->val.args[1];
	if (!arg)
	{
		free_cmds(&first, true);
		printf("exit\n");
		exit(g_exit_status);
	}
	check_if_numeric(cmd, first);
	exit_status = check_long_overflow(cmd, first);
	return (exit_status); // si n'a pas exit avant
}