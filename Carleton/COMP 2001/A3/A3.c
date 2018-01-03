#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* sleep() */
#include <ctype.h> /* isdigit() */
#include <time.h>
#include <sys/time.h>

#define MAXSTRLEN 34
/* 34=10+1+10+1+10+1+1, where 10 is the max length of shapeType and an int */


enum PatternType
{
  ALINE = 'A',
  CCROSS = 'C',
  DSQUARE = 'D',
  GRECTANGLE = 'G'
};


typedef struct Position
{
  int y;
  int x;
} Position;


/* to use pointers to the same "shapes" without their positions */
typedef struct PreShape
{
  enum PatternType shapeType;
  int h; /* height */
  int w; /* width */
  char **c; /* 'array' of characters */
} PreShape;


typedef struct Shape
{
  struct PreShape *ps;
  struct Position p; /* position of top-left corner of c */
} Shape;


typedef struct Node
{
  struct Shape *s;
  struct Node *next;
} Node;


Node *getNodeBefore(Node* list, int index)
{
  Node *u;
  int i;
  for(i=0, u=list; i<index-1 && u->next!=NULL; i++, u=u->next){}
  if(i==index-1)
    return u;
  else
  {
  return NULL;
  }
}


Node *getNode(Node* list, int index)
{
  Node *u = getNodeBefore(list, index+1);
  if((u==NULL) || (u->s==NULL))
  {
    return NULL;
  }
  return u; 
}


char removeNodeAfter(Node *u)
{
  if((u==NULL) || (u->next==NULL))
    return 0; /* cannot remove it */
  Node *del = u->next;
  u->next = del->next;
  free(del->s);
  free(del);
  return 1; /* removed successfully */
}


void clearList(Node *list)
{
  if(list==NULL)
  {
    printf("There's no list to empty!\n");
    return;
  }
  if(list->s==NULL)
  {
    return;
  }
  Node *u, *next;
  u = list->next;
  while(u!=NULL)
  {
    next = u->next;
	free(u->s);
	free(u);
	u = next;
  }
  free(list->s);
  list->s=NULL;
}


char addNode(Node *list, Node *new)
{
  if((list==NULL) || (new==NULL))
  {
    if(new!=NULL)
    {
      clearList(new);
      free(new);
    }
    return 0;
  }
  if(list->s==NULL) /* if the list is empty */
  {
    *list = *new;
	free(new);
    return 1;
  }
  /* find the last node */
  Node *u = list;
  while(u->next!=NULL)
    u = u->next;
  u->next = new;
  return 1;
}


char addNodeIndex(Node *list, int index, Node *new)
{
  if((list==NULL) || (new==NULL))
  {
    if(new!=NULL)
    {
      clearList(new);
      free(new);
    }
    return 0;
  }
  /* find the last node in new list */
  Node *newLast = new;
  while(newLast->next!=NULL)
    newLast = newLast->next;
  if(index==0)
  {
    if(list->s!=NULL) /* if the list isn't empty */
    {
      /* this was tricky (without passing double pointer for head) */
      newLast->next = (Node *)calloc(1, sizeof(Node));
      *(newLast->next) = *list; /* copy list values */
    }
    *list = *new; /* change head values to new values */
    free(new); /* no need for this node anymore */
	return 1;
  }
  Node *prev = getNodeBefore(list, index); /* will work if adding node(s) at end */
  if(prev==NULL)
  {
    clearList(new);
    free(new);
    return 0;
  }
  newLast->next = prev->next;
  prev->next = new;
  return 1;
}


char addShape(Node *list, struct Shape *shape)
{
  if(shape==NULL)
    return 0;
  Node *u = (Node *)calloc(1, sizeof(Node));
  u->next = NULL;
  u->s = shape;
  if(addNode(list, u))
  {
    printf("Shape added.\n");
    return 1;
  }
  free(shape);
  free(u);
  printf("Error.\n");
  return 0;
}


char addShapeIndex(Node *list, int index, struct Shape *shape)
{
  if(shape==NULL)
    return 0;
  Node *u = (Node *)calloc(1, sizeof(Node));
  u->next = NULL;
  u->s = shape;
  if(!addNodeIndex(list, index, u))
  {
    free(shape);
    free(u);
    printf("Invalid index.\n");
	return 0;
  }
  printf("Shape added.\n");
  return 1;
}


char removeShape(Node *list, int index)
{
  if(list->s==NULL) /* if empty */
  {
    printf("Cannot remove from an empty list.\n");
    return 0;
  }
  if(index==0) /* if removing the head */
  {
    if(list->next!=NULL) /* and it has a next */
    {
      free(list->s);
      Node *next = list->next;
      *list = *(list->next); /* copy next's values into head */
      free(next);
    }
    else /* otherwise the list has only the head, so delete its data */
    {
      free(list->s);
      list->s = NULL;
    }
    printf("Shape removed.\n");
    return 1;
  }
  if(!removeNodeAfter(getNodeBefore(list, index)))
  {
    printf("Invalid index. Try again. \n");
    return 0;
  }
  printf("Shape removed.\n");
  return 1;
}


char setShape(Node *list, int index, struct Shape *shape) /* unused */
{
  Node *u = getNode(list, index);
  if((u==NULL) || (u->s==NULL))
  {
    printf("Invalid index.\n");
    return 0;
  }
  struct Shape *oldShape = u->s;
  u->s = shape;
  free(oldShape);
  printf("Shape changed.\n");
  return 1;
}


/* return the size of the list */
int size(Node *list)
{
  if((list==NULL) || (list->s==NULL))
    return 0;
  int i;
  Node *u;
  for(i=0, u=list; u!=NULL; u=u->next, i++){}
  return i; 
}


struct PreShape *initPreShape(enum PatternType type)
{
  struct PreShape *ps = (struct PreShape *)calloc(1, sizeof(struct PreShape));
  int k, j;
  switch(type)
  {
    case ALINE:
	  ps->w = 1;
	  ps->h = 3;
	  break;
	case CCROSS:
      ps->shapeType = CCROSS;
	  ps->w = 7;
	  ps->h = 7;
	  ps->c = (char **)calloc(ps->h, sizeof(char *));
      /* unfortunately, I still have to make a rectangular 2D array */
      /* instead of an array of arrays of different length */
      /* because printMap() could not handle a general shape, as is */
      for(k=0, j=3; k<ps->h; k++)
      {
        ps->c[k] = (char *)calloc(ps->w, sizeof(char));
        ps->c[k][j] = 'C'; /* c[0][3] through c[6][3] */
      } 
      for(j=0, k=3; j<ps->h; j++)
        ps->c[k][j] = 'C'; /* c[3][0] through c[3][6] */
	  return ps;
	case DSQUARE:
	  ps->w = 2;
	  ps->h = 2;
	  break;
	case GRECTANGLE:
	  ps->w = 8;
	  ps->h = 2;
  }
  ps->shapeType = type;
  ps->c = (char **)calloc(ps->h, sizeof(char *));
  for(k=0; k<ps->h; k++)
  {
    ps->c[k] = (char *)calloc(ps->w, sizeof(char)); 
    for(j=0; j<ps->w; j++)
      ps->c[k][j] = (char)type;
  }
  return ps;
}


/* array of PreShapes to be used in main */
struct PreShape **preShapesArray()
{
  struct PreShape **s = (struct PreShape **)calloc(4, sizeof(struct PreShape*));
  s[0] = initPreShape(ALINE);
  s[1] = initPreShape(CCROSS);
  s[2] = initPreShape(DSQUARE);
  s[3] = initPreShape(GRECTANGLE);
  return s;
}


void clearPreShapes(PreShape **preShapes)
{
  int i, k;
  for(i=0; i<4; i++)
  {
    for(k=0; k<preShapes[i]->h; k++)
      free(preShapes[i]->c[k]);
    free(preShapes[i]->c);
    free(preShapes[i]);
  }
  free(preShapes);
}


struct Shape *initShape(enum PatternType type, PreShape **preShapes, int x, int y)
{
  struct Shape *shape = (struct Shape *)calloc(1, sizeof(struct Shape));
  shape->p.x = x;
  shape->p.y = y;
  int i;
  for(i=0; i<4; i++)
  {
    if(preShapes[i]->shapeType==type)
    {
      shape->ps = preShapes[i];
	  return shape;
    }
  }
  free(shape);
  return NULL;
}


char isInt(char *string) /* to check whether a String represents an int or not */
{
  if((string==NULL) || (strlen(string)==0)) /* if it's NULL or empty */
    return 0;
  int j=0; /* loop counter */
  if(string[0]=='-') /* check if negative */
  {
    if(strlen(string)==1) /*if '-' is the only character */
      return 0;
    else j=1; /* start at the second position */
  }
  if((string[j]=='0') && (strlen(string)!=j+1)) /* check if nonzero but starts with 0; -0 is valid */
    return 0;
  for(; j<=strlen(string)-1; j++)
  {
    if(!isdigit(string[j])) /* if any characters are not digits */
      return 0;
  }
  return 1;
}


/* return a random integer between lower and upper, inclusive */
int randomInt(int lower, int upper)
{
  struct timeval t;
  gettimeofday(&t, NULL);
  int seed = t.tv_usec * t.tv_sec; /* multiply the two to avoid cycles */
  srand(seed);
  while(seed==t.tv_usec * t.tv_sec);
  return (int)((double)rand() / ((double)RAND_MAX + 1) * (upper - lower + 1)) + lower;
}


enum PatternType randomType()
{
  while(1)
  {
    switch(randomInt(0,3))
    {
      case 0:
        return ALINE;
      case 1:
        return CCROSS;
      case 2:
        return DSQUARE;
      case 3:
        return GRECTANGLE;
      default:
        printf("Trying randomInt(0,3) again for randomType().\n");
        continue;
    }
  }
}


struct Shape *stringToShape(char *string, PreShape **preShapes)
{
  if(string==NULL)
    return NULL;
 
  unsigned int len = strlen(string);
  int i, count;
  for(i=0, count=0; i<len; i++)
  {
    if(string[i]==' ')
      count++;
  }
  if(count<2)
  {
    printf("Too few space. ");
    return NULL;
  }
  if(count>2)
  {
    printf("Too many spaces. ");
    return NULL;
  }
  char *tokens[len]; /* overshoot, just in case */
  int x, y;
  /* copy the string to avoid segmentation fault, in case of const char* */
  char copy[len+1];
  strcpy(copy, string);
  
  tokens[0] = strtok(copy, " "); 
  
  if(tokens[0]==NULL)
  {
    printf("Invalid input. ");
    return NULL;  
  }
  
  tokens[1] = strtok(NULL, " ");
  if(isInt(tokens[1]))
    x = atoi(tokens[1]);
  else if((strcmp(tokens[1], "R")==0))
    x = randomInt(1, 58);
  else
  {
    printf("Invalid input. ");
    return NULL;
  }
  
  tokens[2] = strtok(NULL, " ");
  if(isInt(tokens[2]))
    y = atoi(tokens[2]);
  else if((strcmp(tokens[2], "R")==0))
    y = randomInt(1, 28);
  else
  {
    printf("Invalid input. ");
    return NULL;
  }
  
  //or first character
  enum PatternType type;
  if((strcmp(tokens[0], "ALINE")==0) || (strcmp(tokens[0], "A")==0))
    type = ALINE;
  else if((strcmp(tokens[0], "CCROSS")==0) || (strcmp(tokens[0], "C")==0))
    type = CCROSS;
  else if((strcmp(tokens[0], "DSQUARE")==0) || (strcmp(tokens[0], "D")==0))
    type = DSQUARE;
  else if((strcmp(tokens[0], "GRECTANGLE")==0) || (strcmp(tokens[0], "G")==0))
    type = GRECTANGLE;
  else if((strcmp(tokens[0], "RANDOM")==0) || (strcmp(tokens[0], "R")==0))
    type = randomType();
  else{
    printf("Invalid input. ");
	return NULL;
  }
  return initShape(type, preShapes, x, y);
}


char* shapeToString(char *string, struct Shape *shape)
{
  if(shape==NULL)
  {
    printf("Stopped before converting a NULL shape into a string.\n");
    return NULL;
  }
  if(shape->ps==NULL)
    printf("Stopped before querying a NULL preShape.\n");
  char typeString[11];
  switch(shape->ps->shapeType)
  {
    case ALINE:
	  strcpy(typeString, "ALINE");
	  break;
	case CCROSS:
	  strcpy(typeString, "CCROSS");
	  break;
	case DSQUARE:
	  strcpy(typeString, "DSQUARE");
	  break;
	case GRECTANGLE:
	  strcpy(typeString, "GRECTANGLE");
  }
  sprintf(string, "%s %d %d", typeString, shape->p.x, shape->p.y);
  return string;
}


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


char save(Node *list)
{
  FILE *fpt = fopen("shapes.txt", "w"); /* write over any existing file */   
  if(fpt==NULL)
  {
    printf("Error opening file.\n");
    return 0; 
  }
  if(list->s!=NULL)
  {
    char string[MAXSTRLEN];
    Node *u;
    for(u=list; u!=NULL; u=u->next)
      fprintf(fpt, "%s\n", shapeToString(string, u->s));
  }
  fclose(fpt);
  return 1;
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
  printf("AFTER.\n");
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


int main(int argc, char **argv)
{
  struct PreShape **preShapes = preShapesArray();
  int input; 
  Node *list = (Node *)calloc(1, sizeof(Node));
  if(argc==1)
  {
    printf("No file entered.\n");
  }
  else
  { 
    int i;
    struct Shape *shape;
    char string[MAXSTRLEN];
    char *p; /* to get rid of newline */
    for(i=1; i<argc; i++) /* try to read from multiple files, in order */
    {
      FILE *fpt = fopen(argv[i], "r"); 
      if(fpt==NULL)
      {
        printf("Error opening file.\n");
        continue;
      }
      while(!feof(fpt))
      {
        if(fgets(string, MAXSTRLEN, fpt)==NULL)
        {
          printf("Error reading line.\n");
          continue;
        }
        p = strchr(string, '\n');
        if(p)
          *p = '\0';
        shape = stringToShape(string, preShapes);
        if(shape!=NULL)
          addShape(list, shape);
        else printf("Unable to add shape.\n");
      }
      fclose(fpt);
    }
  }
  
  while(1)
  {
    sleep(1);
    printMenu();
    input = getchar();
    int c;
    while((c = getchar()!='\n') && (c!= EOF));
    switch(input)
    {
      case '0':
        printMap(list);
        break;
      case '1':
        add(list, preShapes);
        break;
      case '2':
        delete(list);
        break;
      case '3':
        clearList(list);
        break;
      case '4':
        save(list);
        break;
      case 'q':
        surprise(preShapes);
	    clearList(list);
        free(list);
		clearPreShapes(preShapes);
        return 0;
      default: 
        printf("Invalid input. Try again.\n");
    }	
  }
}