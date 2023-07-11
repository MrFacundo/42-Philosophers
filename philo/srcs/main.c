/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:24 by facundo           #+#    #+#             */
/*   Updated: 2023/07/11 17:02:08 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*monitor_starvation(void *arg)
{
	t_global_data	*data;
	int				i;

	data = (t_global_data *)arg;
	ft_usleep(data->die_t);
	while (!check_someone_died(data))
	{
		i = -1;
		while (++i < data->phil_amount)
		{
			check_servings(&data->philosophers[i]);
			if (check_is_dead(&data->philosophers[i]))
			{
				pthread_mutex_lock(&data->someone_died_mutex);
				data->someone_died = 1;
				pthread_mutex_unlock(&data->someone_died_mutex);
				print_status(&data->philosophers[i], S_DIED);
			}
		}
	}
	return (0);
}

void	table_for_one(t_philosopher *ph, pthread_mutex_t *left_fork)
{
	pthread_mutex_lock(left_fork);
	print_status(ph, S_FORK);
	ft_usleep(ph->g_data->die_t);
	pthread_mutex_unlock(left_fork);
}

// modifies left and right fork pointers to avoid deadlocks
void	assign_forks(t_philosopher *ph,
		pthread_mutex_t **left_fork,
		pthread_mutex_t **right_fork)
{
	if (ph->id % 2 == 0)
	{
		*left_fork = ph->right_fork;
		*right_fork = ph->left_fork;
	}
	else
	{
		*left_fork = ph->left_fork;
		*right_fork = ph->right_fork;
	}
}

void	*routine(void *arg)
{
	t_philosopher	*ph;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	ph = (t_philosopher *)arg;
	assign_forks(ph, &left_fork, &right_fork);
	while (!check_servings(ph) && !check_someone_died(ph->g_data))
	{
		print_status(ph, S_THINK);
		if (ph->g_data->phil_amount == 1)
		{
			table_for_one(ph, left_fork);
			break ;
		}
		else
		{
			eat(ph, left_fork, right_fork);
			leave_forks(ph, left_fork, right_fork);
			nap(ph);
		}
	}
	return (0);
}

/* args: number_of_philosophers, time_to_die, time_to_eat, time_to_sleep,
[number_of_times_each_philosopher_must_eat] */
int	main(int argc, char **argv)
{
	t_global_data	global_data;

	if (argc < 5 || argc > 6)
	{
		printf(E_ARGS);
		return (1);
	}
	if (initialize_global_data(&global_data, argc, argv))
		return (1);
	init_philosophers(&global_data);
	if (pthread_helper(&global_data, pthread_mutex_init)
		|| pthread_helper(&global_data, pthread_create) 
		|| pthread_helper(&global_data, pthread_join)
		|| pthread_helper(&global_data, pthread_mutex_destroy))
	{
		free_all(&global_data, E_THREAD);
		return (1);
	}
	print_results(&global_data);
	free_all(&global_data, 0);
	return (0);
}
