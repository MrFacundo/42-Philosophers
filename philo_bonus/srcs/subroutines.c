/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subroutines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:44:03 by facundo           #+#    #+#             */
/*   Updated: 2023/08/02 11:35:17 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	eat(t_philo *philo)
{
	print_status(philo, S_FORK);
	print_status(philo, S_FORK);
	print_status(philo, S_EAT);
	sem_wait(philo->lock);
	philo->is_eating = 1;
	sem_post(philo->lock);
	ft_usleep(philo->g_data->eat_t * 1000);
}

void	leave_forks(t_philo *philo)
{
	sem_wait(philo->lock);
	philo->is_eating = 0;
	philo->last_serving_t = get_time();
	philo->g_data->servings--;
	sem_post(philo->lock);
	sem_post(philo->g_data->forks);
	sem_post(philo->g_data->forks);
}

void	nap(t_philo *philo)
{
	print_status(philo, S_SLEEP);
	ft_usleep(philo->g_data->sleep_t * 1000);
}
