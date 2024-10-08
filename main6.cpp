#include <iostream>
#include <algorithm>
#include <future>
#include <vector>
#include <numeric>
#include <thread>
#include "clicker.hh"

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;
using c_it_t = data_t::const_iterator;
using it_t = data_t::iterator;

value_t sum_data(c_it_t begin, c_it_t end)
{
  return std::accumulate(begin, end, value_t{0});
}

void sum_data_th(c_it_t begin, c_it_t end, it_t res)
{
  *res = sum_data(begin, end);
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

    std::vector< std::future< value_t > > fts;
    fts.reserve(threads - 1);
    std::vector< value_t > results(threads, 0);
    init = cl.millisec();

    size_t per_th = size / threads;
    size_t last_th = per_th + size % threads;
    size_t i = 0;
    auto it = values.cbegin();
    for (; i < threads - 1; ++i)
    {
      auto end = it + per_th;
      fts.emplace_back(std::async(sum_data, it, end));
      it = end;
    }
    results.back() = sum_data(it, it + last_th);
    std::transform(fts.begin(), fts.end(), results.begin(),
     [](auto && ft)
     {
       return ft.get();
     });
    sum = sum_data(results.cbegin(), results.cend());
    total = cl.millisec();
  }
  std::cout << "Total: " << total << "\n";
  std::cout << "Init: " << init << "\n";
  std::cout << "Computations: " << total - init << "\n";
  std::cout << "Threads: " << threads << "\n";
  std::cout << "Result: " << sum << "\n";
}
