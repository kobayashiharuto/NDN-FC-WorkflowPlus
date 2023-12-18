#include <iostream>
#include <chrono>
#include <string>
#include <functional>

namespace ndn
{
  namespace examples
  {
    // 文字列にハッシュ関数を適用する関数
    std::string hashString(const std::string &input, int repeatCount)
    {
      std::string result = input;
      std::hash<std::string> hashFunc;

      for (int i = 0; i < repeatCount; ++i)
      {
        std::size_t hash = hashFunc(result);
        result = std::to_string(hash);
      }

      return result;
    }

  } // namespace examples
} // namespace ndn
