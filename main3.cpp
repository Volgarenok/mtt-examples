#include <iostream>
#include <vector>
#include <thread>
#include "clicker.hh"

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

value_t sum_data(const data_t & data, size_t i, size_t amount)
{
  value_t sum{0};
  for (size_t j = i; j < i + amount; ++j)
  {
    sum += data[j];
  }
  return sum;
}

void sum_data_th(const data_t & data, size_t i, size_t amount,
 value_t & res)
{
  res += sum_data(data, i, amount);
}

int main()
{
  constexpr size_t size{1'000'000'000};
  const size_t threads = std::thread::hardware_concurrency();
  double init{0}, total{0};

  value_t sum{0};
  {
    mtt::Clicker cl;
    data_t values(size, 1);

    std::vector< std::thread > ths;
    size_t threads_use = threads;
    ths.reserve(threads_use);
    init = cl.millisec();

    size_t per_th = size / threads_use;
    size_t last_th = per_th + size % threads_use;
    size_t i = 0;
    for (; i < threads_use - 1; ++i)
    {
      ths.emplace_back(sum_data_th,
       std::ref(values), i * per_th, per_th, std::ref(sum));
    }
    sum_data_th(values, i * per_th, last_th, sum);
    for (auto && th: ths)
    {
      th.join();
    }
    total = cl.millisec();
  }
  std::cout << "Total: " << total << "\n";
  std::cout << "Init: " << init << "\n";
  std::cout << "Computations: " << total - init << "\n";
  std::cout << "Threads: " << threads << "\n";
  std::cout << "Result: " << sum << "\n";
}
