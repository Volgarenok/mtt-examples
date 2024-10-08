#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <limits>

namespace mtt
{
  class Clicker
  {
   public:
    Clicker():
     start_(std::chrono::high_resolution_clock::now())
    {}

    auto millisec() const
    {
      using std::chrono::high_resolution_clock;
      using std::chrono::duration_cast;
      using std::chrono::milliseconds;
      auto t = high_resolution_clock::now();
      return duration_cast< milliseconds >(t - start_).count();
    }

   private:
    std::chrono::time_point< std::chrono::system_clock > start_;
  };
}

int main()
{
  constexpr size_t size{1'000'000'000};
  double init{0}, total{0};
  {
    mtt::Clicker cl;
    using data_t = std::vector< unsigned long long >;
    data_t values(size, 1);
    init = cl.millisec();

    data_t::value_type sum{0};
    for (size_t i = 0; i < values.size(); ++i)
    {
      sum += values[i];
    }
    total = cl.millisec();
  }
  std::cout << "Total: " << total << "\n";
  std::cout << "Init: " << init << "\n";
}
