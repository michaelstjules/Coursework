#include <stdio.h>
#include <string.h>
#include <setjmp.h>

int isInt(char *s){ /* to check whether a String represents an int or not */
	if(strlen(s)==0) /* if it's empty */
		return 0;
	int j=0; /* loop counter */
	s[strlen(s)-1] = '\0'; /* set the end of s */
	if(s[0]=='-'){ /* check if negative */
		if(strlen(s)==1) /*if '-' is the only character */
			return 0;
		else j++; /* start at the second position */
	}
	if((s[0]=='0') && (strlen(s)!=1)) /* check if nonzero but starts with 0 */
		return 0;
	for(; j<=strlen(s)-1; j++){
		if(!isdigit(s[j])) /* if any characters are not digits */
			return 0;
	}
	return 1;
}

void main(){
	char lstr[BUFSIZ], ustr[BUFSIZ]; /* Strings to be read */
	int lower, upper; /* values to be used */
	jmp_buf env; /* environment to which to return */
	int val; /*value for setjmp */
	
	printf("insert lower value of the range of integers\n");
	fgets(lstr, sizeof lstr, stdin);
	while((lstr==NULL) || (isInt(lstr)==0)){ /* while invalid */
		printf("that was not valid; try again\n");
		fgets(lstr, sizeof lstr, stdin);
	}
	lower = atoi(lstr); /* convert to integer */
	
	printf("insert upper value of the range of integers\n");
	val = setjmp(env); /* return here if upper<lower and reentering */
	
	fgets(ustr, sizeof ustr, stdin);
	while((ustr==NULL) || (isInt(ustr)==0)){ 
		printf("that was not valid; try again\n");
		fgets(ustr, sizeof ustr, stdin);
	}
	upper = atoi(ustr);
	
	while(upper < lower){
		printf("lower bound is greater than upper bound\n");
		printf("reverse bounds (1) or reenter upper (2)?\n");
		int c = getchar();
		int temp; /* must be declared outside of switch */
		switch(c){
			case '1':
				temp = lower;
				lower = upper;
				upper = temp;
				break;
			case '2':
				printf("reenter upper\n");
				while(getchar()!='\n'); /* clear stdin */
				longjmp(env, 1);
				break; 
			default: 
				printf("invalid input\n");
				while(getchar()!='\n'); /* clear stdin */
				break; /* restart while-loop */
		}
	}

	
	int i;
	for(i=lower; i<=upper; i++){
		if(i%2==0) /*if it's even, instead of i%2!=1, since e.g. -4%3=-1, which is wrong */
			continue;
		printf("%d\n", i);
		
	}
}