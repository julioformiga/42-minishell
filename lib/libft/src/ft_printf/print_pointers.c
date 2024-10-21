/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pointers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 05:08:42 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/02/09 05:08:42 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_pre_pointers_specifics_space(t_data *data, int i_max)
{
	int	i;

	if (data->format_type == ' ')
	{
		i = -1;
		if (!is_print_null(data) && data->format_type)
			i_max++;
		if ((is_print_null(data) && i_max > 0)
			|| !data->format_type)
			i_max++;
		if (is_print_null(data) && ft_atoi(data->format) < 0)
			i_max--;
		while (i++, i < (int)(i_max - ft_strlen(data->print) - 2))
			data->len += print_char(' ');
		if (!is_print_null(data) || (is_print_null(data)
				&& ft_atoi(data->format) >= 0))
			data->len += print_char(' ');
	}
}

void	print_pre_pointers_specifics(t_data *data)
{
	int	i;
	int	i_max;

	if (data->type != 'p' || !data->format)
		return ;
	i = -1;
	i_max = ft_atoi(data->format);
	print_pre_pointers_specifics_space(data, i_max);
	if ((ft_strchr("+#0", data->format_type) && i_max >= 0))
	{
		if (!is_print_null(data) && data->format_type
			&& ft_strchr("#", data->format_type))
			i_max++;
		if ((is_print_null(data) && ft_strchr("#", data->format_type)
				&& i_max > 0)
			|| !data->format_type)
			i_max++;
		if (is_print_null(data) && data->format_type == '0')
			i_max += 2;
		while (i++, i < (int)(i_max - ft_strlen(data->print) - 2))
			data->len += print_char(' ');
		if (data->format_type != '0'
			&& ft_atoi(data->format) > (int)ft_strlen(data->print))
			data->len += print_char(' ');
	}
}

static int	add_pointers_zeros(t_data *data)
{
	int	len;

	len = 0;
	if (ftype_has(data, "0."))
		len = ft_atoi(data->format) - ft_strlen(data->print);
	if (ftype_has(data, "."))
		len += 2;
	return (len);
}

void	print_pre_pointers(t_data *data)
{
	int		len;
	char	*tmp;
	char	*data_print;

	if (data->type != 'p' || !data->format || !data->format_type
		|| is_print_null(data))
		return ;
	if (data->format_type == '.'
		|| (data->format_type == '0' && ft_atoi(data->format) >= 0))
	{
		len = add_pointers_zeros(data);
		tmp = ft_substr(data->print, 2, ft_strlen(data->print));
		free(data->print);
		while (len--, len >= 0)
		{
			data_print = ft_strjoin("0", tmp);
			free(tmp);
			tmp = data_print;
		}
		data_print = ft_strjoin("0x", tmp);
		free(tmp);
		data->print = data_print;
	}
}

void	print_pos_pointers(t_data *data)
{
	if (data->type == 'p' && ftype_has(data, "+ ")
		& is_print_null(data) && ft_atoi(data->format) < 0)
		data->len += print_char(' ');
}
