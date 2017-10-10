
#include "main.h"

void assert_param(char* file, char line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
 while(1);
}
