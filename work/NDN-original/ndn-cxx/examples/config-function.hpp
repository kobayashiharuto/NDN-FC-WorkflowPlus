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
            "/X/func",
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
              data->setContent(result);
              return data;
            },
        },
        {
            "/Y/func",
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
              data->setContent(result);
              return data;
            },
        },
        {
            "/Z/func",
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
              // auto repeatCount = 10000000;
              // auto hashedData = hashString(result, repeatCount);
              std::cout << "1000ms待機" << std::endl;
              std::this_thread::sleep_for(std::chrono::milliseconds(1000));
              data->setContent(result);
              return data;
            },
        },
    };
  } // namespace examples
} // namespace ndn