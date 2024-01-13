#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
#include <ndn-cxx/data.hpp>

#include <ndn-cxx/face.hpp>

#include "my-utils.hpp"
#include "base64.hpp"

using ndn::examples::dataContentToString;
using ndn::examples::encodeImageToBase64;
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

              std::string imagePath = "/work/experimental_data/mandrill.bmp";
              std::string encodedImage = encodeImageToBase64(imagePath);
              data->setContent(encodedImage);
              return data;
            },
        },
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
        {
            "/G/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("!!!G_DATA!!!");
              return data;
            },
        },
        {
            "/H/data",
            [](const std::string &url) -> std::shared_ptr<ndn::Data>
            {
              auto data = std::make_shared<ndn::Data>();
              data->setContent("!!!H_DATA!!!");
              return data;
            },
        },
    };

  } // namespace examples
} // namespace ndn
