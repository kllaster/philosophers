#include "philo.h"

void	check_died(t_philo *s_philo)
{
	int64_t	time;

	sem_wait(s_philo->print);
	time = time_unix_ms();
	if ((s_philo->s_table->time_life + s_philo->last_eat) < time
		|| s_philo->time_death)
	{
		if (!s_philo->time_death)
			s_philo->time_death = time;
		printf("%llu : %d. %sphilo died%s\n",
			(s_philo->time_death - s_philo->s_table->start_table),
			s_philo->id, CRED, CNRM);
		exit(1);
	}
	sem_post(s_philo->print);
}

void	print_status(t_philo *s_philo, char *text, char *color)
{
	sem_wait(s_philo->print);
	printf("%llu : %d. %s%s%s\n",
		(time_unix_ms() - s_philo->s_table->start_table),
		s_philo->id, color, text, CNRM);
	sem_post(s_philo->print);
}

int8_t	life(t_philo *s_philo)
{
	s_philo->last_eat = time_unix_ms();
	print_status(s_philo, "philo is eating", CBLU);
	timeout(time_unix_ms() + s_philo->s_table->time_eat);
	sem_post(s_philo->forks);
	sem_post(s_philo->forks);
	s_philo->actual_count_eat += 1;
	print_status(s_philo, "philo is sleeping", CYEL);
	timeout(time_unix_ms() + s_philo->s_table->time_sleep);
	print_status(s_philo, "philo is thinking", CCYN);
	return (0);
}

void	*monitor(void *data)
{
	t_philo		*s_philo;

	s_philo = (t_philo *)data;
	while (1)
	{
		if (s_philo->s_table->num_count_eat != 0
			&& s_philo->actual_count_eat == s_philo->s_table->num_count_eat)
		{
			print_status(s_philo, "philo is full", CYEL);
			exit(0);
		}
		check_died(s_philo);
	}
}

void	*philo(void *data)
{
	pthread_t	thread;
	t_philo		*s_philo;

	s_philo = (t_philo *)data;
	if (pthread_create(&thread, NULL, monitor, s_philo) != 0
		|| pthread_detach(thread) != 0)
		exit(2);
	while (1)
	{
		if (!s_philo->s_table->start_table)
			continue ;
		if (s_philo->timeout)
		{
			s_philo->last_eat = s_philo->s_table->start_table;
			timeout(time_unix_ms() + s_philo->timeout);
			s_philo->timeout = 0;
		}
		sem_wait(s_philo->forks);
		print_status(s_philo, "philo has taken fork", CCYN);
		sem_wait(s_philo->forks);
		print_status(s_philo, "philo has taken fork", CCYN);
		if (life(s_philo))
			exit(0);
	}
}
