/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 02:14:57 by julio.formiga     #+#    #+#             */
/*   Updated: 2024/02/06 02:14:57 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
void	show_data(t_data *data)
{
	printf("\n=== show_data ===\n");
	printf("type: %c\n", data->type);
	printf("format_type: |%c|\n", data->format_type);
	printf("format: |%s|\n", data->format);
	printf("numer_neg: |%d|\n", data->number_neg);
	printf("print: |%s|\n", data->print);
	printf("len: |%d|\n", data->len);
	printf("==================\n");
}
*/

int	print_char(char c)
{
	write(1, &c, 1);
	return (1);
}

int	print_string(char *str)
{
	size_t	i;

	if (!str)
		str = "(null)";
	i = ft_strlen(str);
	write(1, str, i);
	return (i);
}

int	is_print_null(t_data *data)
{
	if (ft_strncmp(data->print, "(nil)", 5) == 0)
		return (1);
	if (ft_strncmp(data->print, "(null)", 6) == 0)
		return (1);
	return (0);
}

int	ftype_has(t_data *data, char *has)
{
	if (ft_strchr(has, data->format_type))
		return (1);
	return (0);
}

int	print_pos_format_conditions(t_data *data)
{
	if (ft_strchr("dip", data->type))
	{
		if (ft_strchr("di", data->type) && data->format_type == '0'
			&& ft_atoi(data->format) < 0 && ft_atoi(data->print) == INT_MIN)
			return (1);
		if (ft_strchr("+ ", data->format_type) && !data->number_neg)
			return (1);
		if (ft_strchr("0", data->format_type) && data->number_neg)
			return (1);
	}
	return (0);
}
