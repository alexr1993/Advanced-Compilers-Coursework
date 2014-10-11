/* Accept function as a param */
function twice (function f)
{
    int g (int x)
    {
        /* leverage dynamic scope */
        return f( f(x) );
    }
    return g;
}
