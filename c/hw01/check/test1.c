#include <stdio.h>

int main(){
    int a,b; char c;
    printf("Enter:\n");
    if(scanf(" (%d , %d %c", &a, &b, &c) != 3){
        printf("bad\n");
        return -1;
    }
    
    printf("%d %d c is '%c'\n",a, b, c);

    return 0;
}


    // int status = -1;
    // }else if((x1 == x4 && x2 == x3 && y1 == y2 && y3 == y4)){
    //     if((x4 - x3) == 0 || (y4 - y1) == 0){ 
    //         status = -1;}else{ status= 0;}
    // }else if((x1 == x2 && x3 == x4 && y1 == y4 && y2 == y3)){
    //     if((x2 - x4) == 0 || (y3 - y1) == 0){
    //         status = -1;
    //     }else{status = 0;}      
    // // }else if( (x3 == x1 && y2 == y4 && x3 == (x4 + x2) / 2) || (x1 == x4 && y1 == y3 && x2 == (x1 + x3) / 2) ){
    // }else if(x1 == 0 && x2 == 0 && x3 == 0 && x4 == 0 && y1 == 0 && y2 == 0 &&  y3 == 0 && y4 == 0){
    //     status = -1;
    // }
    // //checking if all are 0