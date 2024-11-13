
#include "chunk.h"
#include "opcodes.h"
#include "vm.h"

int main(void) {

    initVM();
    interpret("\"Hello\"");

    return 0;
}
