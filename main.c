#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
typedef struct long16_int{
    short *array;
    int size;
    int *size_ptr;
}long_int;
uint16_t CRC_16(short *input, unsigned int size)
{
    unsigned int counter = 0;
    uint16_t del = 0x8005; //полином
    uint16_t crc = 0;
    int8_t symbol, bit16, bit8;
    while (counter<size) {
        symbol = input[counter];
        for (int i = 0; i < 8; i++) {
            bit8 = bit16 = 0;
            if((symbol & 0x80)!=0) bit8 = 1;
            symbol <<= 1;
            if((crc & 0x8000) != 0) bit16 = 1;
            crc <<= 1;
            crc += bit8;
            if (bit16) crc ^= del;
        }
        counter++;
    }
    for (int i = 0; i < 16; i++) {
        bit16 = 0;
        if((crc & 0x8000) != 0) bit16 = 1;
        crc <<= 1;
        if (bit16) crc ^= del;
    }
    return crc;
}
short pow_16(short pow)
{
    short result = 1;
    for(int i = 0; i<pow; i++){
        result*=16;
    }
    return result;
}
void vvod(char *input)
{
    short count = 0, symbol;
    while((symbol = getc(stdin))!='\n'){
        input[count] = (char)symbol;
        count++;
    }
    input[count] = '\n';
}
void printff(short int* a, int size,FILE *stream) // вспомогательная функция для печати массива
{
    for(int i = size-1; i>=0; i--){
        if(a[i]<10) fprintf(stream,"%d", a[i]);
        if(a[i]==10) fprintf(stream,"A");
        if(a[i]==11) fprintf(stream,"B");
        if(a[i]==12) fprintf(stream,"C");
        if(a[i]==13) fprintf(stream,"D");
        if(a[i]==14) fprintf(stream,"E");
        if(a[i]==15) fprintf(stream,"F");
    }
    fprintf(stream,"\n");
}
short *slojenie(short *a, short *b,int size1, int size2, int *result_size_ptr, short free_a)
{
    int len, lenm, fl;
    if (size1 > size2) {
        len = size1 + 1;
        lenm = size2;
        fl = 1;
    }
    else {
        len = size2 + 1;
        lenm = size1;
        fl = 2;
    }
    short *xy = (short*)calloc(len,sizeof(short));
    for(int i = 0; i<len; i++) *(xy+i) = 0;
    for (int ix = 0; ix < lenm; ix++){
        *(xy+ix) = *(xy + ix) + *(a + ix) + *(b + ix);
        xy[ix + 1] = (xy[ix] / 16);
        xy[ix] = xy[ix] % 16;
    }
    for (int ix = lenm; ix<len-1; ix++){
        if(fl==1) xy[ix] += a[ix];
        if(fl==2) xy[ix] += b[ix];
        xy[ix + 1] = (xy[ix] / 16);
        xy[ix] = xy[ix] % 16;
    }
    if(xy[len-1]==0){
        while(xy[len-1]==0) len--;
        if(len==0) len = 1;
        short *vsp = (short*)calloc(len, sizeof(short int));
        memcpy(vsp,xy,len*sizeof(short));
        free(xy);
        xy = NULL;
        xy = (short*)calloc(len, sizeof(short));
        memcpy(xy,vsp,len*sizeof(short));
        //free(vsp);
    }
    if(free_a==1) {
        free(a);
        a = NULL;
    }
    if(result_size_ptr!=NULL) *result_size_ptr = len;
    return xy;
}
short int* umnojenie(short int *a, short int *b, int len1, int len2, int *length_of_result, int free_a, short **ptr_a)
{
    int length = len1 + len2 + 1;
    short int *xy = (short int*)calloc(length, sizeof(short int));
    for (int i = 0; i < len1; i++){
        for (int j = 0; j < len2; j++){
            xy[i + j] += a[i] * b[j];
        }
    }

    for (int i = 0; i < length-1; i++){
        xy[i + 1] += xy[i] / 16;
        xy[i] %= 16;
    }
    if(xy[length-1]==0){
        while(xy[length-1]==0&&length>1) length--;
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
        free(*ptr_a);
        a = NULL;
    }
    if(length_of_result!=NULL) *length_of_result = length;
    return xy;
}
short int comparison(short *a, short *b, int size_a, int size_b)//сравнение
{
    if (size_a > size_b) {
        return 1;
    } else if (size_b > size_a) {
        return 2;
    } else {
        for (int i = size_a - 1; i >= 0; i--){
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
void vic(short *a,short *b,short int *c, int lenmax, int lenm)//вспомогательная функция вычитания
{
    for (int ix = 0; ix < lenm; ix++){
        a[ix + 1]--;
        c[ix] += 16 + a[ix];
        c[ix] -= b[ix];
        if (c[ix] / 16 > 0){
            c[ix + 1]++;
            c[ix] %= 16;
        }
    }
    for(int ix = lenm; ix<lenmax; ix++){
        a[ix+1]--;
        c[ix]+=16 + a[ix];
        if(c[ix] / 16 > 0){
            c[ix + 1]++;
            c[ix]%=16;
        }
    }
}
short int* long_subtraction(short int* a, short int* b, int size_a, int size_b, int *size_of_result, short int free_a, short **ptr_a)//вычитание
{
    int length = size_a, lenmin=size_a;
    short int k = 3;
    if (size_a > size_b) {
        length = size_a;
        k = 1;
        lenmin = size_b;
    } else if (size_b > size_a) {
        length = size_b;
        k = 2;
        lenmin = size_a;
    } else if(size_a==size_b){
        for (int i = length - 1; i >= 0; i--){
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
    short *xy = (short*)calloc(length + 1, sizeof(int));
    short *a_copy = (short*)calloc(length + 1, sizeof(int));
    short *b_copy = (short*)calloc(length + 1, sizeof(int));
    memcpy(a_copy,a,size_a* sizeof(short));
    memcpy(b_copy,b,size_b* sizeof(short));
    if (k == 1) vic(a_copy, b_copy, xy, length, lenmin);
    if (k == 2) vic(b_copy, a_copy, xy, length, lenmin);
    if(xy[length-1]==0){
        while(xy[length-1]==0&&length>1) length--;
        short *vsp = (short*)calloc(length, sizeof(short int));
        memcpy(vsp,xy,length*sizeof(short));
        free(xy);
        xy = NULL;
        xy = (short*)calloc(length, sizeof(short));
        memcpy(xy,vsp,length*sizeof(short));
        free(vsp);
    }
    if(free_a==1) {
        free(a);
        a = NULL;
    }
    free(a_copy);
    free(b_copy);
    *size_of_result = length;
    return xy;
}
short int *module_func(short int *number, short *module, int size_number, int size_module, int *pointer_size_number, int free_number, short **ptr_number)
{
    short int *result = NULL, **ptr_result;
    int size_result = size_number, *pointer_size_result = &size_result;
    short int *module_copy = NULL;
    short int *stepen = NULL;
    int len_of_stepen = 0;
    int size_module_copy = 0, *p_size_m_copy = &size_module_copy;
    result = (short*)calloc(size_number, sizeof(short int));
    memcpy(result, number, size_number * sizeof(short int));
    if (comparison(result,module,size_result,size_module)==2){
        if(free_number==1){
            free(*ptr_number);
        }
        return result;
    }
    else{
        char flag = '\0';
        while(flag=='\0'){
            len_of_stepen = size_result-size_module;
            if(len_of_stepen!=0) {
                stepen = (short *) calloc(len_of_stepen, sizeof(short int));
                stepen[len_of_stepen - 1] = 1;
                if(module_copy!=NULL) {
                    free(module_copy);
                    module_copy = NULL;
                }
                module_copy = umnojenie(module, stepen, size_module, len_of_stepen,
                                        p_size_m_copy, 0, NULL);
            }
            if(stepen!=NULL) {
                free(stepen);
                stepen = NULL;
            }
            ptr_result = &result;
            if(len_of_stepen!=0) result = long_subtraction(result, module_copy, size_result, size_module_copy, pointer_size_result, 1,ptr_result);
            if(len_of_stepen==0) result = long_subtraction(result, module, size_result, size_module,pointer_size_result,1,ptr_result);
            if (comparison(result,module,size_result,size_module)==2){
                flag = '1';
            }
        }
        if(free_number==1){
            free(*ptr_number);
        }
        *pointer_size_number = size_result;
        free(module_copy);
        return result;
    }
}
short *simple_euclid(short *a, short *b, int size_a, int size_b, int *result_size_ptr)
{
    int size_a_copy = size_a, size_b_copy = size_b, *size_a_copy_prt = &size_a_copy, *size_b_copy_prt = &size_b_copy;
    short *a_copy = (short *)calloc(size_a_copy, sizeof(short)), *b_copy = (short *)calloc(size_b_copy, sizeof(short)),
    **ptr_a_copy = &a_copy, **ptr_b_copy = &b_copy;
    memcpy(a_copy,a,sizeof(short)*size_a_copy);
    memcpy(b_copy,b,sizeof(short)*size_b_copy);
    while((!(a_copy[0]==0&&size_a_copy==1))&&(!(b_copy[0]==0&&size_b_copy==1))){
        if(comparison(a_copy,b_copy,size_a_copy,size_b_copy)==1){
            a_copy = module_func(a_copy,b_copy,size_a_copy,size_b_copy,size_a_copy_prt,1,ptr_a_copy);
            ptr_a_copy = &a_copy;
        }
        else b_copy = module_func(b_copy,a_copy,size_b_copy,size_a_copy,size_b_copy_prt,1,ptr_b_copy);
        ptr_b_copy = &b_copy;
    }
    return slojenie(a_copy,b_copy,size_a_copy,size_b_copy,result_size_ptr,0);
}
short int *module_pow(short int *number, short int *pow, short *module, int size_number, int size_pow, int size_module, int* size_of_result)
{
    short *result = NULL, fl_for_equality = 0;
    short *counter = (short*)calloc(size_pow, sizeof(short));
    int current_size = size_number, *pointer_current_size = &current_size;
    counter[0] = 1;
    result = (short int*)calloc(current_size, sizeof(short int));
    short **ptr_result = &result;
    for(int i = 0; i<current_size; i++) result[i] = number[i];
    while(fl_for_equality==0){
        result = umnojenie(result,number,current_size,size_number,pointer_current_size,1,ptr_result);
        ptr_result = &result;
        result = module_func(result,module,current_size,size_module,pointer_current_size,1,ptr_result);
        ptr_result = &result;
        counter[0]++;
        for (int i = 0; i < size_pow-1; i++){
            counter[i+1] = counter[i]/16 + counter[i+1];
            counter[i] = counter[i]%16;
        }
       // printff(counter,size_pow,stdout);
        if(comparison(counter, pow, size_pow, size_pow)==0)
            fl_for_equality = 1;
    }
    free(counter);
    *size_of_result = current_size;
    return result;
}
int return_number_10(FILE* stream)
{
    int number = 0;
    char symbol;
    while((symbol = getc(stream))!=(int)'\n'&&symbol!=(int)'\r'){
        number = number*10 + (symbol - 48);
    }
    return number;
}
char* int_to_char_genkey_size(int number)
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
void return_long_number_16(FILE* stream, short int *a, int size_a, char stop)
{
    short int symbol, i = 0;
    while((symbol = getc(stream))!=(int)stop){
        if(symbol<58) a[size_a-1-i] = symbol - 48;
        else a[size_a-1-i] = symbol - 55;
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
void get_p_and_g(int size, short int** p, short int** g, int* sizeof_p, int* sizeof_g)
{
    char* end_of_string = int_to_char_genkey_size(size);
    char name_of_file_p_g[20] = "egkey_";
    strcat(name_of_file_p_g, end_of_string);
    strcat(name_of_file_p_g, ".txt");
    FILE* base_of_p_g = NULL;
    base_of_p_g = fopen(name_of_file_p_g, "r");
    int sizeof_random = return_number_10(base_of_p_g);
    srand(time(NULL));
    int random_number = rand()%sizeof_random;
    int sizeof_p_2 = return_number_10(base_of_p_g);
    int sizeof_g_2 = return_number_10(base_of_p_g);
    *p = (short int*)calloc(sizeof_p_2, sizeof(short int));
    *g = (short int*)calloc(sizeof_g_2, sizeof(short int));
    random_down(random_number,base_of_p_g);
    return_long_number_16(base_of_p_g,*p,sizeof_p_2,';');
    return_long_number_16(base_of_p_g,*g,sizeof_g_2,'\n');
    int i = 0;
    *sizeof_p = sizeof_p_2;
    *sizeof_g = sizeof_g_2;
    while(*g[i]==0){
        i++;
        *sizeof_g--;
    } i = 0;
    while(*p[i]==0){
        i++;
        *sizeof_p--;
    }
    if(sizeof_p_2!=*sizeof_p){
        short int *vsp = (short int*)calloc(*sizeof_p, sizeof(short int));
        for(i = 0; i < *sizeof_p; i++) {
            vsp[*sizeof_p - 1 - i] = *p[sizeof_p_2 - 1 - i];
        }
        free(*p);
        *p = (short int*)calloc(*sizeof_p, sizeof(short int));
        for(i = 0; i < *sizeof_p; i++) {
            *p[i] = vsp[i];
        }
        free(vsp);
    }
    if(sizeof_g_2!=*sizeof_g){
        short int *vsp = (short int*)calloc(*sizeof_g, sizeof(short int));
        for(i = 0; i < *sizeof_g; i++) {
            vsp[*sizeof_g - 1 - i] = *g[sizeof_g_2 - 1 - i];
        }
        free(*g);
        *g = (short int*)calloc(*sizeof_g, sizeof(short int));
        for(i = 0; i < *sizeof_g; i++) {
            *g[i] = vsp[i];
        }
        free(vsp);
    }
}
void gen_q_and_p(int size, short int** p, short int** q, int* sizeof_p, int* sizeof_q)
{
    char* end_of_string = int_to_char_genkey_size(size);
    char name_of_file_q_p[20] = "key_";
    strcat(name_of_file_q_p, end_of_string);
    strcat(name_of_file_q_p, ".txt");
    FILE* base_of_q_p = NULL;
    base_of_q_p = fopen(name_of_file_q_p, "r");
    if(base_of_q_p==NULL)
    {
        printf("Error\n");
        return;
    }
    int sizeof_random = return_number_10(base_of_q_p);
    srand(time(NULL));
    int random_number = rand()%sizeof_random;
    int sizeof_q_p = return_number_10(base_of_q_p);
    *q = (short int*)calloc(sizeof_q_p, sizeof(short int));
    *p = (short int*)calloc(sizeof_q_p, sizeof(short int));
    random_down(random_number, base_of_q_p);
    return_long_number_16(base_of_q_p, *p, sizeof_q_p, '*');
    return_long_number_16(base_of_q_p, *q, sizeof_q_p, '\n');
    int i = 0;
    *sizeof_q = *sizeof_p = sizeof_q_p;
    while(*q[i]==0){
        i++;
        *sizeof_q--;
    } i = 0;
    while(*p[i]==0){
        i++;
        *sizeof_p--;
    }
    if(sizeof_q_p!=*sizeof_p){
        short int *vsp = (short int*)calloc(*sizeof_p, sizeof(short int));
        for(i = 0; i < *sizeof_p; i++) {
            vsp[*sizeof_p - 1 - i] = *p[sizeof_q_p - 1 - i];
        }
        free(*p);
        *p = (short int*)calloc(*sizeof_p, sizeof(short int));
        for(i = 0; i < *sizeof_p; i++) {
            *p[i] = vsp[i];
        }
        free(vsp);
    }
    if(sizeof_q_p!=*sizeof_q){
        short int *vsp = (short int*)calloc(*sizeof_q, sizeof(short int));
        for(i = 0; i < *sizeof_q; i++) {
            vsp[*sizeof_q - 1 - i] = *q[sizeof_q_p - 1 - i];
        }
        free(*q);
        *q = (short int*)calloc(*sizeof_q, sizeof(short int));
        for(i = 0; i < *sizeof_q; i++) {
            *q[i] = vsp[i];
        }
        free(vsp);
    }
}
short int* gen_pubkey(int *size_d) //рандомно заполняем массив-ключ
{
    srand(time(NULL));
    *size_d = rand()%3 + 3;
    short int *random_key = (short int*)calloc(*size_d, sizeof(short int));
    random_key[*size_d-1] = rand()%9+1;
    for(int i = 1; i<*size_d-2; i++) {
        random_key[i] = rand() % 16;
    }
    int fl = 0;
    while(fl==0) {
        random_key[0] = rand() % 15 + 1;
        if(random_key[0]%2!=0) fl = 1;
    }
    return random_key;
}
void get_secret_key(char *name_of_file, short **d, short **q, short **p,
                     int *size_d, int *size_q, int *size_p)
{
    FILE* file = fopen(name_of_file, "r");
    char tt[50];
    int symbol, counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        tt[*size_p] = (char)symbol;
        ++*size_p;
    }
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        ++*size_q;
    }
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        ++*size_d;
    }
    fclose(file);
    *p = (short*)calloc(*size_p, sizeof(short));
    *q = (short*)calloc(*size_q, sizeof(short));
    *d = (short*)calloc(*size_d, sizeof(short));
    file = fopen(name_of_file, "r");
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        if(symbol<58) *(*p + *size_p - counter - 1) = symbol - 48;
        else *(*p + *size_p - counter - 1) = symbol - 55;
        counter++;
    }
    counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        if(symbol<58) *(*q + *size_q - counter - 1) = symbol - 48;
        else *(*q + *size_q - counter - 1) = symbol - 55;
        counter++;
    }
    counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        if(symbol<58) *(*d + *size_d - counter - 1) = symbol - 48;
        else *(*d + *size_d - counter - 1) = symbol - 55;
        counter++;
    }
    fclose(file);
}
void rsa_genkey(int size_of_key, char *nameof_public, char *nameof_secret)
{
    FILE *secret_file = NULL, *public_file = NULL;
    secret_file = fopen(nameof_secret, "w");
    public_file = fopen(nameof_public, "w");
    short int *q = NULL, *p = NULL, *n = NULL, *d = NULL, *e = NULL, **q_ptr = &q, **p_ptr = &p;
    int size_q = 0, size_p = 0, size_n = 0, size_d = 0, size_e = 0;
    int *pointer_q = &size_q, *pointer_p = &size_p, *pointer_d = &size_d, *pointer_e = &size_e, *pointer_n = &size_n;
    gen_q_and_p(size_of_key, q_ptr, p_ptr, pointer_q, pointer_p);
    short _1[1] = {1};
    short *p_1 = NULL,*q_1 = NULL;
    int size_p_1 = 0, *p_1_size_ptr = &size_p_1,size_q_1 = 0, *q_1_size_ptr = &size_q_1;
    p_1 = long_subtraction(p,_1,size_p,1,p_1_size_ptr,0,NULL);
    q_1 = long_subtraction(q,_1,size_q,1,q_1_size_ptr,0,NULL);
    short *pq = NULL;
    int size_pq = 0, *pq_size_ptr = &size_pq;
    pq = umnojenie(p_1,q_1,size_p_1,size_q_1,pq_size_ptr,0,NULL);
    short *eq = NULL;
    int size_eq = 0, *eq_size_ptr = &size_eq;
    pq = umnojenie(p_1,q_1,size_p_1,size_q_1,pq_size_ptr,0,NULL);
    short fl_for_exit = 0;
    while(fl_for_exit==0){
        e = gen_pubkey(pointer_e);
        eq = simple_euclid(e,pq,size_e,size_pq,eq_size_ptr);
        if(eq[0]==1&&size_eq==1) fl_for_exit=1;
        else{
            free(e);
            e = NULL;
        }
    }
    n = umnojenie(q,p,size_q,size_p,pointer_n,0,0);
    //d = ... генерация ключа - секретный ключ
    //записываем все ключи в файлы
    fprintf(public_file,"n:");
    printff(n,size_n,public_file);
    fprintf(public_file,"e:");
    printff(e,size_e,public_file);
    fprintf(secret_file,"p:");
    printff(p,size_p,secret_file);
    fprintf(secret_file,"q:");
    printff(q,size_q,secret_file);
    /*fprintf(secret_file,"d:");
    printff(d,size_d,secret_file);*/
    fclose(secret_file);
    fclose(public_file);
    free(e);free(q);free(p);free(pq);free(n);free(eq);//free(d);
}
void get_public_key(char *name_of_file, short **e, short **n, int *size_e, int *size_n)
{
    FILE* file = fopen(name_of_file, "r");
    int symbol, counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        ++*size_n;
    }
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        ++*size_e;
    }
    fclose(file);
    *n = (short*)calloc(*size_n, sizeof(short));
    *e = (short*)calloc(*size_e, sizeof(short));
    file = fopen(name_of_file, "r");
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        if(symbol<58) *(*n + *size_n - counter - 1) = symbol - 48;
        else *(*n + *size_n - counter - 1) = symbol - 55;
        counter++;
    }
    counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'&&symbol!=(int)'\r'){
        if(symbol<58) *(*e + *size_e - counter - 1) = symbol - 48;
        else *(*e + *size_e - counter - 1) = symbol - 55;
        counter++;
    }
    fclose(file);
}
short *byte_symbols_to_array(short *byte_array, int size)
{
    int size_of_array = size*2;
    short *int_array = (short*)calloc(size_of_array, sizeof(short));
    for(int i = 0, j = size_of_array-1; i<size; i++){
        int current_symbol = byte_array[i];
        for(short k = 1; k>=0; k--){
            int_array[j] = current_symbol / pow_16(k);
            current_symbol%= pow_16(k);
            j--;
        }
    }
    if(int_array[size_of_array-1]==0){
        while(int_array[size_of_array-1]==0) size_of_array--;
        short int* vsp = (short int*)calloc(size_of_array, sizeof(short int));
        for(int i = 0; i<size_of_array; i++){
            vsp[i] = int_array[i];
        }
        free(int_array);
        int_array = NULL;
        int_array = (short int*)calloc(size_of_array, sizeof(short int));
        for(int i = 0; i<size_of_array; i++){
            int_array[i] = vsp[i];
        }
    }
    return int_array;
}
short *array_to_byte_symbols(short *array, int size)
{
    short *output_array = NULL;
    int size_output = size/2;
    output_array = (short*)calloc(size_output, sizeof(short));
    for(int i = 0, j = 0;i<size_output;i++){
        for(short k = 0; k<2; k++) {
            output_array[i] = output_array[i] + array[j] * pow_16(k);
            j++;
            if ((k == 1) || (j == size)) break;
        }
    }
    return output_array;
}
void rsa_encrypt(char *name_of_pubkey, char *name_of_file, char *name_of_encrypt_file)
{
    FILE* input = fopen(name_of_file, "r");
    int symbol, k_symbols = 0, size_of_input;
    while((symbol = getc(input))!=EOF){
        k_symbols++;
    }
    fclose(input);
    short *char_symbols = (short*)calloc(k_symbols, sizeof(short)), counter = 0;
    input = fopen(name_of_file, "r");
    while((symbol = getc(input))!=EOF){
        char_symbols[counter] = symbol;
        counter++;
    }
    fclose(input);
    size_of_input = k_symbols*2;
    short *input_mass = byte_symbols_to_array(char_symbols,k_symbols);
    FILE* output = NULL;
    output = fopen(name_of_encrypt_file, "w");
    short *encrypting = NULL;
    int encrypting_size = 0, *encrypting_size_pointer = &encrypting_size;
    short *n = NULL, *e = NULL, **p_e = &e, **p_n = &n;
    int size_n = 0, size_e = 0, *pointer_size_n = &size_n, *pointer_size_e = &size_e;
    get_public_key(name_of_pubkey, p_e, p_n, pointer_size_e, pointer_size_n);
    encrypting = module_pow(input_mass,e,n,size_of_input,size_e,size_n,encrypting_size_pointer);
    short *encrypting_char = array_to_byte_symbols(encrypting,encrypting_size);
    for(int i = encrypting_size/2-1; i>=0; i--){
        fprintf(output, "%c", encrypting_char[i]);
    }
    fclose(output);
    free(n);free(e);free(input_mass);free(encrypting_char);free(char_symbols);free(encrypting);
}
void rsa_decrypt(char *name_of_secret, char *name_of_infile, char *name_of_outfile)
{
    FILE *infile = fopen(name_of_infile, "r");
    short *infile_char, size_infile_char = 0;
    int symbol = 0;
    while((symbol = getc(infile))!=EOF){
        size_infile_char++;
    }
    fclose(infile);
    int counter = 0;
    infile = fopen(name_of_infile, "r");
    infile_char = (short*)calloc(size_infile_char, sizeof(short));
    while((symbol = getc(infile))!=EOF){
        infile_char[size_infile_char-1-counter] = symbol;
        counter++;
    }
    fclose(infile);
    int size_of_infile_int = size_infile_char*2;
    short *infile_int = byte_symbols_to_array(infile_char,size_infile_char);
    counter = 0;
    short *decrypting = NULL;
    int decrypting_size = 0, *decrypting_size_pointer = &decrypting_size;
    short *p = NULL, *q = NULL, *d = NULL, **p_p = &p, **p_q = &q, **p_d = &d;
    int size_p = 0, size_q = 0, size_d = 0, *pointer_size_p = &size_p, *pointer_size_q = &size_q, *pointer_size_d = &size_d;
    get_secret_key(name_of_secret,p_d,p_q,p_p,pointer_size_d,pointer_size_q,pointer_size_p);
    short *n = NULL; int size_n = 0, *pointer_size_n = &size_n;
    n = umnojenie(q,p,size_q,size_p,pointer_size_n,0,0);
    decrypting = module_pow(infile_int,d,n,size_of_infile_int,size_d,size_n,decrypting_size_pointer);
    FILE* output = NULL;
    short *char_output = array_to_byte_symbols(decrypting,decrypting_size);
    output = fopen(name_of_outfile, "w");
    for(int i = decrypting_size/2-1; i>=0; i--){
        fprintf(output, "%c", char_output[i]);
    }
    fclose(output);
    free(n);free(p);free(q);free(d);free(infile_char);free(decrypting);free(char_output);
}
void rsa_sign(char *name_of_secret, char *name_of_infile, char *name_of_signfile)
{
    FILE *infile = fopen(name_of_infile, "r");
    short *infile_char, size_infile_char = 0;
    int symbol = 0;
    while((symbol = getc(infile))!=EOF){
        size_infile_char++;
    }
    fclose(infile);
    int counter = 0;
    infile = fopen(name_of_infile, "r");
    infile_char = (short*)calloc(size_infile_char, sizeof(short));
    while((symbol = getc(infile))!=EOF){
        infile_char[counter] = symbol;
        counter++;
    }
    fclose(infile);
    uint16_t crc = CRC_16(infile_char,size_infile_char);
    uint16_t crc_copy = crc;
    short crc_size = 4;
    short *crc_array = (short*)calloc(crc_size, sizeof(short));
    for(counter = 0; counter<crc_size; counter++){
        crc_array[counter] = crc_copy%16;
        crc_copy/=16;
    }
    counter = 0;
    short *p = NULL, *q = NULL, *d = NULL, **p_p = &p, **p_q = &q, **p_d = &d;
    int size_p = 0, size_q = 0, size_d = 0, *pointer_size_p = &size_p, *pointer_size_q = &size_q, *pointer_size_d = &size_d;
    get_secret_key(name_of_secret,p_d,p_q,p_p,pointer_size_d,pointer_size_q,pointer_size_p);
    short *n = NULL; int size_n = 0, *pointer_size_n = &size_n;
    n = umnojenie(q,p,size_q,size_p,pointer_size_n,0,0);
    int sign_size = 0, *sign_size_pointer = &sign_size;
    short *sign = NULL;
    sign = module_pow(crc_array,d,n,crc_size,size_d,size_n,sign_size_pointer);
    FILE* output = NULL;
    short *char_output = array_to_byte_symbols(sign,sign_size);
    output = fopen(name_of_signfile, "w");
    for(int i = sign_size/2-1; i>=0; i--){
        fprintf(output, "%c", char_output[i]);
    }
    fclose(output);
    free(n);free(char_output);free(sign);free(d);free(p);free(q);free(infile_char);
}
void rsa_check(char *name_of_pubkey, char *name_of_infile, char *name_of_signfile)
{
    FILE *infile = fopen(name_of_infile, "r");
    short *infile_char, size_infile_char = 0;
    int symbol = 0;
    while((symbol = getc(infile))!=EOF){
        size_infile_char++;
    }
    fclose(infile);
    int counter = 0;
    infile = fopen(name_of_infile, "r");
    infile_char = (short*)calloc(size_infile_char, sizeof(short));
    while((symbol = getc(infile))!=EOF){
        infile_char[counter] = symbol;
        counter++;
    }
    fclose(infile);
    uint16_t crc = CRC_16(infile_char,size_infile_char);
    uint16_t crc_copy = crc;
    short crc_size = 4;
    short *crc_array = (short*)calloc(crc_size, sizeof(short));
    for(counter = 0; counter<crc_size; counter++){
        crc_array[counter] = crc_copy%16;
        crc_copy/=16;
    }
    counter = 0;
    short *check = NULL;
    int check_size = 0, *check_size_pointer = &check_size;
    short *n = NULL, *e = NULL, **p_e = &e, **p_n = &n;
    int size_n = 0, size_e = 0, *pointer_size_n = &size_n, *pointer_size_e = &size_e;
    get_public_key(name_of_pubkey, p_e, p_n, pointer_size_e, pointer_size_n);
    check = module_pow(crc_array,e,n,crc_size,size_e,size_n,check_size_pointer);
    short *check_char = array_to_byte_symbols(check,check_size);
    int check_char_size = check_size/2;
    FILE *signfile = NULL;
    signfile = fopen(name_of_signfile, "r");
    short *sign_char, size_sign_char = 0;
    symbol = 0;
    while((symbol = getc(signfile))!=EOF){
        size_sign_char++;
    }
    fclose(signfile);
    counter = 0;
    signfile = fopen(name_of_signfile, "r");
    sign_char = (short*)calloc(size_sign_char, sizeof(short));
    while((symbol = getc(signfile))!=EOF){
        sign_char[counter] = symbol;
        counter++;
    }
    fclose(signfile);
    if(comparison(check_char,sign_char,check_char_size,size_sign_char)==0) printf("Signature is correct\n");
    else printf("Signature is incorrect\n");
    free(check);free(sign_char);free(n);free(e);free(crc_array);free(infile_char);
}
void eg_genkey(int size_of_key, char *nameof_public, char *nameof_secret)
{
    FILE *secret_file = NULL, *public_file = NULL;
    secret_file = fopen(nameof_secret, "w");
    public_file = fopen(nameof_public, "w");
    short *p = NULL, *g = NULL, *y = NULL, *x = NULL, **p_ptr = &p, **g_ptr = &g;
    int size_p = 0, size_g = 0, size_y = 0, size_x = 0;
    int *p_size_ptr = &size_p, *g_size_ptr = &size_g, *y_size_ptr = &size_y;
    get_p_and_g(size_of_key,p_ptr,g_ptr,p_size_ptr,g_size_ptr);
    //size_x = rand()%(size_p/2)+4;
    size_x = 2;
    x = (short *) calloc(size_x,sizeof(short));
    for(int i = 0; i<size_x;i++){
        if(i==size_x-1) x[i] = rand()%15 + 1;
        else x[i] = rand()%16;
    }
    y = module_pow(g,x,p,size_g,size_x,size_p,y_size_ptr);
    fprintf(public_file,"y:");
    printff(y,size_y,public_file);
    fprintf(public_file,"g:");
    printff(g,size_g,public_file);
    fprintf(public_file,"p:");
    printff(p,size_p,public_file);
    fprintf(secret_file,"x:");
    printff(x,size_x,secret_file);
    fprintf(secret_file,"p:");
    printff(p,size_p,secret_file);
    fclose(public_file);
    fclose(secret_file);
    free(x);free(p);free(g);free(y);
}
void eg_encrypt(char *name_of_pubkey, char *name_of_infile, char *name_of_outfile)
{
    FILE* input = fopen(name_of_infile, "r");
    int symbol, k_symbols = 0, size_of_input;
    while((symbol = getc(input))!=EOF){
        k_symbols++;
    }
    fclose(input);
    short *char_symbols = (short*)calloc(k_symbols, sizeof(short)), counter = 0;
    input = fopen(name_of_infile, "r");
    while((symbol = getc(input))!=EOF){
        char_symbols[counter] = symbol;
        counter++;
    }
    fclose(input);
    size_of_input = k_symbols*2;
    short *input_mass = byte_symbols_to_array(char_symbols,k_symbols);
    counter = 0;
    short *y = NULL, *g = NULL, *p = NULL, **p_p = &p, **p_y = &y, **p_g = &g;
    int size_y = 0, size_g = 0, size_p = 0, *pointer_size_p = &size_p, *pointer_size_y = &size_y, *pointer_size_g = &size_g;
    get_secret_key(name_of_pubkey,p_p,p_g,p_y,pointer_size_p,pointer_size_g,pointer_size_y);//получаем открытый ключ с помощью ранее написанной функции
    short *k = NULL; int size_k = 0, *pointer_size_k = &size_k;
    srand(time(NULL));
    size_k = rand()%2 + 3;
    short _1[1] = {1};
    short *p_1 = NULL;
    int size_p_1 = 0, *p_1_size_ptr = &size_p_1;
    p_1 = long_subtraction(p,_1,size_p,1,p_1_size_ptr,0,NULL);
    k = (short*)calloc(size_k,sizeof(short));
    k[0] = 1;
    k[size_k-1] = rand()%5+1;
    short *vsp_euq = NULL; int vsp_euq_size, *vsp_euq_size_ptr = &vsp_euq_size, summ = 0;
    short fl_end = 0, fl_size_out = 0;
    while(fl_end==0&&fl_size_out==0){
        vsp_euq = simple_euclid(k,p_1,size_k,size_p_1,vsp_euq_size_ptr);
        if(vsp_euq_size==1&&vsp_euq[0]==1) fl_end = 1;
        else{
            k[0]+=2;
            for (int i = 0; i < vsp_euq_size-1; i++){
                k[i+1] = k[i]/16 + k[i+1];
                k[i] = k[i]%16;
            }
            for(int i = 0; i<vsp_euq_size; i++)
                summ+=vsp_euq[i];
            if(summ%15==vsp_euq_size) fl_size_out = 1;
            summ = 0;
        }
        free(vsp_euq);
    }
    short *a = NULL;
    int a_size = 0, *a_size_pointer = &a_size;
    a = module_pow(g,k,p,size_g,size_k,size_p,a_size_pointer);
    short *b = NULL;
    int b_size = 0, *b_size_pointer = &b_size;
    b = module_pow(y,k,p,size_y,size_k,size_p,b_size_pointer);
    b = umnojenie(input_mass,b,size_of_input,b_size,b_size_pointer,0,NULL);
    b = module_func(b,p,b_size,size_p,b_size_pointer,0,NULL);
    FILE *output = fopen(name_of_outfile, "w");
    for(int i = a_size-1; i>=0; i--){
        if(a[i]<10) fprintf(output,"%d", a[i]);
        if(a[i]==10) fprintf(output,"A");
        if(a[i]==11) fprintf(output,"B");
        if(a[i]==12) fprintf(output,"C");
        if(a[i]==13) fprintf(output,"D");
        if(a[i]==14) fprintf(output,"E");
        if(a[i]==15) fprintf(output,"F");
    }
    fprintf(output,"\n");
    for(int i = b_size-1; i>=0; i--){
        if(b[i]<10) fprintf(output,"%d", b[i]);
        if(b[i]==10) fprintf(output,"A");
        if(b[i]==11) fprintf(output,"B");
        if(b[i]==12) fprintf(output,"C");
        if(b[i]==13) fprintf(output,"D");
        if(b[i]==14) fprintf(output,"E");
        if(b[i]==15) fprintf(output,"F");
    }
    free(p_1);free(y);free(p);free(char_symbols);free(k);free(input_mass);free(g);free(b);
    fclose(output);
}
void eg_decrypt(char *name_of_secret, char *name_of_infile, char *name_of_outfile)
{
    FILE *input = fopen(name_of_infile, "r");
    int size_a = 0, size_b = 0, symbol = 0;
    short *a = NULL, *b = NULL;
    while((symbol = getc(input))!=(int)'\n'){
        size_a++;
    }
    while((symbol = getc(input))!=EOF){
        size_b++;
    }
    fclose(input);
    a = (short*)calloc(size_a, sizeof(short));
    b = (short*)calloc(size_b, sizeof(short));
    int counter = 0;
    while((symbol = getc(input))!=(int)'\n'){
        if(symbol<58) a[size_a - counter - 1] = symbol - 48;
        else a[size_a - counter - 1] = symbol - 55;
        counter++;
    }
    counter = 0;
    while((symbol = getc(input))!=(int)'\n'){
        if(symbol<58) b[size_b - counter - 1] = symbol - 48;
        else b[size_b - counter - 1] = symbol - 55;
        counter++;
    }
    fclose(input);
    short *p = NULL, *x = NULL, **p_p = &p, **p_x = &x;
    int size_p = 0, size_x = 0, *pointer_size_p = &size_p, *pointer_size_x = &size_x;
    get_public_key(name_of_secret, p_p, p_x, pointer_size_p, pointer_size_p); //в secret записывается p и x. сначала x, затем p
    short *decrypting = NULL;
    int decrypting_size = 0, *decrypting_size_pointer = &decrypting_size;
    short _1[1] = {1};
    short *pow = NULL; int pow_size = 0, *pow_size_ptr = &pow_size;
    pow = long_subtraction(p,_1,size_p,1,pow_size_ptr,0,NULL);
    pow = long_subtraction(pow,x,pow_size,size_x,pow_size_ptr,1,NULL);
    decrypting = module_pow(a,pow,p,size_a,pow_size,size_p,decrypting_size_pointer);
    decrypting = umnojenie(decrypting,b,decrypting_size,size_b,decrypting_size_pointer,0,0);
    decrypting = module_func(decrypting,p,decrypting_size,size_p,decrypting_size_pointer,0,NULL);
    FILE* output = NULL;
    short *char_output = array_to_byte_symbols(decrypting,decrypting_size);
    output = fopen(name_of_outfile, "w");
    for(int i = decrypting_size/2-1; i>=0; i--){
        fprintf(output, "%c", char_output[i]);
    }
    fclose(output);
    free(pow);free(p);free(x);free(a);free(b);free(char_output);free(decrypting);
}
int main() {
    char fl_for_error = 0, fl_for_exit = 0, counter_for_error = 0;
    char com_encrypt[] = "crypt encrypt";
    char com_decrypt[] = "crypt decrypt";
    char com_genkey[] = "crypt genkey";
    char com_help[] = "crypt --help\n";
    char com_check[] = "crypt check";
    char com_sign[] = "crypt sign";
    char com_exit[] = "exit\n";
    char com_eg[] = "crypt eg\n";
    char com_rsa[] = "crypt rsa\n";
    short fl_rsa = 1, fl_eg = 0;
    char main_string[500] = {'\0'};
    while (fl_for_exit==0){
        vvod(main_string);
        if(strncmp(main_string, com_help,13)==0){
            int symbol = 0;
            FILE *help = fopen("help.txt", "r");
            while((symbol = getc(help))!=EOF){
                printf("%c",symbol);
            }
            fclose(help);
        }
        else counter_for_error++;
        if(strncmp(main_string,com_rsa,10)==0) {
            if(fl_rsa!=1) printf("Successfuly switched to RSA algorythm.\n");
            else printf("RSA is already on.\n");
            fl_rsa = 1;
            fl_eg = 0;
        } else counter_for_error++;
        if(strncmp(main_string,com_eg,9)==0) {
            if(fl_eg!=1) printf("Successfuly switched to El-Gamal algorythm.\n");
            else printf("El-Gamal is already on.\n");
            fl_rsa = 0;
            fl_eg = 1;
        } else counter_for_error++;
        if(strstr(main_string, com_genkey)!=NULL){
            char *char_size = "--size";
            char *char_pubkey = "--pubkey";
            char *char_secret = "--secret";
            char *name_pubkey;
            char *name_secret;
            int size = 0;
            char *start = strstr(main_string, char_size);
            short count = 0, count_file = 0;
            if(start==NULL){
                if(fl_for_error==0) printf("Error, please try again\n");
                fl_for_error = 1;
            }
            else {
                start += (strlen(char_size));
                count = 0, count_file = 0;
                while((*(start + count)>'9')||(*(start + count)<'0'))
                    count++;
                while((*(start + count)<='9')&&(*(start + count)>='0')){
                    size = size*10 + (int)(*(start + count)) - 48;
                    count++;
                }
                if(*(start + count)!=' '||size%256!=0){
                    printf("Wrong size, please try again\n");
                    fl_for_error = 1;
                }
            }
            start = strstr(main_string, char_secret);
            if(start==NULL){
                if(fl_for_error==0) printf("Error, please try again\n");
                fl_for_error = 1;
            }
            else {
                start += (strlen(char_secret));
                count = 0, count_file = 0;
                while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                start+=count; count = 0;
                while(*(start+count)!=' ') count++;
                name_secret = (char*)calloc(count+1,sizeof(char));
                count = 0;
                while(*(start+count)!=' '&&*(start+count)!='\n'){
                    name_secret[count_file] = *(start+count);
                    count_file++;
                    count++;
                }
                FILE* secret = NULL;
                secret = fopen(name_secret,"r");
                if (secret==NULL) {
                    printf("Error with file %s, please try again\n", name_secret);
                    fl_for_error = 1;
                } else fclose(secret);
            }
            start = strstr(main_string, char_pubkey);
            if(start==NULL){
                if(fl_for_error==0) printf("Error, please try again\n");
                fl_for_error = 1;
            }
            else {
                start += (strlen(char_pubkey));
                count = 0, count_file = 0;
                while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                start+=count; count = 0;
                while(*(start+count)!=' '&&*(start+count)!='\n') count++;
                name_pubkey = (char*)calloc(count+1,sizeof(char));
                count = 0;
                while(*(start+count)!=' '&&*(start+count)!='\n'){
                    name_pubkey[count_file] = *(start+count);
                    count_file++;
                    count++;
                }
                FILE* pubkey = NULL;
                pubkey = fopen(name_pubkey,"r");
                if (pubkey==NULL) {
                    printf("Error with file %s, please try again\n", name_pubkey);
                    fl_for_error = 1;
                } else fclose(pubkey);
            }
            if (fl_for_error==0){
                if(fl_rsa==1) rsa_genkey(size,name_pubkey,name_secret);
                if(fl_eg==1) eg_genkey(size,name_pubkey,name_secret);
                printf("Success!\n");
            } else fl_for_error = 0;
        }
        else counter_for_error++;
        if(strstr(main_string, com_encrypt)!=NULL){
            char *char_infile = "--infile";
            char *char_outfile = "--outfile";
            char *char_pubkey = "--pubkey";
            char *name_infile;
            char *name_pubkey;
            char *name_outfile;
            char *start = strstr(main_string, char_infile);
            short count = 0, count_file = 0;
            if(start==NULL){
                if(fl_for_error==0) printf("Error, please try again\n");
                fl_for_error = 1;
            }
            else {start += (strlen(char_infile));
                count = 0, count_file = 0;
                while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                start+=count; count = 0;
                while(*(start+count)!=' ') count++;
                name_infile = (char*)calloc(count+1,sizeof(char));
                count = 0;
                while(*(start+count)!=' '){
                    name_infile[count_file] = *(start+count);
                    count_file++;
                    count++;
                }
                FILE* infile = NULL;
                infile = fopen(name_infile,"r");
                if (infile==NULL) {
                    printf("Error with file %s, please try again\n", name_infile);
                    fl_for_error = 1;
                } else fclose(infile);
                //
                start = strstr(main_string, char_pubkey);
                if(start==NULL){
                    if(fl_for_error==0) printf("Error, please try again\n");
                    fl_for_error = 1;
                }
                else {
                    start += (strlen(char_pubkey));
                    count = 0, count_file = 0;
                    while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                    &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                    start+=count; count = 0;
                    while(*(start+count)!=' ') count++;
                    name_pubkey = (char*)calloc(count+1,sizeof(char));
                    count = 0;
                    while(*(start+count)!=' '){
                        name_pubkey[count_file] = *(start+count);
                        count_file++;
                        count++;
                    }
                    FILE* pubkey = NULL;
                    pubkey = fopen(name_pubkey,"r");
                    if (pubkey==NULL) {
                        printf("Error with file %s, please try again\n", name_pubkey);
                        fl_for_error = 1;
                    } else fclose(pubkey);
                }
                //
                start = strstr(main_string, char_outfile);
                if(start==NULL){
                    if(fl_for_error==0) printf("Error, please try again\n");
                    fl_for_error = 1;
                }
                else {
                    start += (strlen(char_outfile));
                    count = 0, count_file = 0;
                    while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                    &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                    start+=count; count = 0;
                    while(*(start+count)!=' '&&*(start+count)!='\n') count++;
                    name_outfile = (char*)calloc(count+1,sizeof(char));
                    count = 0;
                    while(*(start+count)!=' '&&*(start+count)!='\n'){
                        name_outfile[count_file] = *(start+count);
                        count_file++;
                        count++;
                    }
                    FILE* outfile = NULL;
                    outfile = fopen(name_outfile,"r");
                    if (outfile==NULL) {
                        printf("Error with file %s, please try again\n", name_outfile);
                        fl_for_error = 1;
                    } else fclose(outfile);
                }
                if (fl_for_error==0){
                    if(fl_rsa==1) rsa_encrypt(name_pubkey,name_infile,name_outfile);
                    if(fl_eg==1) eg_encrypt(name_pubkey,name_infile,name_outfile);
                    printf("Success!\n");
                } else fl_for_error = 0;
            }
        }
        else counter_for_error++;
        if(strstr(main_string, com_decrypt)!=NULL){
            char *char_infile = "--infile";
            char *char_outfile = "--outfile";
            char *char_secret = "--secret";
            char *name_infile;
            char *name_secret;
            char *name_outfile;
            char *start = strstr(main_string, char_infile);
            short count = 0, count_file = 0;
            if(start==NULL){
                if(fl_for_error==0) printf("Error, please try again\n");
                fl_for_error = 1;
            }
            else {start += (strlen(char_infile));
                count = 0, count_file = 0;
                while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                start+=count; count = 0;
                while(*(start+count)!=' ') count++;
                name_infile = (char*)calloc(count+1,sizeof(char));
                count = 0;
                while(*(start+count)!=' '){
                    name_infile[count_file] = *(start+count);
                    count_file++;
                    count++;
                }
                FILE* infile = NULL;
                infile = fopen(name_infile,"r");
                if (infile==NULL) {
                    printf("Error with file %s, please try again\n", name_infile);
                    fl_for_error = 1;
                } else fclose(infile);
                //
                start = strstr(main_string, char_secret);
                if(start==NULL){
                    if(fl_for_error==0) printf("Error, please try again\n");
                    fl_for_error = 1;
                }
                else {
                    start += (strlen(char_secret));
                    count = 0, count_file = 0;
                    while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                    &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                    start+=count; count = 0;
                    while(*(start+count)!=' ') count++;
                    name_secret = (char*)calloc(count+1,sizeof(char));
                    count = 0;
                    while(*(start+count)!=' '){
                        name_secret[count_file] = *(start+count);
                        count_file++;
                        count++;
                    }
                    FILE* secret = NULL;
                    secret = fopen(name_secret,"r");
                    if (secret==NULL) {
                        printf("Error with file %s, please try again\n", name_secret);
                        fl_for_error = 1;
                    } else fclose(secret);
                }
                start = strstr(main_string, char_outfile);
                if(start==NULL){
                    if(fl_for_error==0) printf("Error, please try again\n");
                    fl_for_error = 1;
                }
                else {
                    start += (strlen(char_outfile));
                    count = 0, count_file = 0;
                    while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                    &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                    start+=count; count = 0;
                    while(*(start+count)!=' '&&*(start+count)!='\n') count++;
                    name_outfile = (char*)calloc(count+1,sizeof(char));
                    count = 0;
                    while(*(start+count)!=' '&&*(start+count)!='\n'){
                        name_outfile[count_file] = *(start+count);
                        count_file++;
                        count++;
                    }
                    FILE* outfile = NULL;
                    outfile = fopen(name_outfile,"r");
                    if (outfile==NULL) {
                        printf("Error with file %s, please try again\n", name_outfile);
                        fl_for_error = 1;
                    } else fclose(outfile);
                }
                if (fl_for_error==0){
                    if(fl_rsa==1) rsa_decrypt(name_secret,name_infile,name_outfile);
                    if(fl_eg==1) eg_decrypt(name_secret,name_infile,name_outfile);
                    printf("Success!\n");
                } else fl_for_error = 0;
            }
        }
        else counter_for_error++;
        if(strstr(main_string, com_sign)!=NULL){
            char *char_infile = "--infile";
            char *char_sigfile = "--sigfile";
            char *char_secret = "--secret";
            char *name_infile;
            char *name_secret;
            char *name_sigfile;
            char *start = strstr(main_string, char_infile);
            short count = 0, count_file = 0;
            if(start==NULL){
                if(fl_for_error==0) printf("Error, please try again\n");
                fl_for_error = 1;
            }
            else {start += (strlen(char_infile));
                count = 0, count_file = 0;
                while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                start+=count; count = 0;
                while(*(start+count)!=' ') count++;
                name_infile = (char*)calloc(count+1,sizeof(char));
                count = 0;
                while(*(start+count)!=' '){
                    name_infile[count_file] = *(start+count);
                    count_file++;
                    count++;
                }
                FILE* infile = NULL;
                infile = fopen(name_infile,"r");
                if (infile==NULL) {
                    printf("Error with file %s, please try again\n", name_infile);
                    fl_for_error = 1;
                } else fclose(infile);
                //
                start = strstr(main_string, char_secret);
                if(start==NULL){
                    if(fl_for_error==0) printf("Error, please try again\n");
                    fl_for_error = 1;
                }
                else {
                    start += (strlen(char_secret));
                    count = 0, count_file = 0;
                    while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                    &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                    start+=count; count = 0;
                    while(*(start+count)!=' ') count++;
                    name_secret = (char*)calloc(count+1,sizeof(char));
                    count = 0;
                    while(*(start+count)!=' '){
                        name_secret[count_file] = *(start+count);
                        count_file++;
                        count++;
                    }
                    FILE* secret = NULL;
                    secret = fopen(name_secret,"r");
                    if (secret==NULL) {
                        printf("Error with file %s, please try again\n", name_secret);
                        fl_for_error = 1;
                    } else fclose(secret);
                }
                //
                start = strstr(main_string, char_sigfile);
                if(start==NULL){
                    if(fl_for_error==0) printf("Error, please try again\n");
                    fl_for_error = 1;
                }
                else {
                    start += (strlen(char_sigfile));
                    count = 0, count_file = 0;
                    while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                    &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                    start+=count; count = 0;
                    while(*(start+count)!=' '&&*(start+count)!='\n') count++;
                    name_sigfile = (char*)calloc(count+1,sizeof(char));
                    count = 0;
                    while(*(start+count)!=' '&&*(start+count)!='\n'){
                        name_sigfile[count_file] = *(start+count);
                        count_file++;
                        count++;
                    }
                    FILE* sigfile = NULL;
                    sigfile = fopen(name_sigfile,"r");
                    if (sigfile==NULL) {
                        printf("Error with file %s, please try again\n", name_sigfile);
                        fl_for_error = 1;
                    } else fclose(sigfile);
                }
                if (fl_for_error==0){
                    rsa_sign(name_secret,name_infile,name_sigfile);
                    printf("Success!\n");
                } else fl_for_error = 0;
            }
        }
        else counter_for_error++;
        if(strstr(main_string, com_check)!=NULL){
            char *char_infile = "--infile";
            char *char_sigfile = "--sigfile";
            char *char_pubkey = "--pubkey";
            char *name_infile;
            char *name_pubkey;
            char *name_sigfile;
            char *start = strstr(main_string, char_infile);
            short count = 0, count_file = 0;
            if(start==NULL){
                if(fl_for_error==0) printf("Error, please try again\n");
                fl_for_error = 1;
            }
            else {start += (strlen(char_infile));
                count = 0, count_file = 0;
                while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                start+=count; count = 0;
                while(*(start+count)!=' ') count++;
                name_infile = (char*)calloc(count+1,sizeof(char));
                count = 0;
                while(*(start+count)!=' '){
                    name_infile[count_file] = *(start+count);
                    count_file++;
                    count++;
                }
                FILE* infile = NULL;
                infile = fopen(name_infile,"r");
                if (infile==NULL) {
                    printf("Error with file %s, please try again\n", name_infile);
                    fl_for_error = 1;
                } else fclose(infile);
                //
                start = strstr(main_string, char_pubkey);
                if(start==NULL){
                    if(fl_for_error==0) printf("Error, please try again\n");
                    fl_for_error = 1;
                }
                else {
                    start += (strlen(char_pubkey));
                    count = 0, count_file = 0;
                    while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                    &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                    start+=count; count = 0;
                    while(*(start+count)!=' ') count++;
                    name_pubkey = (char*)calloc(count+1,sizeof(char));
                    count = 0;
                    while(*(start+count)!=' '){
                        name_pubkey[count_file] = *(start+count);
                        count_file++;
                        count++;
                    }
                    FILE* pubkey = NULL;
                    pubkey = fopen(name_pubkey,"r");
                    if (pubkey==NULL) {
                        printf("Error with file %s, please try again\n", name_pubkey);
                        fl_for_error = 1;
                    } else fclose(pubkey);
                }
                //
                start = strstr(main_string, char_sigfile);
                if(start==NULL){
                    if(fl_for_error==0) printf("Error, please try again\n");
                    fl_for_error = 1;
                }
                else {
                    start += (strlen(char_sigfile));
                    count = 0, count_file = 0;
                    while( ((*(start + count)>'z')||(*(start + count)<'a'))&&((*(start + count)>'Z')||(*(start + count)<'A'))
                    &&((*(start + count)>'9')||(*(start + count)<'0')) && (*(start + count)!='_') ) count++;
                    start+=count; count = 0;
                    while(*(start+count)!=' '&&*(start+count)!='\n') count++;
                    name_sigfile = (char*)calloc(count+1,sizeof(char));
                    count = 0;
                    while(*(start+count)!=' '&&*(start+count)!='\n'){
                        name_sigfile[count_file] = *(start+count);
                        count_file++;
                        count++;
                    }
                    FILE* sigfile = NULL;
                    sigfile = fopen(name_sigfile,"r");
                    if (sigfile==NULL) {
                        printf("Error with file %s, please try again\n", name_sigfile);
                        fl_for_error = 1;
                    } else fclose(sigfile);
                }
                if (fl_for_error==0){
                    rsa_check(name_pubkey,name_infile,name_sigfile);
                    //printf("Success!\n");
                } else fl_for_error = 0;
            }
        }
        else counter_for_error++;
        if(strncmp(main_string, com_exit,5)==0){
            fl_for_exit = 1;
        }
        else counter_for_error++;
        if (counter_for_error==9){
            printf("Unknown command, please try again\n");
        }
        if (counter_for_error<8){
            printf("Error, please try again\n");
        }
        for(int i = 0; i<150; i++){
            main_string[i] = '\0';
        }
        counter_for_error = 0;
    }
    return 0;
}
