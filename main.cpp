#include <iostream>
#include <vector>
#include <ctime>

int main() {

    srand(time(nullptr));

    std::cout << "Hello, World!" << std::endl;
    std::vector<int> a(10);
    for (int i=0; i<10; i++) {
        a.at(i) = rand()%10;
    }

    for (std::vector<int>::iterator aItr = a.begin(); aItr < a.end(); aItr++) {
        std::cout << *aItr << std::endl;
    }
    return 0;
}
