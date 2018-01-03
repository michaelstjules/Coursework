#include "header.h"

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