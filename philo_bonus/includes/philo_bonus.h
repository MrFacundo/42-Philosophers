/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: facu <facu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:12:01 by facundo           #+#    #+#             */
/*   Updated: 2023/09/16 15:32:33 by facu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H

# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/time.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>

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
# define E_SEM "Error: semaphore function failed\n"

typedef struct s_philo
{
	int						id;
	int						pid;
	long					last_serving_t;
	pthread_t				monitor_feeding;
	pthread_t				monitor_shotdown;
	pthread_t				routine;
	sem_t					*lock;
	struct s_global_data	*g_data;
	int						is_eating;
}	t_philo;

typedef struct s_global_data
{
	int						phil_amount;
	int						die_t;
	int						eat_t;
	int						sleep_t;
	int						servings;
	sem_t					*printf_lock;
	sem_t					*forks;
	sem_t					*shotdown;
	sem_t					*total_servings;
	sem_t					*term_lock;
	int						term;
	t_philo					*philosophers;
	long					start_time;
}	t_global_data;

/* checks.c */
int		check_argv(t_global_data *data);
void	check_argc(int argc);
int		check_is_dead(long timestamp, t_philo *philo);
int		check_servings(t_philo *philo);
int		check_term(t_philo *philo);

/* debug.c */
void	print_results(t_global_data *global_data);

/* init.c */
void	init_philo(t_global_data *data, int i);
void	initialize_global_data(t_global_data *data, char **argv);

/* libft_utils.c */
int		ft_atoi(char *str);
int		ft_strlen(char *s);
int		ft_strncmp(char *str_1, char *str_2, int n);
void	ft_usleep(unsigned int n);

/* main.c */

/* subroutines.c */
void	eat(t_philo *philo);
void	leave_forks(t_philo *philo);
void	nap(t_philo *philo);

/* threads.c */
void	*monitor_shotdown(void *arg);
void	*monitor_starvation(void *arg);
void	*routine(void *arg);

/* utils.c */
void	print_status(t_philo *philo, char *status);
long	get_time(void);
void	exit_routine(t_global_data *global_data, char *msg);
void	close_semaphores(t_global_data *data);
void	close_child_process(t_philo *ph);

#endif
