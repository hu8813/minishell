/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:13:42 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/16 17:33:37 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*ptr;

	ptr = (char *)s;
	while (*ptr != (char)c && *ptr != '\0')
		ptr++;
	if (*ptr == (char)c)
		return ((char *)ptr);
	if (!c && *ptr == '\0')
		return ((char *)ptr);
	return (NULL);
}
/*
int	main(void)
{
	printf("%s\n", ft_strchr ("hello",'l'));
	printf("%s\n", strchr ("hello",'o'));
	return (0);
}
*/