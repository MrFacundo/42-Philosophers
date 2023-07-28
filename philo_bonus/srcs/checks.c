/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:05:03 by facundo           #+#    #+#             */
/*   Updated: 2023/07/28 13:33:23 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

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

void	check_argc(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf(E_ARGS);
		exit(1);
	}
}

int	check_is_dead(long timestamp, t_philo *philo)
{
	int	ret;

	sem_wait(philo->lock);
	ret = timestamp - philo->last_serving_t > philo->g_data->die_t;
	sem_post(philo->lock);
	return (ret);
}

int	check_servings(t_philo *philo)
{
	int	ret;

	sem_wait(philo->lock);
	ret = philo->g_data->servings;
	sem_post(philo->lock);
	return (ret);
}