/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:57:49 by Helene            #+#    #+#             */
/*   Updated: 2023/06/20 09:28:32 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Returns the number in a long long if the later is in the interval [0, LONG_LONG_MAX].
Returns -1 in case it exceeds LONG_LONG_MAX.
The number sent as argument has to be a positive integer*/
long long is_numeric(char *str_nb)
{
    int                 i;
    unsigned long long  nb;
    
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

/* Exit retourne 0 lorsqu'aucun argument n'est donné.
Si le premier argument n'est pas numerique, il affiche une erreur et exit le shell en retournant 1.
Si non, il analyse le nomnre d'arguments.
Si trop d'arguments sont donnés (ie plus qu'un), il affiche une erreur et retourne 1, sans exit le shell.
Sinon, il retourne l'argument donné, modulo 255.
*/
int ft_exit(t_cmd *cmd, t_cmd *first)
{
    int             i;
    unsigned char   exit_status;
    long long       initial_nb;
    char            *arg;
    
    i = 0;
    arg = NULL;
    if (cmd)
        arg = cmd->val.args[1];
    if (!arg) // ie aucun argument
    {
       // free all
    //    free_pwd(cmd->env);
        free_cmds(&first, true);
        printf("exit\n");
        // write(STDOUT_FILENO, "exit\n", 5);
        exit(0); // a modifier !! returns the exit status of the last command
    }
    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
        {
            write(STDERR_FILENO, "exit\n", 5);
            printf("minishell : exit : %s : numeric argument required\n", arg);
            // free all
            free_cmds(&first, true);
            // free_pwd(cmd->env);
            exit(NOT_A_NUM);
        }
        i++;
    }
    initial_nb = is_numeric(arg);
    if (initial_nb == -1)
    {
        write(STDERR_FILENO, "exit\n", 5);
        printf("minishell : exit : %s : numeric argument required\n", arg);
        // free all
        free_cmds(&first, true);
        // free_pwd(cmd->env);
        exit(NOT_A_NUM);
    }
    if (cmd->val.args[2]) // ie a au moins deux arguments
    {
        write(STDERR_FILENO, "exit\n", 5);
        printf("minishell : exit : too many arguments\n");
        return (1); 
    }
    exit_status = (unsigned char)initial_nb;
    // free all
    // free_pwd(cmd->env);
    free_cmds(&first, true);
    printf("exit\n");
    exit(exit_status);
}