/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:13:13 by Helene            #+#    #+#             */
/*   Updated: 2023/06/20 04:19:30 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char    *search_from_end(char *d_name, char *suffix)
{
    int i;
    int j;

    i = ft_strlen(d_name) - 1;
    j = ft_strlen(suffix) - 1;
    while (i >= 0 && j >= 0)
    {
        if (d_name[i] != suffix[j])
            return (NULL);
        i--;
        j--;
    }
    if (j >= 0) // ie n'a pas parcouru tout le suffix
        return (NULL);
    return (d_name);
}

/*
Renvoie NULL si le nom du directory stream donne en argument ne commence pas par prefix,
et pointe vers le charactere suivant la fin du prefix dans la string dans le cas contraire
*/
char    *search_from_start(char *d_name, char *prefix)
{
    int i;
    
    i = 0;
    while (d_name[i] && prefix[i])
    {
        if (d_name[i] != prefix[i])
            return (NULL);
        i++;
    }
    if (prefix[i])
        return (NULL);
    return (d_name);
}

void    add_filename(t_filename **filenames, char *new)
{
    t_filename  *current;
    t_filename  *new_path;
    if (!new || !(*filenames))
        return ;
    new_path = ft_calloc(sizeof(t_filename), 1);
    if (!new_path)
        return ;
    new_path->filename = ft_strdup(new);
    if (!(*filenames)->filename)
    {
        free(*filenames);
        (*filenames) = new_path;
    }
    else
    {
        current = (*filenames);
        while (current->next)
            current = current->next;
        current->next = new_path;
    }
}


int     is_in_quotes(t_token_list *current, size_t index)
{
    t_word_data *current_w;
    
    if (!(current->merged_words))
        return (current->quotes);
    current_w = current->merged_words;
    while (current_w)
    {
        if (index >= current_w->word_start_index)
        {
            if ((!current_w->next && current_w->length > index)
                || current_w->next->word_start_index > index)
                return (current_w->quotes);
        }
        current_w = current_w->next;
    }
    return (-1);
}

int     is_a_hidden_dir(struct dirent *dir_content)
{
    if (ft_strlen(dir_content->d_name) 
        && *(dir_content->d_name) == '.') 
        return (1);
    return (0);
}



/*
Retourne les filenames associes au wildcard, ou NULL lorsqu'aucun filename ne correspond 
IL S'AGIT DU PREMIER TRI

 Note : readdir() renvoie NULL si il n'y a plus d'entité de directory à lire
*/
t_filename  *first_sort(DIR *dir, char *prefix, char *suffix) // prefix and suffix indexes in the big (aka merged, final) word
{
    size_t          prefix_len;
    size_t          suffix_len;
    char            *curr_filename;
    t_filename      *filenames;            
    struct dirent   *dir_content;
    
    if (!prefix || !suffix)
        return (NULL);
    filenames = ft_calloc(sizeof(t_filename), 1);
    if (!filenames)
        return (NULL); // ?
    prefix_len = ft_strlen(prefix);
    suffix_len = ft_strlen(suffix);
    dir_content = readdir(dir);
    if (!prefix_len && !suffix_len)
    {
        while (dir_content)
        {
            if (!is_a_hidden_dir(dir_content))
                add_filename(&filenames, dir_content->d_name);
            dir_content = readdir(dir);
        }
        return (filenames);
    }
    if (prefix_len == 2 && prefix[0] == '.' && prefix[1] == '.')
        return (NULL);
    while (dir_content)
    {
        if (prefix && *prefix != '.' && is_a_hidden_dir(dir_content))
            ;
        else if (prefix_len && suffix_len)
        {
            curr_filename = search_from_start(dir_content->d_name, prefix);
            if (curr_filename)
            {
                curr_filename = search_from_end(dir_content->d_name, suffix);
                if (curr_filename)
                    add_filename(&filenames, dir_content->d_name); 
            } 
        }
        else if (prefix_len)
        {
            curr_filename = search_from_start(dir_content->d_name, prefix);
            if (curr_filename)
                add_filename(&filenames, dir_content->d_name); 
        }
        else if (suffix_len)
        {
            curr_filename = search_from_end(dir_content->d_name, suffix);
            if (curr_filename)
                add_filename(&filenames, dir_content->d_name); 
        }
        dir_content = readdir(dir);
    }
    return (filenames);
}

// retourne la string suivant le dernier '*' non contenu dans des quotes
char *get_suffix(t_token_list *current)
{
    size_t      i;
    t_word_data *wd;
    
    wd = current->merged_words;
    i = current->length - 1;
    while (i >= 0)
    {
        if (current->content[i] == '*' && !is_in_quotes(current, i))
            return (ft_substr(current->content, i + 1, current->length));
        i--;
    }
    return (NULL);
}

char    *ft_search_str_in_str(char *s, char *to_find)
{
    int i;
    int j;

    if (!s || ft_strlen(to_find) > ft_strlen(s))
        return (NULL);
    if (!ft_strlen(to_find))
        return (s);
    i = 0;
    j = 0;
    while (s[i])
    {
        if (s[i] == to_find[j])
        {
            while (s[i] == to_find[j])
            {
                i++;
                j++;
            }
            if (s[i] || (!s[i] && !to_find[j]))
                return (&s[i - j + 1]);
            i -= j;
            j = 0;
        }
        i++;
    }
    return (NULL);
}

int  get_next_wcard_index(t_token_list *current, size_t index)
{
    int  i;

    i = index;
    while (current->content[i])
    {
        if (current->content[i] == '*' && !is_in_quotes(current, i))
            return (i);
        i++;
    }
    return (-1); 
}

void    del_filename(t_filename **first, t_filename *to_del)
{
    t_filename  *current;
    
    current = (*first);
    while (current->next && current->next != to_del)
        current = current->next;
    if (!current->next)
        return ;
    current->next = to_del->next;
    free(to_del->filename);
    free(to_del);
    to_del = NULL;
}

/*
Passe le prefix. 
Tombe ainsi sur un '*'. Itère sur le d_name jusqu'a ce que l'on trouve dans le d_name la meme string que
celle suivant le '*' trouvé dans le mot originel, ou que l'on ait parcouru l'entièreté du d_name. 
Si ce qui suit le '*' est la chaine vide (ie on a "*\0"), arrête et recommence l'opération sur le d_name suivant.
Sinon : en continuant dans le mot originel, si tombe sur un autre '*' : si ce qui suit est non vide :
    Si a entièrement parcouru le d_name précédemment, supprime le d_name 
    Sinon, itère à nouveau sur le d_name jusqu'a trouver la première apparition de la string recherchée,
    ou jusqu'à ce que la fin du d_name soit reached.
Etc...

Sous chaine entre deux '*' : 
    taille de la sous chaine : next_wcard_index - (curr_wcard_index + 1)
    index du debut de la sous chaine : curr_wcard_index + 1
*/
void    second_sort(t_filename **filenames, t_token_list *current, char *prefix)
{
    int i;
    int         next_wcadr_index;
    char        *filename_pos;
    size_t      prefix_len;
    t_filename  *current_f;

    if (!filenames)
        return ;
    if (!(*filenames))
        return ;
    prefix_len = ft_strlen(prefix);
    current_f = (*filenames);
    while (current_f)
    {
        i = prefix_len + 1;
        filename_pos = current_f->filename + prefix_len;
        while (i < current->length)
        {
            
            next_wcadr_index = get_next_wcard_index(current, i);
            if (next_wcadr_index >= 0)
            {
                // printf("filename_pos : %s\n", filename_pos);
                // printf("current->content : %s\n", current->content);
                // printf("i : %d\nnext_wcadr_index : %d\nnext_wcadr_index - i : %d\n", i, next_wcadr_index, next_wcadr_index - i);
                // printf("substring to find : %s\n\n\n", ft_substr(current->content, i, next_wcadr_index - i));
                filename_pos = ft_search_str_in_str(filename_pos, ft_substr(current->content, i, next_wcadr_index - i));
                if (!filename_pos) // ft_search_str_in_str() renvoie NULL en cas d'échec
                {
                    del_filename(filenames, current_f);
                    break;
                }
                i = next_wcadr_index + 1;
                while (current->content[i] && current->content[i] == '*' && !is_in_quotes(current, i)) // skip les autres '*' s'il y en a plusieurs successifs
                    i++;
            }
            else
                break ;
        }
        current_f = current_f->next;
    }
}

void    free_filenames(t_filename **filenames)
{
    t_filename *current;

    current = (*filenames);
    while (current)
    {
        current = (*filenames)->next;
        if ((*filenames)->filename)
            free((*filenames)->filename);
        free((*filenames));
    }
    //free(filenames); // POURQUOI ÇA MARCHE PAS ??
}



/*
Replaces current->content (ie the current token's content) with the associated files/directories names, 
in case the wildcard search revealed itself successful
*/
void    insert_filenames(t_token_list **first, t_token_list **current, t_filename **filenames)
{
    t_token_list    *tmp;
    t_filename      *current_f;

    if (!filenames || !(*filenames) || !(*filenames)->filename) // ie si probleme de malloc ou alors que n'a trouvé aucun filename correspondant
        return ; // ?
    current_f = (*filenames);
    tmp = (*current);
    while(current_f)
    {
        tk_add_word_in_list(current, current_f->filename);
        //printf("%s inserted\n", (*filenames)->filename);
        *current = (*current)->next;
        current_f = current_f->next;
    }
    tk_del_one(first, tmp);

    //free_filenames(filenames); // POURQUOI CA MARCHE PAS 
}



/*
Une fois qu'a choppé les prefix et suffix, fait un premier tri dans la liste des d_name
pour trouver ceux ayant des prefixes et suffixes identiques.
Une fois qu'a fait ce premier tri, itère sur chaque d_name qui matchait, 
et essayant cette fois ci de trouver des matchs pour les wildcards a l'interieur du mot (s'il y en a)
Si on arrive au dernier index du current d_name sans etre arrivé à la fin du mot, alors supprime ce d_name
de la liste de d_name résultant du premier tri.
*/
void    perform_wildcard_exp(t_data *data)
{
    char            *wildcard_start;
    size_t          wildcard_index;
    char            *current_dir;
    DIR             *dir;
    t_token_list    *current;
    t_filename      *filenames;

    // faire une fonction auxiliaire open_current_dir() pour simplifier le code de cette fonction ?
    current_dir = *get_pwd(data->env);
    if (!current_dir)
        return ; // ?
    dir = opendir(current_dir);
    if (!dir)
    {
        perror("opendir ");
        return ; // ?
    }
    current = (*(data->first));
    while (current)
    {
        if (current->type == l_parenthesis) // n'expand pas ce qui est entre parenthèses, le fera dans le subshell
        {
            while (current && current->type != r_parenthesis)
                current = current->next;
            if (current) // ie current = ')'
                current = current->next;
            continue ;
        }
        wildcard_start = ft_strchr(current->content, '*');
        if (wildcard_start)
        {
            // avance dans le mot jusqu'a ce que l'on tombe sur un '*' qui n'est PAS entre quotes, ou qu'on arrive a la fin du mot
            wildcard_index = current->length - ft_strlen(wildcard_start); // index du '*' dans le mot
            while (wildcard_start && is_in_quotes(current, wildcard_index))
            {
                wildcard_start = ft_strchr(wildcard_start, '*'); // a le droit d'ecrire ca ?
                wildcard_index = current->length - ft_strlen(wildcard_start);
            }
            if (wildcard_start) // ie a trouvé un '*' n'étant pas entre quotes (et n'est donc pas arrivé a la fin du mot sans en trouver)
            {
                {
                    char *prefix = ft_substr(current->content, 0, wildcard_index);
                    char *suffix = get_suffix(current);
                    filenames = first_sort(dir, prefix, suffix);
                    second_sort(&filenames, current, prefix); // on peut sans doute merge les first_sort et second_sort ensembles : le deuxieme fait deja le premier, en plus approfondi
                }
                insert_filenames(data->first, &current, &filenames); // passe deja au current->next ici
            }
        }
        current = current->next;
    }
    if (closedir(dir) == -1)
        perror("closedir ");
}