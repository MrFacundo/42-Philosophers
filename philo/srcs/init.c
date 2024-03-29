/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:13:15 by facundo           #+#    #+#             */
/*   Updated: 2023/09/16 14:34:43 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_limits(t_global_data *data)
{
	if (data->phil_amount < MIN_PHIL_AMOUNT
		|| data->phil_amount > MAX_PHIL_AMOUNT
		|| data->die_t < MIN_T || data->die_t > MAX_T
		|| data->eat_t < MIN_T || data->eat_t > MAX_T
		|| data->sleep_t < MIN_T || data->sleep_t > MAX_T)
		return (1);
	return (0);
}

void	init_philosophers(t_global_data *d)
{
	int	i;

	i = 0;
	while (i < d->phil_amount)
	{
		d->philosophers[i].id = i + 1;
		d->philosophers[i].left_fork = d->forks + i;
		d->philosophers[i].right_fork = d->forks + (i + 1) % d->phil_amount;
		d->philosophers[i].lock = d->lock + i;
		d->philosophers[i].servings = d->servings;
		d->philosophers[i].last_serving_t = d->start_time;
		d->philosophers[i].is_eating = 0;
		d->philosophers[i].is_dead = 0;
		d->philosophers[i].g_data = d;
		i++;
	}
}

int	parse_arguments(t_global_data *data, char **argv)
{
	data->phil_amount = ft_atoi(argv[1]);
	data->die_t = ft_atoi(argv[2]);
	data->eat_t = ft_atoi(argv[3]);
	data->sleep_t = ft_atoi(argv[4]);
	if (argv[5])
		data->servings = ft_atoi(argv[5]);
	else
		data->servings = -1;
	data->philosophers = malloc(sizeof(t_philosopher) * data->phil_amount);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->phil_amount);
	data->lock = malloc(sizeof(pthread_mutex_t) * data->phil_amount);
	data->someone_died = 0;
	data->start_time = get_time();
	if (!data->philosophers || !data->forks || !data->lock)
		return (free_all(data, E_MALLOC));
	if (check_limits(data))
		return (free_all(data, E_ARGS));
	return (0);
}

int	initialize_global_data(t_global_data *data, int argc, char **argv)
{
	if (parse_arguments(data, argv))
		return (1);
	if (pthread_helper(data, initialize_global_mutexes))
		return (free_all(data, E_THREAD));
	return (0);
}
