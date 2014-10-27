int f(int x)
{
    int z = 2, y = 3;
    return g(x + z);
}

int g(int y, int z)
{
    return 3 + y;
}

int main(void)
{
    return f(4);
}
