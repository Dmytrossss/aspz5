# Звіт до лабораторної роботи №5 

**Виконав: Гнилицький Дмитро**  

**Група: ТВ-33**

**Система:** Linux Mint 22.1 'Xia' MATE Edition (VirtualBox)  

**Варіант: №3**

## 1. Завдання до варіанту №3

Змоделюйте ситуацію, коли одна й та сама змінна випадково використовується як покажчик, хоча ним не є — і що з цього випливає.

## 2. Виконання завдання

### 2.1. Підготовка робочого середовища
Створив робочу директорію та перейшов у неї:
```bash
mkdir -p ~/aspz5
cd ~/aspz5
```

### 2.2. Розробка коду
Створив файл `wild_pointer.c` з наступним змістом:
```c
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
```

### 2.3. Компіляція та запуск
Скомпілював програму з відлагоджувальною інформацією:
```bash
gcc -g wild_pointer.c -o wild_pointer
```
Запустив програму та отримав очікувану помилку:

![image](https://github.com/user-attachments/assets/279939ce-98f4-4d86-b48f-18142bb37807)
```bash
dima@dima-VirtualBox:~/aspz5$ ./wild_pointer

Помилка адресування (збережено знімок оперативної пам’яті)
```

### 2.4. Аналіз за допомогою Valgrind
Встановив Valgrind та запустив аналіз:
```bash
sudo apt install valgrind
valgrind --leak-check=full --track-origins=yes ./wild_pointer
```

**Результати Valgrind:**

![image](https://github.com/user-attachments/assets/8d0ad751-85a6-44fb-89f4-214314b86557)

```bash
dima@dima-VirtualBox:~/aspz5$ valgrind --leak-check=full --track-origins=yes ./wild_pointer

==2259== Memcheck, a memory error detector

==2259== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.

==2259== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info

==2259== Command: ./wild_pointer

==2259== 

==2259== Invalid write of size 4

==2259==    at 0x10916A: main (wild_pointer.c:12)

==2259==  Address 0xdeadbeef is not stack'd, malloc'd or (recently) free'd

==2259== 

==2259== 

==2259== Process terminating with default action of signal 11 (SIGSEGV)

==2259==  Access not within mapped region at address 0xDEADBEEF

==2259==    at 0x10916A: main (wild_pointer.c:12)

==2259==  If you believe this happened as a result of a stack

==2259==  overflow in your program's main thread (unlikely but

==2259==  possible), you can try to increase the size of the

==2259==  main thread stack using the --main-stacksize= flag.

==2259==  The main thread stack size used in this run was 8388608.

==2259== 

==2259== HEAP SUMMARY:

==2259==     in use at exit: 0 bytes in 0 blocks

==2259==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated

==2259== 

==2259== All heap blocks were freed -- no leaks are possible

==2259== 

==2259== For lists of detected and suppressed errors, rerun with: -s

==2259== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

Помилка адресування (збережено знімок оперативної пам’яті)
```

## 3. Висновки
1. **Основна причина проблеми**: Небезпечне приведення типу з `unsigned long` до `int*` без перевірки валідності адреси
2. **Наслідки помилки**:
   - 100% аварійне завершення програми
   - Потенційне пошкодження системних структур даних
   - Можливість використання як вектора атаки

3. **Ефективність інструментів**:
   - **Valgrind**: Чітко ідентифікував тип помилки (Invalid write) та місце її виникнення

4. **Запобіжні заходи**:
   - Використовувати `uintptr_t` замість `unsigned long` для роботи з адресами
   - Перевіряти валідність адрес перед доступом
   - Використовувати сучасні інструменти аналізу пам'яті (ASan)
   - Уникати небезпечних приведень типів
