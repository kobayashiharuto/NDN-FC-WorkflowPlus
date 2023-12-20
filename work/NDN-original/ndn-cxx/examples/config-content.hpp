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
        {
            "/D/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("!!!D_DATA!!!");
              return data;
            },
        },
        {
            "/E/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("!!!E_DATA!!!");
              return data;
            },
        },
        {
            "/F/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("!!!F_DATA!!!");
              return data;
            },
        },
    };

  } // namespace examples
} // namespace ndn
