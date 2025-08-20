#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct loop {
	size_t 	count;
	void 	*p;

	char 	*c;
	int 	chr;

	int		nil;
} loop;

loop startlp(void *ptr) {
	loop lp = {
		.p = ptr,
		.count = 0,
		.chr = ((char *)ptr)[0],
		.c = ((void **)ptr)[0]
	};


	return lp;
}

void lp_increase_str(loop *lp) {
	if(!lp)
		return;

	lp->nil = ((lp->chr = ((char *)lp->p)[++lp->count]) == '\0' ? 1 : 0);
}

void lp_increase_arr(loop *lp) {
	if(!lp)
		return;

	lp->nil = ((lp->c = ((void **)lp->p)[++lp->count]) == NULL ? 1 : 0);
}

int count_char(char *data, char delim)
{
	if(!data || delim == '\0')
		return -1;

	int i = 0, count = 0;
	for(; data[i] != '\0'; i++)
	{
		if(data[i] == delim)
			count++;
	}

	return count;
}

char **split_str(char *data, char delim)
{
	if(!data || delim == '\0')
		return NULL;

	int count;
	if((count = count_char(data, delim)) == -1)
		return NULL;

	printf("%d\n", count);
	char **arr = (char **)malloc(sizeof(char *) * count + 1);
	memset(arr, '\0', sizeof(char *) * count);
	int idx = 0;

	char BUFF[1024] = {0};
	for(int i = 0; data[i] != '\0'; i++) {
		if(data[i] == delim) {
			arr[idx] = strdup(BUFF);
			idx++;

			memset(BUFF, '\0', 1024);
			continue;
		}

		strncat(BUFF, &data[i], 1);
	}
	arr[idx] = strdup(BUFF);

	return arr;
}

void split_string(char *data, char delim)
{
	if(!data)
		return;

	size_t len = strlen(data);
	for(int i = 0; i < len; i++)
	{
		if(data[i] == delim)
			data[i] = '\0';
	}
}

int main() {
	char TEST[100] = {0};
	strcpy(TEST, "TESTING THIS DUMBASS BULLSHIT");
	size_t len = strlen(TEST);

	for(loop i = startlp(&TEST); i.count < len; lp_increase_str(&i)) {
		printf("[%zu]: %c\n", i.count, i.chr);
	}

	char *ARR[] = {"TESTING", "THIS", NULL, "SHIT", NULL};
	for(loop i = startlp(&ARR); i.count < 4; lp_increase_arr(&i)) {
		if(i.nil)
			continue;

		printf("[%zu]: %s\n", i.count, i.c);
	}

	char **arr = split_str(TEST, ' ');
	for(loop i = startlp(arr); i.nil != 1; lp_increase_arr(&i)) {
		printf("%s\n", (char *)i.c);
	}

	split_string((char *)&TEST, ' ');
	int offset = 0, i = 0;
	char *p = (char *)&TEST;
	while(i != 4) {
		printf("%s : %p\n", p, p);
		offset = strlen(p);
		p += offset + 1;
		i++;
	}
	return 0;
}
