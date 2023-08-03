/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:08:05 by facundo           #+#    #+#             */
/*   Updated: 2023/08/03 11:15:24 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	open_semaphores(t_global_data *data)
{
	sem_unlink("printf_lock");
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("term_lock");
	sem_unlink("shotdown");
	sem_unlink("total_servings");
	data->term_lock = sem_open("term_lock", O_CREAT, S_IRWXU, 1);
	data->forks = sem_open("forks", O_CREAT, S_IRWXU, data->phil_amount);
	data->shotdown = sem_open("shotdown", O_CREAT, S_IRWXU, 0);
	data->printf_lock = sem_open("printf_lock", O_CREAT, S_IRWXU, 1);
	data->total_servings = sem_open("total_servings", O_CREAT, S_IRWXU, 0);
}

void	initialize_global_data(t_global_data *data, char **argv)
{
	data->phil_amount = ft_atoi(argv[1]);
	data->die_t = ft_atoi(argv[2]);
	data->eat_t = ft_atoi(argv[3]);
	data->sleep_t = ft_atoi(argv[4]);
	data->servings = -1;
	if (argv[5])
		data->servings = ft_atoi(argv[5]);
	data->philosophers = malloc(sizeof(t_philo) * data->phil_amount);
	data->term = 0;
	open_semaphores(data);
	data->start_time = get_time();
	if (!data->philosophers)
		exit_routine(data, E_MALLOC);
	if (data->term_lock == SEM_FAILED || data->forks == SEM_FAILED
		|| data->shotdown == SEM_FAILED || data->printf_lock == SEM_FAILED
		|| data->total_servings == SEM_FAILED)
		exit_routine(data, E_SEM);
	if (check_argv(data))
		exit_routine(data, E_ARGS);
}
