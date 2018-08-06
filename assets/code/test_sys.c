#include <stdio.h>

int main() {
#ifdef _WIN32
    printf("windows 32bit %dbit\n", (int)(sizeof(int*) * 8));
#endif // _WIN32

#ifdef _WIN64
    printf("windows 64bit %dbit\n", (int)(sizeof(int*) * 8));
#endif // _WIN64

#ifdef _AIX
    printf("IBM AIX %dbit\n", (int)(sizeof(int*) * 8));
#endif // _AIX

#ifdef __sun
    printf("SUN OS %dbit\n", (int)(sizeof(int*) * 8));
#endif // __sun

#ifdef _LINUX
    printf("Linux %dbit\n", (int)(sizeof(int*) * 8));
#endif // _LINUX

#ifdef _UNIX
    printf("Unix %dbit\n", (int)(sizeof(int*) * 8));
#endif // _UNIX

return 0;

}
