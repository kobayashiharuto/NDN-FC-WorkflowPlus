#include <iostream>
#include <string>
#include <vector>

#include <ndn-cxx/face.hpp>

#include "my-utils.hpp"
#include "config-function.hpp"

using ndn::examples::dataContentToString;
using ndn::examples::functionProviders;
using ndn::examples::myLog;

namespace ndn
{
  namespace examples
  {
    std::string executeFunction(const std::vector<std::string> &dataResults, const std::string &functionName)
    {
      myLog("function処理を実行します。 function: " + functionName);

      // 関数提供マップから関数を取得
      auto functionIt = functionProviders.find(functionName);
      if (functionIt != functionProviders.end())
      {
        // 関数を実行
        return functionIt->second(dataResults, functionName);
      }
      else
      {
        // 関数が見つからなかった場合
        return "Function not found";
      }
    }
  }
}
