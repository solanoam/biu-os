#include <stdio.h>
int main(){
    int num = 0;
    int i;
    for (i = 5; i > 0; i--){
        scanf("%d",&num);
        switch(num){
            case 0:
                printf("0\n");
                break;
            case 1:
                printf("1\n");
                break;
            default:
                printf("bla\n");
        }
    }
    return 0;
}
