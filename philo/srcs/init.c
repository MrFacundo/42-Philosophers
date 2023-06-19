/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:13:15 by facundo           #+#    #+#             */
/*   Updated: 2023/06/19 15:31:27 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void init_philosophers(t_global_data *global_data)
{
	int i;

	i = 0;
	while (i < global_data->phil_amount)
	{
		global_data->philosophers[i].id = i + 1;
		global_data->philosophers[i].left_fork = global_data->forks + i;
		global_data->philosophers[i].right_fork = global_data->forks + (i + 1) % global_data->phil_amount;
		global_data->philosophers[i].lock = global_data->lock + i;
		global_data->philosophers[i].servings = global_data->servings;
		global_data->philosophers[i].last_serving_t = global_data->start_time;
		global_data->philosophers[i].is_eating = 0;
		global_data->philosophers[i].has_left_fork = 0;
		global_data->philosophers[i].has_right_fork = 0;
		global_data->philosophers[i].is_dead = 0;
		global_data->philosophers[i].g_data = global_data;
		i++;
	}
}

int initialize_global_data(t_global_data *global_data, int argc, char **argv)
{
	global_data->phil_amount = ft_atoi(argv[1]);
	global_data->die_t = ft_atoi(argv[2]);
	global_data->eat_t = ft_atoi(argv[3]);
	global_data->sleep_t = ft_atoi(argv[4]);
	if (argc == 6)
		global_data->servings = ft_atoi(argv[5]);
	else
		global_data->servings = -1;
	global_data->philosophers = malloc(sizeof(t_philosopher) * global_data->phil_amount);
	global_data->forks = malloc(sizeof(pthread_mutex_t) * global_data->phil_amount);
	global_data->lock = malloc(sizeof(pthread_mutex_t) * global_data->phil_amount);
	global_data->someone_died = 0;
	global_data->start_time = get_time();
	if (global_data->phil_amount > 200 || global_data->die_t < 60 || global_data->eat_t < 60 || global_data->sleep_t < 60)
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	return (0);
}