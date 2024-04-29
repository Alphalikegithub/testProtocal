#include <stdint.h>
#include <iostream>
using namespace std;

bool bigCheck()
{
    union Check
    {
        char a;
        uint32_t data;
    };
    
    Check c;
    c.data = 1;
    
    if (1 == c.a)
    {
        return false;
    }
    
    return true;
}

int main()
{
    if (bigCheck())
    {
        cout << "big" << endl;
    }
    else
    {
        cout << "small" << endl;
    }
    return 0;
}