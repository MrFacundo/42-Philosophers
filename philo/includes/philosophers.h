/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:51 by facundo           #+#    #+#             */
/*   Updated: 2023/06/12 15:15:46 by facundo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <time.h>
# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
#include <sys/time.h>

# define NUM_PHILOSOPHERS 20
# define SERVINGS 4
# define EAT_T 500
# define SLEEP_T 500

typedef struct philosophers
{
	int				id;
	pthread_t		lifecycle;
	pthread_t		monitoring;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*someone_died_mutex;
	int				servings;
	int				phil_amount;
	int				*someone_died;
	struct timeval	start_time;
	long			*total_elapsed_t;
	struct timeval	last_serving_t;
	long			elapsed_last_serving_t;
	int				eat_t;
	int				sleep_t;
	int				starvation_t;
	int				is_eating;
}	t_philosopher;

typedef struct global_data
{
	int				phil_amount;
	int				someone_died;
	pthread_mutex_t	someone_died_mutex;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	long			total_elapsed_t;
	struct timeval	start_time;
	int				eat_t;
	int				sleep_t;
	int				starvation_t;
	int				servings;
}	t_global_data;

//debug
void	print_results(t_global_data *global_data);

//libft
int		ft_atoi(const char *nptr);

//main
void	*check_starvation(void *arg);
void	*routine(void *arg);

//init
void	init_philosophers(t_global_data *global_data);
int		initialize_global_data(t_global_data *global_data, int argc, char **argv);

//subroutines
void	assign_forks(t_philosopher *phil, pthread_mutex_t **left_fork, pthread_mutex_t **right_fork);
void	eat(t_philosopher *phil, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork);
void	leave_forks(t_philosopher *phil, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork);
void	nap(t_philosopher *phil);

//utils
void	update_elapsed_total_time(t_philosopher *phil);
long	get_timestamp(t_philosopher *phil);
void	update_elapsed_last_serving_time(t_philosopher *phil);
int		pthread_helper(t_global_data *global_data, void*f);
int		check_someone_died(t_philosopher *phil);



# endif