/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:14:44 by facundo           #+#    #+#             */
/*   Updated: 2023/09/16 14:04:57 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_status(t_philosopher *ph, char *status)
{
	long	timestamp;

	timestamp = get_time() - ph->g_data->start_time;
	if ((check_is_dead(ph) && !ft_strncmp(status, S_DIED, 5))
		|| !check_someone_died(ph->g_data))
	{
		pthread_mutex_lock(&ph->g_data->printf_mutex);
		printf("%ld %d %s\n", timestamp, ph->id, status);
		pthread_mutex_unlock(&ph->g_data->printf_mutex);
	}
}

void	ft_usleep(long time_in_ms)
{
	long	start_time;

	start_time = 0;
	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

long	get_time(void)
{
	long			timestamp;
	struct timeval	current_time;

	gettimeofday(&current_time, 0);
	timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (timestamp);
}

int	free_all(t_global_data *global_data, char *msg)
{
	if (global_data->philosophers)
		free(global_data->philosophers);
	if (global_data->forks)
		free(global_data->forks);
	if (global_data->lock)
		free(global_data->lock);
	if (msg)
		printf("%s\n", msg);
	return (1);
}
