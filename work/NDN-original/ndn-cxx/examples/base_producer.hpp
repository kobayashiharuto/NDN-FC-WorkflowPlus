/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2023 Regents of the University of California.
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
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/signing-helpers.hpp>

#include <iostream>
#include <mutex>
#include <memory>
#include <map>
#include <vector>
#include <thread>
#include <future>

#include "my-utils.hpp"
#include "provide-content.hpp"
#include "provide-function.hpp"

using ndn::examples::dataContentToString;
using ndn::examples::executeFunction;
using ndn::examples::extractFirstLevelArgs;
using ndn::examples::extractFunctionName;
using ndn::examples::getContent;
using ndn::examples::myLog;
using ndn::examples::urlDecodeAndTrim;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn
{
  // Additional nested namespaces should be used to prevent/limit name conflicts
  namespace examples
  {
    class BaseProducer
    {
    public:
      virtual ~BaseProducer() {} // 仮想デストラクタを追加

    public:
      virtual void run() = 0;

    public:
      void executeAsync(std::function<void()> callback = [] {})
      {
        futures.push_back(std::async(std::launch::async, [callback]
                                     {
                                       callback(); // コールバック実行
                                     }));
      }

    public:
      void
      onInterest(const Interest &interest)
      {
        std::cout << ">> I: " << interest << std::endl;

        myLog("Interest を受信しました。\nURL: " + interest.getName().toUri());

        // [前提知識]:
        // 関数として振る舞うことを期待されているリクエストを function リクエストと呼ぶ
        // 対して、データとして振る舞うことを期待されているリクエストを data リクエストと呼ぶ
        if (isFunctionRequest(interest))
        {
          processFunctionRequest(interest);
        }
        else
        {
          processDataRequest(interest);
        }
      }

      // name 構造から、function リクエストかを調べる
      // /A/(〇〇) のような構造であれば function リクエストなので、/(が存在すれば関数と判断する
      // そうでなければ、data リクエストなので、データを返す
    public:
      bool isFunctionRequest(const ndn::Interest &interest)
      {
        std::string url = urlDecodeAndTrim(interest.getName().toUri());
        return (url.find("/(") != std::string::npos);
      }

      // data リクエストだったときの処理
    public:
      void processDataRequest(const ndn::Interest &interest)
      {
        std::string dataName = urlDecodeAndTrim(interest.getName().getPrefix(-1).toUri());
        uint64_t segment = interest.getName().get(-1).toSegment();

        // セグメントが0でない場合は、そのセグメントのデータを送信
        if (segment > 0)
        {
          std::cout << "セグメント分割済みのデータを送信します: Seg No." << segment << std::endl;
          m_keyChain.sign(*(dataSegments[dataName][segment]));
          m_face.put(*(dataSegments[dataName][segment]));
        }
        else
        { // セグメントが0の時はセグメント分割して格納し、final block ID を返信
          myLog("final block ID を送信します");
          auto data = getContent(interest);
          auto segmentedData = segmentData(data, dataName);
          dataSegments[dataName] = segmentedData;

          auto initData = segmentedData[0];
          m_keyChain.sign(*initData);
          m_face.put(*initData);
        }
      }

      // URLをパース(渡すURLはすでにデコード済みである必要がある)
    public:
      std::vector<std::string> parseFunctionArgs(const std::string &decodedUrl)
      {
        std::vector<std::string> args = extractFirstLevelArgs(decodedUrl);
        return args;
      }

    public:
      void processFunctionRequest(const ndn::Interest &interest)
      {
        std::string url = urlDecodeAndTrim(interest.getName().getPrefix(-1).toUri());
        uint64_t segment = interest.getName().get(-1).toSegment();

        // セグメントが0でない場合は、そのセグメントのデータを送信
        if (segment > 0)
        {
          m_keyChain.sign(*(dataSegments[url][segment]));
          m_face.put(*(dataSegments[url][segment]));
          std::cout << "Functioned Sending Data for Seg.: " << segment << std::endl;
          return;
        }

        // セグメントが0の時はデータを集めて関数にかけて、セグメント分割して格納し、final block ID を返信
        std::string functionName = extractFunctionName(url);

        myLog("function リクエストとして処理をします。\nURL: " + url + "\n関数名: " + functionName);

        // URL を解析し、引数として含まれる URL のリストを取得
        std::vector<std::string> args = parseFunctionArgs(url);

        // データを集めるための一時的な場所を用意
        auto dataList = std::make_shared<std::vector<std::string>>(args.size());

        // データが揃ったかどうかを確認するためのカウンタ
        auto counter = std::make_shared<std::atomic<size_t>>(0);

        // 引数の数
        size_t total = args.size();

        // データを集めて、全部揃ったら関数を実行し、結果を返す
        for (size_t i = 0; i < args.size(); ++i)
        {
          myLog("データを取得します。\nURL: " + args[i]);
          sendInterest(args[i], [this, i, interest, url, functionName, dataList, counter, total](const std::string &data)
                       {
                         myLog("データ取得完了: " + data + "\nURL: " + url + "\n");

                         (*counter)++;
                         (*dataList)[i] = data;

                         myLog("カウンタ: " + std::to_string(*counter) + "\n");
                         myLog("トータル: " + std::to_string(total) + "\n");

                         // カウンタをインクリメントして、全てのデータが揃ったかどうかを確認
                         if (*counter == total)
                         {
                           // 全てのデータが揃ったら executeFunction を呼び出す
                           myLog("全てのデータが揃いました。");
                           executeAsync([this, dataList, url, functionName, interest]
                                        {
                                          auto functionResultData = executeFunction(*dataList, functionName);
                                          myLog("関数の実行結果を受け取りました。");

                                          myLog("データをセグメント化します");
                                          auto segmentedData = segmentData(functionResultData, url);
                                          dataSegments[url] = segmentedData;

                                          auto data = segmentedData[0];
                                          m_keyChain.sign(*data);
                                          m_face.put(*data);
                                          //
                                        });
                         }
                         //
                       });
        }
      }

      // セグメント対応とかもやってくれるので、callbackでは結合されたデータが返ってくるよ
    public:
      void sendInterest(const std::string &name, const std::function<void(const std::string &)> &callback)
      {
        Name interestName(name);
        interestName.appendSegment(0);

        Interest interest(interestName);
        interest.setMustBeFresh(true);
        interest.setInterestLifetime(100_s);

        myLog("Interest を送信しました。");
        m_face.expressInterest(
            interest,
            [this, callback](const Interest &interest, const ndn::Data &data)
            {
              onData(interest, data, callback);
            },
            std::bind(&BaseProducer::onNack, this, _1, _2),
            std::bind(&BaseProducer::onTimeout, this, _1));
      }

      // データを受け取った際のセグメント対応などを行い、最終的にデータを結合した結果をコールバック関数を呼び出す
      // データがセグメンテーション分割されている場合は、それそれのセグメントにリクエストを行い、それぞれのデータが返ってくるたび onData を呼び出す
      // そして、最後のセグメントが返ってきたら、データを結合してコールバック関数を呼び出す形
    public:
      void onData(const Interest &interest, const ndn::Data &data, const std::function<void(const std::string &)> &callback)
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
                  [this, callback](const Interest &interest, const ndn::Data &data)
                  {
                    onData(interest, data, callback);
                  },
                  std::bind(&BaseProducer::onNack, this, _1, _2),
                  std::bind(&BaseProducer::onTimeout, this, _1));
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

          // コールバック関数を呼び出し
          callback(combinedData);

          // バッファをクリア
          receiveBuffer[dataName].clear();
        }
      }

    public:
      void
      onNack(const Interest &interest, const lp::Nack &nack) const
      {
        std::cout << "Received Nack with reason " << nack.getReason()
                  << " for " << interest << std::endl;
      }

    public:
      void
      onTimeout(const Interest &interest) const
      {
        std::cout << "Timeout for " << interest << std::endl;
      }

    public:
      void
      onRegisterFailed(const Name &prefix, const std::string &reason)
      {
        std::cerr << "ERROR: Failed to register prefix '" << prefix
                  << "' with the local forwarder (" << reason << ")\n";
        m_face.shutdown();
      }

      // 与えられたデータをセグメントに分割する
      std::vector<std::shared_ptr<Data>> segmentData(const std::string &inputData, const std::string &baseName)
      {
        std::vector<std::shared_ptr<Data>> dataSegment;
        Name dataName(baseName);
        Block encodedName = dataName.wireEncode();
        size_t nameSize = encodedName.size();
        size_t availableSpace = maxSegmentSize - nameSize;

        std::cout << "Available space for content: " << availableSpace << std::endl;

        std::string::const_iterator dataIterator = inputData.begin();

        while (dataIterator != inputData.end())
        {
          Name segmentName(dataName);
          size_t segmentDataSize = std::min(static_cast<size_t>(availableSpace), static_cast<size_t>(inputData.end() - dataIterator));
          std::vector<uint8_t> buffer(dataIterator, dataIterator + segmentDataSize);
          dataIterator += segmentDataSize;

          auto segment = std::make_shared<Data>(segmentName.appendSegment(dataSegment.size()));
          std::cout << "Preparing Data segment: " << segment->getName() << " Segment #: " << dataSegment.size() << std::endl;
          segment->setFreshnessPeriod(10_s);

          std::string_view content(reinterpret_cast<const char *>(buffer.data()), buffer.size());
          segment->setContent(content);
          dataSegment.push_back(segment);
        }

        // final block id を設定
        auto finalBlockId = name::Component::fromSegment(dataSegment.size() - 1);
        std::cout << "Final Block ID: " << finalBlockId.toSegment() << std::endl;
        for (auto &segment : dataSegment)
        {
          segment->setFinalBlock(finalBlockId);
        }

        return dataSegment;
      }

    public:
      std::vector<std::future<void>> futures;
      Face m_face;
      KeyChain m_keyChain;
      ScopedRegisteredPrefixHandle m_certServeHandle;
      std::size_t maxSegmentSize = 100;

      std::map<std::string, std::map<uint64_t, std::shared_ptr<const Data>>> receiveBuffer;
      std::map<std::string, std::vector<std::shared_ptr<Data>>> dataSegments;
      std::map<std::string, uint64_t> incomingFinalBlockNumber;
    }; // namespace examples
  }    // namespace ndn
}