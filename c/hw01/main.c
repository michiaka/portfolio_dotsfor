#include <stdio.h>
#include <math.h>
#include <float.h>
#define EP DBL_EPSILON*1000


int main(){
    
    //using 64 bits 
    long double x1,x2,x3,x4,y1,y2,y3,y4;
    char c;

    printf("Point #1:\n");
    if(scanf(" (%Lf , %Lf %c", &x1, &y1, &c) != 3){
        printf("Invalid input.\n");
        return -1;
    }else if(c != ')'){
        printf("Invalid input.\n");
        return -1;        
    }
    
    printf("Point #2:\n");
    if(scanf(" (%Lf , %Lf %c", &x2, &y2, &c) != 3){
        printf("Invalid input.\n");
        return -1;
    }else if(c != ')'){
        printf("Invalid input.\n");
        return -1;      
    }

    printf("Point #3:\n");
    if(scanf(" (%Lf , %Lf %c", &x3, &y3, &c) != 3){
        printf("Invalid input.\n");
        return -1;
    }else if(c != ')'){
        printf("Invalid input.\n");
        return -1;      
    }


    printf("Point #4:\n");
    if(scanf(" (%Lf , %Lf %c", &x4, &y4, &c) != 3){
        printf("Invalid input.\n");
        return -1;
    }else if(c != ')'){
        printf("Invalid input.\n");
        return -1;      
    }

    //checking if it is a recatangle without degree
    if(fabs(x1 - x2) <= (EP*fabs(fabs(x1) + fabs(x1))) && fabs(x3 - x4) <= (EP*fabs(fabs(x3) + fabs(x4))) && fabs(y1 - y4) <= (EP*fabs(fabs(y1) + fabs(y4))) && fabs(y2 - y3) <= (EP*fabs(fabs(y2) + fabs(y3)))){
        if(fabs(x2 - x4) <= (EP*fabs(fabs(x2) + fabs(x4))) || fabs(y3 - y1) <= (EP*fabs(fabs(y3) + fabs(y1)))){
        printf("The points do not form a rectangle.\n");
        return -1;
        }else{printf("The points form a rectangle.\n"); return 0;}
    }else if(fabs(x1 - x4) <= (EP*fabs(fabs(x1) + fabs(x4))) && fabs(x2 - x3) <= (EP*fabs(fabs(x2) + fabs(x4)))  && fabs(y1 - y2) <= (EP*fabs(fabs(y1) + fabs(y2))) && fabs(y3 - y4) <= (EP*fabs(fabs(y3) + fabs(y4)))){
        if(fabs(x4 - x3) <= (EP*fabs(fabs(x4) + fabs(x3))) || fabs(y4 - y1) <= (EP*fabs(fabs(y4) + fabs(y1)))){ 
        printf("The points do not form a rectangle.\n");
        return -1;
        }else{printf("The points form a rectangle.\n"); return 0;}
    }

    //calculaing the power of length of the rectangle
    int d1, d2, d3, d4;
    d1 = pow(x3 - x4, 2) + pow(y3 - y4, 2);
    d2 = pow(x2 - x1, 2) + pow(y2 - y1, 2);
    d3 = pow(x4 - x1, 2) + pow(y4 - y1, 2);
    d4 = pow(x3 - x2, 2) + pow(y3 - y2, 2);

    //checking if all line has length more than 0
    if(d1 == 0 || d2 == 0 || d3 == 0 || d4 == 0){
        printf("The points do not form a rectangle.\n");
        return -1;
    }


    //checking if one of courners is 90 degree
    if((fabs((y1 - y2)*(y2 - y3) + (x1 - x2)*(x2 - x3))) <= (10000*EP*fabs(fabs(y1) + fabs(y2) + fabs(x1) + fabs(x2) + fabs(x3)))){
        if(fabs(d1 - d2) <= (EP*fabs(fabs(d1) + fabs(d2))) &&  fabs(d3 - d4) <= (EP*fabs(fabs(d3) + fabs(d4)))) {
            printf("The points form a rectangle.\n");
            return 0;
        }else{printf("The points do not form a rectangle.\n"); return -1;}
    }else{ 
        printf("The points do not form a rectangle.\n"); 
        return -1;
    }

}