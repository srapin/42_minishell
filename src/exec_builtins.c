/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 20:57:20 by srapin            #+#    #+#             */
/*   Updated: 2023/06/18 23:48:07 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int ft_echo(t_cmd *cmd)
{
    char end[2];
    int i;

    end[0] = '\n';
    end[1] = '\0';
    i = 1;
    if (ft_strisequal(cmd->val.args[i], "-n"))
    {
        i++; 
        end[0] = '\0';
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
        return &ft_echo;
    if (ft_strisequal(str, "cd"))
        return &ft_cd;
    if (ft_strisequal(str, "pwd"))
        return &ft_pwd;
    if (ft_strisequal(str, "export"))
        return &ft_export;
    if (ft_strisequal(str, "unset"))
        return &ft_unset;
    if (ft_strisequal(str, "env"))
        return &ft_env;
    if (ft_strisequal(str, "exit"))
        return &ft_exit;
    return NULL; // return -1 plutot ? si les builtins retournent un int >= 0 
}


int is_builtins(char * str)
{
    if (ft_strisequal(str, "echo"))
        return 1;
    if (ft_strisequal(str, "cd"))
        return 1;
    if (ft_strisequal(str, "pwd"))
        return 1;
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



int try_to_exec_builtins(t_cmd *cmd, bool is_child)
{
    int (*foo)(t_cmd *);
    int ret;
    int old_in;
    int old_out;

    // dprintf(1, "in try_to_exec_builtins()\n");
    //num = is_builtins(cmd->val.value);
    ret = -1;
    foo = get_builtins_foo(cmd->val.value); 
    // dprintf(1, "get_builtins() return value : %p\n", foo);
    if (!foo)
        return ret;
    if (!is_child)
    {
        old_in = dup(STDIN_FILENO); 
        old_out = dup(STDOUT_FILENO); 
        dup_cmd_file(cmd);
    }
    ret = foo(cmd);
    if (is_child)
    {
        exit(ret); // n'exit pas forcément avec 0 !! dépend de la valeur de retour du builtin
    }
    g_exit_status = ret;
    dup2(old_in, STDIN_FILENO);
    dup2(old_out, STDOUT_FILENO);
    close(old_in);
    close(old_out);
    return ret;
}