#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <vector>


template<typename Iterator>
void parallel_for_each(Iterator first, Iterator last, std::function<void(int)> func)
{
	unsigned long const length = std::distance(first, last);
	unsigned long const max_chunk_size = 25;
	if (length <= max_chunk_size)
	{
		std::for_each(first, last, func);
	}
	else
	{
		Iterator mid_point = first;
		std::advance(mid_point, length / 2);
		std::future<void> first_half_result = std::async(std::launch::async, parallel_for_each<Iterator>, first, mid_point, func);
		std::future<void> second_half_result = std::async(std::launch::async, parallel_for_each<Iterator>, mid_point, last, func);
	}
}

int main()
{
	std::vector<int> V1(1000);
	std::generate(V1.begin(), V1.end(), []() {return rand() % 10; });
	auto start1 = std::chrono::high_resolution_clock::now();
	std::for_each(V1.begin(), V1.end(), [](int val) {++val; });
	auto stop1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> res = stop1 - start1;
	std::cout << res.count() << std::endl;

	auto start2 = std::chrono::high_resolution_clock::now();
	parallel_for_each(V1.begin(), V1.end(), [](int val) {++val; });
	auto stop2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> res2 = stop2 - start2;
	std::cout << res2.count() << std::endl;

	return 0;
}
