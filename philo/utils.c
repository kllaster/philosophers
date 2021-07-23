#include "philo.h"

int64_t			ft_atoi(const char *str)
{
	unsigned int	res;
	int				sign;

	res = 0;
	sign = 1;
	while (*str == '\t' || *str == '\n' ||
		   *str == '\v' || *str == '\f' ||
		   *str == '\r' || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
		sign = (*str++ == '-') ? -1 : 1;
	while (*str >= '0' && *str <= '9')
	{
		if ((res * 10) < res)
			return ((sign < 1) ? 0 : -1);
		res = res * 10 + (*str++ - '0');
	}
	return ((int)res * sign);
}

int64_t			time_unix_ms(void)
{
	struct timeval	s_time;

	gettimeofday(&s_time, NULL);
	return (s_time.tv_sec * 1000 + (int)(s_time.tv_usec * 0.001));
}
