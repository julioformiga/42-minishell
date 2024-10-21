/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_get_strings.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julio.formiga <julio.formiga@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 03:51:02 by julio.formiga     #+#    #+#             */
/*   Updated: 2023/11/14 03:51:02 by julio.formiga    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*get_string(char *str)
{
	if (!str)
	{
		free(str);
		return (ft_strdup("(null)"));
	}
	return (ft_strdup(str));
}

char	*get_nbr(int nbr)
{
	char	*str;

	str = ft_itoa(nbr);
	return (str);
}

char	*get_ptr_addrs(void *ptr)
{
	char			*str;
	char			*dest;
	unsigned long	i;

	i = (unsigned long)ptr;
	str = ft_uitoa_base(i, HEX_STR, 16);
	if (!str || !i)
	{
		free(str);
		return (ft_strdup("(nil)"));
	}
	dest = ft_strjoin("0x", str);
	free(str);
	return (dest);
}

char	*get_unsigned_hex(unsigned int nbr, t_data *data)
{
	char			*str;
	char			*nb;
	unsigned long	i;

	i = -1;
	nb = ft_uitoa_base((unsigned long)nbr, HEX_STR, 16);
	if (data->format_type == '#' && nbr != 0)
		str = ft_strjoin("0x", nb);
	else
		str = ft_strdup(nb);
	free(nb);
	if (data->type == 'X')
		while (str[++i])
			str[i] = ft_toupper(str[i]);
	return (str);
}

void	get_arg_format(char c, t_data *data)
{
	if (ft_strchr(PRINTF_FORMAT, c)
		&& data->format_type == 0 && !data->format)
		data->format_type = c;
	else
	{
		if (!data->format)
			data->format = ft_calloc(1, sizeof(char *));
		data->format[ft_strlen(data->format)] = c;
	}
	if (data->format_type == 0 && ft_strchr(data->format, '.'))
		data->format_type = '.';
}
