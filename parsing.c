/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza <mtarza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 22:06:55 by mtarza            #+#    #+#             */
/*   Updated: 2025/04/10 22:18:40 by mtarza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	num_valid(int ac, char **av)
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

int	check_argment(int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s invalid \n ", argv[0]);
		return (1);
	}
	if (num_valid(argc, argv) != 0 || check_is_digit(argv, argc) != 0)
		return (printf("Arguments must be positive integers\n"), 1);
	return (0);
}
