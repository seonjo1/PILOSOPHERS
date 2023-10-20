/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_library.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:51:43 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/20 22:32:50 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*sign_check(char *str, long long *minus)
{
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*minus = *minus * -1;
		str++;
	}
	return (str);
}

long long	overflow_check(long long num, long long i)
{
	if (i > 10)
		return (-1);
	if (num < -2147483648 || num > 2147483647)
		return (-1);
	return (num);
}

long long	philo_atoi(char *str)
{
	long long	minus;
	long long	i;
	long long	num;

	i = 0;
	num = 0;
	minus = 1;
	while (*str >= 9 && *str <= 13)
		str++;
	str = sign_check(str, &minus);
	if (!(*str >= '0' && *str <= '9'))
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		str++;
		if (num > 0)
			i++;
	}
	return (overflow_check(num * minus, i));
}

void	*philo_error(t_philo *philos, t_arg *arg, int n)
{
	int	i;

	free(arg);
	i = 1;
	while (i <= n)
	{
		pthread_mutex_unlock(philos[i].left_fork);
		pthread_metex_destroy(philos[i].left_fork);
		i++;
	}
	free(philos);
	return (NULL);
}
int	get_len(long long n)
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
	size = get_len(n);
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

int	philo_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\n')
		i++;
	return (i);
}
