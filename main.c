#include <stdio.h>
#include <malloc.h>
short int* gen_module(unsigned long long size_of_open_key)
{

}
short int comparison(short int* a, short int* b, unsigned long long size_a, unsigned long long size_b)//сравнение
{
    if (size_a > size_b) {
        return 1;
    } else if (size_b > size_a) {
        return 2;
    } else {
        for (unsigned long long int i = size_a - 1; i >= 0; i--){
            if (a[i] > b[i]){
                return 1;
            }
            if (b[i] > a[i]){
                return 2;
            }
        }
    }
    return 0;
}
void vic(short int *a,short int *b,short int *c, unsigned long long int len, unsigned long long int lenm)//вспомогательная функция вычитания
{
    for (int ix = 0; ix < lenm; ix++){
        a[ix + 1]--;
        c[ix] += 10 + a[ix];
        c[ix] -= b[ix];
        if (c[ix] / 10 > 0){
            c[ix + 1]++;
            c[ix] %= 10;
        }
    }
    for(int ix = lenm; ix<len; ix++){
        a[ix+1]--;
        c[ix]+=10 + a[ix];
        if(c[ix] / 10 > 0){
            c[ix + 1]++;
            c[ix]%=10;
        }
    }
}
short int* long_subtraction(short int* a, short int* b, unsigned long long size_a, unsigned long long size_b)//вычитание
{
    unsigned long long int length = size_a, lenm=size_a;
    short int k = 3;
    if (size_a > size_b) {
        length = size_a;
        k = 1;
        lenm = size_b;
    } else if (size_b > size_a) {
        length = size_b;
        k = 2;
        lenm = size_a;
    } else {
        for (unsigned long long int i = length - 1; i >= 0; i--){
            if (a[i] > b[i]){
                k = 1;
                break;
            }
            if (b[i] > a[i]){
                k = 2;
                break;
            }
        }
    }
    short int *xy = (short int*) calloc(length + 1, sizeof(int));
    for(int i = 0; i<length; i++) *(xy+i) = 0;
    if (k == 1)
    {
        vic(a, b, xy, length, lenm);
        while (xy[length - 1] == 0) length--;
        return xy;
    }
    if (k == 2)
    {
        vic(b, a, xy, length, lenm);
        while (xy[length - 1] == 0) length--;
        printf("-");
        return xy;
    }
    if (k == 3) return NULL;
}


void gen_pubkey(short int *pubkey, unsigned int size)
{
    //рандомно заполняем массив-ключ
}
void genkey (short int *pubkey, unsigned int sizeof_pubkey)
{
    // генерируем ключ
}
/*void umnojenie(int *a, int *b, int len1, int len2)
{
    int length = len1 + len2 + 1;
    int *xy = (int*)malloc(length*sizeof(int));
    for(int i = 0; i<length; i++) *(xy+i) = 0;
    for (int i = 0; i < len1; i++)
        for (int j = 0; j < len2; j++)
            xy[i + j] += a[i] * b[j];

    for (int i = 0; i < length-1; i++){
        xy[i + 1] += xy[i] / 10;
        xy[i] %= 10;
    }
    while (xy[length - 1] == 0) length--;
    for (int i = length - 1; i >= 0; i--) printf("%d", xy[i]); printf("\n");
    free(xy);
}*/
void ExtendedGCD()
{
    // находим секретный ключ
}
int main() {

    return 0;
}
