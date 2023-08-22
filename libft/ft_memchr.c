/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:13:42 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/16 17:33:37 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		if (*str == (unsigned char)c)
			return (str);
		str++;
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
	char		str[50] = "he.ee.gaegea";;
	char		ch = '.';
	char		*ret;

	ret = ft_memchr(str, ch, 29);
	//ret = memchr(str, ch, 29);
	printf("String after |%c| is - |%s|\n", ch, (char *)ft_memchr(str, ch, 29));
	return (0);
}
*/