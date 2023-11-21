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
    std::shared_ptr<ndn::Data> getContent(const ndn::Interest &interest)
    {
      std::string url = urlDecodeAndTrim(interest.getName().toUri());
      myLog("data リクエストとして処理をします。\nURL: " + url);

      // マップからコンテンツを取得
      auto providerIt = contentProviders.find(url);
      if (providerIt != contentProviders.end())
      {
        return providerIt->second(url);
      }
      else
      {
        auto data = std::make_shared<ndn::Data>();
        data->setContent("not found");
        return data;
      }
    }
  }
}
