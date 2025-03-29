
#include "philo.h"

void	ft_putstr(char *s)
{
	while (*s)
	{
		write(1, s, 1);
		s++;
	}
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atol(char *str)
{
	int		count;
	long	result;
	int		sign;

	count = 0;
	result = 0;
	sign = 1;

	while (str[count] == '\r' || str[count] == '\t' || str[count] == ' '
		|| str[count] == '\f' || str[count] == '\v' || str[count] == '\n')
		count++;

	if (str[count] == '-' || str[count] == '+')
	{
		if (str[count] == '-')
			sign = -1;
		count++;
	}

	if (!(str[count] >= '0' && str[count] <= '9'))
		return (0);

	while (str[count] >= '0' && str[count] <= '9')
		result = result * 10 + (str[count++] - '0');

	return (result * sign);
}


int	check_is_digit(char *av[], int ac)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		if (!av[i][0])
			return (1);
		
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j++]))
				return (1);
		}
		i++;
	}
	return (0);
}


int	check_num(int ac, char **av)
{
	long	num;
	int		i;

	if (ac == 6 && ft_atol(av[5]) <= 0)
		return (1);

	i = 1;
	while (i < ac)
	{
		num = ft_atol(av[i]);
		if (num <= 0)
			return (1);
		i++;
	}
	return (0);
}

void	print_usage(void)
{
	
	ft_putstr("\nArguments:\n");
	ft_putstr("    when all philosophers have eaten this many times\n");
}