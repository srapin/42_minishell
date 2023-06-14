/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:57:49 by Helene            #+#    #+#             */
/*   Updated: 2023/06/14 21:47:33 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Exit retourne 0 lorsqu'aucun argument n'est donné.
Si trop d'arguments sont donnés, il affiche une erreur et retourne 1
Sinon, il retourne l'argument donné, modulo 255 
*/
int ft_exit(t_cmd *cmd) // void ou int ?
{
    int             i;
    unsigned char   nb;
    char            *arg;
    
    i = 0;
    arg = NULL;
    if (cmd)
        arg = cmd->val.args[1];
    if (!arg) // ie aucun argument
    {
       // free all
       ft_putstr_fd("exit\n", 1);
       exit(0); // a modifier !! returns the exit status of the last command
    }
    if (cmd->val.args[2]) // ie a au moins deux arguments
    {
        printf("exit\n");
        printf("minishell : exit : too many arguments\n");
        // free pas all du coup ?
        return (1); 
        // !! n'exit pas le shell ici !!!
    }
    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
        {
            printf("exit\n");
            printf("minishell : exit : %s : numeric argument required\n", arg);
            // free all
            exit(NOT_A_NUM);
        }
        i++;
    }
    nb = (unsigned char)ft_atoi(arg); // vérifier ce que ca fait si overflow le long long dans atoi
    // free all
    printf("exit\n");
    exit(nb);
}