#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <chrono>
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
              for (auto &dataResult : dataResults)
              {
                result += dataContentToString(dataResult);
              }

              // 時間がかかる処理をシミュレート
              auto repeatCount = 1000000;
              auto hashedData = hashString(result, repeatCount);

              data->setContent(hashedData);
              return data;
            },
        },
        {
            "/B/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              std::string result;
              for (auto &dataResult : dataResults)
              {
                result += dataContentToString(dataResult);
              }

              // 時間がかかる処理をシミュレート
              auto repeatCount = 1000000;
              auto hashedData = hashString(result, repeatCount);

              data->setContent(hashedData);
              return data;
            },
        },
        {
            "/C/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              std::string result;
              for (auto &dataResult : dataResults)
              {
                result += dataContentToString(dataResult);
              }

              // 時間がかかる処理をシミュレート
              auto repeatCount = 1000000;
              auto hashedData = hashString(result, repeatCount);

              data->setContent(hashedData);
              return data;
            },
        },
    };
  } // namespace examples
} // namespace ndn
