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


Node *getNodeBefore(Node* list, int index);
Node *getNode(Node* list, int index);
char removeNodeAfter(Node *u);
void clearList(Node *list);
char addNode(Node *list, Node *new);
char addNodeIndex(Node *list, int index, Node *new);
char addShape(Node *list, struct Shape *shape);
char addShapeIndex(Node *list, int index, struct Shape *shape);
char removeShape(Node *list, int index);
char setShape(Node *list, int index, struct Shape *shape); /* unused */
int size(Node *list);
struct PreShape *initPreShape(enum PatternType type);
struct PreShape **preShapesArray();
void clearPreShapes(PreShape **preShapes);
struct Shape *initShape(enum PatternType type, PreShape **preShapes, int x, int y);
char isInt(char *string);
int randomInt(int lower, int upper);
enum PatternType randomType();
struct Shape *stringToShape(char *string, PreShape **preShapes);
char* shapeToString(char *string, struct Shape *shape);
char save(Node *list);

void printList(Node *list);
char shapePara(enum PatternType *type);
char intPara(const char ch, int *n);
struct Shape *addByPara(PreShape **preShapes);
struct Shape *addByString(PreShape **preShapes);
void nRandomShapes(Node *list, int index, struct PreShape **preShapes);
void printNewShapeMenu();
struct Shape *newShape(Node* list, int index, PreShape **preShapes);
void printAddShapeMenu();
void add(Node *list, struct PreShape **preShapes);
void delete(Node *list);
void printMap(struct Node *list);
void move(Node* list2, int v[2][2]);
void surprise(struct PreShape **preShapes);
void printMenu();