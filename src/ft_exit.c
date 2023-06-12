/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 00:57:49 by Helene            #+#    #+#             */
/*   Updated: 2023/06/12 23:13:49 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Exit retourne 0 lorsqu'aucun argument n'est donné.
Si trop d'arguments sont donnés, il affiche une erreur et retourne 1
Sinon, il retourne l'argument donné, modulo 255 
*/
void ft_exit(t_cmd *cmd) // void ou int ?
{
    char    nb;
    char    *arg;
    
    arg = cmd->val.args[1];
    if (!arg) // ie aucun argument
    {
       // free all
       printf("exit\n");
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
    
    arg = 
    
    
    
}