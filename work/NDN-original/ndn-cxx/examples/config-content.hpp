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

    using ContentProvider = std::function<std::string(const std::string &)>;
    using ContentProviderMap = std::unordered_map<std::string, ContentProvider>;

    inline const ContentProviderMap contentProviders = {
        {
            "/A/data",
            [](const std::string &url) -> std::string
            {
              return "Vhsg sgd qzohc hmbqdzrd hm sgd mtladq ne HnS cduhbdr hm qdbdms xdzqr, sgd deedbshud tshkhyzshnm zmc lzmzfdldms ne sgdrd cduhbdr gzr adbnld hmbqdzrhmfkx hlonqszms. Hm ozqshbtkzq, HnS sdbgmnknfx hr adhmf trdc hm z vhcd qzmfd ne zookhbzshnmr, hmbktchmf rlzqs bhshdr, sgd ztsnlnshud hmctrsqx, zmc sgd ldchbzk ehdkc, vghbg qdpthqd kzqfd zlntmsr ne czsz zmc ghfg-roddc oqnbdrrhmf fdmdqzsdc ax sghr sdbgmnknfx.";
            },
        },
        {
            "/B/data",
            [](const std::string &url) -> std::string
            {
              return "Vhsg sgd qzohc hmbqdzrd hm sgd mtladq ne HnS cduhbdr hm qdbdms xdzqr, sgd deedbshud tshkhyzshnm zmc lzmzfdldms ne sgdrd cduhbdr gzr adbnld hmbqdzrhmfkx hlonqszms. Hm ozqshbtkzq, HnS sdbgmnknfx hr adhmf trdc hm z vhcd qzmfd ne zookhbzshnmr, hmbktchmf rlzqs bhshdr, sgd ztsnlnshud hmctrsqx, zmc sgd ldchbzk ehdkc, vghbg qdpthqd kzqfd zlntmsr ne czsz zmc ghfg-roddc oqnbdrrhmf fdmdqzsdc ax sghr sdbgmnknfx.";
            },
        },
        {
            "/C/data",
            [](const std::string &url) -> std::string
            {
              std::string imagePath = "/work/experimental_data/mandrill.bmp";
              std::string encodedImage = encodeImageToBase64(imagePath).substr(0, 300);
              return encodedImage;
            },
        },
        {
            "/D/data",
            [](const std::string &url) -> std::string
            {
              std::string imagePath = "/work/experimental_data/mandrill.bmp";
              std::string encodedImage = encodeImageToBase64(imagePath).substr(0, 500);
              return encodedImage;
            },
        },
        {
            "/E/data",
            [](const std::string &url) -> std::string
            {
              return "!!!E_DATA!!!";
            },
        },
        {
            "/F/data",
            [](const std::string &url) -> std::string
            {
              return "!!!F_DATA!!!";
            },
        },
        {
            "/G/data",
            [](const std::string &url) -> std::string
            {
              return "!!!G_DATA!!!";
            },
        },
        {
            "/H/data",
            [](const std::string &url) -> std::string
            {
              return "!!!H_DATA!!!";
            },
        },
    };

  } // namespace examples
} // namespace ndn
