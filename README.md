# courcework
мужиииик надо делать курсач
!!!!!

short* delenie(short* a, short* b, int* la, int* lb, int* len_rez)
{
    short* current;
    int lr = *la;
    short* rez = 0;
    int ctr = 0;
    int max_i = 1;
    rez = (short*)calloc(*la, sizeof(short));
    current = (short*)calloc(*la, sizeof(short));
    for (int i = 0; i < *la; i++)
        current[i] = a[i];
    while (comparison(current, b, lr, *lb) != 2)
    {
        current = long_subtraction(current, b, lr, *lb, &lr, NULL, 0);
        int i = 0;
        ctr++;
        rez[i]++;
        while (rez[i] == 16)
        {
            if (i + 2 > max_i)
            {
                max_i++;
            }
            rez[i + 1]++;
            rez[i] = 0;
            i++;
        }
    }
    *len_rez = max_i;
    return rez;
}

short* long_gcd(short* a, short* b, short* x, short* y, int* len_a, int* len_b, int* len_x, int* len_y, int* len_rez)
{
    if (*len_a == 1 && a[0] == 0)
    {
        for (int i = 0; i < *len_x; i++)
            x[i] = 0;
        for (int i = 1; i < *len_y; i++)
            y[i] = 0;
        y[0] = 1;
        *len_rez = *len_b;
        return b;
    }

    int len_ba = *len_a, len_x1 = *len_x, len_y1 = *len_y;
    short* d, * b_a;
    short* x1 = (short*)calloc(len_x1, sizeof(short));
    short* y1 = (short*)calloc(len_y1, sizeof(short));
    b_a = module_func(b, a, *len_b, *len_a, &len_ba, 0, NULL);
    d = long_gcd(b_a, a, x1, y1, &len_ba, len_a, &len_x1, &len_y1, len_rez);
    b_a = delenie(b, a, len_b, len_a, &len_ba);
    b_a = umnojenie(b_a, x1, len_ba, len_x1, &len_ba, 0, NULL);

    b_a = long_subtraction(b_a, y1, len_ba, len_y1, &len_ba, 0, NULL);

    *len_x = len_ba;
    *len_y = len_y1;
    memcpy(x, b_a, len_ba * sizeof(short));
    memcpy(y, x1, len_x1 * sizeof(short));

    free(b_a);
    free(y1);
    free(x1);
    return d;
}
