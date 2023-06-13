/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 01:12:19 by Helene            #+#    #+#             */
/*   Updated: 2023/06/13 02:10:28 by srapin           ###   ########.fr       */
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
char *replace_prev_or_actual_dir(char *path)
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
    printf("in the end of replace_prev_or_actual(), path = %s\n", path);
    return (path);
}

/* 
Implements the cd builtin with only a relative or absolute path.
Returns 0 IF directory is successfully changed

QUE FAIRE SI PWD EST UNSET ?
*/
int     ft_cd(t_cmd *cmd)
{
    char    *full_path;
    char    *tmp;
    char    *pwd;
    char    *after_rel;
    char    *new_pwd;

    //dprintf(1, "coucou depuis ft_cd()\n");
    if (!cmd->val.args[1]) // si n'a aucun argument
        return (EXIT_OK);
    if (cmd->val.args[2]) // si il y a plus d'un argument. val.args est null-terminated
        return (CD_TOO_MANY_ARGS);

    if (cmd->val.args[1][0] == '.') // chemin relatif)
    {
        tmp = ft_strjoin("/", cmd->val.args[1]);
        full_path = get_full_path(cmd->env, tmp);
        free(tmp);
        tmp = NULL;
    }
    else // chemin absolu
        full_path = get_full_path(cmd->env, cmd->val.args[1]);
    printf("full path avant chdir : %s\n", full_path);
    if (chdir(full_path) == -1)
    {
        perror("chdir ");
        exit(errno); // Quel code erreur ?
    }
    
    // si le dossier est accessible, et que s'y est bien déplacé :

    tmp = ft_strdup(full_path);
    free(full_path);
    full_path = replace_prev_or_actual_dir(tmp);
    // free(tmp); pourquoi ce free() fait tout couiller ??
    printf("path before last if = %s\n", full_path);
    if (ft_strlen(full_path) > 1 && full_path[ft_strlen(full_path) - 1] == '/')
    {
        printf("dans le if, full_path = %s\n", full_path);
        tmp = ft_substr(full_path, 0, ft_strlen(full_path) - 1);
        free(full_path);
        full_path = tmp;
    }
    printf("path after last if = %s\n", full_path);
    ht_modify_value(cmd->env, "PWD", full_path);
    printf("dans export() : PWD apres ht_modify() = %s\n", ht_search(cmd->env, "PWD"));
    
    
    return (EXIT_OK);
}