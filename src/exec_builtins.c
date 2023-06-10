/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 20:57:20 by srapin            #+#    #+#             */
/*   Updated: 2023/06/10 20:40:15 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void free_all()
{
    
}


void ft_echo(t_cmd *cmd)
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
}

void ft_cd(t_cmd *cmd)
{
    
}

void ft_pwd(t_cmd *cmd)
{
    
}



void ft_unset(t_cmd *cmd)
{
    
}

// void ft_env(t_cmd *cmd)
// {
    
// }

void ft_exit(t_cmd *cmd)
{
    
}


void (*get_builtins_foo(char *str))(t_cmd *)
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
    return NULL;
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
    void (*foo)(t_cmd *);

    //num = is_builtins(cmd->val.value);
    foo = get_builtins_foo(cmd->val.value);
    if (!foo)
        return;
        
    foo(cmd);
    free_all();        
    exit(0);
}