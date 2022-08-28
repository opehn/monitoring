#include "collect.h"

char *skip_bracket(char *src)
{
    char    *dest;
    int     len;
    int     i = 0;

    if (!src)
        return (NULL);

    len = strlen(src) - 1;
    dest = (char *)malloc(sizeof(char) * len);

    src++;
    while (src[i] != ')')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';

    return (dest);
}

int	nullguard_strlen(char *str)
{
	if (!str)
		return (0);
	return (strlen(str));
}

int isnum(char *str)
{
    int i = 0;
    int res = 0;

    while(str[i])
    {
        if (!isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

char	*my_strcpy(char *str)
{
	int		len;
	char	*res;
	int		i = 0;

	if (!str)
		return (NULL);
	len = strlen(str);
	res = (char *)malloc(sizeof(char) * (len + 1));
	while (str[i])
	{
		str[i] = res[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*my_strjoin(char *s1, char *s2)
{
	int		len;
	char	*res;
	int		i = 0;
	int		j = 0;

	if (!s1 || !s2)
		return (NULL);
	len = strlen(s1) + strlen(s2);
	res = (char *)malloc(sizeof(char) * (len + 1));
	while(s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	while(s2[j])
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	return (res);
}

int     find_newline(char *str)
{
    int i;

    i = 0;
    if (!str)
        return (-1);
    while (str[i])
    {
        if (str[i] == '\n')
            return (i);
        i++;
    }
    return (-1);
}

void	free_all(char **arr)
{
	int	i = 0;

	while(arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return ;
}

char    *my_strndup(char *origin, int len)
{
    char    *res;
    int     i;

    i = 0;
    if (!origin || len <= 0)
        return (NULL);
    res = (char *)malloc(sizeof(char) * len + 1);
    while (i < len)
    {
        res[i] = origin[i];
        i++;
    }
    res[i] = '\0';
    return (res);
}

void   serialize_header(int signature, int length, int agent_id, char *payload)
{
	packet_header *pheader;
	
	pheader = (packet_header *)payload;
	pheader->signature = htons((uint16_t)signature);
	pheader->length = htonl((uint32_t) length);
	pheader->agent_id = htonl((uint16_t) agent_id);
}
