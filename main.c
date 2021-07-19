#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

short int* gen_pubkey(int *size_d) //рандомно заполняем массив-ключ
{
    srand(time(NULL));
    *size_d = rand()%10 + 3;
    short int *random_key = (short int*)calloc(*size_d, sizeof(short int));
    random_key[*size_d-1] = rand()%9+1;
    for(int i = 1; i<*size_d-2; i++) {
        random_key[i] = rand() % 10;
    }
    int fl = 0;
    while(fl==0) {
        random_key[0] = rand() % 9 + 1;
        if(random_key[0]%2!=0) fl = 1;
    }
    return random_key;
}

short int* umnojenie(short int *a, short int *b, int len1, int len2, int *length_of_result, int free_a, int free_b)
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

void printff(short int* a, int size) // вспомогательная функция для печати массива
{
    for(int i = size-1; i>=0; i--){
        printf("%d", a[i]);
    }
    printf("\n");
}

int return_number(FILE* stream)
{
    int number = 0;
    char symbol;
    while((symbol = getc(stream))!=(int)'\n'){
        number = number*10 + (symbol - 48);
    }
    return number;
}
char* int_to_char(int number)
{
    char* pointer;
    int help_number = number, size = 0, i = 0;
    while(help_number!=0){
        help_number/=10;
        size++;
    }
    pointer = (char*)calloc(size+1, sizeof(char));
    while(number!=0){
        pointer[size-1-i] = (char)(number%10 + 48);
        number/=10;
        i++;
    }
    return pointer;
}
void return_long_number(FILE* stream, short int *a, short int *b, int size)
{
    short int symbol, i = 0;
    while((symbol = getc(stream))!=(int)'*'){
        a[size-1-i] = symbol - 48;
        i++;
    } i = 0;
    while((symbol = getc(stream))!=(int)'\n'){
        b[size-1-i] = symbol - 48;
        i++;
    }
}
void random_down(int size, FILE* stream)
{
    short int symbol, k = 0;
    while(k!=size){
        symbol = getc(stream);
        if(symbol == (int)'\n') k++;
    }
}
void gen_q_and_p(int size, short int* p, short int* q, int* sizeof_p, int* sizeof_q)
{
    char* end_of_string = int_to_char(size);
    char name_of_file_q_p[20] = "key_";
    strcat(name_of_file_q_p, end_of_string);
    strcat(name_of_file_q_p, ".txt");
    FILE* base_of_q_p = NULL;
    if((base_of_q_p = fopen(name_of_file_q_p, "r"))==NULL)
    {
        printf("Error\n");
        return;
    }
    int sizeof_random = return_number(base_of_q_p);
    srand(time(NULL));
    int random_number = rand()%sizeof_random;
    int sizeof_q_p = return_number(base_of_q_p);
    q = (short int*)calloc(sizeof_q_p, sizeof(short int));
    p = (short int*)calloc(sizeof_q_p, sizeof(short int));
    random_down(random_number, base_of_q_p);
    return_long_number(base_of_q_p, p, q, sizeof_q_p);
    int i = 0;
    *sizeof_q = *sizeof_p = sizeof_q_p;
    while(q[i]==0){
        i++;
        *sizeof_q--;
    } i = 0;
    while(p[i]==0){
        i++;
        *sizeof_p--;
    }
    if(sizeof_q_p!=*sizeof_p){
        short int *vsp = (short int*)calloc(*sizeof_p, sizeof(short int));
        for(i = 0; i < *sizeof_p; i++) {
            vsp[*sizeof_p - 1 - i] = p[sizeof_q_p - 1 - i];
        }
        free(p);
        p = (short int*)calloc(*sizeof_p, sizeof(short int));
        for(i = 0; i < *sizeof_p; i++) {
            p[i] = vsp[i];
        }
        free(vsp);
    }
    if(sizeof_q_p!=*sizeof_q){
        short int *vsp = (short int*)calloc(*sizeof_q, sizeof(short int));
        for(i = 0; i < *sizeof_q; i++) {
            vsp[*sizeof_q - 1 - i] = q[sizeof_q_p - 1 - i];
        }
        free(q);
        q = (short int*)calloc(*sizeof_q, sizeof(short int));
        for(i = 0; i < *sizeof_q; i++) {
            q[i] = vsp[i];
        }
        free(vsp);
    }
}
void put_in_file(FILE *secret, FILE *public, short int *q,short int *p,short int *n,short int *d,short int *e,
                 int size_q, int size_p, int size_n, int size_d, int size_e)
{
    fprintf(secret, "p:");
    for(int i = size_p-1; i>=0; i--){
        fprintf(secret, "%d", p[i]);
    }
    fprintf(secret, "\n");
    fprintf(secret, "q:");
    for(int i = size_q-1; i>=0; i--){
        fprintf(secret, "%d", q[i]);
    }
    fprintf(secret, "\n");
    fprintf(secret, "e:");
    for(int i = size_e-1; i>=0; i--){
        fprintf(secret, "%d", e[i]);
    }
    fprintf(public, "n:");
    for(int i = size_n-1; i>=0; i--){
        fprintf(public, "%d", n[i]);
    }
    fprintf(public, "\n");
    fprintf(public, "d:");
    for(int i = size_d-1; i>=0; i--){
        fprintf(public, "%d", d[i]);
    }
}
int main() {
    int size_of_key = 256*4; // вводимая пользователем длина ключа
    char *nameof_public, *nameof_secret;
    FILE *secret_file = NULL, *public_file = NULL;
    secret_file = fopen(nameof_secret, "a+");
    public_file = fopen(nameof_public, "a+");
    fclose(secret_file);
    fclose(public_file);
    secret_file = fopen(nameof_secret, "w");
    public_file = fopen(nameof_public, "w");
    short int *q = NULL, *p = NULL, *n = NULL, *d = NULL, *e = NULL;
    int size_q = 0, size_p = 0, size_n = 0, size_d = 0, size_e = 0;
    int *pointer_q = &size_q, *pointer_p = &size_p, *pointer_d = &size_d, *pointer_e = &size_e, *pointer_n = &size_n;
    d = gen_pubkey(pointer_d);
    gen_q_and_p(size_of_key, q, p, pointer_q, pointer_p);
    n = umnojenie(q,p,size_q,size_p,pointer_n,0,0);
    //e = ... генерация ключа е - написать
    //записываем все ключи в файлы
    put_in_file(secret_file,public_file,q,p,n,d,e,size_q,size_p,size_n,size_d,size_e);
    fclose(secret_file);
    fclose(public_file);
    return 0;
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
