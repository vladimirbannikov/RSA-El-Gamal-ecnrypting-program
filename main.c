#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
short int two_127[39] = {1,7,0,1,4,1,1,8,3,4,6,0,4,6,9,2,3,1,7,3,1,6,8,7,3,0,3,7,1,5,8,8,4,1,0,5,7,2,8};
short int two_128[39] = {3,4,0,2,8,2,3,6,6,9,2,0,9,3,8,4,6,3,4,6,3,3,7,4,6,0,7,4,3,1,7,6,8,2,1,1,4,5,6};
void reverse_mass(short int* arr1,short int* arr2, int size)
{
    short int sw;
    for(int i = 0; i<size/2; i++){
        sw = arr1[i];
        arr1[i] = arr1[size-i-1];
        arr1[size-i-1] = sw;
        sw = arr2[i];
        arr2[i] = arr2[size-i-1];
        arr2[size-i-1] = sw;
    }
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


short int* gen_pubkey(unsigned int size)
{
    //рандомно заполняем массив-ключ
}
void genkey (short int *pubkey, unsigned int sizeof_pubkey)
{
    // генерируем ключ
}
short int* umnojenie(short int *a, short int *b, int len1, int len2, int *length_of_result, int free_a, int free_b) //1 для free - очистить память у одного у указателей до return
{
    int length = len1 + len2 + 1;
    short int *xy = (short int*)calloc(length, sizeof(short int));
    for (int i = 0; i < len1; i++)
        for (int j = 0; j < len2; j++)
            xy[i + j] += a[i] * b[j];

    for (int i = 0; i < length-1; i++){
        xy[i + 1] += xy[i] / 10;
        xy[i] %= 10;
    }
    if(xy[length-1]==0){
        while(xy[length-1]==0) length--;
        short int* vsp = (short int*)calloc(length, sizeof(short int));
        for(int i = 0; i<length; i++){
            vsp[i] = xy[i];
        }
        free(xy);
        xy = NULL;
        xy = (short int*)calloc(length, sizeof(short int));
        for(int i = 0; i<length; i++){
            xy[i] = vsp[i];
        }
        free(vsp);
        vsp = NULL;
    }
    if(free_a==1) {
        free(a);
        a = NULL;
    }
    if(free_b==1) {
        free(b);
        b = NULL;
    }
    *length_of_result = length;
    return xy;
}
void ExtendedGCD()
{
    // находим секретный ключ
}
void printff(short int* a, int size)
{
    for(int i = size-1; i>=0; i--){
        printf("%d", a[i]);
    }
}
void gen_q_and_p(int size)/*, short int* p, short int* q, int* sizeof_p, int* sizeof_q)*/
{
    int n = size/2/128;
    int k = n-1;
    short int* start_point;
    int lengthof_start_point = 1;
    int* pointer_to_lengthof_start_point = &lengthof_start_point;
    start_point = (short int*)calloc(lengthof_start_point, sizeof(short int));
    start_point[0] = 1;
    for(int i = 0; i<k; i++){
        start_point = umnojenie(two_128, start_point, 39, lengthof_start_point, pointer_to_lengthof_start_point, 0, 1);
    }
    short int* start_point_low, start_point_high;
    start_point_high =  umnojenie(two_128, start_point, 39, lengthof_start_point, pointer_to_lengthof_start_point, 0, 0);
    start_point_low = umnojenie(two_127, start_point, 39, lengthof_start_point, pointer_to_lengthof_start_point, 0, 0);
    /* вот тут надо написать генерацию рандомных поинтов, чтобы начиналось с какого-то */
    //printff(start_point_low, lengthof_start_point_low);
}
int main() {
    reverse_mass(two_127, two_128, 39);
    int size_of_key = 256*4; // вводимая пользователем длина ключа
    gen_q_and_p(256*4);
    return 0;
} //Паша, тебе нужно написать расширенный алгоритм евклида для поиска обратного по модулю числа. 
//И еще нужно переделать все функции длинки, кроме умножения, чтобы они возвращали указатель на массив(сейчас они просто выводят) и чтобы этот массив был БЕЗ ВЕДУЩИХ НУЛЕЙ
//Посмотри на структуру функции умножения и переделай так же, мне кажется, это довольно удобно
