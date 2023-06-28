#include <stdio.h>

int main()
{
	int *i;
	dprintf(2, "lol");
	i += 10000;
	dprintf(2, "lol");
	*i = 6;
	dprintf(2, "lol");
}