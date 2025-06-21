#include <stdio.h>
#include <stdlib.h>

int main() {
    // Змінна, яка не є покажчиком
    unsigned long fake_pointer = 0xDEADBEEF;
    
    // Небезпечне приведення типу
    int* ptr = (int*)fake_pointer;
    
    // Спроба запису за невалідною адресою
    *ptr = 42;
    
    // Спроба читання з іншої адреси
    int* another_ptr = (int*)(fake_pointer + 100);
    printf("Значення: %d\n", *another_ptr);
    
    return 0;
}
