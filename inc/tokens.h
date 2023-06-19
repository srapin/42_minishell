/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 18:15:56 by Helene            #+#    #+#             */
/*   Updated: 2023/06/19 17:52:58 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# define WHITESPACES        " \t"

enum e_t_types
{
    word, // anything that isn't one of the above
    simple_quote,
    double_quote,
    r_parenthesis,
    l_parenthesis,
    l_io_redirect, // '<'
    r_io_redirect, // '>' 
    and_tk, // '&'
    or_tk, // |
    whitespace // ' ', '\t', or '\n'
};

enum e_quotes
{
    none,
    single,
    double_q
};

typedef struct  s_token
{
    int     type; /* Token type. */
    char    content;
}               t_token;

typedef struct s_word_data
{
    char                *content;
    size_t              length;
    int                 quotes;
    size_t              word_start_index; // index of this word in the merged words' resulting word (ie the big one)
    struct s_word_data  *next;
} t_word_data;

// utilisé dans l'expansion de filenames (ie gestion des wildcards)
typedef struct s_filename
{
    char                *filename;
    struct s_filename   *next;
}               t_filename;

typedef struct  s_token_list
{
    int             type; /* Token type. */
    char            *content; /* Zero terminated string. */
    size_t          length; /* Content length. Useful ? */
    int             quotes; // 0 : token does not contain quotes. 1 : token contains single quotes. 2 : contains double quotes
    t_word_data     *merged_words; // lien vers les <word> suivants parmi ceux ayant été merged ; est à NULL s'il s'agit d'un token "individuel" (ie pas un <word> ayant été merged avec d'autres <word>)
    //t_list          *before_exp;
    
    struct s_token_list *prev;
    struct s_token_list *next;
}               t_token_list;

typedef struct  s_data
{
    t_ht_hash_table *env;
    t_token_list    **first;
    t_list          *exp_history;
}               t_data;


t_token_list    *tk_new_elem(char *t_stream, size_t t_len, int t_type, int is_quote);
void            tk_add(t_token_list **first, t_token_list *new); // add at the end
void            tk_add_word_in_list(t_token_list **current, char *content); // add after the current
void            tk_del_one(t_token_list **first, t_token_list *to_del);

#endif