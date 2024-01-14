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
            "/A/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("21.4");
              return data;
            },
        },
        {
            "/B/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("22.6");
              return data;
            },
        },
        {
            "/C/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("10.3");
              return data;
            },
        },
        {
            "/D/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("-1.2");
              return data;
            },
        },
        {
            "/E/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("-15.1");
              return data;
            },
        },
        {
            "/F/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("10.1");
              return data;
            },
        },
        {
            "/G/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("29.1");
              return data;
            },
        },
        {
            "/H/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("3.0");
              return data;
            },
        },
    };

  } // namespace examples
} // namespace ndn
