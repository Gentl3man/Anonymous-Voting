#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INIT_SIZE 1024
#define SOLVE 0
#define RANDOM 1
#define VOTES 2
#define URANDOM "/dev/urandom"
#define DEFAULT_VOTE_FILE "anon_votes.txt"
short option;
FILE* input;
void gauss_elimination(int m, int n, double a[m][n], double x[n-1]){
    int i,j,k;
    long double temp;
    long double  term;
    for(i=0;i<m;i++)
    {
        printf("a[%d][0]==%f\n",i+1,a[i][0]);
    }
    for(i=0;i<m-1;i++){
        //Partial Pivoting
        for(k=i+1;k<m;k++){
            //If diagonal element(absolute vallue) is smaller than any of the terms below it
            if(fabs(a[i][i])<fabs(a[k][i])){
                //Swap the rows
                for(j=0;j<n;j++){                
                    
                    temp=a[i][j];
                    a[i][j]=a[k][j];
                    a[k][j]=temp;
                }
            }
        }
        //Begin Gauss Elimination
        for(k=i+1;k<m;k++){
            term=a[k][i]/ a[i][i];
            for(j=0;j<n;j++){
                a[k][j]=a[k][j]-term*a[i][j];
            }
        }
         
    }
    //Begin Back-substitution
    for(i=m-1;i>=0;i--){
        x[i]=a[i][n-1];
        for(j=i+1;j<n-1;j++){
            x[i]=x[i]-a[i][j]*x[j];
        }
        x[i]=x[i]/a[i][i];
    }
             
}


void solve(int argc, char const *argv[],int rows);
void random_voting(int argc, char const *argv[]);
void argument_check(int argc, char const *argv[])
{
    if ( argc < 3)
    {
        printf("Format:\n./anon_voting solve <file_name>\n\t");

        printf("Given the file <file_name> decrypts the number of votes.\n\tfirst line of the file must contain the number of voters\n\n");
        
        printf("./anon_voting random <number>\n\t");

        printf("Creates the file: \'anon_votes\' with random <number> decrypted votes\n\n");

        printf("./anon_voting votes <file_name>\n\tFile name contains the names and votes (0 or 1) of the voters and decrypts them\n\tformat: <Name> <number>\n\n");

        printf("\nRULE FORMAT: (x,f(x)) ,where f(x) is a polynomial equation of <voters> degree.\n");
        exit(EXIT_SUCCESS);
    }

    if ( !strcmp(argv[1],"solve" ) )
    {
        option = SOLVE;
        return;
    }

    if ( !strcmp(argv[1],"random") )
    {
        option = RANDOM;
        return;
    }
    
    if(!strcmp(argv[1],"votes"))
    {
        option = VOTES;
        return;
    }
    printf("Invalid Option.\nRun: ./anon_voting for help\n");
    exit(EXIT_FAILURE);

}

void init()
{

}



int main(int argc, char const *argv[])
{
    argument_check(argc,argv);
    int total_points,buff_counter;
    char buff[128],c;
    buff_counter =0;
    switch (option)
    {
    case SOLVE  :   input = fopen(argv[2],"r");
                    c = fgetc(input);
                    while(c!='\n')
                    {
                        buff[buff_counter] = c;
                        buff_counter++;
                        c = fgetc(input);
                    }
                    buff[buff_counter]= '\0';
                    total_points = atoi(buff);
                    //printf("total points ==%d\n",total_points);
                    solve(argc, argv,total_points);
                    fclose(input);                
                    break;
    case RANDOM : random_voting     (argc, argv); break;
    case VOTES  : votes_from_file   (argc, argv); break;
    default:
        printf("Something unexpected happen\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}


void random_voting(int argc, char const *argv[])
{
    unsigned int total_votes = atoi( argv[2] );  
    unsigned int *vote_array;
    int test_vote =0;
    int **polynomials;
    //double **points;
    double *points;
    int i,j,k,random_num;
    char in_str[1024];
    if( total_votes == 0)
    {
        printf("<Number> must be a higher than 0 integer\nExiting...\n");
        exit(EXIT_FAILURE);
    }

    FILE *urandom = fopen(URANDOM,"r");
    FILE *output_file;
    vote_array = (unsigned int*)malloc(sizeof(unsigned int)*total_votes);

    polynomials =  (int**)malloc(sizeof(int*)*total_votes);

    points =    (double*)malloc(sizeof(double)*total_votes);


    for(i=0; i<total_votes; i++)
    {
        random_num = (int)fgetc(urandom);
        if ( random_num == EOF )
        {
            printf("Failed to generate a random number, please try again.\n");
            exit(EXIT_FAILURE);
        }
        vote_array[i] = random_num % 2; //0 or 1
        if (vote_array[i]) test_vote++;
        polynomials[i] = (int*)    malloc(sizeof(int)*total_votes);
        //points[i] = (double*)   malloc(sizeof(double)*total_votes);

    }
    //create the polynomials
    for (i = 0; i<total_votes; i++)
    {
        for (j = 0; j<total_votes-1; j++)
        {
            random_num = (int)fgetc(urandom);

            if ( random_num == EOF )
            {
                printf("Failed to generate a random number, please try again.\n");
                exit(EXIT_FAILURE);
            }
            
            polynomials[i][j] = random_num % 11; //prime

        }
        polynomials[i][j] = vote_array[i]; //actual vote
    }

    
    //create the points (x,f(x))
    for (i=0; i<total_votes; i++)
    {
        points[i] = 0;
        for (j=0; j<total_votes; j++)
        {
            for (k=0; k<total_votes; k++)
            {
                
                points[i] = points[i] + (polynomials[j][k] * pow( (double)(i+1) , (double)(total_votes-1-k) ) );
            }
        }
    }
    output_file = fopen(DEFAULT_VOTE_FILE,"w");
    fprintf(output_file,"%s\n",argv[2]);
    
    
    for(i=0; i<total_votes; i++)
    {
        sprintf(in_str,"(%d,%f)\n",i+1,points[i]);
        fprintf(output_file,"%s",in_str);
    }
    fclose(urandom);
    fclose(output_file);
    printf("Done\n");
    printf("Positive votes: %d\n",test_vote);
}

void solve(int argc, char const *argv[],int rows)
{
    //printf("hello\n");
    char buff[1024],c;
    char *ptr; //for strtod to work
    int buff_counter,total_points;
    int i,j,k; //giati fusika
    double sol[rows];    //solution, you ll get the final votes from here
    double a[rows][rows+1];     //create an array as a polynomial and solve it using gauss elimination
    
    
    total_points = rows;
    c = fgetc(input) ;
    i = 0;
    //printf("rows == %d\n",rows); 
    while ( c != EOF && i < rows)
    {
        while (c != ',' && c != EOF )
        {
            c = fgetc(input);
        }

        buff_counter = 0;

        c = fgetc(input);

        while( c != ')' && c != EOF )
        {
            buff[buff_counter] = c;
            buff_counter++;
            c = fgetc(input);
        }
        buff[buff_counter] = '\0';
        sol[i] = strtod(buff,&ptr);
        //printf("sol[%d] == %f\n\n",i,sol[i]);
        //printf("ok edw1 %d\n",i);

        i++;
    }

    //But first gimme some space
    //printf("Total points:%d\n",total_points);
    for(i = 0; i< total_points; i++)
    {
        //a[i] = (double*)malloc(sizeof(double)*total_points+1);
        for(j=0; j<total_points; j++)
        {
            a[i][j] = pow( (double)(i+1), (double)(total_points-j-1) );
            

        }
        a[i][j] = sol[i];
        
        sol[i] = 0;
    }
    
    gauss_elimination(total_points,total_points+1,a,sol);

    printf("Total votes = %f\n",sol[total_points-1]);

}

