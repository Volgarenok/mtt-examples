#include <iostream>
#include <vector>
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

int main()
{
  constexpr size_t size{1'000'000'000};
  double init{0}, total{0};
  value_t sum{0};
  {
    mtt::Clicker cl;
    data_t values(size, 1);
    init = cl.millisec();
    sum = sum_data(values, 0, size);
    total = cl.millisec();
  }
  std::cout << "Total: " << total << "\n";
  std::cout << "Init: " << init << "\n";
  std::cout << "Computations: " << total - init << "\n";
  std::cout << "Result: " << sum << "\n";
}
