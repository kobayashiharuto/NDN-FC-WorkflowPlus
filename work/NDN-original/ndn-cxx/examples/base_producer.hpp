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
      bool disableCache = false;
      Face m_face;
      KeyChain m_keyChain;
      std::vector<std::future<void>> futures;
      ScopedRegisteredPrefixHandle m_certServeHandle;

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
        ndn::Data data;

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
        auto data = getContent(interest);

        data->setName(interest.getName());
        data->setFreshnessPeriod(10_s);

        m_keyChain.sign(*data);
        m_face.put(*data);
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
        std::string url = urlDecodeAndTrim(interest.getName().toUri());
        std::string functionName = extractFunctionName(url);

        myLog("function リクエストとして処理をします。\nURL: " + url + "\n関数名: " + functionName);

        // URL を解析し、引数として含まれる URL のリストを取得
        std::vector<std::string> args = parseFunctionArgs(url);

        // データを集めるための一時的な場所を用意
        auto dataList = std::make_shared<std::vector<Data>>(args.size());

        // データが揃ったかどうかを確認するためのカウンタ
        auto counter = std::make_shared<std::atomic<size_t>>(0);

        // 引数の数
        size_t total = args.size();

        // データを集めて、全部揃ったら関数を実行し、結果を返す
        for (size_t i = 0; i < args.size(); ++i)
        {
          myLog("データを取得します。\nURL: " + args[i]);
          sendInterest(args[i], [this, i, interest, url, functionName, dataList, counter, total](const ndn::Data &data)
                       {
                         auto dataStr = dataContentToString(data);
                         myLog("データ取得完了: " + dataStr + "\nURL: " + url + "\n");

                         (*counter)++;
                         (*dataList)[i] = data;

                         myLog("カウンタ: " + std::to_string(*counter) + "\n");
                         myLog("トータル: " + std::to_string(total) + "\n");

                         // カウンタをインクリメントして、全てのデータが揃ったかどうかを確認
                         if (*counter == total)
                         {
                           // 全てのデータが揃ったら executeFunction を呼び出す
                           myLog("全てのデータが揃いました。");
                           executeAsync([this, dataList, functionName, interest]
                                        {
                                          auto functionResultData = executeFunction(*dataList, functionName);
                                          myLog("関数の実行結果を受け取りました。");

                                          // 送信
                                          functionResultData->setName(interest.getName());
                                          functionResultData->setFreshnessPeriod(10_s);

                                          this->m_keyChain.sign(*functionResultData);
                                          this->m_face.put(*functionResultData);
                                          //
                                        });
                         }
                         //
                       });
        }
      }

    public:
      void sendInterest(const std::string &name, const std::function<void(const ndn::Data &)> &callback)
      {
        Name interestName(name);

        if (disableCache)
        {
          interestName.appendVersion();
        }

        Interest interest(interestName);

        if (disableCache)
        {
          interest.setMustBeFresh(true);
        }
        else
        {
          interest.setMustBeFresh(false);
        }

        interest.setInterestLifetime(100_s);

        myLog("Interest を送信しました。\nURL: " + urlDecodeAndTrim(interest.getName().toUri()));
        m_face.expressInterest(
            interest,
            [this, callback](const Interest &interest, const ndn::Data &data)
            {
              onData(interest, data, callback);
            },
            std::bind(&BaseProducer::onNack, this, _1, _2),
            std::bind(&BaseProducer::onTimeout, this, _1));
      }

    public:
      void onData(const Interest &interest, const ndn::Data &data, const std::function<void(const ndn::Data &)> &callback) const
      {
        // std::cout << "Received Data " << data << std::endl;

        if (data.hasContent())
        {
          myLog("コンテンツを受け取りました！");
          // コールバック関数を呼び出してデータを渡す
          callback(data);
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

      void
      onRegisterFailed(const Name &prefix, const std::string &reason)
      {
        std::cerr << "ERROR: Failed to register prefix '" << prefix
                  << "' with the local forwarder (" << reason << ")\n";
        m_face.shutdown();
      }
    };
  } // namespace examples
} // namespace ndn
