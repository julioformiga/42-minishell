/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:24:21 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/10/27 22:24:21 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	reset_data(t_data *data)
{
	data->type = 0;
	data->format_type = 0;
	data->format = NULL;
	data->number_neg = 0;
	data->print = NULL;
}

char	*get_data_char(char c)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * 2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

void	get_data_strings(t_data *data, va_list args)
{
	if (data->type == '%')
		data->print = get_data_char('%');
	if (data->type == 'c')
		data->print = get_data_char(va_arg(args, int));
	else if (data->type == 's')
		data->print = get_string(va_arg(args, char *));
	else if (data->type == 'p')
		data->print = (char *)get_ptr_addrs(va_arg(args, void *));
	else if (data->type == 'u')
		data->print = ft_uitoa(va_arg(args, int));
	else if (ft_strchr("xX", data->type))
		data->print = get_unsigned_hex(va_arg(args, int), data);
	else if (ft_strchr("di", data->type))
	{
		data->print = ft_itoa(va_arg(args, int));
		if (ft_atoi(data->print) < 0)
			data->number_neg = 1;
	}
}

void	print_data(t_data *data)
{
	int	iformat;

	iformat = print_format_null(data);
	if (iformat == -1)
		return ;
	if (data->format_type == '.' && iformat < 0)
	{
		data->len += print_string("%.0");
		data->len += print_string(data->format);
		data->len += print_char(data->type);
	}
	else
	{
		print_pre_format(data);
		if (data->type == 'c' || data->type == '%')
			data->len += print_char(data->print[0]);
		else
			data->len += print_string(data->print);
		print_pos_format(data);
	}
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	t_data	data;

	reset_data(&data);
	data.len = 0;
	va_start(args, s);
	print_args(s, &data, args);
	va_end(args);
	return (data.len);
}
