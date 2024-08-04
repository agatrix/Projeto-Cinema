#include <iostream>
#include <string>
#include <set>
#include <sstream>

// Função para dividir uma string por vírgula e adicionar ao set
void addToSet(const std::string& str, std::set<std::string>& uniqueSet) {
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, ',')) {
        uniqueSet.insert(item);
    }
}

int main() {
    // Strings de entrada com diferentes elementos
    std::string x = "a,b";
    std::string y = "b,a";
    std::string z = "c,d,e";

    // Conjunto para armazenar elementos únicos
    std::set<std::string> uniqueSet;

    // Adiciona elementos das strings ao set
    addToSet(x, uniqueSet);
    addToSet(y, uniqueSet);
    addToSet(z, uniqueSet);

    // Imprime os elementos únicos
    std::cout << "Elementos únicos:" << std::endl;
    for (const auto& elem : uniqueSet) {
        std::cout << elem << std::endl;
    }

    return 0;
}
