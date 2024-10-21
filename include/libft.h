/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 20:01:57 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/10/18 20:01:57 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
// ======= GET_NEXT_LINE ========
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# define MAX_FD 10240
// ========= FT_PRINTF ==========
# define PRINTF_TYPES "cspdiuxX%"
# define PRINTF_FORMAT "-0.# +"
# define PRINTF_NUMBERS "0123456789"
# define HEX_STR "0123456789abcdef"
// ==============================

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <stdarg.h>

int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(const char *str);
void	*ft_bzero(void *s, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strtrim(const char *s1, const char *set);
char	**ft_split(const char *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *newlst);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *newlst);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

char	*get_next_line(int fd);

// ======= PRINT_F =========

typedef struct s_data
{
	char			type;
	char			format_type;
	char			*format;
	char			*print;
	int				number_neg;
	int				len;
}	t_data;

int		print_char(char c);
int		print_string(char *str);
void	print_args(const char *str, t_data *data, va_list args);
void	print_data(t_data *data);
void	print_pre_format(t_data *data);
void	print_pos_format(t_data *data);
int		print_pos_format_conditions(t_data *data);
void	print_pre_format_specifics(t_data *data);
void	print_pre_format_number(t_data *data);
void	print_pre_format_string(t_data *data);
void	print_pre_pointers(t_data *data);
void	print_pre_pointers_specifics(t_data *data);
void	print_pos_pointers(t_data *data);
int		print_format_null(t_data *data);

char	*get_string(char *str);
char	*get_nbr(int nbr);
char	*get_ptr_addrs(void *ptr);
char	*get_unsigned_hex(unsigned int nbr, t_data *data);
void	get_data_strings(t_data *data, va_list args);
void	get_arg_format(char c, t_data *data);

void	show_data(t_data *data);
void	reset_data(t_data *data);
char	*ft_uitoa_base(unsigned long nbr, char *b_to, unsigned long b_size);
char	*ft_uitoa(unsigned int nbr);
int		ft_printf(const char *s, ...);
int		is_print_null(t_data *data);
int		ftype_has(t_data *data, char *has);

#endif
