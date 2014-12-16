int fact(int n)
{
    /* n arg must shadow n in lexical scope */
    int inner_fact(int n, int a)
    {
        if (n==0) return a;
        return inner_fact(n-1,a*n);
    }
    return inner_fact(n,1);
}
