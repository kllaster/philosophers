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
	usleep(s_philo->s_table->time_eat * 1000);
	sem_post(s_philo->forks);
	sem_post(s_philo->forks);
	s_philo->actual_count_eat += 1;
	if (s_philo->actual_count_eat == s_philo->s_table->num_count_eat)
	{
		print_status(s_philo, "philo is full", CYEL);
		return (1);
	}
	print_status(s_philo, "philo is sleeping", CYEL);
	usleep(s_philo->s_table->time_sleep * 1000);
	check_died(s_philo);
	print_status(s_philo, "philo is thinking", CCYN);
	return (0);
}

void	*philo(void *data)
{
	t_philo	*s_philo;

	s_philo = (t_philo *)data;
	while (1)
	{
		if (!s_philo->s_table->start_table)
			continue ;
		check_died(s_philo);
		sem_wait(s_philo->forks);
		print_status(s_philo, "philo has taken fork", CCYN);
		check_died(s_philo);
		sem_wait(s_philo->forks);
		print_status(s_philo, "philo has taken fork", CCYN);
		check_died(s_philo);
		if (life(s_philo))
			exit(2);
	}
}