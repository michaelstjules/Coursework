#include "header.h"

void printList(Node *list)
{
  printf("\n");
  if(list==NULL || list->s==NULL)
  {
    printf("The list is currently empty.\n");
    printf("\n");
    return;
  }
  int i;
  Node *u;
  char string[MAXSTRLEN];
  for(i=0, u=list; u!=NULL; i++, u=u->next)
  {
    printf("[%d] %s\n", i, shapeToString(string, u->s));
  }
  printf("\n");
}


char shapePara(enum PatternType *type)
{
  sleep(1);
  printf("\n********************************************************\n");
  printf("To select a shapeType, enter the first letter, capatalized, of one of\n");
  printf("\"ALINE\", \"CCROSS\", \"DSQUARE\" and \"GRECTANGLE\" or R for random.\n");
  printf("Enter q to quit and return to the menu for adding shapes.\n");
  printf("********************************************************\n\n");
  int input = getchar();
  int c;
  while((c = getchar()!='\n') && (c!= EOF));
  if((input=='A') || (input=='C') || (input=='D') || (input=='G'))
  {
    *type = (enum PatternType)input;
    return 0;
  }
  if(input=='q')
    return 'q';
  if(input=='R')
  {
    *type = randomType();
    return 0;
  }
  printf("Invalid input. Try again.\n");
  return -1;
}


char intPara(const char ch, int *n)
{
  sleep(1);
  printf("\n********************************************************\n");
  printf("Enter the %c coordinate for the top-left corner, R for random within the map.\n", ch);
  printf("Enter q to quit and return to the menu for adding shapes.\n");
  printf("********************************************************\n\n");
  char input[BUFSIZ];
  int c;
  if(scanf("%s", input)!=1)
  {
    printf("Invalid index. Try again.\n");
    while((c = getchar()!='\n') && (c!= EOF));
    return -1;
  }
  while((c = getchar()!='\n') && (c!= EOF));
  if(strcmp(input, "q")==0)
    return 'q';
  if(isInt(input))
  {
    *n = atoi(input);
    return 0;
  }
  if(strcmp(input, "R")==0)
  {
   switch(ch)
   {
     case 'x':
       *n = randomInt(1, 58);
       return 0;
     case 'y':
       *n = randomInt(1, 28);
       return 0;
   }
  }
  printf("Invalid input. Try again.\n");
  return -1;
}


struct Shape *addByPara(PreShape **preShapes)
{
  enum PatternType type;
  while(1)
  {
    char c = shapePara(&type);
    if(c=='q')
    {
      return NULL;
    }
    if(c==0)
    {
      break;
    }
  }
  int x;
  while(1)
  {
    char c = intPara('x', &x);
    if(c=='q')
    {
      return NULL;
    }
    if(c==0)
    {
      break;
    }
  }
  int y;
  while(1)
  {
    char c = intPara('y', &y);
    if(c=='q')
    {
      return NULL;
    }
    if(c==0)
    {
      break;
    }
  }
  return initShape(type, preShapes, x, y);
}


struct Shape *addByString(PreShape **preShapes)
{
  char input[BUFSIZ], *p;
  while(1)
  {
    sleep(1);
    printf("\n********************************************************\n");
    printf("Enter a string formatted as \"SHAPETYPE x y\" or \"S x y\",\n");
    printf("where SHAPETYPE is one of:\n");
    printf("\"ALINE\", \"CCROSS\", \"DSQUARE\", \"GRECTANGLE\" and \"RANDOM\",\n");
    printf("or S is the first character of one of the above,\n");
    printf("and x and y are the coordinates of the top-left corner,\n");
    printf("with R for a random coordinate within the map.\n");
    printf("Enter q to quit and return to the menu for adding shapes.\n");
    printf("********************************************************\n\n");
    
    if(fgets(input, BUFSIZ, stdin)==NULL)
    {
      printf("Invalid input. Try again.\n"); 
      continue;
    }
    p = strchr(input, '\n');
    if(p)
      *p = '\0';
    else
    {
      printf("BUFFER OVERFLOW. Try again.\n");
      continue;
    }
    if(strcmp(input, "q")==0)
    {
      return NULL;
    }
    struct Shape *shape = stringToShape(input, preShapes);
    if(shape!=NULL)
    {
      printf("\n"); /* since stringToShape does not print newlines on error */
      return shape;
    }
    printf("Try again.\n"); /* appears on the same line as error from stringToShape */
  }
}


void nRandomShapes(Node *list, int index, struct PreShape **preShapes)
{
  char input[BUFSIZ]; 
  int c, n;
  while(1)
  {
    sleep(1);
    printf("\n********************************************************\n");
    printf("Enter a positive integer for the number of random shapes to add.\n");
    printf("Enter q to quit and return to the menu for adding shapes.\n");
    printf("********************************************************\n\n");
    if(scanf("%s", input)!=1)
    {
      printf("Invalid input. Try again.\n");
      while((c = getchar()!='\n') && (c!= EOF));
      continue;
    }
    while((c = getchar()!='\n') && (c!= EOF));
    if(strcmp(input, "q")==0)
      return;
    if(isInt(input))
    {
      n = atoi(input);
      if(n<=0)
      {
        printf("Invalid input. Try again.\n");
        continue;
      }
    }
    else
    {
      printf("Invalid input. Try again.\n");
      continue;
    }
    int i;
    if(index==-1)
    {
      for(i=0; i<n; i++)
      {
        addShape(list, stringToShape("RANDOM R R", preShapes));
      }
      return;
    }
    else
    {
      for(i=0; i<n; i++)
      {
        addShapeIndex(list, index+i, stringToShape("RANDOM R R", preShapes));
      }
      return;
    }
  }
}


void printNewShapeMenu()
{
    printf("\n********************************************************\n");
    printf("Menu for constructing shapes.\n");
    printf("(0) -- construct shape by entering individual parameters,\n");
	printf("with possibility of random parameters.\n");
	printf("(1) -- construct shape by entering a formatted string of parameters\n");
	printf("of the form \"SHAPETYPE x y\", e.g. \"ALINE 10 10\".\n");
    printf("(2) -- add several random shapes.\n");
	printf("(q) -- quit and return to previous menu.\n");
	printf("Enter your option by choosing number 0, 1, 2 or q:\n");
	printf("********************************************************\n\n");
}


struct Shape *newShape(Node* list, int index, PreShape **preShapes)
{
  int input, c;
  while(1)
  {
    sleep(1);
    printNewShapeMenu();
	input = getchar();
    while((c = getchar()!='\n') && (c!= EOF));
    switch(input)
	{
      case '0':
	    return addByPara(preShapes);
      case '1':
	    return addByString(preShapes);
      case '2':
        nRandomShapes(list, index, preShapes);
        return NULL;
      case 'q':
		return NULL;
      default:
        printf("Invalid input. Try again.\n");
	}
  }
}


void printAddShapeMenu()
{
  printf("\n********************************************************\n");
  printf("Menu for adding shapes.\n");
  printf("(0) -- add shape(s) to end of current list.\n");
  printf("(1) -- add shape(s) into a position in the current list, shifting the list.\n");
  printf("(q) -- quit and return to main menu.\n");
  printf("Enter your option by choosing 0, 1 or q:\n");
  printf("********************************************************\n\n");
}


void add(Node *list, struct PreShape **preShapes)
{
  int input, c;
  char input2[BUFSIZ]; 
  struct Shape *shape;
  while(1)
  {
    sleep(1);
    printAddShapeMenu();
	input = getchar();
    while((c = getchar()!='\n') && (c!= EOF));
    switch(input)
	{
      case '0':
        shape = newShape(list, -1, preShapes);
	    addShape(list, shape);
        break;
      case '1':
        while(1)
        {
          int n = size(list);
          printf("\n********************************************************\n");
          printf("Enter an index in the range 0-%d.\n", n);
          printf("Enter q to quit and return to previous menu.\n");
          printf("********************************************************\n");
          printList(list);
          printf("Enter an index or q: \n");
          
          if(scanf("%s", input2)!=1)
          {
            printf("Invalid input. Try again.\n");
            while((c = getchar()!='\n') && (c!= EOF));
            continue;
          }
          while((c = getchar()!='\n') && (c!= EOF));
          if(strcmp(input2, "q")==0)
            break;
          if(isInt(input2))
          {
            int i = atoi(input2);
            if((i>=0) && (i<=n))
            {
              shape = newShape(list, i, preShapes);
              addShapeIndex(list, i, shape);
              break;
            }
            else
            {
              printf("Invalid index. Try again.\n");
            }
          }
          else
          {
            printf("Invalid input. Try again.\n");
          }
        }
        break;
      case 'q':
		return;
      default:
        printf("Invalid input. Try again.\n");
	}
  }
}


void delete(Node *list)
{
  char input[BUFSIZ];
  int c;
  while(1)
  {
    sleep(1);
    int n = size(list);
    printf("\n********************************************************\n");
    printf("Enter the index of one of the following shapes to delete it.\n");
    printf("Enter q to quit and return to main menu.\n");
    printf("********************************************************\n");
    printList(list); 
    if(scanf("%s", input)!=1)
    {
      printf("Invalid input. Try again.\n");
      while((c = getchar()!='\n') && (c!= EOF));
      continue;
    }
    while((c = getchar()!='\n') && (c!= EOF));
    if(strcmp(input, "q")==0)
      return;
    if(isInt(input))
    {
      int i = atoi(input);
      if((i>=0) && (i<=n-1))
        removeShape(list, i);
      else
      {
        printf("Invalid index. Try again.\n");
      }
    }
    else
    {
      printf("Invalid input. Try again.\n");
    }
  }
}


void printMap(struct Node *list)
{
  /* initialize to an empty background with borders */
  char map[30][61]; 
  int i, k, j, mx, my; 
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
  if(list->s!=NULL)
  {
    /* loop through each shape, changing characters in map to shape characters */
    struct Node *u; 
    for(u=list; u!=NULL; u=u->next)
    {
      //if the starting position falls on or outside the borders of the map
      if((u->s->p.x<=0) || (u->s->p.x>=59) || (u->s->p.y<=0) || (u->s->p.y>=29))
	    continue;
      for(k=0; k<u->s->ps->h; k++) /* for each row */
	  {
        for(j=0; j<u->s->ps->w; j++) /* for each column */
        {
          my = k + u->s->p.y;
          mx = j + u->s->p.x;
          if((u->s->ps->c[k][j]!='\0')  && (my<29) && (my>0) && (mx<59) && (mx>0)) /* for each non-null character that's within the region */
            map[my][mx]=u->s->ps->c[k][j];
        }
      }
    }
  }
  for(i=0; i<30; i++)
    printf("%s\n", map[i]);
  printf("\nNOTE: Shapes with coordinates x and y, such that x<=0, x>=59,\n");
  printf("y<=0 or y>=29 are NOT displayed.\n");
}


void move(Node* list2, int v[2][2])
{
  Node *u;
  int i;
  for(i=0, u=list2; u!=NULL && u->s!=NULL; u=u->next, i++)
  {
    if(u->s->p.x<=1)
      v[i][0]=-v[i][0];
    if(u->s->p.y<=1)
      v[i][1]=-v[i][1];
    if(u->s->p.x+u->s->ps->w>=59)
      v[i][0]=-v[i][0];
    if(u->s->p.y+u->s->ps->h>=29)
      v[i][1]=-v[i][1];
    /* increment position */
    u->s->p.x+=v[i][0];
    u->s->p.y+=v[i][1];
  }
}


void surprise(struct PreShape **preShapes)
{
  time_t time1;
  struct timeval t2, t3;
  long double deltaT;
  Node *list2 = (Node *)calloc(1, sizeof(Node));
  addShape(list2, stringToShape("RANDOM 20 20", preShapes));
  addShape(list2, stringToShape("RANDOM 40 10", preShapes));
  int v[2][2], i, j;
  for(i=0; i<2; i++)
  {
    for(j=0; j<2; j++)
    {
      if(randomInt(0,1))
        v[i][j]=1;
      else v[i][j]=-1;
    }
  }
  time1 = time(NULL);
  while(difftime(time(NULL), time1) < 7) /* for 7 seconds */
  {
    gettimeofday(&t2, NULL);
    do
    {
      gettimeofday(&t3, NULL);
      deltaT = (long double)(t3.tv_sec-t2.tv_sec)+((long double)(t3.tv_usec-t2.tv_usec))/1000000.0;
    } while(deltaT<0.1);
    printMap(list2);
    move(list2, v);
  }
  clearList(list2);
  free(list2);
}


void printMenu()
{
  printf("\n********************************************************\n");
  printf("Menu for displaying character patterns.\n");
  printf("(0) -- print all drawings.\n");
  printf("(1) -- add a shape or several random shapes.\n");
  printf("(2) -- delete a shape.\n");
  printf("(3) -- clear the list.\n");
  printf("(4) -- save all shapes.\n");
  printf("(q) -- exit.\n");
  printf("Enter your option by choosing one of 1, 2, 3, 4, 5 or q:\n");
  printf("********************************************************\n\n");
}