/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_pre_format_space_plus.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 09:51:45 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/11/15 09:51:45 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_pre_plus_dip(t_data *data)
{
	int	i;
	int	i_max;

	if (data->format_type != '+')
		return ;
	if ((ft_strchr("di", data->type))
		|| (data->format_type == '#' && data->type == 'p'))
	{
		i = -1;
		i_max = ft_atoi(data->format) - ft_strlen(data->print) - 1;
		if (ft_atoi(data->print) < 0)
			i_max++;
		if (is_print_null(data))
			i_max -= 5;
		while (i++, i < i_max)
			data->len += print_char(' ');
	}
	if (ft_strchr("dip", data->type)
		&& ft_atoi(data->print) >= 0
		&& !is_print_null(data))
		data->len += print_char(data->format_type);
	if (is_print_null(data) && ft_atoi(data->format) > 0)
		data->len += print_char(' ');
}

static void	print_pre_space_dis(t_data *data)
{
	if (data->format_type == ' ' && data->format == NULL)
	{
		if (ft_strchr("s", data->type)
			&& ft_strlen(data->print) > 1
			&& !is_print_null(data))
			data->len += print_char(' ');
		if (ft_strchr("di", data->type) && ft_atoi(data->print) >= 0)
			data->len += print_char(' ');
	}
}

void	print_pre_format_specifics(t_data *data)
{
	int		i_format;
	int		i_lenprint;
	char	*tmp;

	print_pre_pointers(data);
	if (data->type == 's' && data->format_type == '.')
	{
		if (!data->format)
			ft_memcpy(data->print, "", 1);
		else
		{
			i_format = ft_atoi(data->format);
			i_lenprint = ft_strlen(data->print);
			if (i_format < i_lenprint)
			{
				tmp = ft_calloc(i_format + 1, sizeof(char));
				ft_memcpy(tmp, data->print, i_format);
				ft_memcpy(data->print, tmp, i_format + 1);
				free(tmp);
			}
		}
	}
	print_pre_pointers_specifics(data);
	print_pre_plus_dip(data);
	print_pre_space_dis(data);
}

int	print_format_null(t_data *data)
{
	int	iformat;

	iformat = 0;
	if (data->format)
		iformat = ft_atoi(data->format);
	else if (ft_strchr("dip", data->type)
		&& (data->format_type == '+' || data->format_type == ' ')
		&& !is_print_null(data) && data->number_neg == 0)
	{
		data->len += print_char(data->format_type);
	}
	else if (ft_strchr("s", data->type) && data->format_type == '.')
		return (-1);
	return (iformat);
}
