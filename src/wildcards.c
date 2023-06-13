/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:13:13 by Helene            #+#    #+#             */
/*   Updated: 2023/06/13 10:17:54 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *search_from_end(char *d_name, char *suffix)
{
    int i;
    int j;

    i = ft_strlen(d_name) - 1;
    j = ft_strlen(suffix) - 1;
    while (i >= 0 && j >= 0)
    {
        // if (ft_rev_strchr(suffix + j, '*') && !is_in_quotes(wd, )) // implementer le calcul de l'index de suffix[i] dans le mot final : le garder en mémoire dès que calcule le suffix ? et le mettre dans une structure que passe en argument, avec tout le reste des variables ?
        // {
            
        // }
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
/*
Renvoie le dir_content->name si ca match avec le prefix, et null le cas echeant
*/
char    *search_from_start(char *d_name, char *prefix)
{
    int i;
    
    i = 0;
    while (d_name[i] && prefix[i])
    {
        // if (prefix[i] == '*') // 
        // {
            
        // }
        if (d_name[i] != prefix[i])
            return (NULL);
        i++;
    }
    if (prefix[i]) // ie n'a pas parcouru tout le prefix
        return (NULL);
    return (d_name);
}

void    add_filename(t_filename **filenames, char *new) // new a deja ete malloc. PAS SUR (s'agit de l'attribut d_name de la struct dirent ; et s'agit d'un char[256])
{
    t_filename  *current;
    t_filename  *new_path;
    if (!new || !(*filenames))
        return ;
    new_path = ft_calloc(sizeof(t_filename), 1);
    if (!new_path)
        return ;
    new_path->filename = ft_strdup(new);
    if (!(*filenames)->filename) // ie la liste est pour l'instant vide
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

/*
!!!!!! si est entre quotes, il faut quand meme regarder si un autre wildcard est present apres celui ci, et ce avant de re parser quoi que ce soit dans la struct dirent
*/
int     is_in_quotes(t_token_list *current, size_t index) // determines if the index is in between quotes. index = index of the wildcard in the whole (final) word
{
    t_word_data *current_w;
    
    if (!(current->merged_words)) // ie le token final n'est constitué que d'un seul token, le token initial (et donc pas d'un merge d'une succession de tokens séparés par rien ou des quotes)
        return (current->quotes);
    current_w = current->merged_words;
    while (current_w)
    {
        if (index >= current_w->word_start_index)
        {
            if ((!current_w->next && current_w->length > index) // rien ne suit et index est bien contenu dans le dernier mot
                || current_w->next->word_start_index > index) // index est dans la range d'indexes du current_w word
                return (current_w->quotes);
        }
        current_w = current_w->next;
    }
    return (-1); // cas d'erreur, ie cas où l'index donné n'est pas dans l'intervalle d'indexes du mot (ie [0, ft_strlen(mot) - 1])
}

int     is_a_hidden_dir(struct dirent *dir_content)
{
    if (ft_strlen(dir_content->d_name) 
        && *(dir_content->d_name) == '.') 
        return (1);
    // if (ft_strlen(dir_content->d_name) == 2
    //     && dir_content->d_name[0] == '.'
    //     && dir_content->d_name[1] == '.')
    //     return (1);
    return (0);
}

/*
Retourne les filenames associes au wildcard, ou NULL lorsqu'aucun filename ne correspond 
IL S'AGIT DU PREMIER TRI
*/
t_filename  *first_sort(DIR *dir, char *prefix, char *suffix) // prefix and suffix indexes in the big (aka merged, final) word
{
    size_t          prefix_len;
    size_t          suffix_len;
    char            *curr_filename;
    t_filename      *filenames;            
    struct dirent   *dir_content;
    
    if (!prefix || !suffix) // le malloc n'a pas fonctionné
        return (NULL);
    filenames = ft_calloc(sizeof(t_filename), 1);
    if (!filenames)
        return (NULL); // ?
    prefix_len = ft_strlen(prefix);
    suffix_len = ft_strlen(suffix);
    dir_content = readdir(dir);
    if (!prefix_len && !suffix_len) // retourne l'entierete du contenu du repo courant
    {
        while (dir_content)
        {
            if (!is_a_hidden_dir(dir_content)) // skip . et ..
                add_filename(&filenames, dir_content->d_name);
            dir_content = readdir(dir);
        }
        return (filenames);
    }
    if (prefix_len == 2 && prefix[0] == '.' && prefix[1] == '.') // si ..* alors ne doit rien modifier
        return (NULL);
    while (dir_content)
    {
        //printf("d_name = %s\n", dir_content->d_name);
        if (prefix && *prefix != '.' && is_a_hidden_dir(dir_content)) // skip . et ..
            ;
        else if (prefix_len && suffix_len) // est de la forme prefix*suffix
        {
            curr_filename = search_from_start(dir_content->d_name, prefix); // ca marche ou est-ce que doit malloc ?
            if (curr_filename)// si le prefix est present dans le d_name
            {
                curr_filename = search_from_end(dir_content->d_name, suffix);
                if (curr_filename) // si le suffix est aussi present dans le d_name
                    add_filename(&filenames, dir_content->d_name); 
            } 
        }
        else if (prefix_len) // est de la forme prefix*
        {
            curr_filename = search_from_start(dir_content->d_name, prefix); // ca marche ou est-ce que doit malloc ?
            if (curr_filename) // si le prefix est present dans le d_name
                add_filename(&filenames, dir_content->d_name); 
        }
        else if (suffix_len) // est de la forme *suffix
        {
            curr_filename = search_from_end(dir_content->d_name, suffix); // ca marche ou est-ce que doit malloc ?
            if (curr_filename) // si le suffix est present dans le d_name
                add_filename(&filenames, dir_content->d_name); 
        }
        dir_content = readdir(dir); // readdir() renvoie NULL si il n'y a plus d'entité de directory à lire
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
            return (ft_substr(current->content, i + 1, current->length)); // ou return (current->content + i + 1) ?
        i--;
    }
    return (NULL); // cas ou ne trouve pas ne serait-ce qu'un '*' (impossible sinon n'aurait jamais appelé cette fonction)
}

char    *ft_search_str_in_str(char *s, char *to_find)
{
    int i;
    int j;

    if (!s || ft_strlen(to_find) > ft_strlen(s))
        return (NULL);
    if (!ft_strlen(to_find)) // to_find n'est jamais NULL : il a été malloc avec substr et est au moins la chaine vide (ie "\0")
        return (s); // retourne s ou NULL ?
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
            if (s[i] || (!s[i] && !to_find[j])) // ie est sorti soit car a parcouru l'entièreté de to_find
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
    //t_filename  *tmp;

    current = (*first);
    while (current->next && current->next != to_del)
        current = current->next;
    if (!current->next)
        return ; // a parcouru toute la liste sans trouver to_del
    //tmp = to_del->next;
    current->next = to_del->next;
    free(to_del->filename);
    free(to_del); // ?
    to_del = NULL; // ?
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
    if (!(*filenames)) // ie si n'a trouvé aucun match dans le repo courant pour ce prefixe et suffixe 
        return ;
    prefix_len = ft_strlen(prefix);
    current_f = (*filenames);
    while (current_f) // regarde chaque filename un par un
    {
        i = prefix_len + 1; // si prefix non vide, i est initialisé à l'index suivant la fin du prefixe, + 1 pour "sauter" le '*'. Sinon, i est initialisé à 1
        filename_pos = current_f->filename + prefix_len; // les filenames commencent tous par le prefixe
        while (i < current->length) // ok ou c'est mieux de mettre i < ft_strlen(word) (ie i < current->length). Au cas ou i dépasse la taille de word avant de revenir dans la condition du while (aurait alors un segfault)
        {
            // sous chaine entre deux '*' : 
            // taille de la sous chaine : next_wcard_index - (curr_wcard_index + 1)
            // index du debut de la sous chaine : curr_wcard_index + 1
            next_wcadr_index = get_next_wcard_index(current, i);
            if (next_wcadr_index >= 0) // get_next_wcard() renvoie -1 en cas d'échec
            {
                // printf("filename_pos : %s\n", filename_pos);
                // printf("current->content : %s\n", current->content);
                // printf("i : %d\nnext_wcadr_index : %d\nnext_wcadr_index - i : %d\n", i, next_wcadr_index, next_wcadr_index - i);
                // printf("substring to find : %s\n\n\n", ft_substr(current->content, i, next_wcadr_index - i));
                filename_pos = ft_search_str_in_str(filename_pos, ft_substr(current->content, i, next_wcadr_index - i));
                if (!filename_pos) // ft_search_str_in_str() renvoie NULL en cas d'échec
                {
                    // remove current_f from the *filenames list
                    del_filename(filenames, current_f);
                    break;
                }
                i = next_wcadr_index + 1;
                while (current->content[i] && current->content[i] == '*' && !is_in_quotes(current, i)) // skip les autres '*' s'il y en a plusieurs successifs
                    i++;
            }
            else
                break ; //avec ça, sort du while (word[i]) et reste dans while (current_f), ouais ?
        }
        current_f = current_f->next;
    }
}

void    free_filenames(t_filename **filenames)
{
    t_filename *current;

    current = (*filenames);
    //printf("in free_filenames()\n");
    while (current)
    {
        current = (*filenames)->next;
        if ((*filenames)->filename)
        {
            //printf("(*filenames)->filename = %s\n", (*filenames)->filename);
            free((*filenames)->filename);
        }
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
    
    //tk_del_one(first, tmp);
    
    while(current_f)
    {
        tk_add_word_in_list(current, current_f->filename); // invalid read of size 1
        //printf("%s inserted\n", (*filenames)->filename);
        *current = (*current)->next; // current->next est le nouvel élément ajouté (il existe donc forcément)
        current_f = current_f->next;
    }

    
    tk_del_one(first, tmp);

    
    // new_content = ft_strdup((*filenames)->filename);
    // while (current_f->next)
    // {
    //     tmp = new_content;
    //     tmp2 = ft_strjoin(tmp, " ");
    //     new_content = ft_strjoin(tmp2, current_f->next->filename);
    //     free(tmp);
    //     free(tmp2);
    //     current_f = current_f->next;
    // }
    // free((*current)->content);
    // (*current)->content = new_content;
    // (*current)->length = ft_strlen(new_content);
    
    //free_filenames(filenames); // POURQUOI CA MARCHE PAS 
}


//A faire APRES avoir regroupé les tokens mots n'étant pas séparés par des whitespaces ou des control_op 
// (Et donc après avoir viré les quotes)

/*
Une fois qu'a choppé les prefix et suffix, fait un premier tri dans la liste des d_name
pour trouver ceux ayant des prefixes et suffixes identiques.
Une fois qu'a fait ce premier tri, itère sur chaque d_name qui matchait, 
et essayant cette fois ci de trouver des matchs pour les wildcards a l'interieur du mot (s'il y en a)
Si on arrive au dernier index du current d_name sans etre arrivé à la fin du mot, alors supprime ce d_name
de la liste de d_name résultant du premier tri.
*/
void    perform_wildcard_exp(t_ht_hash_table *ht, t_token_list **first)
{
    char            *wildcard_start;
    size_t          wildcard_index;
    char            *current_dir;
    DIR             *dir;
    t_token_list    *current;
    t_filename      *filenames;

    // faire une fonction auxiliaire open_current_dir() pour simplifier le code de cette fonction ?
    current_dir = ht_search(ht, "PWD");
    if (!current_dir)
        return ; // ?
    dir = opendir(current_dir);
    if (!dir)
    {
        perror("opendir ");
        return ; // ?
    }
    current = (*first);
    while (current)
    {
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
                insert_filenames(first, &current, &filenames); // passe deja au current->next ici
            }
            // else
            //     current = current->next;
        }
        //else
        current = current->next;
    }
}