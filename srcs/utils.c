#include "__hopt_.h"

unsigned int
strclen(const char* restrict s, char sep)
{
	unsigned int	i = 0;
	while (s[i] && s[i] != sep)
		++i;
	return (i);
}

int
count_string(const char* restrict s, char sep)
{
	int	count = 0;
	for (unsigned int i = 0 ; s[i] ; ++i)
		if ((i == 0 && s[i] != sep) || (s[i] == sep && s[i + 1] != sep && s[i + 1]))
			++count;
	return (count);
}

char**
strsplit(const char* restrict s, char sep)
{
	if (!s)
		return (NULL);

	char**			ret;
	unsigned int	size;
	unsigned int	i = 0;
	unsigned int	j = 0;
	unsigned int	count = count_string(s, sep);

	ret = malloc((count + 1) * sizeof(char*));
	//printf("count_string : %d\n", count);
	if (ret == NULL)
		return (NULL);
	while (j < count)
	{
		size = strclen(&s[i], sep);
		if (size > 0)
		{
			ret[j] = strndup(&s[i], size);
			//printf("ret[j] = %s (%d bytes)\n", ret[j], size);
			if (ret[j] == NULL)
				return (NULL);
			ret[j++][size] = 0;
			i += size;
		}
		++i;
	}
	ret[j] = NULL;
	return (ret);
}

unsigned int
strlen2(char* restrict* restrict s)
{
	unsigned int	i = 0;
	while (s[i])
	{
		for (unsigned int j = 0 ; s[i][j] ; ++j)
			;
		++i;
	}
	return (i);
}

void
free2(void**  ptr2)
{
	if (ptr2)
	{
		for (unsigned int i = 0 ; ptr2[i] ; ++i)
			free(ptr2[i]);
		free(ptr2);
	}
}
