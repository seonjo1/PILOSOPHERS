/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_itoa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 17:35:31 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/24 17:37:27 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\n')
		i++;
	return (i);
}

int	philo_get_len(long long n)
{
	int	size;

	size = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		size++;
	while (n)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

char	*philo_itoa(long long n)
{
	char			*s;
	long long		num;
	int				size;

	num = n;
	size = philo_get_len(n);
	s = malloc(sizeof(char) * (size + 1));
	if (s == NULL)
		return (NULL);
	if (num == 0)
		s[0] = '0';
	if (num < 0)
	{
		s[0] = '-';
		num = -num;
	}
	s[size--] = 0;
	while (num)
	{
		s[size--] = num % 10 + '0';
		num = num / 10;
	}
	return (s);
}
