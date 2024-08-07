#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int a[10000];           //set global variable
static int max_sti, max_stj, max_eni, max_enj;       //corner of maximum submatrix
static int sum_max;             //muximum sum of submatrix

clock_t start, stop;            //set clock
double duration, duration_tick;

void Find_Max1(int n);
void Find_Max2(int n);
void Find_Max3(int n);

int main(){

    max_sti = -1;

    /*require the input n*/
    int n;
    printf("Please enter a positive integer as n:");
    scanf("%d",&n);   

    /*randomly generate a matrix*/
    printf("The randomly generated matrix is:\n");
    int i, j;
    
    srand((unsigned)time(NULL));      /*continuously generate random numbers*/
    for (i = 0; i < n; i++){

        for(j = 0; j < n; j++){
            a[i*n + j] = rand() % 19 - 9;        //set the range of -9 to 9
            printf("%3d", a[i*n + j]);
        }

        printf("\n");            //reach the end of a line
    }

    /*require the input k1*/
    int k;
    printf("Please enter a positive integer as k for function1:");
    scanf("%d",&k);

    /*measure the k times of performances of algorithm1 */
    start = clock();

    for (i = 0; i < k; i++)Find_Max1(n);          //run function1 for k times

    stop = clock();
    duration = ((double)(stop - start))/CLK_TCK;
    duration_tick = (double)(stop - start);

    if(max_sti < 0){           //in case of every elements in matrix are smaller than 0, which is forbidden in this prolblem
        printf("all the elements are smaller than 0, please generate a new matrix.");
        return 0;              //ask the user to run the program again until getting a legal matrix
    }

    printf("The maximum submatrix is:\n");       /*print out the max submatrix*/
    for (i = max_sti; i <= max_eni; i++){

        for (j = max_stj; j <= max_enj; j++){

            printf("%3d", a[i*n + j]);
        }
        printf("\n");           //reach the end of a line
    }

    printf("and has the sum of %d\n", sum_max);                 //print out the duration
    printf("It takes %f second to run the function1 for %d times.\n", duration, k);
    printf("It takes %f ticks to run the function1 for %d times.\n", duration_tick, k);
  
   /*require the input k2*/
    printf("Please enter a positive integer as k for function2:");
    scanf("%d",&k);

    /*measure the k times of performances of algorithm2 */
    start = clock();

    for (i = 0; i < k; i++)Find_Max2(n);       //loop the function2 for k times

    stop = clock();
    duration = ((double)(stop - start))/CLK_TCK;
    duration_tick = (double)(stop - start);

    printf("The maximum submatrix is:\n");               /*print out the max submatrix*/
    for (i = max_sti; i <= max_eni; i++){

        for (j = max_stj; j <= max_enj; j++){

            printf("%3d", a[i*n + j]);
        }
        printf("\n");                        //reach the end of a line
    }

    printf("and has the sum of %d\n", sum_max);                     //print out the duration
    printf("It takes %f second to run the function2 for %d times.\n", duration, k);
    printf("It takes %f ticks to run the function2 for %d times.\n", duration_tick, k);

    /*require the input k3 */
    printf("Please enter a positive integer as k for function3:");
    scanf("%d",&k);

    /*measure the k times of performances of algorithm3 */
    start = clock();

    for (i = 0; i < k; i++)Find_Max3(n);

    stop = clock();
    duration = ((double)(stop - start))/CLK_TCK;
    duration_tick = (double)(stop - start);

    printf("The maximum submatrix is:\n");
    for (i = max_sti; i <= max_eni; i++){              /*print out the max submatrix*/

        for (j = max_stj; j <= max_enj; j++){

            printf("%3d", a[i*n + j]);
        }
        printf("\n");                 //reach the end of a line
    }

    printf("and has the sum of %d\n", sum_max);          //print out the duration
    printf("It takes %f second to run the function3 for %d times.\n", duration, k);
    printf("It takes %f ticks to run the function3 for %d times.\n", duration_tick, k);


    char c;
    printf("hit any number and enter to finish the program.");
    scanf("%d",&n);

   
    return 0;


}

void Find_Max1(int n){          /*get the maximum submatrix using algorythm1*/

    int st_i, st_j, en_i, en_j, i, j;      //set the corner
    int sum;
    sum_max=0;
    
    for (st_i = 0; st_i < n; st_i++){           /*st_i,st_j ensure the top left corner*/
        for (st_j = 0; st_j < n; st_j++){
            
            for (en_i = st_i; en_i < n; en_i++){          /*en_i,en_j ensure the lower right corner*/
                for (en_j = st_j; en_j < n; en_j++){

                    sum = 0;                 //reset the sum
                    for (i = st_i; i <= en_i; i++){        /*loop to get the sum of submatrix*/
                        for (j = st_j; j <= en_j; j++){
                            sum += a[i*n + j];
                        }}
                                
                    if (sum >= sum_max){       /*if the sum larger than max*/
                        sum_max = sum;        //set the max
                        max_sti = st_i;       //set the corner of max submatrix
                        max_stj = st_j;
                        max_eni = en_i;
                        max_enj = en_j;

                    }

                }}
        }}
        return;
}

void Find_Max2(int n){          /*get the maximum submatrix using algorythm2*/

    int st_i, st_j, i, j;
    static int en_sum[10001][10001] ;      //en_sum[i][j] stores the sum of submatrix with lower right corner of (i,j)
    sum_max=0;
   
    for (st_i = 0; st_i < n; st_i++){           /*st_i,st_j ensure the top left corner*/
        for (st_j = 0; st_j < n; st_j++){
            
            for (i = 0; i < n; i++){          /*memset(en_sum, 0, sizeof(en_sum));*/
                for (j = 0; j < n; j++){
                    en_sum[i+1][j+1] = 0;         //loop quicker than the function memset
            }}
            
            for (i = st_i; i < n; i++){       /*loop to get new sum*/
                for (j = st_j; j < n; j++){
                    en_sum[i+1][j+1] = en_sum[i+1][j] + en_sum[i][j+1] - en_sum[i][j] + a[i*n + j];
                   
                    if (en_sum[i+1][j+1]>=sum_max){           /*if the sum larger than max*/
                        sum_max = en_sum[i+1][j+1];
                        max_sti = st_i;                //set the corner of max submatrix
                        max_stj = st_j;
                        max_eni = i;
                        max_enj = j;

                    }

                }}
        }}
		return;
        
}

void Find_Max3(int n){       /*get the maximum submatrix using algorythm3*/

    int st_i, st_j, i, j, en_i, en_j;
    static int en_sum[10001][10001];      //en_sum[i][j] stores the sum of submatrix with lower right corner of (i,j)
    int sum;
    sum_max=0;

    for (i = 0; i < n; i++){       /*set en_sum as the sum of submatrix with (i,j) as lower right corner*/
        for (j = 0; j < n; j++){
            if(i==0||j==0)en_sum[i][j] = 0;
            en_sum[i+1][j+1] = en_sum[i][j+1] + en_sum[i+1][j] - en_sum[i][j] + a[i*n + j];


    }}
 
    for (st_i = 0; st_i < n; st_i++){               /*st_i,en_i ensure the range of rows*/
        for (en_i = st_i; en_i < n; en_i++){

            st_j = 0;                    //reset the start column
            sum = 0;                    //reset sum
            
            for (en_j = 0; en_j < n; en_j++){           /*loop to get new sum*/
                sum = en_sum[en_i+1][en_j+1] - en_sum[en_i+1][st_j] - en_sum[st_i][en_j+1] + en_sum[st_i][st_j];
                   
                if (sum >= sum_max){                /*if the sum larger than max*/
                    sum_max = sum;
                    max_sti = st_i;                 //set the corner of max submatrix
                    max_stj = st_j;
                    max_eni = en_i;                        
                    max_enj = en_j;

                }

                if (sum < 0) st_j = en_j + 1;     //if current submatrix less than 0, throw it away

            }
    }}
    return;
}
