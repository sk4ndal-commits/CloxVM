
#include "chunk.h"
#include "opcodes.h"
#include "vm.h"

int main(void) {

    initVM();
    interpret("var x = 5; \n fun hello(name) {print (name)}");

    return 0;
}
