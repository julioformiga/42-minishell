/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_cspudi.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 03:46:48 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/11/14 03:46:48 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_pre_format_number(t_data *data)
{
	int		i;
	int		i_max;

	i = -1;
	if (ft_strchr("diu", data->type) && data->format)
	{
		if (ft_strchr("#", data->format_type)
			|| (data->type == 'u' && ft_strchr("+", data->format_type)))
		{
			i_max = ft_atoi(data->format);
			while (i++, i < (int)(i_max - ft_strlen(data->print)))
				data->len += print_char(' ');
		}
	}
}

void	print_pre_format_string(t_data *data)
{
	int		i;
	int		i_max;

	i = 0;
	i_max = 0;
	if (ft_strchr("sxX", data->type)
		&& data->format_type == '.'
		&& !ft_strchr(data->format, '.'))
		return ;
	if (data->type == 'c' && !data->format_type && data->format)
	{
		i_max = ft_atoi(data->format);
		while (i++, i < (int)(i_max - 1))
			data->len += print_char(' ');
	}
	if (data->format && ((ft_strchr("xX", data->type)
				&& (!ft_strchr(" 0-.", data->format_type)
					|| !data->format_type))
			|| (data->type == 's' && ft_strchr("# 0+.", data->format_type))))
	{
		i = -1;
		i_max = ft_atoi(data->format);
		while (i++, i < (int)(i_max - ft_strlen(data->print)))
			data->len += print_char(' ');
	}
}

void	print_args(const char *s, t_data *data, va_list args)
{
	while (*s)
	{
		reset_data(data);
		if (*s == '%')
		{
			while (s++, !ft_strchr(PRINTF_TYPES, *s))
				get_arg_format(*s, data);
			if (ft_strchr(PRINTF_TYPES, *s))
				data->type = *s;
			get_data_strings(data, args);
			print_data(data);
			free(data->print);
			free(data->format);
		}
		else
			data->len += print_char(*s);
		s++;
	}
}
