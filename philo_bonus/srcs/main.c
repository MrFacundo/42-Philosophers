/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:12:48 by facundo           #+#    #+#             */
/*   Updated: 2023/07/27 16:37:23 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../includes/philo_bonus.h"

void	exit_routine(t_global_data *global_data, char *msg)
{
	if (global_data->philosophers)
		free(global_data->philosophers);
	if (msg)
		printf("%s\n", msg);
	exit(1);
}

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

static void	init_philo(t_global_data *global_data, int i)
{
	global_data->philosophers[i].id = i + 1;
	global_data->philosophers[i].last_serving_t = get_time();
	global_data->philosophers[i].forks = global_data->forks;
	global_data->philosophers[i].printf_sem = global_data->printf_sem;
	global_data->philosophers[i].g_data = global_data;
}

static void	waiter(t_global_data *global_data)
{
	int	i;
	int	status;

	i = -1;
	status = 0;
	while (++i < global_data->phil_amount)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) && (WEXITSTATUS(status) > 0))
		{
			while (i < global_data->phil_amount)
				if (++i != WEXITSTATUS(status))
					kill(global_data->philosophers[i - 1].pid, SIGKILL);
			return ;
		}
	}
	return ;
}

static int helper(t_global_data *data) {
	int		i;
	t_philo	*ph;

	i = -1;
	while (++i < data->phil_amount) {
		init_philo(data, i);
		ph = &data->philosophers[i];
		ph->pid = fork();
		if (!ph->pid) {
			if (pthread_create(&ph->lifecycle, 0, routine, ph)
				|| pthread_create(&ph->monitoring, 0, monitor_starvation, ph)
				|| pthread_join(ph->lifecycle, 0)
				|| pthread_join(ph->monitoring, 0))
					return 1;
			free(data->philosophers);
			exit(0);
		} else if (ph->pid < 0) {
			return 1;
		}
	}
	waiter(data);
	return 0;
}

void initialize_global_data(t_global_data *data, char **argv)
{
	data->phil_amount = ft_atoi(argv[1]);
	data->die_t = ft_atoi(argv[2]);
	data->eat_t = ft_atoi(argv[3]);
	data->sleep_t = ft_atoi(argv[4]);
	if (argv[5])
		data->servings = ft_atoi(argv[5]);
	data->philosophers = malloc(sizeof(t_philo) * data->phil_amount);
	sem_unlink("printf_sem");
	sem_unlink("forks");
	data->forks = sem_open("forks", O_CREAT, S_IRWXU, data->phil_amount);
	data->printf_sem = sem_open("printf_sem", O_CREAT, S_IRWXU, 1);
	data->start_time = get_time();
	if (!data->philosophers)
		exit_routine(data, E_MALLOC);
	if (data->forks == SEM_FAILED || data->printf_sem == SEM_FAILED)
		exit_routine(data, E_SEM);
	if (check_limits(data))
		exit_routine(data, E_ARGS);
}

int	main(int argc, char **argv)
{
	t_global_data	global_data;

	check_argc(argc);
	initialize_global_data(&global_data, argv);
	if (helper(&global_data))
		exit_routine(&global_data, E_THREAD);
	sem_close(global_data.printf_sem);
	sem_close(global_data.forks);
	sem_unlink("printf_sem");
	sem_unlink("forks");
	free(global_data.philosophers);
	return (0);
}
