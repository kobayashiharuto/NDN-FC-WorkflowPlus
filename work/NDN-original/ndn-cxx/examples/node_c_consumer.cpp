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
      bool disableCache = false;

    public:
      void
      run()
      {
        m_scheduler.schedule(1_s, [this]
                             { sendInterest("/X/func/( /B/data )"); });

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

      // データを受け取った際のセグメント対応などを行い、最終的にデータを結合した結果をコールバック関数を呼び出す
      // データがセグメンテーション分割されている場合は、それそれのセグメントにリクエストを行い、それぞれのデータが返ってくるたび onData を呼び出す
      // そして、最後のセグメントが返ってきたら、データを結合してコールバック関数を呼び出す形
    public:
      void onData(const Interest &interest, const ndn::Data &data)
      {
        std::string decodedUrl = urlDecodeAndTrim(interest.getName().toUri());
        std::cout << "Url: " << decodedUrl << std::endl;
        std::cout << "Segment No.: " << data.getName().get(-1).toSegment() << std::endl;
        std::cout << "Final Block No.: " << data.getFinalBlock()->toSegment() << std::endl;

        // Add Segments to Buffer
        std::cout << "Loading receiveBuffer: " << data.getName().get(-1).toSegment() << std::endl;
        std::string dataName = urlDecodeAndTrim(data.getName().getPrefix(-1).toUri()); // セグメント番号を除いた名前の文字列を取得
        receiveBuffer[dataName][data.getName().get(-1).toSegment()] = data.shared_from_this();

        // セグメント番号が0のときは final block id が返ってくるタイプなので、Prefixとファイル名を取得
        if (data.getName().get(-1).toSegment() == 0)
        {
          incomingFinalBlockNumber[dataName] = data.getFinalBlock()->toSegment();

          // もし、FinalBlockNumberが0以上の場合、それぞれのセグメントをInterestで要求する
          if (incomingFinalBlockNumber[dataName] > 0)
          {
            for (uint64_t i = 1; i <= incomingFinalBlockNumber[dataName]; i++)
            {
              Name name(dataName);
              name.appendSegment(i);
              Interest newInterest(name);
              newInterest.setInterestLifetime(interest.getInterestLifetime());
              newInterest.setMustBeFresh(interest.getMustBeFresh());
              std::cout << "Sending Interests: " << name << std::endl;
              m_face.expressInterest(
                  newInterest,
                  [this](const Interest &interest, const ndn::Data &data)
                  {
                    onData(interest, data);
                  },
                  std::bind(&MyConsumer::onNack, this, _1, _2),
                  std::bind(&MyConsumer::onTimeout, this, _1));
            }
          }
        }

        // 最後のセグメント番号と同じ数だけデータを受け取ったら、データを結合
        if (receiveBuffer[dataName].size() == incomingFinalBlockNumber[dataName] + 1)
        {
          std::cout << "全データが揃いました: " << dataName << std::endl;
          // データの結合
          std::string combinedData;
          for (const auto &segment : receiveBuffer[dataName])
          {
            const std::string segmentContent = dataContentToString(*(segment.second));
            combinedData += segmentContent; // 各セグメントを文字列に変換して結合
          }

          if (combinedData.size() < 1000)
          {
            myLog("コンテンツを受け取りました！: " + combinedData);
          }
          else
          {
            myLog("コンテンツを受け取りました！: " + combinedData.substr(0, 1000) + "...");
          }

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
          // バッファをクリア
          receiveBuffer[dataName].clear();
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
        interestName.appendSegment(0);

        Interest interest(interestName);
        interest.setMustBeFresh(true);
        interest.setInterestLifetime(100_s);

        myLog("Interest を送信しました。");

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
      std::map<std::string, std::map<uint64_t, std::shared_ptr<const Data>>> receiveBuffer;
      std::map<std::string, uint64_t> incomingFinalBlockNumber;
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