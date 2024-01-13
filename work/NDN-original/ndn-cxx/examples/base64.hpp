#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace ndn
{
  namespace examples
  {

    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string base64_encode(unsigned char const *bytes_to_encode, unsigned int in_len)
    {
      std::string ret;
      int i = 0;
      int j = 0;
      unsigned char char_array_3[3];
      unsigned char char_array_4[4];

      while (in_len--)
      {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
          char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
          char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
          char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
          char_array_4[3] = char_array_3[2] & 0x3f;

          for (i = 0; (i < 4); i++)
            ret += base64_chars[char_array_4[i]];
          i = 0;
        }
      }

      if (i)
      {
        for (j = i; j < 3; j++)
          char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
          ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
          ret += '=';
      }

      return ret;
    }

    // BMP画像を読み込み、Base64エンコードする関数
    std::string encodeImageToBase64(const std::string &imagePath)
    {
      std::ifstream file(imagePath, std::ios::binary);
      if (!file)
      {
        throw std::runtime_error("ファイルを開けません: " + imagePath);
      }

      // ファイルの内容を読み込む
      std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});

      // Base64にエンコード
      return base64_encode(buffer.data(), buffer.size());
    }

  } // namespace examples
} // namespace ndn
