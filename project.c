#include <stdio.h>
#include <xmmintrin.h>

#define F1 3
#define F2 3
#define I1 8
#define I2  8
#define I3  8
#define IN  10

#define NUM_OF_TILES 4
#define TD 6



void conv(float inp[IN][IN],float filter[F1][F2],float res[IN-2][IN-2]){
  float sum;
  for (int i = 0; i <IN-F1+1  ; i++) 
      for (int j = 0; j < IN-F2+1; j++) {
      sum=0;
        for (int k = 0; k < F1; k++)
            for(int q=0;q<F2;q++)
                sum = sum + inp[i+k][j+q]*filter[k][q];
        res[i][j] = sum; 
      }
    for (int row=0; row<IN-2; row++)
    {
    for(int columns=0; columns<IN-2; columns++)
         printf("%f     ", res[row][columns]);
    printf("\n");
    }
    
  
}


void calc_u33(float filter[F1][F2],float res[6][6]){
    int m=6;
    float r1[6][F1];
    float sum;
    float g[6][3]={ {1.0/4,0,0},
            { -1.0/6,-1.0/6, -1.0/6  },
            { -1.0/6, 1.0/6, -1.0/6}, 
            {1.0/24,1.0/12,1.0/6},
            {1.0/24,-1.0/12,1.0/6},
            {0,0,1}
            };
    
     
    for (int i = 0; i < F1 ; i++){
        r1[0][i]=filter[0][i]/4;
        r1[1][i]=(filter[0][i]+filter[1][i]+filter[2][i])/(-6);
        r1[2][i]=(-filter[0][i]+filter[1][i]-filter[2][i])/(6);
        r1[3][i]=filter[0][i]/24+filter[1][i]/12+filter[2][i]/6;
        r1[4][i]= filter[0][i]/24-filter[1][i]/12+filter[2][i]/6;
        r1[5][i]=filter[2][i];
    }
    for (int i = 0; i < m ; i++){
        res[i][0]=r1[i][0]/4;
        res[i][1]=(r1[i][0]+r1[i][1]+r1[i][2])/(-6);
        res[i][2]=(-r1[i][0]+r1[i][1]-r1[i][2])/(6);
        res[i][3]=r1[i][0]/24+r1[i][1]/12+r1[i][2]/6;
        res[i][4]=r1[i][0]/24-r1[i][1]/12+r1[i][2]/6;
        res[i][5]=r1[i][2];
    }
    
    /*
      
      for (int i = 0; i < m ; i++) 
        for (int j = 0; j < m; j++) {
        sum=0;
        for(int k = 0; k < F1; k++)
          sum += r1[i][k]*g[j][k];
        res[i][j] = sum;
        } 
        
        */   
   
}

void calc_v33(float inp[IN][IN],float v[NUM_OF_TILES][TD][TD]){
    float bt[TD][TD]={{4,0,-5,0,1,0},
                  {0,-4,-4,1,1,0},
                  {0,4,-4,-1,1,0},
                  {0,-2,-1,2,1,0},
                  {0,2,-1,-2,1,0},
                  {0,4,0,-5,0,1}};
    float r1[TD][TD];
    float sum;
    int index=0;
    for(int i=0;i<IN-TD;i+=4)
        for(int j=0;j<IN-TD;j+=4){
            for (int k = 0; k < TD  ; k++) 
                for (int q = 0; q < TD; q++) {
                    sum=0;
                    for(int r = 0; r < TD; r++)
                        sum =sum+ bt[k][r]*inp[r+i][q+j];
                    r1[k][q] = sum;
                }
      
            for (int k = 0; k < TD ; k++) 
                for (int q = 0; q < TD; q++) {
                    sum=0;
                    for(int r = 0; r< TD; r++)
                        sum += r1[k][q]*bt[q][r];
                    v[index][k][q] = sum;
                }
       
            index++;
        }
  
    
}

    
void     calc_Elem_wise(float u[TD][TD], float v[NUM_OF_TILES][TD][TD],float m[NUM_OF_TILES][TD][TD]){
    for(int i=0;i<NUM_OF_TILES;i++)
        for(int j=0;j<TD;j++)
             for(int k=0;k<TD;k++)
                 m[i][j][k]=u[j][k]*v[i][j][k];
     for (int row=0; row<TD; row++)
    {
    for(int columns=0; columns<TD; columns++)
         printf("%f     ", m[0][row][columns]);
    printf("\n");
    }
    
    
}
  
void    calc_y(float m[NUM_OF_TILES][TD][TD],float y[NUM_OF_TILES][TD][TD]){
    float at[TD-2][TD]={{1,1,1,1,1,0},
                       {0,1,-1,2,-2,0},
                       {0,1,1,4,4,0},
                       {0,1,-1,8,-8,1}};
    float t[TD][TD];
    for(int i=0;i<NUM_OF_TILES;i++){
        for(int j=0;j<TD;j++){
            t[0][j]=m[i][0][j]+m[i][1][j]+m[i][2][j]+m[i][3][j]+m[i][4][j];
            t[1][j]=m[i][1][j]-m[i][2][j]+2*(m[i][3][j]-m[i][4][j]);
            t[2][j]=m[i][1][j]+m[i][2][j]+4*(m[i][3][j]+m[i][4][j]);
            t[3][j]=m[i][1][j]-m[i][2][j]+8*(m[i][3][j]+m[i][4][j]);

        }
        for(int j=0;j<TD;j++){
            y[i][j][0]=t[j][0]+t[j][1]+t[j][2]+t[j][3]+t[j][4];
            y[i][j][1]=t[j][1]-t[j][2]+2*(t[j][3]-t[j][4]);
            y[i][j][2]=t[j][1]+t[j][2]+4*(t[j][3]*t[j][4]);
            y[i][j][3]=t[j][1]+t[j][2]+8*(t[j][3]*t[j][4]);
        }
        
    }
}
    

void conv33(float inp[IN][IN],float filter[F1][F2],float res[I1][I2]){
    float u[F1*2][F1*2];
    float v[NUM_OF_TILES][TD][TD];
    float m[NUM_OF_TILES][TD][TD];
    float y[NUM_OF_TILES][TD][TD];

    calc_u33(filter,u);
    calc_v33(inp,v);
    calc_Elem_wise(u,v,m);
    calc_y(m,y);
    for (int row=0; row<4; row++)
    {
    for(int columns=0; columns<4; columns++)
         printf("%f     ", y[0][row][columns]);
    printf("\n");
    }
    
}

int main(int argc, char *argv[]){
    float filter[F1][F2];
    float inp[IN][IN];
    float res[IN-2][IN-2];
    for(int i=0;i<F1;i++){
        for(int j=0;j<F2;j++){
            filter[i][j]=1;
        }
    }
     for(int i=0;i<IN;i++){
        for(int j=0;j<IN;j++){
            inp[i][j]=i*IN+j;
        }
    }
    conv(inp,filter,res);

    conv33(inp,filter,filter);
    
    
    return 0;
}