/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priv.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:02:00 by srapin            #+#    #+#             */
/*   Updated: 2023/06/19 19:24:33 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PRIV_H
#define PRIV_H

#include "struct.h"
#include "lib.h"

//t_cmd
void init_cmd(t_cmd *cmd, char **envp);
void add_args_to_cmd(t_cmd *cmd, char *args);
void add_value_to_cmd(t_cmd *cmd, char *val); //do not use!

//t_file_struct
void init_file_struct(t_file *file_struct);

#endif