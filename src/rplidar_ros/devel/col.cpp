#include <iostream>
#include "stdio.h"

int main(int, char**)
{

printf("\033[0;31m"); //Set the text to the color red
printf("Hello\n"); //Display Hello in red
printf("\033[0m"); //Resets the text to default color

//	std::cout<<"Hi there"<<std::endl;

return 0;
}