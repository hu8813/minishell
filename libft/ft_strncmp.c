/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 13:13:43 by hakaraka          #+#    #+#             */
/*   Updated: 2023/07/16 17:33:19 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while ((*s1 || *s2) && (n > 0))
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		n--;
		s1++;
		s2++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int main ()
{
	char	str1[50]="za";
	char	str2[50]="";
	printf("ft_cmp:%d\n", ft_strncmp(str1, str2, 5));
	printf("cmp:%d\n", strncmp(str1, str2, 5));
}
*/