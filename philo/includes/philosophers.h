/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facundo <facundo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 09:12:51 by facundo           #+#    #+#             */
/*   Updated: 2023/06/19 15:31:27 by facundo          ###   ########.fr       */
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

struct global_data;
struct philosopher;
typedef struct philosophers
{
    struct global_data *g_data;
	int				id;
	pthread_t		lifecycle;
	pthread_t		monitoring;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*lock;
	int				has_left_fork;
	int				has_right_fork;
	int				servings;
	long			last_serving_t;
	int				is_eating;
	int				is_dead;
}	t_philosopher;

typedef struct global_data
{
	int				phil_amount;
	int				someone_died;
	pthread_mutex_t	someone_died_mutex;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*lock;
	long			start_time;
	int				eat_t;
	int				sleep_t;
	int				die_t;
	int				servings;
}	t_global_data;

//debug
void	print_results(t_global_data *global_data);

//libft
int		ft_atoi(const char *nptr);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

//main
void	*check_starvation(void *arg);
void	*routine(void *arg);

//init
void	init_philosophers(t_global_data *global_data);
int		initialize_global_data(t_global_data *global_data, int argc, char **argv);

//subroutines
void	assign_forks(t_philosopher *ph, pthread_mutex_t **left_fork, pthread_mutex_t **right_fork);
void	eat(t_philosopher *ph, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork);
void	leave_forks(t_philosopher *ph, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork);
void	nap(t_philosopher *ph);

//utils
void	print_status(t_philosopher *ph, char *status);
void	ft_usleep(long int time_in_ms);
long	get_time(void);
void	update_elapsed_last_serving_time(t_philosopher *ph);
int		pthread_helper(t_global_data *global_data, void*f);
int		check_someone_died(t_philosopher *ph);



# endif