/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 16:37:19 by facundo           #+#    #+#             */
/*   Updated: 2023/07/11 16:37:43 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	create_monitor_thread(t_global_data *data)
{
	return (pthread_create(&data->monitoring, 0, monitor_starvation, data));
}

int	create_lifecycle_threads(t_philosopher *ph)
{
	return (pthread_create(&ph->lifecycle, 0, routine, ph));
}

int	initialize_global_mutexes(t_global_data *data)
{
	return (pthread_mutex_init(&data->someone_died_mutex, 0)
		|| pthread_mutex_init(&data->printf_mutex, 0));
}

int	initialize_mutexes(t_global_data *data, int i)
{
	return (pthread_mutex_init(&data->forks[i], 0)
		|| pthread_mutex_init(&data->lock[i], 0));
}

int	destroy_mutexes(t_global_data *data, int i)
{
	return (pthread_mutex_destroy(&data->forks[i])
		|| pthread_mutex_destroy(&data->lock[i]));
}
