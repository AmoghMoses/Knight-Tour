/* Do not make any changes in the skeleton. Your submission will be invalidated if the skeleton is changed */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
 
int N;
int arr1[51][51];

int min_deg[8];
int gc = 0;
struct possibleMoves p;
int min[] =  {-1,-1};

struct possibleMoves // This will store all possible movements baased on xDisp and yDisp respectively, and -1 whenever the knight may move out of bounds
{
    int possible1[8][2];
};
 
int xa[10001];
int ya[10001];
 
struct possibleMoves possible(int x, int y )
{
    struct possibleMoves pos;
    for(int i = 0; i < 8; i++)
    {
        pos.possible1[i][0] = -1;
        pos.possible1[i][1] = -1;
    }
    
    int xDisp[] = {2, 1, 2, 1, -2, -1, -2, -1}; // Displacement for x
    int yDisp[] = {1, 2, -1, -2, 1, 2, -1, -2}; // Displacement for y
    // This shows all possible movements for the knight in a board, which is 2 steps in one direction and 1 move perpendicular to it, or vice versa
    
    for(int i = 0; i < 8; i++)
    {
        if (x + xDisp[i] >= 0 && x + xDisp[i] < N && y + yDisp[i] >= 0 && y + yDisp[i] < N && arr1[x + xDisp[i]][y + yDisp[i]] == 0) 
        {
            pos.possible1[i][0] = x + xDisp[i];
            pos.possible1[i][1] = y + yDisp[i];
        }            
    }
    return pos;
}
 
int possible2(int x, int y)
{
    int xDisp[] = {2, 1, 2, 1, -2, -1, -2, -1};
    int yDisp[] = {1, 2, -1, -2, 1, 2, -1, -2};
    int count1 = 0; // This checks the total valid movements among the possible 8, from the Disp variables
    
    for(int i = 0; i < 8; i++)
    {
        if (x + xDisp[i] >= 0 && x + xDisp[i] < N && y + yDisp[i] >= 0 && y + yDisp[i] < N && arr1[x+xDisp[i]][y+yDisp[i]] == 0)
        {
            count1 += 1;
        }            
    }
    return count1;
}
 
void *threading21(void *args)
{
    int a[2];
    int xDisp[] = {2, 1, 2, 1, -2, -1, -2, -1};
    int yDisp[] = {1, 2, -1, -2, 1, 2, -1, -2};

    int x = *((int*)(args+0*sizeof(int)));
    int y = *((int*)(args+1*sizeof(int)));

    if((p.possible1[2*gc][0] == -1 && p.possible1[2*gc][1] == -1)&&(p.possible1[2*gc+1][0] == -1 && p.possible1[2*gc+1][1] == -1))
    {
        return NULL;
    }
 
    else if(p.possible1[2*gc][0] == -1 && p.possible1[2*gc][1] == -1)
    {
        if(possible2(p.possible1[2*gc + 1][0], p.possible1[2*gc + 1][1]) < possible2(min[0], min[1]))
        {
            min[0] = p.possible1[2*gc + 1][0];
            min[1] = p.possible1[2*gc + 1][1];
        }
    }
 
    else if(p.possible1[2*gc + 1][0] == -1 && p.possible1[2*gc + 1][1] == -1)
    {
        if(possible2(p.possible1[2*gc][0], p.possible1[2*gc][1]) < possible2(min[0], min[1]))
        {
            min[0] = p.possible1[2*gc][0];
            min[1] = p.possible1[2*gc][1];
        }
    }
    
    else
    {
        if(possible2(p.possible1[2*gc][0], p.possible1[2*gc][1]) < possible2(min[0], min[1]))
        {
            min[0] = p.possible1[2*gc][0];
            min[1] = p.possible1[2*gc][1];
        }
    
        if(possible2(p.possible1[2*gc+1][0], p.possible1[2*gc+1][1]) < possible2(min[0], min[1]))
        {
            min[0] = p.possible1[2*gc+1][0];
            min[1] = p.possible1[2*gc+1][1];
        }
    }
    return NULL;
}
 
void finalFunc(int x, int y)
{
    int xDisp[] = {2, 1, 2, 1, -2, -1, -2, -1}; // Displacement for x
    int yDisp[] = {1, 2, -1, -2, 1, 2, -1, -2}; // Displacement for y  
    
    
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            arr1[i][j] = 0;
        }
    }
    arr1[x][y] = 1;
    int counter = 2;
    int c = 0;
    for(int i = 0; i < (N * N) - 1; i++)
    {
       
        p = possible(x, y);
        int pCount = possible2(x, y);
        for(int i = 0; i < 8; i++)
        {
            if(p.possible1[i][0] == -1)
            {
                continue;
            }
            else
            {
                min[0] = p.possible1[i][0];
                min[1] = p.possible1[i][1];
                break;
            }
        }

        int arguments[4];
        arguments[0] = x;
        arguments[1] = y;
        
        pthread_t threads[4];
        int k = 0;
        for(gc = 0; gc < 4; gc++)
        {
            pthread_create(&threads[gc], NULL, threading21, (void*)arguments);
            pthread_join(threads[gc], NULL);
        }
        
        int min_degree = N;
        int min_idx = -1; 
        x = min[0];
        y = min[1]; 
        arr1[x][y] = counter;
        counter++;
    }
    
    int flag = 0;
    
    for(int i = 0; i < N;i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(arr1[i][j] == 0)
            {
                flag = 1;
            }
        }
    }
    
    if (flag == 1)
        printf("No Possible Tour");
    
    if(!flag)
    {
        for(int i = 0; i < N;i++)
        {
            for(int j = 0; j < N; j++)
            {
                xa[arr1[i][j]]=i;
                ya[arr1[i][j]]=j;
            }
        }
        for(int i = 1; i <= N * N; i++)
        {
  		    printf("%d,%d|", xa[i], ya[i]);
        }
    }
 
}
 
int main(int argc, char *argv[])
{
    if (argc != 4) 
    {
		printf("Usage: ./Knight.out grid_size StartX StartY");
		exit(-1);
	}
	
	N = atoi(argv[1]);
	int StartX=atoi(argv[2]);
	int StartY=atoi(argv[3]);
    
    if(N < 4)
    {
        printf("No Possible Tour");
        return 0;
    }
 
    if(N % 2 != 0)
    {
        if((StartX + StartY) % 2 != 0)
        {
            printf("No Possible Tour");
            return 0;
        }
    }
        
    finalFunc(StartX,StartY);
    
    return 0;
 
}