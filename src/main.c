/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:32:17 by Helene            #+#    #+#             */
/*   Updated: 2023/06/09 23:51:48 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ht_hash_table *ht_get_env(char **envp)
{
    int i;
    int j;
    t_ht_hash_table *ht;

    i = 0;
    ht = ht_new(HT_INITIAL_SIZE);
    if (!ht)
        return (NULL);
    while (envp[i])
    {
        j = 0;
        while (envp[i][j] && envp[i][j] != '=')
            j++;
        // printf("j = %d\n", j);
        // printf("ok, env[%d] = %s\n", i, envp[i]);
        // printf("key = %s\n", ft_substr(envp[i], 0, j));
        // printf("value = %s\n", ft_substr(envp[i], j + 1, ft_strlen(envp[i])));
        ht_insert_item(ht, ft_substr(envp[i], 0, j), ft_substr(envp[i], j + 1, ft_strlen(envp[i])));
        i++;
    }
    //printf("ok2\n");
    return (ht);
}

char *ft_strcpy(char *str)
{
    int i;
    char *cpy;
    
    if (!str)
        return (NULL);
    i = -1;
    cpy = ft_calloc(sizeof(char), ft_strlen(str));
    if (!cpy)
        return (NULL);
    while (str[++i])
        cpy[i] = str[i];
    cpy[i] = '\0';
    return (cpy);
}

// seulement dans le cas env -i, ou aussi lorsque l'env passé au main est nul ? 
t_ht_hash_table *get_minimal_env() // comment indiquer a quel niveau du shell se situe ? (pour la SHLVL variable de l'env)
{
    int             shell_lvl = 0; // juste histoire de pas me faire insulter par le compilateur, mais faut le faire autrement c'est inutile ça
    size_t          size;
    char            *pwd;
    t_ht_hash_table *ht;
    
    ht = ht_new(HT_INITIAL_SIZE);
    size = GETCWD_INITIAL_BUF_SIZE;
    pwd = ft_calloc(sizeof(char), size);
    if (!pwd)
        return (NULL); // ?
    getcwd(pwd, size);
    while (!pwd)
    {
        free(pwd);
        size *= 2;
        pwd = ft_calloc(sizeof(char), size);
        if (!pwd)
            return (NULL); // ?
        getcwd(pwd, size);
    }
    ht_insert_item(ht, ft_strcpy("SHLVL"),ft_itoa(shell_lvl));
    ht_insert_item(ht, ft_strcpy("PWD"), pwd); // que mettre ? getcwd() ?
    ht_insert_item(ht, ft_strcpy("_"), ft_strcpy("/usr/bin/env")); // que mettre ? 
    return (ht);
}

/* Prints the hash map */
void    print_ht(t_ht_hash_table *ht)
{
    printf("ht->count = %ld\n\n", ht->count);
    for (int i = 0; i < ht->size; i++)
    {
        if (ht->items[i])
        {
            printf("hash->items[%d] :\nkey = %s \nvalue = %s\n\n", \
            i, ht->items[i]->key, ht->items[i]->value);
        }
    }
}

void    exec_script(t_ht_hash_table *ht, char *path)
{
    int     fd;
    char    *line;
    t_token_list    *tk_list;

    fd = open(path, O_RDONLY);
    if (!fd)
    {
        perror("open ");
        // return ; ?
    }
    line = get_next_line(fd);
    tk_list = tokenise(ht, assign_type(line, ft_strlen(line)), ft_strlen(line), line);
    parse(ht, tk_list);
    
    // normalement, n'a qu'une seule ligne !! car ne doit pas gerer les ';' dans minishell    
    while (line)
    {
        free(line);
        line = get_next_line(fd);
    }

    close(fd);
}

int main (int argc, char **argv, char **envp)
{
    t_ht_hash_table *hash_map;
    
    if (argc > 2)
    {
        printf("error : too many arguments\n"); //idk
        return (0);
    }
    
    if (!envp)
        hash_map = get_minimal_env();
    else
        hash_map = ht_get_env(envp);
    if (!hash_map)
        return (EXIT_FAILURE); // do we return ? which exit status ?

    // for (int i = 0; envp[i]; i++)
    //     printf("envp[%d] = %s\n", i, envp[i]);
    // printf("\n---------------------------\n");

    // for (int i = 0; envp[i]; i++)
    //     printf("envp[%d] = %s\n", i, envp[i]);
    // printf("\n\n\n\n\n\n\n");
    // print_ht(hash_map);

    if (argc == 1)
        read_lines(hash_map);
    else // peut avoir plus qu'un fichier ? part du principe que non ici ?
        exec_script(hash_map, argv[1]);
    
    
    //parse(hash_map, tokenise(hash_map, assign_type(argv[1], ft_strlen(argv[1])), ft_strlen(argv[1]), argv[1]));

    /* 
            ------------ Tests ------------
    */
    // if (argc != 2)
    //     return (printf("Mauvais nombre d'args : argc = %d\n", argc), 0);
    

    // t_ht_hash_table *ht = ht_new(HT_INITIAL_SIZE);
    // if (!envp)
    //     return (printf("pas d'env\n"));
    // int j;
    // for (int i = 0; envp[i]; i++)
    // {
    //     j = 0;
    //     while (envp[i][j] != '=')
    //         j++;
    //     ht_insert_item(ht, ft_substr(envp[i], 0, j), ft_substr(envp[i], j + 1, ft_strlen(envp[i])));
    // }
    
    
    // Print the envp variable */
    // printf("\n---------------------------\n");

   
    

    // printf("key = PATH, value = %s\n", ht_search(ht, "PATH"));
}