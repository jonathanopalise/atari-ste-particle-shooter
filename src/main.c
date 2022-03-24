#include <mint/osbind.h>
#include <mint/sysbind.h>
#include 

void main_supervisor() {
    initialise();
    while (1) {}
}

int main(int argc, char **argv)
{
   Supexec(&main_supervisor, 0,0,0,0,0);
   return 0;
}
