#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include "clicker.hh"

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

value_t sum_data(const data_t::const_iterator begin, const data_t::const_iterator end)
{
  return std::accumulate(begin, end, value_t{0});
}

void sum_data_th(const data_t::const_iterator begin, const data_t::const_iterator end, value_t & res)
{
  res += sum_data(begin, end);
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
    ths.reserve(threads);
    init = cl.millisec();

    size_t per_th = size / threads;
    size_t last_th = per_th + size % threads;
    size_t i = 0;
    auto it = values.cbegin();
    for (; i < threads - 1; ++i)
    {
      auto end = it + per_th;
      ths.emplace_back(sum_data_th, it, end, std::ref(sum));
      it = end;
    }
    sum_data_th(it, it + last_th, sum);
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
