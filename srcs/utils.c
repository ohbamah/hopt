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

char*
hopt_strjoin(const char* __restrict__ s1, const char* __restrict__ s2)
{
	char*			ret;
	unsigned int	size = strlen(s1) + strlen(s2);
	unsigned int	j = 0;

	ret = malloc((size + 1) * sizeof(char*));
	if (!ret)
		return (NULL);
	for (int i = 0 ; s1[i] ; ++i)
		ret[j++] = s1[i];
	for (int i = 0 ; s2[i] ; ++i)
		ret[j++] = s2[i];
	ret[size] = '\0';
	return (ret);
}

char*
hopt_strvajoin(unsigned int count, ...)
{
	char*			ret;
	unsigned int	size = 0;
	unsigned int 	at = 0;

	va_list	va;
	va_start(va, count);
	for (unsigned int i = 0 ; i < count ; ++i)
		size += strlen(va_arg(va, char*));
	va_end(va);

	ret = malloc((size + 1) * sizeof(char*));
	if (!ret)
		return (NULL);
	ret[size] = '\0';

	va_start(va, count);
	for (unsigned int i = 0 ; i < count ; ++i)
	{
		char*			tmp = va_arg(va, char*);
		memcpy(&ret[at], tmp, strlen(tmp));
		at += strlen(tmp);
	}
	va_end(va);
	return (ret);
}

char*
hopt_strndup(const char* __restrict__ s, unsigned int n)
{
	char*	ret;

	ret = malloc((n + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	memcpy(ret, s, n);
	ret[n] = '\0';
	return (ret);
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
			ret[j] = hopt_strndup(&s[i], size);
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
