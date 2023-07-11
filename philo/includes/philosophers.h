/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:51 by facundo           #+#    #+#             */
/*   Updated: 2023/07/11 16:38:43 by facundo          ###   ########.fr       */
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
# include <sys/time.h>

// limits
# define MIN_PHIL_AMOUNT 1
# define MAX_PHIL_AMOUNT 200
# define MIN_T 60
# define MAX_T 60000

// messages
# define S_FORK "has taken a fork"
# define S_EAT "is eating"
# define S_SLEEP "is sleeping"
# define S_THINK "is thinking"
# define S_DIED "died"
# define E_MALLOC "Error: malloc failed\n"
# define E_THREAD "Error: pthread function failed\n"
# define E_ARGS "Error: bad arguments\n"

struct	s_global_data;
struct	s_philosopher;
typedef struct s_philosopher
{
	struct s_global_data	*g_data;
	int						id;
	pthread_t				lifecycle;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	pthread_mutex_t			*lock;
	int						servings;
	long					last_serving_t;
	int						is_eating;
	int						is_dead;
}	t_philosopher;

typedef struct s_global_data
{
	int						phil_amount;
	pthread_t				monitoring;
	int						someone_died;
	pthread_mutex_t			someone_died_mutex;
	pthread_mutex_t			printf_mutex;
	t_philosopher			*philosophers;
	pthread_mutex_t			*forks;
	pthread_mutex_t			*lock;
	long					start_time;
	int						eat_t;
	int						sleep_t;
	int						die_t;
	int						servings;
}	t_global_data;

//checks
int		check_is_dead(t_philosopher *ph);
int		check_servings(t_philosopher *ph);
int		check_someone_died(t_global_data *data);

//debug
void	print_results(t_global_data *global_data);

//libft
int		ft_atoi(const char *nptr);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

//main
void	*monitor_starvation(void *arg);
void	*routine(void *arg);

//init
void	init_philosophers(t_global_data *global_data);
int		initialize_global_data(t_global_data *global_data,
			int argc, char **argv);

//pthread_functions
int		initialize_global_mutexes(t_global_data *data);
int		pthread_helper(t_global_data *data, void*f);

//pthread_utils
int		create_monitor_thread(t_global_data *data);
int		create_lifecycle_threads(t_philosopher *ph);
int		initialize_global_mutexes(t_global_data *data);
int		initialize_mutexes(t_global_data *data, int i);
int		destroy_mutexes(t_global_data *data, int i);

//subroutines
void	assign_forks(t_philosopher *ph, pthread_mutex_t **left_fork,
			pthread_mutex_t **right_fork);
void	eat(t_philosopher *ph, pthread_mutex_t *left_fork,
			pthread_mutex_t *right_fork);
void	leave_forks(t_philosopher *ph, pthread_mutex_t *left_fork,
			pthread_mutex_t *right_fork);
void	nap(t_philosopher *ph);

//utils
void	print_status(t_philosopher *ph, char *status);
void	ft_usleep(long time_in_ms);
long	get_time(void);
void	update_elapsed_last_serving_time(t_philosopher *ph);
int		pthread_helper(t_global_data *global_data, void*f);
void	free_all(t_global_data *global_data, char *msg);

#endif