#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <ndn-cxx/face.hpp>
#pragma once

namespace ndn
{
  namespace examples
  {

    // 現在時刻を文字列で取得
    std::string getCurrentTimeWithMilliseconds()
    {
      // 現在の時刻を取得
      auto now = std::chrono::system_clock::now();

      // 時刻を秒単位で切り捨てて、time_t型に変換
      auto now_c = std::chrono::system_clock::to_time_t(now);

      // ミリ秒部分を計算
      auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

      // tm構造体に変換
      std::tm now_tm = *std::localtime(&now_c);

      // stringstreamを使用してフォーマット
      std::stringstream ss;
      ss << std::put_time(&now_tm, "%H:%M:%S");
      ss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

      return ss.str();
    }

    // ログを表示
    void myLog(const std::string &message)
    {
      // return;
      std::cout << "MY_LOG[" << getCurrentTimeWithMilliseconds() << "]: " << message << std::endl;
    }

    // URLデコードを行う
    std::string urlDecodeAndTrim(const std::string &encoded)
    {
      std::ostringstream oss;
      for (size_t i = 0; i < encoded.length(); ++i)
      {
        if (encoded[i] == '%' && i + 2 < encoded.length())
        {
          std::string hex = encoded.substr(i + 1, 2);
          char decodedChar = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
          oss << decodedChar;
          i += 2;
        }
        else if (encoded[i] == '+')
        {
          oss << ' ';
        }
        else
        {
          oss << encoded[i];
        }
      }
      std::string decodedUrl = oss.str();

      // バージョン制御に使われるようで、consumerが送ったURLの末尾に /v= という文字列が勝手に付与されている
      // 邪魔なので、/v= 以降を削除する
      std::string processedUrl = decodedUrl;
      size_t vPos = processedUrl.find("/v=");
      if (vPos != std::string::npos)
      {
        processedUrl.erase(vPos);
      }

      return processedUrl;
    }

    std::string trim(const std::string &s)
    {
      size_t start = s.find_first_not_of(" \t\n\r");
      size_t end = s.find_last_not_of(" \t\n\r");
      return (start == std::string::npos || end == std::string::npos) ? "" : s.substr(start, end - start + 1);
    }

    // URLから初期レベルの引数を抽出するやつ
    std::vector<std::string> extractFirstLevelArgs(const std::string &decodedUrl)
    {
      std::vector<std::string> args;
      if (decodedUrl.find('(') == std::string::npos)
      {
        args.push_back(trim(decodedUrl));
        return args;
      }

      // functionName を正しく抽出する
      size_t endOfFunctionName = decodedUrl.find("/(");
      size_t startOfArgs = endOfFunctionName + 2;
      std::string argsStr = decodedUrl.substr(startOfArgs, decodedUrl.size() - startOfArgs - 1);

      size_t start = 0;
      int bracketCount = 0;
      for (size_t i = 0; i < argsStr.size(); ++i)
      {
        if (argsStr[i] == '(')
        {
          bracketCount++;
        }
        else if (argsStr[i] == ')')
        {
          bracketCount--;
        }
        else if (argsStr[i] == ',' && bracketCount == 0)
        {
          args.push_back(trim(argsStr.substr(start, i - start)));
          start = i + 1;
        }
      }
      args.push_back(trim(argsStr.substr(start)));

      return args;
    }

    // URLからfunctionNameを抽出する
    std::string extractFunctionName(const std::string &decodedUrl)
    {
      size_t endOfFunctionName = decodedUrl.find("/(");
      std::string functionName = decodedUrl.substr(0, endOfFunctionName);
      return functionName;
    }

    // Data の Content 内容を文字列に変換する
    std::string dataContentToString(const Data &data)
    {
      if (!data.hasContent())
      {
        return "";
      }

      const Block &content = data.getContent();
      const uint8_t *buffer = content.value();
      size_t size = content.value_size();
      std::string contentStr(reinterpret_cast<const char *>(buffer), size);
      return contentStr;
    }
  }
}