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
            "/A/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              std::string result;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                result += dataContentToString(*it);
                // 最後の要素でなければ、"-A-"を追加
                if (std::next(it) != dataResults.end())
                {
                  result += "-A-";
                }
              }

              // 時間がかかる処理をシミュレート
              auto repeatCount = 10000000;
              auto hashedData = hashString(result, repeatCount);
              data->setContent(result);
              return data;
            },
        },
        {
            "/B/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              std::string result;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                result += dataContentToString(*it);
                // 最後の要素でなければ、"-B-"を追加
                if (std::next(it) != dataResults.end())
                {
                  result += "-B-";
                }
              }

              // 時間がかかる処理をシミュレート
              auto repeatCount = 10000000;
              auto hashedData = hashString(result, repeatCount);
              data->setContent(result);
              return data;
            },
        },
        {
            "/C/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              std::string result;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                result += dataContentToString(*it);
                // 最後の要素でなければ、"-C-"を追加
                if (std::next(it) != dataResults.end())
                {
                  result += "-C-";
                }
              }

              // 時間がかかる処理をシミュレート
              auto repeatCount = 10000000;
              auto hashedData = hashString(result, repeatCount);
              data->setContent(result);
              return data;
            },
        },
    };
  } // namespace examples
} // namespace ndn