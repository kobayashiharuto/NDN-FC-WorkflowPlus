#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <ndn-cxx/data.hpp>

#include <ndn-cxx/face.hpp>

#include "my-utils.hpp"
#include "benchmark.hpp"

using ndn::examples::dataContentToString;
using ndn::examples::hashString;
using ndn::examples::myLog;

namespace ndn
{
  namespace examples
  {
    using FunctionProvider = std::function<std::shared_ptr<ndn::Data>(const std::vector<ndn::Data> &, const std::string &)>;
    using FunctionProviderMap = std::unordered_map<std::string, FunctionProvider>;

    inline const FunctionProviderMap functionProviders = {
        {
            // 最大値を返す関数
            "/X/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              double max = -10000;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                auto data = dataContentToString(*it);
                auto value = std::stod(data);
                if (value > max)
                {
                  max = value;
                }
              }
              auto result = std::to_string(max);
              data->setContent(result);
              return data;
            },
        },
        {
            // 最小値を返す関数
            "/Y/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              double min = 10000;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                auto data = dataContentToString(*it);
                auto value = std::stod(data);
                if (value < min)
                {
                  min = value;
                }
              }
              auto result = std::to_string(min);
              data->setContent(result);
              return data;
            },
        },
        {
            // 華氏を摂氏に変換する関数
            "/Z/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              double fahrenheit = 0.0;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                auto data = dataContentToString(*it);
                auto value = std::stod(data);
                fahrenheit = value;
              }
              auto result = std::to_string((fahrenheit - 32) * 5 / 9);
              data->setContent(result);
              return data;
            },
        },
        {
            // 結果をまとめて返す関数 (例: 報告結果: 〇〇℃, △△℃, □□℃, ...)
            "/W/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              std::string result = "報告結果: ";
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                auto data = dataContentToString(*it);
                // 最後にカンマをつけない
                if (it == dataResults.end() - 1)
                {
                  result += data;
                }
                else
                {
                  result += data + ", ";
                }
              }
              data->setContent(result);
              return data;
            },
        },
    };
  } // namespace examples
} // namespace ndn