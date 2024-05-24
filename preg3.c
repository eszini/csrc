
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
regex_t regex;
int reti;
char msgbuf[1024];

int main(int argc, char const *argv[])
{
    int		i,j,k,p1,p2,f1,f2,f3;
    char	s[1024];
    char	c;

    while(1)
    {
        fgets( msgbuf, 1024, stdin );


        /* Execute regular expression */
	strcpy (s,msgbuf);
        printf("%s\n", s);

	p1 = 0;
	f1 = 1;

	while (f1)
	{
		i  = 0;
		f2 = 1;
		f3 = 1;


		while ( f1 && f2 && f3 )
		{
			c = s[p1 + i];
			k = 0;

			
			if ( c >= 'a' && c <= 'z' )
				k = 1;

			if ( c >= '0' && c <= '9' )
				k = 2;

			if ( c == ' ' )
				k = 3;

			if ( c == '_' )
				k == 4;

			switch (k)
			{
				case:	1
					break;

				case:	2
					break;


			}			
			


		}



	}
	
}





