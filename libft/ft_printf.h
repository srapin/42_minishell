/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srapin <srapin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 21:40:46 by srapin            #+#    #+#             */
/*   Updated: 2023/02/12 13:11:46 by srapin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "mylibft/libft.h"
# include "gnl/get_next_line_bonus.h"
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# define TYPES_SYMBOL "cspdiuxX%"
# define OPTIONS_SYMBOL "-0# +"
# define DIGIT_SYMBOL "0123456789"
# define FORMAT_SYMBOL "cspdiuxX%-0# +.123456789"

enum		e_option
{
	minus,
	zero,
	hashtag,
	space,
	plus,
	point,
	neg
};

enum		e_len
{
	width = 0,
	precision = 1,
	num_len = 2,
	str_len = 2,
	prefix_len = 3
};

typedef struct s_print
{
	char	type;
	int		option[7];
	int		len[4];
	int		counter;
}			t_print;

int			ft_max(int a, int b);
int			ft_count_nbr_len(unsigned long long ui, int base_size);
void		set_int_len(unsigned long long ui, int base_size,
				t_print *to_print);
char		*parse_format(char *format, t_print *to_print, va_list test);
void		ft_print_arg(t_print *to_print, va_list args);
void		ft_print_char(t_print *to_print, char c);
void		ft_print_str(t_print *to_print, char *str);
void		ft_print_mod(t_print *to_print);
void		ft_print_int(t_print *to_print, long i);
void		ft_print_hex(t_print *to_print, unsigned int ui, int maj);
void		ft_print_unsigned(t_print *to_print, unsigned int i);
void		ft_print_ptr(t_print *to_print, unsigned long long i);
int			ft_printf(const char *format, ...);
void		ft_putnbr_base(unsigned long long nbr, char *base,
				t_print *to_print);
void		ft_reset_struct(t_print *to_print);
void		ft_init_struct(t_print *to_print);
size_t		ft_strnlen(const char *s, size_t n);
int			ft_strstr(char *str, char *to_find, int len);
void		write_and_count(char *str, t_print *to_print, int len);
size_t		ft_strcat(char *dst, const char *src);
char		*get_next_line(int fd);
#endif
