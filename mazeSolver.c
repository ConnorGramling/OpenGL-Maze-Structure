/*
 * initMaze.c
 *
 *  Created on: November 24, 2024
 *      Author: Alli Batell
 */

// int marked[][];


// int longpath[][];
// int bestpath[][];

// int stack_i [];
// int stack_j [];
// int stack_count;
// int i=1;
// int j=1;
// int m;
// int n;

// int done =-1;

// void brute_force(maze [][]){
//     m=sizeof(maze)/sizeof(chamber);
//     n= sizeof(maze[])/sizeof(chamber);

//     stack_i[m];
//     stack_j[n];
//     marked[m][n];
//     stack_i[stack_count]=1, stack_j[stack_count]=1;
//     stack_count++;
//     for (int x = 0; x<= m, x++){
//         for (int y = 0; y<= n, y++){
//             marked[x][y]=-1;
//             bestpath[x][y]=-1;
//         }
//     }
//     step(i,j)
    
// }

// void step(int i, int j){
//     marked[i][j]=1;
//     if (!(i==m && j==n) & !done){
//         if (maze[i][j].west ==1 & marked != 1){
//             stack_i[stack_count] =i,stack_j[stack_count]= j;
//             stack_count++
//             step();
//         }
//         if (maze[i][j].south ==1 & marked != 1){
//             stack_i[stack_count] =i,stack_j[stack_count]= j;
//             stack_count++
//             step();
//         }
//         if (maze[i][j].east ==1 & marked != 1){
//             stack_i[stack_count] =i,stack_j[stack_count]= j;
//             stack_count++
//             step();
//         }
//         if (maze[i][j].north ==1 & marked != 1){
//             stack_i[stack_count] =i,stack_j[stack_count]= j;
//             stack_count++
//             step();
//         }
//     }
//     if (i==m && j==n){
//         done = 1;
//         for(int x = 0; x< stack_count; x++){
//             bestpath[stack_i[x]][stack_j[x]];
//         }
//     }
//     stack_count--;
    // stack_i[stack_count] = null;
    // stack_j[stack_count] = null;
//     // return;
// }
