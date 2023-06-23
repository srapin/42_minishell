/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 17:25:15 by Helene            #+#    #+#             */
/*   Updated: 2023/06/23 14:59:35 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# define GETCWD_INITIAL_BUF_SIZE    50 /* Initial buffer size for getcwd()'s call*/

// parsing
# define EXIT_OK                    0
# define SYNTAX_ERROR               2

// exec
# define NO_FILE_OR_DIR             1 // "no such file or directory" error's exit status
# define CMD_NOT_EXECUTABLE         126 // cannot execute the command (ex : `./` : is a directory)
# define CMD_NOT_FOUND              127

// builtins
# define CD_TOO_MANY_ARGS           1
# define INVALID_VAR_ID             1 // "not a valid identifier"'s export error message
# define CANNOT_ACCESS_DIR          1 // chdir error 
# define HOME_NOT_SET               1
# define NOT_A_NUM                  2 // "numeric argument required" error in exit()

#endif