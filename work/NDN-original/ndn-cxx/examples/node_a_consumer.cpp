/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2022 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/util/scheduler.hpp>

#include <boost/asio/io_service.hpp>
#include <iostream>
#include <chrono>

#include "my-utils.hpp"
using ndn::examples::myLog;
using ndn::examples::urlDecodeAndTrim;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn
{
  // Additional nested namespaces should be used to prevent/limit name conflicts
  namespace examples
  {

    class MyConsumer
    {
    public:
      void
      run()
      {
        m_scheduler.schedule(1_s, [this]
                             { sendInterest("/B/data"); });
        m_scheduler.schedule(2_s, [this]
                             { sendInterest("/C/data"); });
        m_scheduler.schedule(3_s, [this]
                             { sendInterest("/D/data"); });
        m_scheduler.schedule(4_s, [this]
                             { sendInterest("/E/data"); });
        m_scheduler.schedule(5_s, [this]
                             { sendInterest("/F/data"); });
        m_scheduler.schedule(6_s, [this]
                             { sendInterest("/X/func/( /F/data, /D/data )"); });
        m_scheduler.schedule(7_s, [this]
                             { sendInterest("/X/func/( /F/data, /Y/func/( /F/data, /D/data ) )"); });
        m_scheduler.schedule(8_s, [this]
                             { sendInterest("/X/func/( /F/data, /Y/func/( /F/data, /Z/func/( /F/data, /Y/func/( /F/data, /D/data ) ) ) )"); });
        m_scheduler.schedule(15_s, [this]
                             { sendInterest("/X/func/( /F/data, /Y/func/( /F/data, /Z/func/( /F/data, /Y/func/( /F/data, /D/data ) ) ) )"); });

        m_ioService.run();
      }

    private:
      void
      onData(const Interest &, const Data &data) const
      {
        // std::cout << "Received Data " << data << std::endl;

        if (data.hasContent())
        {
          const Block &content = data.getContent();
          const uint8_t *buffer = content.value();
          size_t size = content.value_size();
          std::string contentStr(reinterpret_cast<const char *>(buffer), size);
          myLog("コンテンツを受け取りました！: " + contentStr);

          auto end = std::chrono::high_resolution_clock::now();
          // 経過時間を計算（ミリ秒単位）
          std::chrono::duration<double, std::milli> elapsed = end - start_time;
          std::cout << "経過時間: " << elapsed.count() << " ms\n";
        }
      }

      void
      onNack(const Interest &interest, const lp::Nack &nack) const
      {
        std::cout << "Received Nack with reason " << nack.getReason()
                  << " for " << interest << std::endl;
      }

      void
      onTimeout(const Interest &interest) const
      {
        std::cout << "Timeout for " << interest << std::endl;
      }

      void sendInterest(std::string name)
      {
        Name interestName(name);
        // interestName.appendVersion();

        Interest interest(interestName);
        interest.setMustBeFresh(false);
        interest.setInterestLifetime(100_s);

        myLog("Interest を送信しました。\nURL: " + urlDecodeAndTrim(interest.getName().toUri()));
        start_time = std::chrono::high_resolution_clock::now();
        m_face.expressInterest(interest,
                               std::bind(&MyConsumer::onData, this, _1, _2),
                               std::bind(&MyConsumer::onNack, this, _1, _2),
                               std::bind(&MyConsumer::onTimeout, this, _1));
      }

    private:
      // Explicitly create io_service object, which will be shared between Face and Scheduler
      boost::asio::io_service m_ioService;
      Face m_face{m_ioService};
      Scheduler m_scheduler{m_ioService};
      std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    };

  } // namespace examples
} // namespace ndn

int main(int argc, char **argv)
{
  try
  {
    ndn::examples::MyConsumer consumer;
    consumer.run();
    return 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}