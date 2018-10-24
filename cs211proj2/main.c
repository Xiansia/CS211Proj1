#include <stdio.h>
#include <stdlib.h>

//Stack and variables
int debugMode = 0;
typedef struct mazeStruct
{
    char **arr;  /* allows for a maze of size 30x30 plus outer walls */
    int xsize, ysize;
    int xstart, ystart;
    int xend, yend;
} maze;

typedef struct Stack{
    int xcoor;
    int ycoor;
    struct Stack *next;
    struct Stack *previous;
}Stack;

typedef struct list_struct{
    Stack *front;
    Stack *back;
}LIST;

void deallocateList(LIST *l){
    Stack *p = l->front;
    Stack *pnext;
    if(debugMode == 1){
        printf("Freeing List...\n");
    }
    while(p != NULL) {
        pnext = p->next;   // keeps us from de-referencing a freed ptr
        free(p);
        p = pnext;
    }
    // now free the LIST
    printf("List is now empty\n");
}

//Deallocate the struct of maze
void deallocateMaze(maze m1){
    int i;
    if(debugMode == 1){
        printf("Deallocating Maze...\n");
    }
    
    //Deallocating the rows
    for(i = 0; i<m1.xsize; i++){
        free(m1.arr[i]);
    }
    
    //Free whole array
    free(m1.arr);
}

maze troyCode(int argc, char **argv){
    maze m1;
    
    int xpos, ypos;
    int i,j;
    
    FILE *src;
    
    /* verify the proper number of command line arguments were given */
    if(argc < 2) {
        printf("Usage: %s <input file name>\n", argv[0]);
        exit(-1);
    }
    else if(argc > 3){
        fprintf(stderr, "Too many files or commands.\n");
        exit(0);
    }
    
    //Check if there are 3 arguments in terminal
    if(argc == 3){
        if((argv[1][0] == '-' && argv[1][1] == 'd') || (argv[2][0] == '-' && argv[2][1] == 'd') ) {
            printf("\nDebug Mode Activated...\n");
            debugMode = 1;
        }
    }
    
    if(debugMode == 1){
        printf("Opening Files...");
    }
    /* Try to open the input file. */
    if ( ( src = fopen( argv[1], "r" )) == NULL)
    {
        if (( src = fopen( argv[2], "r" )) == NULL ){
            fprintf (stderr, "Can't open input file: %s", argv[2] );
            exit(-1);
        }
        else{
            
        }
    }
    
    
    /* read in the size, starting and ending positions in the maze */
    fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
    if(debugMode == 1){
        printf("Scanning Size of Maze...\n");
    }
    if(m1.xsize <= 0 || m1.ysize <= 0){
        fprintf(stderr, "Can't have a maze, if there is no path!\n");
        exit(0);
    }
    if(m1.xsize == EOF || m1.ysize == EOF){
        fprintf(stderr, "Text file needs more inputs.\n");
        exit(0);
    }
    m1.arr = (char**)malloc(sizeof(char *) * ((m1.xsize)+2));
    for (i = 0; i < (m1.xsize+2); i++){
        m1.arr[i] = (char*)malloc(sizeof(char) * ((m1.ysize)+2));
    }
    
    fscanf (src, "%d %d", &m1.xstart, &m1.ystart);
    if(debugMode == 1){
        printf("Checking Start Positions...\n");
    }
    if(m1.xstart > m1.xsize || m1.xstart < 0){
        fprintf(stderr, "X's start should be within 0 or Max Size of X\n");
        exit(0);
    }
    if(m1.ystart > m1.ysize || m1.ystart < 0){
        fprintf(stderr, "Y's start should be within 0 or Max Size of Y\n");
        exit(0);
    }
    if(m1.xstart == EOF || m1.ystart == EOF){
        fprintf(stderr, "Text file needs more inputs.\n");
        exit(0);
    }
    fscanf (src, "%d %d", &m1.xend, &m1.yend);
    if(debugMode == 1){
        printf("Checking End Positions...\n");
    }
    if(m1.xend > m1.xsize || m1.xend < 0){
        fprintf(stderr, "X's end should be within 0 or Max Size of X\n");
        exit(0);
    }
    if(m1.yend > m1.ysize || m1.yend < 0){
        fprintf(stderr, "Y's end should be within 0 or Max Size of \nY");
        exit(0);
    }
    if(m1.xend == EOF || m1.yend == EOF){
        fprintf(stderr, "Text file needs more inputs.\n");
        exit(0);
    }
    /* print them out to verify the input */
    printf ("size: %d, %d\n", m1.xsize, m1.ysize);
    printf ("start: %d, %d\n", m1.xstart, m1.ystart);
    printf ("end: %d, %d\n", m1.xend, m1.yend);
    
    /* initialize the maze to empty */
    for (i = 0; i < m1.xsize+2; i++)
        for (j = 0; j < m1.ysize+2; j++)
            m1.arr[i][j] = '.';
    
    
    if(debugMode == 1){
        printf("Aligning Boreders...\n");
    }
    /* mark the borders of the maze with *'s */
    for (i=0; i < m1.xsize+2; i++)
    {
        m1.arr[i][0] = '*';
        m1.arr[i][m1.ysize+1] = '*';
    }
    for (i=0; i < m1.ysize+2; i++)
    {
        m1.arr[0][i] = '*';
        m1.arr[m1.xsize+1][i] = '*';
    }
    
    
    if(debugMode == 1){
        printf("Marking End and Start Positions...\n");
    }
    /* mark the starting and ending positions in the maze */
    m1.arr[m1.xstart][m1.ystart] = 's';
    m1.arr[m1.xend][m1.yend] = 'e';
    
    if(debugMode == 1){
        printf("Placing Block Positions...\n");
    }
    /* mark the blocked positions in the maze with *'s */
    while (fscanf (src, "%d %d", &xpos, &ypos) != EOF)
    {
        if(xpos > m1.xsize || xpos < 0){
            fprintf(stderr, "Position of Block should be within 0 or Max Size of X\n");
            continue;
        }
        if(ypos > m1.ysize || ypos < 0){
            fprintf(stderr, "Position of Block should be within 0 or Max Size of Y\n");
            continue;
        }
        if(xpos == m1.xstart && ypos == m1.ystart){
            fprintf(stderr, "Position of Block should not be at start\n");
            continue;
        }
        if(xpos == m1.xend && ypos == m1.yend){
            fprintf(stderr, "Position of Block should not be at end\n");
            continue;
        }
        m1.arr[xpos][ypos] = '*';
    }
    
    printf("Starting Maze\n");
    /* print out the initial maze */
    for (i = 0; i < m1.xsize+2; i++)
    {
        for (j = 0; j < m1.ysize+2; j++)
            printf ("%c", m1.arr[i][j]);
        printf("\n");
    }
    fclose(src);
    return m1;
}

//Initializing the stack
LIST initializeStack(){
    //Debugger
    if(debugMode == 1){
        printf("Initializing...\n");
    }
    
    //Initializing the stack
    LIST *list;
    list = (LIST*)malloc(sizeof(LIST));
    list->back = NULL;
    list->front = NULL;
    return *list;
}

void push_front_stack(LIST *l, int xcoor, int ycoor) {
    Stack *p = malloc(sizeof(Stack));
    p->xcoor = xcoor;
    p->ycoor = ycoor;
    if(l->front == NULL){
        l->front = p;
        p->next = NULL;
        p->previous = NULL;
    }
    p->next = l->front;
    l->front->previous = p;
    p->previous = NULL;
    
    l->front = p;
    if(l->back == NULL)   // was empty, now one elem
        l->back = p;
}

void push_back_stack(LIST *l, int xcoor, int ycoor) {
    if(l->back == NULL){
        push_front_stack(l, xcoor, ycoor);
    }
    Stack *p = malloc(sizeof(Stack));
    //Fix the position coordinates
    p->xcoor = xcoor;
    p->ycoor = ycoor;
    p->previous = l->back;
    p->next = NULL;
    l->back->next = p;
    l->back = p;

}
void pop_back_stack(LIST *l, maze m1){
    //Declare a pointer to the stack
    Stack *target;
    if(l->front == NULL){
        printf("There is no solution to the maze.");
    }
    
    //Free the node if there is only one node
    if(l->front == l->back) {  // one element
        free(l->front);
        l->front = NULL;
        l->back = NULL;
    }
    else{
        //Decrement back to not lose the pointer.
        target = l->back;
        l->back = l->back->previous;
        free(target);
    }
    return;
}

//Print out the board for visual debugging purposes
void printBoard(maze m1){
    int i;
    int j;
    /* print out the initial maze */
    for (i = 0; i < m1.xsize+2; i++)
    {
        for (j = 0; j < m1.ysize+2; j++)
            printf ("%c", m1.arr[i][j]);
        printf("\n");
    }
}

//Check if list is empty.
int checkEmpty_stack(LIST *l){
    if(l->front == NULL && l->back == NULL){
        printf("List is empty\n");
        return 1;
    }
    return 0;
}

//This function will print the path from start to end.
void print_path(LIST *l) {
    Stack *p = l->front;
    if(debugMode == 1){
        printf("Printing Path to End from Start...\n");
    }
    //Traverse through list and print out contents.
    while(p != NULL) {
        printf("(%d, %d)\n", p->xcoor, p->ycoor);
        p = p->next;
    }
    printf("\n");
}

void accessTop(LIST *l){
    Stack *p = l->back;
    printf("Current Position: (%d, %d)\n", p->xcoor, p->ycoor);
}

void userInput(int argc, char **argv){
    maze m1 = troyCode(argc, argv);
    LIST list;
    list = initializeStack();
    
    if(debugMode == 1){
        printf("Pushing Start to Stack...\n");
    }
    push_front_stack(&list, m1.xstart, m1.ystart);
    m1.arr[m1.xstart][m1.ystart] = 'x';
    int flag = 0;
    int xpos = m1.xstart;
    int ypos = m1.ystart;
    
    //add 1 to x coordinate goes down
    //add 1 to y coordinate goes right
    while(flag == 0){
        
        //Check if we can go up
        if(m1.arr[xpos - 1][ypos] == '.'||m1.arr[xpos-1][ypos] == 'e'){
            xpos = xpos - 1;
            ypos = ypos;
            push_back_stack(&list, xpos, ypos);
            
            if(debugMode == 1){
                printf("Going Up... ");
                accessTop(&list);
            }
            //Check if up is ending
            if(m1.arr[xpos][ypos] == 'e'){
                flag = 1;
                break;
            }
            m1.arr[xpos][ypos] = 'x';
            continue;
        }
        //Check if we can go right
        else if(m1.arr[xpos][ypos + 1] == '.'||m1.arr[xpos][ypos+1] == 'e'){
            xpos = xpos;
            ypos = ypos + 1;
            push_back_stack(&list, xpos, ypos);
            if(debugMode == 1){
                printf("Going Right...");
                accessTop(&list);
            }
            //Check if right is ending
            if(m1.arr[xpos][ypos] == 'e'){
                flag = 1;
                break;
            }
            m1.arr[xpos][ypos] = 'x';
            continue;
        }
        //Check if we can go down
        else if(m1.arr[xpos + 1][ypos] == '.'||m1.arr[xpos+1][ypos] == 'e'){
            xpos = xpos + 1;
            ypos = ypos;
            push_back_stack(&list, xpos, ypos);
            if(debugMode == 1){
                printf("Going Down...");
                accessTop(&list);
            }
            //Check if down is ending
            if(m1.arr[xpos][ypos] == 'e'){
                flag = 1;
                break;
            }
            m1.arr[xpos][ypos] = 'x';
            continue;
        }
        //Check if we can go left
        else if(m1.arr[xpos][ypos - 1] == '.'||m1.arr[xpos][ypos-1] == 'e'){
            xpos = xpos;
            ypos = ypos - 1;
            push_back_stack(&list, xpos, ypos);
            if(debugMode == 1){
                printf("Going Left...");
                accessTop(&list);
            }
            //Check if left is ending
            if(m1.arr[xpos][ypos] == 'e'){
                flag = 1;
                break;
            }
            m1.arr[xpos][ypos] = 'x';
            continue;
        }
        //Go back if nowhere to go and if list is empty then break out of loop.
        else{
            pop_back_stack(&list, m1);
            if(checkEmpty_stack(&list) == 1){
                if(debugMode == 1){
                    printf("Checking if List is empty...\n");
                }
                break;
            }
            xpos = list.back->xcoor;
            ypos = list.back->ycoor;
            continue;
        }
    }
    
    printf("Ending Results\n");
    printBoard(m1);
    
    if(debugMode == 1){
        printf("Checking if end was reached...\n");
    }
    if(flag == 1){
        printf("The end has been found!\n");
        print_path(&list);
    }
    else{
        printf("The end could not be reached.\n");
        exit(0);
    }
    deallocateMaze(m1);
}

int main(int argc, char **argv)
{
    userInput(argc, argv);
    if(debugMode == 1){
        printf("Done...\n");
    }
    return 0;
}
