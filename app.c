#include "app.h"

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        fprintf(stderr, "%s\n", "Incorrect amount of arguments");
        exit(EXIT_FAILURE);
    }

    /* Si tengo menos argumentos que CHILDREN_AMOUNT, creo
    ** tantos hijos como argumentos, ya que no quiero tener mas hijos que argumentos
    */
    int childrenAmount = (CHILDREN_AMOUNT > argc - 1)? argc - 1 : CHILDREN_AMOUNT;
}

int createChildren(int chirldrenAmount) {
    
}