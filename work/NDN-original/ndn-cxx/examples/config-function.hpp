#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
#include <ndn-cxx/data.hpp>

#include <ndn-cxx/face.hpp>

#include "my-utils.hpp"

using ndn::examples::dataContentToString;
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
                result += "-A-";
                result += dataContentToString(dataResult);
              }
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
              for (auto &dataResult : dataResults)
              {
                result += "-B-";
                result += dataContentToString(dataResult);
              }
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
              for (auto &dataResult : dataResults)
              {
                result += "-C-";
                result += dataContentToString(dataResult);
              }
              data->setContent(result);
              return data;
            },
        },
    };
  } // namespace examples
} // namespace ndn
