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
#include <map>
#include <thread>
#include <future>

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
        std::cout << "/B/data: " << std::endl;
        for (size_t i = 1; i < 31; i += 3)
        {
          m_scheduler.schedule(time::seconds{i}, [this]
                               { sendInterest("/B/data"); });
        }

        m_scheduler.schedule(time::seconds{31}, [this]
                             { std::cout << "/A/func/(/B/data, /C/data): " << std::endl; });
        for (size_t i = 31; i < 61; i += 3)
        {
          m_scheduler.schedule(time::seconds{i}, [this]
                               { sendInterest("/A/func/(/B/data, /C/data)"); });
        }

        m_scheduler.schedule(time::seconds{61}, [this]
                             { std::cout << "/A/func/( /B/func/(/B/data, /C/data) ): " << std::endl; });
        for (size_t i = 61; i < 161; i += 10)
        {
          m_scheduler.schedule(time::seconds{i}, [this]
                               { sendInterest("/A/func/( /B/func/(/B/data, /C/data) )"); });
        }

        m_scheduler.schedule(time::seconds{161}, [this]
                             { std::cout << "/A/func/( /B/func/(/B/data, /C/data), /C/func/(/B/data, /C/data) ): " << std::endl; });
        for (size_t i = 161; i < 261; i += 10)
        {
          m_scheduler.schedule(time::seconds{i}, [this]
                               { sendInterest("/A/func/( /B/func/(/B/data, /C/data), /C/func/(/B/data, /C/data) )"); });
        }

        m_scheduler.schedule(time::seconds{261}, [this]
                             { std::cout << "/A/func/( /A/func/(/B/data, /C/data), /B/func/(/B/data, /C/data), /C/func/(/B/data, /C/data) ): " << std::endl; });
        for (size_t i = 261; i < 361; i += 10)
        {
          m_scheduler.schedule(time::seconds{i}, [this]
                               { sendInterest("/A/func/( /A/func/(/B/data, /C/data), /B/func/(/B/data, /C/data), /C/func/(/B/data, /C/data) )"); });
        }

        m_ioService.run();
      }

    private:
      void excuteAsync(std::function<void()> callback = [] {})
      {
        futures.push_back(std::async(std::launch::async, [callback]
                                     {
                                       callback(); // コールバック実行
                                     }));
      }

    private:
      void
      onData(const Interest &interest, const Data &data) const
      {
        // std::cout << "Received Data " << data << std::endl;

        if (data.hasContent())
        {
          const Block &content = data.getContent();
          const uint8_t *buffer = content.value();
          size_t size = content.value_size();
          std::string contentStr(reinterpret_cast<const char *>(buffer), size);
          myLog("コンテンツを受け取りました！: " + contentStr);
          myLog("URL: " + interest.getName().toUri());

          // Interest 名に対応する開始時刻をマップから取得
          auto it = start_times.find(interest.getName().toUri());
          if (it != start_times.end())
          {
            auto start_time = it->second;
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start_time;
            std::cout << "経過時間: " << elapsed.count() << " ms\n";

            // 使用した時刻をマップから削除
            start_times.erase(it);
          }
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
        interestName.appendVersion();

        Interest interest(interestName);
        interest.setMustBeFresh(true);
        interest.setInterestLifetime(100_s);

        myLog("Interest を送信しました。\nURL: " + urlDecodeAndTrim(interest.getName().toUri()));

        // この Interest の開始時刻をマップに記録
        start_times[interest.getName().toUri()] = std::chrono::high_resolution_clock::now();

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
      std::vector<std::future<void>> futures;
      mutable std::map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> start_times;
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