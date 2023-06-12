/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 20:57:20 by srapin            #+#    #+#             */
/*   Updated: 2023/06/12 16:43:11 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void free_all()
{
    
}


int ft_echo(t_cmd *cmd)
{
    char end[2];
    int i;

    end[0] = '\0';
    end[1] = '\0';
    i = 1;
    if (ft_strisequal(cmd->val.args[1], "-n"))
    {
        i++; 
        end[0] = '\n';
    }
    while(cmd->val.args[i])
    {
        printf("%s", cmd->val.args[i]);
        i++;
        if (cmd->val.args[i])
            printf(" ");
    }
    printf("%s", end);
    return (EXIT_OK); // à modifier, rajouté pour plus avoir de warning a la compilation
}

// void ft_cd(t_cmd *cmd)
// {
    
// }

// void ft_pwd(t_cmd *cmd)
// {
    
// }



// void ft_unset(t_cmd *cmd)
// {
    
// }

// // void ft_env(t_cmd *cmd)
// // {
    
// // }

// void ft_exit(t_cmd *cmd)
// {
    
// }


/* 
!! Tout ce qui modifie l'environnement doit etre fait dans le processus parent !!!
*/
int (*get_builtins_foo(char *str))(t_cmd *)
{
    
    if (ft_strisequal(str, "echo"))
        return ft_echo;
    if (ft_strisequal(str, "cd"))
        return ft_cd;
    if (ft_strisequal(str, "pwd"))
        return ft_pwd;
    // if (ft_strisequal(str, "export"))
    //     return ft_export;
    if (ft_strisequal(str, "unset"))
        return ft_unset;
    // if (ft_strisequal(str, "env"))
    //     return ft_env;
    if (ft_strisequal(str, "exit"))
        return ft_exit;
    return NULL; // return -1 plutot ? si les builtins retournent un int >= 0 
}


int is_builtins(char * str)
{
    if (ft_strisequal(str, "echo"))
        return 0;
    if (ft_strisequal(str, "cd"))
        return 1;
    if (ft_strisequal(str, "pwd"))
        return 2;
    if (ft_strisequal(str, "export"))
        return 3;
    if (ft_strisequal(str, "unset"))
        return 4;
    if (ft_strisequal(str, "env"))
        return 5;
    if (ft_strisequal(str, "exit"))
        return 6;
    return -1;
}


void try_to_exec_builtins(t_cmd *cmd)
{
    int (*foo)(t_cmd *);

    dprintf(1, "in try_to_exec_builtins()\n");
    //num = is_builtins(cmd->val.value);
    foo = get_builtins_foo(cmd->val.value); // probleme ca retourne tout le temps NULL
    dprintf(1, "get_builtins() return value : %p\n", foo);
    if (!foo)
        return;
        
    foo(cmd);
    free_all(); // le faire dans le builtin ?  + quand mettre à jour le dernier exit_status ?
    exit(0); // n'exit pas forcément avec 0 !! dépend de la valeur de retour du builtin
}