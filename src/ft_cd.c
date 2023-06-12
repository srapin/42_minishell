/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 01:12:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/12 19:27:21 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Quelles erreurs peut-on avoir a gérer ? 
Est-ce que cd ~ est à gérer ? Non car il s'agit d'un token, et qu'il n'est pas à gérer dans minishell
*/

/* Obtient le path complet (chemin absolu) depuis la racine
Néanmoins, des './' ou '../' peuvent encore être présents
*/
char    *get_full_path(t_ht_hash_table *env, char *arg_path)
{
    char    *pwd;

    pwd = ht_search(env, "PWD"); // que faire si pwd = NULL ?
    return (ft_strjoin(pwd, arg_path));
}

/* Parses the path, and replaces any occurence of 
'./', or '../'
*/
void    replace_prev_or_actual_dir(char *path)
{
    int     i;
    int     j;
    char    *before;
    char    *after;

    i = 0;
    while (path[i])
    {
        // ne peux pas avoir path[0] == '.' car a récupéré pwd, donc path[0] = '/' obligatoirement
        if (path[i] == '.' && path[i - 1] == '/')
        {
            printf("i = %d\n", i);
            printf("path before : %s\n", path);
            // simply deletes the '.' or './'
            if ((!path[i + 1] || path[i + 1] == '/')) // '.' ou './'
            {
                before = ft_substr(path, 0, i);
                after = ft_substr(path, i + 2, ft_strlen(path));
                free(path);
                path = ft_strjoin(before, after);
                free(before);
                free(after);
                before = NULL;
                after = NULL;
                printf("path after : %s\n", path);
            }
            else if (path[i + 1] && path[i + 1] == '.')
            {
                /* Deletes the '..' or '../', along with the <dir_name>/ that 
                comes previously, ONLY if there's something before the 
                previous '/'
                */
                if ((!path[i + 2] || path[i + 2] == '/')) // '..' ou '../'
                {
                    
                    if (i - 2 >= 0) // ie n'est pas à la racine
                    {
                        // détermine l'index du précédent '/' (par ex le premier '/' de "cd ./dir1/..")
                        j = i - 2; // ie l'index de l'élément précédant le '/' précédant le premier '.'
                        while (j >= 0 && path[j] != '/')
                            j--;
                        before = ft_substr(path, 0, j + 1);
                    }
                    else // ie est à la racine -> supprime alors juste '..' (ou '../')
                        before = ft_strdup("/");
                    after = ft_substr(path, i + 3, ft_strlen(path));
                    if (i - 2 >= 0)
                        i = j + 1; // ou juste i = j, change rien 
                    else 
                        i = 0; // ouais ?
                    free(path);
                    path = ft_strjoin(before, after);
                    free(before);
                    free(after);
                    before = NULL;
                    after = NULL;
                    printf("path after : %s\n", path);
                }
            }
        }
        else
            i++;
        
    }
}

/* 
Implements the cd builtin with only a relative or absolute path.
Returns 0 IF directory is successfully changed

QUE FAIRE SI PWD EST UNSET ?
*/
int     ft_cd(t_cmd *cmd)
{
    char    *full_path;
    char    *pwd;
    char    *after_rel;
    char    *new_pwd;

    dprintf(1, "coucou depuis ft_cd()\n");
    if (!cmd->val.args[1]) // si n'a aucun argument
        return (EXIT_OK);
    if (cmd->val.args[2]) // si il y a plus d'un argument. val.args est null-terminated
        return (CD_TOO_MANY_ARGS);

    if (chdir(cmd->val.args[1]) == -1)
    {
        perror("chdir ");
        exit(errno); // Quel code erreur ?
    }
    

    // si le dossier est accessible, et que s'y est bien déplacé :

    full_path = get_full_path(cmd->env, cmd->val.args[1]);

    replace_prev_or_actual_dir(full_path);
    ht_modify_value(cmd->env, "PWD", full_path);
    
    // if (cmd->val.args[1][0] == '.') // chemin relatif
    // {
    //     if (cmd->val.args[1][1] && cmd->val.args[1][1] == '.')
    //     {
    //         // cas cd .. 
    //         // peut avoir cd ../../../.. par ex !!!! A gérer !
    //     }
    //     else
    //     {
    //         pwd = ft_strdup(ht_search(cmd->env, "PWD"));
    //         after_rel = ft_substr(cmd->val.args[1], 1, ft_strlen(cmd->val.args[1]));
    //         new_pwd = ft_strjoin(pwd, after_rel);
    //         ht_modify_value(cmd->env, "PWD", new_pwd);
    //         free(pwd);
    //         free(after_rel);
    //         pwd = NULL;
    //         after_rel = NULL;
    //     }
    // }
    // else // chemin absolu
    // {
    //     new_pwd = ft_strdup(cmd->val.args[1]);
    //     ht_modify_value(cmd->env, "PWD", new_pwd);
    // }
    return (EXIT_OK);
}