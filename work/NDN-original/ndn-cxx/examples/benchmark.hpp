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

    std::string decryptCaesarCipher(const std::string &encryptedText, int shift)
    {
      std::string decryptedText = encryptedText;

      // 各文字に対してシフトを適用
      for (char &c : decryptedText)
      {
        if (isalpha(c))
        {
          char base = islower(c) ? 'a' : 'A';
          c = static_cast<char>((c - base - shift + 26) % 26 + base);
        }
      }

      return decryptedText;
    }

  } // namespace examples
} // namespace ndn
