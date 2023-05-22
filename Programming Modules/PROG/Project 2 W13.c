#include <stdio.h>
float costinitial();

void main(void)
{
    float initial, ending;
    int input;
    char reply;
    reply = 'y';
    
    while(reply == 'y')
    {
        initial = 0;
        
        while(initial <= 0)
        {
            initial = costinitial();
            ending  = initial * 1.07;
            if(initial > 0)
            {
                printf("The cost with inclusive 7%% GST is $%.2f", ending);
            }
            else
            {
                printf("Invalid input, please try again, remember to put a positive value!\n");
            }
        }
        printf("\nWould you like to continue with another cost? If so, type y: ");
        fflush(stdin);
        scanf("%c", &reply);
    }
    printf("Have a nice day!");
}

float costinitial()
{
    float initial;
    printf("Enter your initial cost: ");
    scanf("%f", &initial);
    return initial;
}
