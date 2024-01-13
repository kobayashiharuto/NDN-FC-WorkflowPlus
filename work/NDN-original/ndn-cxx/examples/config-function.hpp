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
                if (std::next(it) != dataResults.end())
                {
                  result += "-X-";
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
                if (std::next(it) != dataResults.end())
                {
                  result += "-Y-";
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
                if (std::next(it) != dataResults.end())
                {
                  result += "-Z-";
                }
              }
              data->setContent(result);
              return data;
            },
        },
        {
            "/W/func",
            [](const std::vector<ndn::Data> &dataResults, const std::string &functionName) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              std::string result;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                result += dataContentToString(*it);
                if (std::next(it) != dataResults.end())
                {
                  result += "-W-";
                }
              }
              data->setContent(result);
              return data;
            },
        },
    };
  } // namespace examples
} // namespace ndn