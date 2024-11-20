#include "vm/vm.h"

int main(void) {

    initVM();
    interpret("-3 + 5 * 6");

    return 0;
}
