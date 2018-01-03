#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> /* sleep() */

#define DEF_MAX_SHAPES 6

enum PatternType
{
  ALINE = 0,
  CCROSS = 1,
  DSQUARE = 2,
  GRECTANGLE = 3
};

struct Position
{
  int y;
  int x;
};

struct Shape
{
  char c[7][8]; /* an array of length 7 of arrays of lenth 8, height by width (easier for printing) */
  struct Position p; /* position of top-left corner of c */
  enum PatternType shapeType;
  int w; /* width */
  int h; /* height */
};


/* initialize shape */
void initShape(struct Shape *shape, enum PatternType shapeType, int x, int y)
{
  shape->p.y = y;
  shape->p.x = x;
  shape->shapeType = shapeType;
  int j, k;
  for(k=0; k<7; k++)
  {
    for(j=0; j<8; j++)
      shape->c[k][j] = '\0'; /* initialize all to null characters */
  }
  
  switch(shapeType)
  {
    case ALINE:
      shape->c[0][0] = shape->c[1][0] = shape->c[2][0] = 'A';
      shape->w = 1;
      shape->h = 3;
      break;
    case CCROSS:
      for(k=0, j=3; k<7 ; k++)
      {
        shape->c[k][j] = 'C'; /* c[0][3] through c[6][3] */
        shape->c[j][k] = 'C'; /* c[3][0] through c[3][6] */
      }
      shape->w = 7;
      shape->h = 7;
      break;
    case DSQUARE:
      shape->c[0][0] = shape->c[0][1] = shape->c[1][0] = shape->c[1][1] = 'D';
      shape->w = 2;
      shape->h = 2;
      break;
    case GRECTANGLE:
      for(k=0; k<2; k++)
      {
        for(j=0; j<8; j++)
          shape->c[k][j] = 'G';
      }
      shape->w = 8;
      shape->h = 2;
      break;
    }
}


/* print all of shapes */
void printMap(struct Shape *shapes)
{
  /* initialize to an empty background with borders */
  char map[30][61]; 
  int j, k; 
  for(k=0; k<30; k++)
  {
    for(j=0; j<61; j++)
    {
      if(k==0 || k==29) /* top and bottom borders */
        map[k][j] = '-';
      else if(j==0 || j==59) /* right and left borders */
        map[k][j] = '|';
      else 
        map[k][j] = ' '; /* initialize all to spaces */
      if(j==60) /* last column */
        map[k][j] = '\0'; /* null character to mark the end */
    }
  }
  /* loop through each shape, changing characters in map to shape characters */
  int i;
  int mx, my; /* character position on map */
  for(i=0; i<DEF_MAX_SHAPES; i++) /* for each shape */
  {
    //if the starting position falls on or outside the borders of the map
    if((shapes[i].p.x<=0) || (shapes[i].p.x>=59) || (shapes[i].p.y<=0) || (shapes[i].p.y>=29))
	  continue;
    for(k=0; k<shapes[i].h; k++) /* for each row */
	{
      for(j=0; j<shapes[i].w; j++) /* for each column */
      {
        my = k+shapes[i].p.y;
        mx = j+shapes[i].p.x;
        if((shapes[i].c[k][j]!='\0')  && (my<29) && (my>0) && (mx<59) && (mx>0)) /* for each non-null character that's within the region */
          map[my][mx]=shapes[i].c[k][j];
      }
    }
  }
  for(i=0; i<30; i++)
    printf("%s\n", map[i]);
}


/* print the menu */
void printMenu()
{
/* code for multiple printf calls looks nicer than a really long string; also not worth storing strings */
  printf("\n********************************************************\n");
  printf("Menu for displaying character patterns.\n");
  printf("(0) -- print all drawings.\n");
  printf("(1) -- transform current drawings by shifting ONCE ONLY.\n");
  printf("(2) -- randomize the shapes.\n");
  printf("(3) -- untransform the current drawings.\n");
  printf("(4) -- SURPRISE ME!\n");
  printf("(5) -- exit.\n");
  printf("Enter your option [0-5] by choosing number 0 or 1 or 2 or 3 or 4 or 5:\n");
  printf("********************************************************\n\n");
}


/* transform the pattern */
void transformMap(struct Shape *shapes) 
{
/* does not bitshift borders */
/* 'A' gets bitshifted to ' ' */
  int i, k, j;
  for(i=0; i<DEF_MAX_SHAPES; i++) /* for each shape */
  {
    for(k=0; k<shapes[i].h; k++) /* for each row */
	{
      for(j=0; j<shapes[i].w; j++) /* for each column */
      {
        if(shapes[i].c[k][j]!='\0') /* if it's not null */
          shapes[i].c[k][j]=shapes[i].c[k][j]>>1; /* right-shift by 1 */
      }
    }
  }
}

void randomize(struct Shape *shapes)
{
/* use (int)((double)rand() / ((double)RAND_MAX + 1) * n) instead of rand()%n, */
/* since lower order bits aren't so random */
  unsigned int seed = (unsigned int)time(NULL);
  srand(seed);
  int i, j;
  for(i=0; i<DEF_MAX_SHAPES; i++)
  {
    for(j=0; j<3; j++)
    {
      if(j==0)
        shapes[i].shapeType = (int)((double)rand() / ((double)RAND_MAX + 1) * 4);
      else if(j==1)
        shapes[i].p.x = (int)((double)rand() / ((double)RAND_MAX + 1) * 58) + 1;
      else if(j==2)
        shapes[i].p.y = (int)((double)rand() / ((double)RAND_MAX + 1) * 28) + 1;
    }
  }
}

void facepalm() 
{
/* I had to double the '\' and '%' characters. This was very hard work! */
  printf("............................................________\n\n");
  printf(".............................,.-”...................................“-.,\n\n");
  printf(".....................,?......................................................\\,\n\n");
  printf(".................../...........................................................,}\n\n");
  printf("................./......................................................,:`^`..}\n\n");
  printf(".............../...................................................,:”........./\n\n");
  printf("..............?.....__.........................................:`.........../\n\n");
  printf("............./__.(.....“~-,_..............................,:`........../\n\n");
  printf(".........../(_....”~,_........“~,_....................,:`........_/\n\n");
  printf("..........{.._$;_......”=,_.......“-,_.......,.-~-,},.~”;/....}\n\n");
  printf("...........((.....*~_.......”=-._......“;,,./`..../”............../\n\n");
  printf("...,,,___.\\`~,......“~.,....................`.....}............../\n\n");
  printf("............(....`=-,,.......`........................(......;_,,-”\n\n");
  printf("............/.`~,......`-...............................\\....../\\n\n");
  printf(".............\\`~.*-,.....................................|,./.....\\,__\n\n");
  printf(",,_..........}.>-._\\...................................|..............`=~-,\n\n");
  printf(".....`=~-,_\\_......`\\,.................................\\\n\n");
  printf("...................`=~-,,.\\,...............................\\\n\n");
  printf("................................`:,,...........................`\\..............__\n\n");
  printf(".....................................`=-,...................,%%`>--==``\n\n");
  printf("........................................_\\..........._,-%%.......`\\\n\n");
  printf("...................................,<`.._|_,-&``................`\\\n\n");
  sleep(1);
  printf("Invalid input. Try again.\n");
  sleep(1);
}

int main()
{
  /* initialize shapes */
  struct Shape allshapes[DEF_MAX_SHAPES];
  
  initShape(&allshapes[0], ALINE, 10, 10);
  initShape(&allshapes[1], ALINE, 30, 20); 
  initShape(&allshapes[2], GRECTANGLE, 45, 25); 
  initShape(&allshapes[3], CCROSS, 20, 15);
  initShape(&allshapes[4], DSQUARE, 15, 15);
  initShape(&allshapes[5], DSQUARE, 40, 7);
  
  char unused = 1; /* indicates that (1) transformation hasn't been chosen */
  int input, i; 
  
  while(1)
  {
    printMenu();
    input = getchar();
    while(getchar()!='\n');
    switch(input)
    {
      case '0':
        printMap(allshapes);
        break;
      case '1':
        if(unused)
        {
          unused = 0;
          transformMap(allshapes);
        }
        else
        {
          printf("One-time transformation ONLY.\n This option is not usable anymore.\n\n");
          sleep(1);
        }
        break;
      case '2': 
        randomize(allshapes); 
        for(i=0; i<DEF_MAX_SHAPES; i++)
          initShape(&allshapes[i], allshapes[i].shapeType, allshapes[i].p.x, allshapes[i].p.y);
          unused = 1; /* allow transformation again */
        break;
      case '3': /* too bad for the if statement, otherwise I could have omitted break in '2' */
        if(!unused)
        {
          /* reinitialize */
          for(i=0; i<DEF_MAX_SHAPES; i++)
            initShape(&allshapes[i], allshapes[i].shapeType, allshapes[i].p.x, allshapes[i].p.y);
          unused = 1; /* allow transformation again */
        }
        else 
        {
          printf("The current map has not been transformed.\n");
          sleep(1);
        }
        break;
      case '4':
        unused = 1;
        time_t time1, time2;
        time1 = time(NULL);
        while(difftime(time(NULL), time1) < 10) /* for 10 seconds */
        {
          /* use time to set a delay < 1 second */
          /* nanosleep is too complicated to be worth it */
          time2 = time(NULL);
          while(difftime(time(NULL), time2) < 0.5){}
          randomize(allshapes);
          for(i=0; i<DEF_MAX_SHAPES; i++)
            initShape(&allshapes[i], allshapes[i].shapeType, allshapes[i].p.x, allshapes[i].p.y);
          printMap(allshapes);
          time2 = time(NULL);
          while(difftime(time(NULL), time2) < 0.5){}
          transformMap(allshapes);
          printMap(allshapes);
          unused = 0;
        }
        break;
      case '5':
        return(0);
        break; 
      default: 
        facepalm();
        break;
    }	
  }
}