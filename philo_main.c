/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seonjo <seonjo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:43:58 by seonjo            #+#    #+#             */
/*   Updated: 2023/10/26 12:31:28 by seonjo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*arg_check(t_arg *arg)
{
	if (arg->number_of_philo <= 0 || arg->time_to_die <= 0 || \
		arg->time_to_eat <= 0 || arg->time_to_sleep <= 0 || \
		arg->eat_num_limit <= 0)
	{
		free(arg);
		return (NULL);
	}
	return (arg);
}

void	*arg_setting(int argc, char **argv)
{
	t_arg	*arg;

	arg = malloc(sizeof(t_arg));
	if (arg == NULL)
		return (NULL);
	arg->number_of_philo = philo_atoi(argv[1]);
	arg->time_to_die = philo_atoi(argv[2]);
	arg->time_to_eat = philo_atoi(argv[3]);
	arg->time_to_sleep = philo_atoi(argv[4]);
	if (argc == 6)
	{
		arg->eat_num_limit = philo_atoi(argv[5]);
		arg->is_have_eat_num_limit = 1;
	}
	else
	{
		arg->eat_num_limit = 1;
		arg->is_have_eat_num_limit = 0;
	}
	return (arg_check(arg));
}

void	*setting_philo(t_philo *philos, t_arg *arg, \
						pthread_mutex_t *print_mutex, int i)
{
	philos[i].philo_num = i;
	philos[i].arg = arg;
	philos[i].eat_num = 0;
	philos[i].dead = 0;
	philos[i].print_mutex = print_mutex;
	philos[i].left_fork = malloc(sizeof(pthread_mutex_t));
	if (philos[i].left_fork == NULL)
		return (philo_free(philos, arg, i - 1, 0));
	philos[i].dead_mutex = malloc(sizeof(pthread_mutex_t));
	if (philos[i].dead_mutex == NULL)
		return (philo_free(philos, arg, i - 1, 1));
	if (pthread_mutex_init(philos[i].left_fork, NULL) != 0)
		return (philo_free(philos, arg, i - 1, 2));
	if (pthread_mutex_init(philos[i].dead_mutex, NULL) != 0)
		return (philo_free(philos, arg, i - 1, 3));
	return (philos);
}

void	*make_philos(t_philo *philos, t_arg *arg)
{
	pthread_mutex_t	*print_mutex;
	int				i;

	print_mutex = malloc(sizeof(pthread_mutex_t));
	if (print_mutex == NULL)
		philo_free(philos, arg, 0, -2);
	if (pthread_mutex_init(print_mutex, NULL) != 0)
		philo_free(philos, arg, 0, -1);
	i = 1;
	while (i <= arg->number_of_philo)
		if (setting_philo(philos, arg, print_mutex, i++) == NULL)
			return (NULL);
	i = 1;
	while (i <= arg->number_of_philo)
	{
		philos[i].right_fork = philos[(i % arg->number_of_philo) + 1].left_fork;
		i++;
	}
	return (philos);
}

void	*start_philos(t_philo *philos, t_arg *arg, int p_num)
{
	struct timeval	tv;
	int				i;

	if (gettimeofday(&tv, NULL) != 0)
		return (philo_free(philos, arg, p_num, 0));
	i = 1;
	while (i <= p_num)
	{
		philos[i].start_time = philo_get_time(tv.tv_sec, tv.tv_usec);
		philos[i].last_eating_time = philo_get_time(tv.tv_sec, tv.tv_usec);
		if (pthread_create(&(philos[i].thread_id), NULL, (void *)philo_action, \
		&philos[i]) != 0)
			return (philo_free(philos, arg, p_num, 0));
		i++;
	}
	return (philos);
}

int	main(int argc, char **argv)
{
	t_philo	*philos;
	t_arg	*arg;
	int		errflag;

	if (!(argc == 5 || argc == 6))
		return (1);
	arg = arg_setting(argc, argv);
	if (arg == NULL)
		return (philo_error());
	philos = malloc(sizeof(t_philo) * (arg->number_of_philo + 1));
	if (make_philos(philos, arg) == NULL)
		return (philo_error());
	if (start_philos(philos, arg, arg->number_of_philo) == NULL)
		return (philo_error());
	errflag = philo_monitoring(philos, arg);
	philo_join(philos);
	philo_free(philos, arg, arg->number_of_philo, 0);
	return (errflag);
}
