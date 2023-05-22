#include <stdio.h>
float costinitial();
float costsum(float added, float initialsum);
char continues();

void main(void)
{
    float initial, ending, sum;
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
        sum   = costsum(ending, sum);
        reply = continues();
    }
    printf("The total cost of all your item with GST is $%.2f", sum);
    printf("\nHave a nice day!");
}

float costinitial()
{
    float initial;
    printf("Enter your initial cost: ");
    scanf("%f", &initial);
    return initial;
}

float costsum(float added, float initialsum)
{
    float sum;
    sum = initialsum + added;
    return sum;
}
char continues()
{
    fflush(stdin);
    char reply;
    printf("\nWould you like to continue with another cost? If so type y: ");
    scanf("%c", &reply);
    return reply;
}
