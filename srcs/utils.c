/*
 * MIT License
 * 
 * Copyright (c) 2025 OhBama (Yann Manchon)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

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
