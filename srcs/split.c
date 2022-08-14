#include "monitoring.h"

static int	count_same(char *str, char c)
{
	int cnt;

	cnt = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str != c)
			cnt++;
		while (*str && *str != c)
		{
			str++;
		}
		if (!*str)
			return (cnt);
		str++;
	}
	return (cnt);
}

static int	find_len(char *str, char c)
{
	int	 len;

	len = 0;
	while (*str && *str != c)
	{
		len++;
		str++;
	}
	return (len);
}

static void mem_free(char **array, int index)
{
	while (index--)
		free(array[index]);
	free(array);
}

static void make_array(char **array, char *str, char c, int array_len)
{
	int str_len;
	int i;
	int j;

	i = 0;
	while (i < array_len)
	{
		j = 0;
		while (*str == c)
			str++;
		str_len = find_len((char *)str, c);
		array[i] = (char *)malloc(str_len + 1);
		if (!array)
		{
			mem_free(array, i);
			return ;
		}
		while (j < str_len)
			array[i][j++] = *str++;
		array[i][j] = 0;
		i++;
	}
	array[i] = 0;
}

char	**split(char const *str, char c)
{
	int	 array_len;
	char	**array;

	array_len = count_same((char *)str, c);
	array = (char **)malloc(sizeof(char *) * (array_len + 1));
	if (!array)
		return (0);
	make_array(array, (char *)str, c, array_len);
	return (array);
}
