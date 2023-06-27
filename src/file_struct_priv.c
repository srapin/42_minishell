/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_struct_priv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:08:04 by srapin            #+#    #+#             */
/*   Updated: 2023/06/27 18:48:27 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_file_struct(t_file *file_struct)
{
	file_struct->name = NULL;
	file_struct->fd = -1;
	file_struct->flag = -1;
}
