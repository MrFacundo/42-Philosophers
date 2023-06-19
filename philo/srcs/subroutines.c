/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subroutines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:28:30 by facundo           #+#    #+#             */
/*   Updated: 2023/06/19 16:39:04 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	eat(t_philosopher *ph, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork)
{
	pthread_mutex_lock(left_fork);
	print_status(ph, "has taken a fork");
	pthread_mutex_lock(right_fork);
	print_status(ph, "has taken a fork");
	pthread_mutex_lock(ph->lock);
	ph->is_eating = 1;
	pthread_mutex_unlock(ph->lock);
	print_status(ph, "is eating");
	ft_usleep(ph->g_data->eat_t);
}

void	leave_forks(t_philosopher *ph, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork)
{
	pthread_mutex_lock(ph->lock);
	ph->last_serving_t = get_time();
	ph->servings--;
	ph->is_eating = 0;
	pthread_mutex_unlock(ph->lock);
	pthread_mutex_unlock(left_fork);
	pthread_mutex_unlock(right_fork);
}

void	nap(t_philosopher *ph)
{
	print_status(ph, "is sleeping");
	ft_usleep(ph->g_data->sleep_t);
}
