#include "Utils.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

int saveArrayAsText1(void *array_src, int arraysize,
	char *const filename)
{
	int i;
	FILE *fp = NULL;

	if(array_src == NULL || arraysize==0)
	{ return false; }

	if((fp=fopen(filename, "w")) == NULL)
	{ return false; }

	for(i=0; i < arraysize; i++)
	{
		fprintf(fp, "%f\n", ((double*)array_src)[i]);
	}
	fclose(fp);

	return true;
}