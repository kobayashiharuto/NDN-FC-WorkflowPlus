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

    using ContentProvider = std::function<std::shared_ptr<ndn::Data>(const std::string &)>;
    using ContentProviderMap = std::unordered_map<std::string, ContentProvider>;

    inline const ContentProviderMap contentProviders = {
        {
            "/B/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("!!!B_DATA!!!");
              return data;
            },
        },
        {
            "/C/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("!!!C_DATA!!!");
              return data;
            },
        },
        // 他のコンテンツ提供関数を追加する
    };

  } // namespace examples
} // namespace ndn
