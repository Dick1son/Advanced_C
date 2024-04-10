//=========================================================================================

//Task 2. Инверсия старших

// #include <stdio.h>
// #include <stdint.h>

// void DecToBin(uint32_t);
// uint32_t InverseHigherBits(uint32_t);

// int main() {
//     uint32_t N;
//     printf("Enter: ");
//     scanf("%d", &N);
//     printf("Input number: \nDecimal: %u\nBinary: ", N);
//     DecToBin(N);
//     printf("\nOutput number: \nDecimal: %u\nBinary: ", InverseHigherBits(N));
//     DecToBin(InverseHigherBits(N));
//     return 0;
// }

// void DecToBin(uint32_t N){
//     for (int i = 1; i <= 32; i++) {
//         printf("%c", (N & 0x80000000) ? '1' : '0');
//         N <<= 1;
//         if(i % 4 == 0) 
//         printf(" ");
//     }
//     printf("\n");
// }

// uint32_t InverseHigherBits(uint32_t N){
//     return (~(N >> 24) << 24) | ((N << 8) >> 8);
// }

//=========================================================================================

//Task 3. Не пара

// #include <stdio.h>
// #include <stdint.h>

// int main()
// {
//     uint32_t size;
//     int32_t num;
//     int32_t res = 0;
//     printf("Enter N: ");
//     scanf("%d", &size);

//     for(int i = 0; i < size; i++){
//         printf("Enter num %d: ", i + 1);
//         scanf("%d", &num);
//         res ^= num;
//     }

//     printf("Unpaired number is %d", res);

//     return 0;
// }

//=========================================================================================

//Task 4. Шифр Цезаря

#include <stdio.h>
#include <ctype.h>
#include <conio.h>

char encrypt(char ch, int shift) {
    if (isupper(ch)) {
        return 'A' + ((ch - 'A' + shift) % 26);

    } else if (islower(ch)) {
        return 'a' + ((ch - 'a' + shift) % 26);

    }
    return ch;
}

int main() {
    int shift, ch;
    printf("Enter key: ");
    scanf("%d", &shift);
    getchar();
    while ((ch = getch()) != '.') {
        putchar(encrypt(ch, shift));
    }
    putchar('.');
    
    return 0;
}