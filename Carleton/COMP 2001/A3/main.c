#include "header.h"

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