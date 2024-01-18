#include <iostream>
#include <string>
#include <vector>

#include <ndn-cxx/face.hpp>

#include "my-utils.hpp"
#include "config-content.hpp"

using ndn::examples::dataContentToString;
using ndn::examples::myLog;

namespace ndn
{
  namespace examples
  {
    std::string getContent(const ndn::Interest &interest)
    {
      std::string url = urlDecodeAndTrim(interest.getName().getPrefix(-1).toUri());
      // マップからコンテンツを取得
      auto providerIt = contentProviders.find(url);
      if (providerIt != contentProviders.end())
      {
        return providerIt->second(url);
      }
      else
      {
        return "not found";
      }
    }
  }
}
