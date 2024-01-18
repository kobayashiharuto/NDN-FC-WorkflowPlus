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
using ndn::examples::decryptCaesarCipher;
using ndn::examples::hashString;
using ndn::examples::myLog;

namespace ndn
{
  namespace examples
  {
    using FunctionProvider = std::function<std::string(const std::vector<std::string> &, const std::string &)>;
    using FunctionProviderMap = std::unordered_map<std::string, FunctionProvider>;

    inline const FunctionProviderMap functionProviders = {
        {
            "/X/func",
            [](const std::vector<std::string> &dataResults, const std::string &functionName) -> std::string
            {
              std::string result = decryptCaesarCipher(dataResults[0], -1);
              return result;
            },
        },
        {
            "/Y/func",
            [](const std::vector<std::string> &dataResults, const std::string &functionName) -> std::string
            {
              std::string result;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                result += *it;
                if (std::next(it) != dataResults.end())
                {
                  result += "-Y-";
                }
              }
              return result;
            },
        },
        {
            "/Z/func",
            [](const std::vector<std::string> &dataResults, const std::string &functionName) -> std::string
            {
              std::string result;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                result += *it;
                if (std::next(it) != dataResults.end())
                {
                  result += "-Z-";
                }
              }
              return result;
            },
        },
        {
            "/W/func",
            [](const std::vector<std::string> &dataResults, const std::string &functionName) -> std::string
            {
              std::string result;
              for (auto it = dataResults.begin(); it != dataResults.end(); ++it)
              {
                result += *it;
                if (std::next(it) != dataResults.end())
                {
                  result += "-W-";
                }
              }
              return result;
            },
        },
    };
  } // namespace examples
} // namespace ndn