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
                result += dataContentToString(dataResult);
                // 最後以外は-A-でつなぐ
                if (&dataResult != &dataResults.back())
                {
                  result += "-A-";
                }
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
                result += dataContentToString(dataResult);
                // 最後以外は-B-でつなぐ
                if (&dataResult != &dataResults.back())
                {
                  result += "-B-";
                }
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
              // 最後以外は-C-でつなぐ
              if (&dataResult != &dataResults.back())
              {
                result += "-C-";
              }
              data->setContent(result);
              return data;
            },
        },
    };
  } // namespace examples
} // namespace ndn
