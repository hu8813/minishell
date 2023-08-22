/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:13:42 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/26 12:14:57 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	i;

	i = ft_strlen(s) + 1;
	str = (char *)ft_calloc(sizeof(char), i);
	if (!str)
		return (NULL);
	while (i--)
		str[i] = s[i];
	return (str);
}
/*
int	main(void)
{
	char	*s1;
	char	*s2;

	s1 = "test";
	s2 = ft_strdup(s1);
	printf("%s\n", s2);
	return (0);
}
*/
