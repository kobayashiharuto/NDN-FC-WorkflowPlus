/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2022,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NFD_TESTS_DAEMON_FACE_FACTORY_TEST_COMMON_HPP
#define NFD_TESTS_DAEMON_FACE_FACTORY_TEST_COMMON_HPP

#include "face/protocol-factory.hpp"

#include "tests/test-common.hpp"

namespace nfd::tests {

struct CreateFaceExpectedResult
{
  enum { FAILURE, SUCCESS } result;
  uint32_t status;
  std::string reason;
};

inline void
createFace(face::ProtocolFactory& factory,
           const FaceUri& remoteUri,
           const std::optional<FaceUri>& localUri,
           const face::FaceParams& params,
           const CreateFaceExpectedResult& expected,
           const std::function<void(const Face&)>& extraChecks = nullptr)
{
  factory.createFace({remoteUri, localUri, params},
                     [result = expected.result, extraChecks] (const auto& face) {
                       BOOST_CHECK_EQUAL(CreateFaceExpectedResult::SUCCESS, result);
                       if (extraChecks) {
                         extraChecks(*face);
                       }
                     },
                     [expected] (uint32_t actualStatus, const auto& actualReason) {
                       BOOST_CHECK_EQUAL(CreateFaceExpectedResult::FAILURE, expected.result);
                       BOOST_CHECK_EQUAL(actualStatus, expected.status);
                       BOOST_CHECK_EQUAL(actualReason, expected.reason);
                     });
}

/**
 * \brief Check that channels in a factory equal given channel URIs
 */
inline void
checkChannelListEqual(const face::ProtocolFactory& factory, std::set<std::string> expectedUris)
{
  for (const auto& channel : factory.getChannels()) {
    auto uri = channel->getUri().toString();
    if (expectedUris.erase(uri) == 0) {
      BOOST_ERROR("Unexpected channel " << uri);
    }
  }
  for (const auto& uri : expectedUris) {
    BOOST_ERROR("Missing channel " << uri);
  }
}

} // namespace nfd::tests

#endif // NFD_TESTS_DAEMON_FACE_FACTORY_TEST_COMMON_HPP
