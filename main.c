#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
short pow_16(short pow)
{
    short result = 1;
    for(int i = 0; i<pow; i++){
        result*=16;
    }
    return result;
}
void printff(short int* a, int size) // вспомогательная функция для печати массива
{
    for(int i = size-1; i>=0; i--){
        if(a[i]<10) printf("%d", a[i]);
        if(a[i]==10) printf("A");
        if(a[i]==11) printf("B");
        if(a[i]==12) printf("C");
        if(a[i]==13) printf("D");
        if(a[i]==14) printf("E");
        if(a[i]==15) printf("F");
    }
    printf("\n");
}
short int* umnojenie(short int *a, short int *b, int len1, int len2, int *length_of_result, int free_a, int free_b)
{
    int length = len1 + len2 + 1;
    short int *xy = (short int*)calloc(length, sizeof(short int));
    for (int i = 0; i < len1; i++)
        for (int j = 0; j < len2; j++)
            xy[i + j] += a[i] * b[j];

        for (int i = 0; i < length-1; i++){
            xy[i + 1] += xy[i] / 16;
            xy[i] %= 16;
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
            //free(vsp);
            //vsp = NULL;
        }
        if(free_a==1) {
            free(a);
            a = NULL;
        }
        if(free_b==1) {
            free(b);
            b = NULL;
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
short int* long_subtraction(short int* a, short int* b, int size_a, int size_b, int *size_of_result, short int free_a)//вычитание
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
        while(xy[length-1]==0) length--;
        short *vsp = (short*)calloc(length, sizeof(short int));
        memcpy(vsp,xy,length*sizeof(short));
        free(xy);
        xy = NULL;
        xy = (short*)calloc(length, sizeof(short));
        memcpy(xy,vsp,length*sizeof(short));
        free(vsp);
    }
    free(a);
    *size_of_result = length;
    return xy;
}
short int *module_func(short int *number, short *module, int size_number, int size_module, int *pointer_size_number, int free_number)
{
    short int *result = NULL;
    int size_result = size_number, *pointer_size_result = &size_result;
    short int *module_copy = NULL;
    short int *stepen = NULL;
    int len_of_stepen = 0;
    int size_module_copy = 0, *p_size_m_copy = &size_module_copy;
    result = (short*)calloc(size_number, sizeof(short int));
    memcpy(result, number, size_number * sizeof(short int));
    if (comparison(result,module,size_result,size_module)==2){
        if(free_number==1){
            free(number);
            number = NULL;
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
                                        p_size_m_copy, 0, 0);
            }
            if(stepen!=NULL) {
                free(stepen);
                stepen = NULL;
            }
            if(len_of_stepen!=0) result = long_subtraction(result, module_copy, size_result, size_module_copy, pointer_size_result, 1);
            if(len_of_stepen==0) result = long_subtraction(result, module, size_result, size_module,pointer_size_result,1);
            if (comparison(result,module,size_result,size_module)==2){
                flag = '1';
            }
        }
        if(free_number==1){
            free(number);
            number = NULL;
        }
        *pointer_size_number = size_result;
        return result;
    }
}
short int *module_pow(short int *number, short int *pow, short *module, int size_number, int size_pow, int size_module, int* size_of_result)
{
    short *result = NULL, fl_for_equality = 0;
    short *counter = (short*)calloc(size_pow, sizeof(short));
    int current_size = size_number, *pointer_current_size = &current_size;
    counter[0] = 1;
    result = (short int*)calloc(current_size, sizeof(short int));
    for(int i = 0; i<current_size; i++) result[i] = number[i];
    while(fl_for_equality==0){
        result = umnojenie(result,number,current_size,size_number,pointer_current_size,1,0);
        result = module_func(result,module,current_size,size_module,pointer_current_size,1);
        counter[0]++;
        for (int i = 0; i < size_pow-1; i++){
            counter[i+1] = counter[i]/16 + counter[i+1];
            counter[i] = counter[i]%16;
        }
        if(comparison(counter, pow, size_pow, size_pow)==0)
            fl_for_equality = 1;
    }
    *size_of_result = current_size;
    return result;
}
int return_number_10(FILE* stream)
{
    int number = 0;
    char symbol;
    while((symbol = getc(stream))!=(int)'\n'){
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
void return_long_number_16(FILE* stream, short int *a, short int *b, int size)
{
    short int symbol, i = 0;
    while((symbol = getc(stream))!=(int)'*'){
        if(symbol<58) a[size-1-i] = symbol - 48;
        else a[size-1-i] = symbol - 55;
        i++;
    } i = 0;
    while((symbol = getc(stream))!=(int)'\n'){
        if(symbol<58) b[size-1-i] = symbol - 48;
        else b[size-1-i] = symbol - 55;
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
    char* end_of_string = int_to_char_genkey_size(size);
    char name_of_file_q_p[20] = "key_";
    strcat(name_of_file_q_p, end_of_string);
    strcat(name_of_file_q_p, ".txt");
    FILE* base_of_q_p = NULL;
    if((base_of_q_p = fopen(name_of_file_q_p, "r"))==NULL)
    {
        printf("Error\n");
        return;
    }
    int sizeof_random = return_number_10(base_of_q_p);
    srand(time(NULL));
    int random_number = rand()%sizeof_random;
    int sizeof_q_p = return_number_10(base_of_q_p);
    q = (short int*)calloc(sizeof_q_p, sizeof(short int));
    p = (short int*)calloc(sizeof_q_p, sizeof(short int));
    random_down(random_number, base_of_q_p);
    return_long_number_16(base_of_q_p, p, q, sizeof_q_p);
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
void put_in_key_files(FILE *secret, FILE *public, short int *q,short int *p,short int *n,short int *d,short int *e,
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
    fprintf(secret, "d:");
    for(int i = size_d-1; i>=0; i--){
        fprintf(secret, "%d", d[i]);
    }
    fprintf(public, "n:");
    for(int i = size_n-1; i>=0; i--){
        fprintf(public, "%d", n[i]);
    }
    fprintf(public, "\n");
    fprintf(public, "e:");
    for(int i = size_e-1; i>=0; i--){
        fprintf(public, "%d", e[i]);
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
void get_private_key(char *name_of_file, short **d, short **q, short **p,
                     int *size_d, int *size_q, int *size_p)
{
    FILE* file = fopen(name_of_file, "r");
    int symbol, counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        ++*size_p;
    }
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        ++*size_q;
    }
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        ++*size_d;
    }
    fclose(file);
    *p = (short*)calloc(*size_p, sizeof(short));
    *q = (short*)calloc(*size_q, sizeof(short));
    *d = (short*)calloc(*size_d, sizeof(short));
    file = fopen(name_of_file, "r");
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        if(symbol<58) *(*p + *size_p - counter - 1) = symbol - 48;
        else *(*p + *size_p - counter - 1) = symbol - 55;
        counter++;
    }
    counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        if(symbol<58) *(*q + *size_q - counter - 1) = symbol - 48;
        else *(*q + *size_q - counter - 1) = symbol - 55;
        counter++;
    }
    counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        if(symbol<58) *(*d + *size_d - counter - 1) = symbol - 48;
        else *(*d + *size_d - counter - 1) = symbol - 55;
        counter++;
    }
    fclose(file);
}
void crypt_genkey(int size_of_key, char *nameof_public, char *nameof_secret)
{
    FILE *secret_file = NULL, *public_file = NULL;
    secret_file = fopen(nameof_secret, "w");
    public_file = fopen(nameof_public, "w");
    short int *q = NULL, *p = NULL, *n = NULL, *d = NULL, *e = NULL;
    int size_q = 0, size_p = 0, size_n = 0, size_d = 0, size_e = 0;
    int *pointer_q = &size_q, *pointer_p = &size_p, *pointer_d = &size_d, *pointer_e = &size_e, *pointer_n = &size_n;
    e = gen_pubkey(pointer_e);
    gen_q_and_p(size_of_key, q, p, pointer_q, pointer_p);
    n = umnojenie(q,p,size_q,size_p,pointer_n,0,0);
    //d = ... генерация ключа - секретный ключ
    //записываем все ключи в файлы
    put_in_key_files(secret_file,public_file,q,p,n,d,e,size_q,size_p,size_n,size_d,size_e);
    fclose(secret_file);
    fclose(public_file);
}
void get_public_key(char *name_of_file, short **e, short **n, int *size_e, int *size_n)
{
    FILE* file = fopen(name_of_file, "r");
    int symbol, counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        ++*size_n;
    }
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        ++*size_e;
    }
    fclose(file);
    *n = (short*)calloc(*size_n, sizeof(short));
    *e = (short*)calloc(*size_e, sizeof(short));
    file = fopen(name_of_file, "r");
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
        if(symbol<58) *(*n + *size_n - counter - 1) = symbol - 48;
        else *(*n + *size_n - counter - 1) = symbol - 55;
        counter++;
    }
    counter = 0;
    while((symbol = getc(file))!=(int)':');
    while((symbol = getc(file))!=(int)'\n'){
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
void encrypt(char *name_of_pubkey, char *name_of_file, char *name_of_encrypt_file)
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
    printff(input_mass,size_of_input);
    FILE* output = NULL;
    output = fopen(name_of_encrypt_file, "w");
    short *encrypting = NULL;
    int encrypting_size = 0, *encrypting_size_pointer = &encrypting_size;
    short *n = NULL, *e = NULL, **p_e = &e, **p_n = &n;
    int size_n = 0, size_e = 0, *pointer_size_n = &size_n, *pointer_size_e = &size_e;
    get_public_key(name_of_pubkey, p_e, p_n, pointer_size_e, pointer_size_n);
    printff(n,size_n);
    printff(e,size_e);
    encrypting = module_pow(input_mass,e,n,size_of_input,size_e,size_n,encrypting_size_pointer);
    printff(encrypting,encrypting_size);
    short *encrypting_char = array_to_byte_symbols(encrypting,encrypting_size);
    for(int i = encrypting_size/2-1; i>=0; i--){
        fprintf(output, "%c", encrypting_char[i]);
    }
    fclose(output);
}
void decrypt(char *name_of_infile, char *name_of_secret, char *name_of_outfile)
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
    get_private_key(name_of_secret,p_d,p_q,p_p,pointer_size_d,pointer_size_q,pointer_size_p);
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
}

int main() {
    /*char *infile = "infile.txt", *outfile = "outfile.txt", *pubkey = "pubkey.txt";
    encrypt(pubkey,infile,outfile);*/
    printf("zdarova vvedi komandu\n");
    //скоро напишу функцию для определения и вызова команды
    return 0;
}
