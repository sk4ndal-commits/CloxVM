
#include "chunk.h"
#include "opcodes.h"
#include "vm.h"

int main(void) {

    initVM();
    interpret("123.32");

    return 0;
}
