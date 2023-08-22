/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:34:45 by hakaraka          #+#    #+#             */
/*   Updated: 2023/08/21 02:38:07 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* counts occurrence of a char in a string */
int	ft_countc(char *s, char c)
{
	int	count;

	if (!s)
		return (0);
	count = 0;
	while (*s)
	{
		if (*s == c)
			count++;
		s++;
	}
	return (count);
}

/* returns index of a char (from a set of chars) in a string
	if any of the chars is found in the string, returns the index
	if none of chars found in string, -1 is returned */
int	ft_strchrs_idx(const char *s, char *set)
{
	int	i;

	i = 0;
	if (!s)
		return (-1);
	while (s[i] != '\0')
	{
		if (ft_strchr(set, s[i]))
			return (i);
		i++;
	}
	return (-1);
}

/* returns index of a char in a string if found, else -1 if not found */
int	ft_strchr_idx(const char *s, int c)
{
	unsigned char	c_unsigned;
	int				i;

	i = 0;
	if (!s)
		return (-1);
	c_unsigned = (unsigned char)c;
	while (s[i])
	{
		if (s[i] == c_unsigned)
			return (i);
		i++;
	}
	if (c_unsigned == '\0')
		return (i);
	return (-1);
}

/* to join 3 strings, used only in env file to setenv */
char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*ret;

	ret = NULL;
	tmp = ft_strjoin(s1, s2);
	if (tmp)
	{
		ret = ft_strjoin(tmp, s3);
		ft_free_all(tmp, NULL, NULL, NULL);
	}
	return (ret);
}

int	ft_printf_fd(int fd, char *format, ...)
{
	char	*str;
	va_list	ap;

	va_start(ap, format);
	while (*format)
	{
		if (*format != '%')
			write(fd, format, 1);
		else
		{
			if (*++format == 's')
			{
				str = va_arg(ap, char *);
				if (!str)
					str = "(null)";
				while (*str)
					write(fd, str++, 1);
			}
		}
		format++;
	}
	va_end(ap);
	return (1);
}
