#include <iostream>
#include <vector>
#include <algorithm>
#include <future>

/*
//Синхронная реализация
std::vector<int>::iterator get_min_index(std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
    return  std::min_element(begin, end);
}
*/

//Асинхронная реализация
void get_min_index(std::vector<int>::iterator begin, std::vector<int>::iterator end, std::promise<std::vector<int>::iterator> Prom)
{
    Prom.set_value(std::min_element(begin, end));
}

int main()
{
    const size_t vector_size = 20;
    std::vector<int> Vect(vector_size);
    std::generate(Vect.begin(), Vect.end(), []() { return rand() % 100; });
    std::cout << "Source:\n";
    std::for_each(Vect.begin(), Vect.end(), [](const int val) {std::cout << val << " "; });
    std::cout << "\nSotred:" << std::endl;

    for (size_t i = 1; i < vector_size; ++i)
    {
        //auto min_iterator = get_min_index(Vect.begin() + i, Vect.end()); //Синхронный вызов
        std::promise<std::vector<int>::iterator> Prom;
        std::future<std::vector<int>::iterator> Fut = Prom.get_future();
        std::future<void> res = std::async(get_min_index, Vect.begin() + i, Vect.end(), std::move(Prom));
        auto min_iterator = Fut.get();
        if (*(min_iterator) < Vect.at(i - 1))
        {
            std::swap(*min_iterator, Vect.at(i - 1));
        }
    }
    std::for_each(Vect.begin(), Vect.end(), [](const int val) {std::cout << val << " "; });
    std::cout << std::endl;
    return 0;
}
