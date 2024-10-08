#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include "clicker.hh"

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;
using c_it_t = data_t::const_iterator;
using it_t = data_t::iterator;

value_t sum_data(const data_t & data, size_t i, size_t amount)
{
  value_t sum{0};
  for (size_t j = i; j < i + amount; ++j)
  {
    sum += data[i];
  }
  return sum;
}

void sum_data_th(const data_t & data, size_t i, size_t amount, value_t & res)
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
    ths.reserve(threads - 1);
    std::vector< value_t > results(threads, 0);
    init = cl.millisec();

    size_t per_th = size / threads;
    size_t last_th = per_th + size % threads;
    size_t i = 0;
    for (; i < threads - 1; ++i)
    {
      ths.emplace_back(sum_data_th, std::cref(values),
       i * per_th, per_th, std::ref(results[i]));
    }
    sum_data_th(values, i * per_th, last_th, results.back());
    for (auto && th: ths)
    {
      th.join();
    }
    sum = std::accumulate(results.cbegin(), results.cend(), 0);
    total = cl.millisec();
  }
  std::cout << "Total: " << total << "\n";
  std::cout << "Init: " << init << "\n";
  std::cout << "Computations: " << total - init << "\n";
  std::cout << "Threads: " << threads << "\n";
  std::cout << "Result: " << sum << "\n";
}
