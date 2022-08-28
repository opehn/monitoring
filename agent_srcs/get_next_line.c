#include "collect.h"

static char    *split_line(char **backup, int idx)
{
    int     len;
    char    *line;
    char    *temp;

    len = strlen(*backup);
    line = strndup(*backup, idx + 1);
    temp = strndup(&(*backup)[idx + 1], len - idx - 1);
    free(*backup);
    *backup = temp;
    return (line);
}

static char    *final_line(char **backup)
{
    int     newline_idx;
    int     len;
    char    *res;

    if (!backup)
        return (NULL);
    newline_idx = find_newline(*backup);
    if (newline_idx >= 0)
        return (split_line(backup, newline_idx));
    len = strlen(*backup);
    res = my_strndup(*backup, len);
    free(*backup);
    *backup = NULL;
    return (res);
}

static int     append_backup(char **backup, char *buf)
{
    char    *res;

    res = NULL;
    if (!*backup)
        res = my_strndup(buf, strlen(buf));
    else
    {
        res = my_strjoin(*backup, buf);
        free(*backup);
    }
    *backup = res;
    return (find_newline(*backup));
}

char    *get_next_line(int fd, int flag)
{
    static char *backup;
    char        *buf;
    int         idx;
    int         read_size;

	if (flag)
	{
		if (backup)
		{
			free(backup);
			backup = NULL;
		}
		return (NULL);
	}
    idx = -1;
    buf = (char *)calloc(BUFFER_SIZE, sizeof(char));
    if (!buf || fd < 0 || BUFFER_SIZE < 1)
        return (NULL);
    if (backup)
        idx = find_newline(backup);
    if (idx >= 0)
    {
        free(buf);
        return (split_line(&backup, idx));
    }
    while ((read_size = read(fd, buf, BUFFER_SIZE)) > 0)
    {
        idx = -1;
            idx = append_backup(&backup, buf);
        if (idx >= 0)
        {
            free(buf);
            buf = NULL;
            return (split_line(&backup, idx));
        }
        free(buf);
        buf = (char *)calloc(BUFFER_SIZE, sizeof(char));
    }
    if (buf)
        free (buf);
    return (final_line(&backup));
}
