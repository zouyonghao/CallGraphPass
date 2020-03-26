#include <stdio.h>

void a() {}

void b() {
    a();
    a();
}

void c() {
    b();
    a();
}

int main() {
    a();
    c();
    printf("test\n");
    printf("test\n");
    printf("test\n");
    return 0;
}
