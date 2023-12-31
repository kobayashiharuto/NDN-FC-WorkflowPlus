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

#include "ndn-cxx/security/signing-helpers.hpp"

#include "tests/boost-test.hpp"

namespace ndn::tests {

using ndn::security::SigningInfo;

BOOST_AUTO_TEST_SUITE(Security)
BOOST_AUTO_TEST_SUITE(TestSigningHelpers)

BOOST_AUTO_TEST_CASE(Identity)
{
  Name identity("/identity");
  SigningInfo info = signingByIdentity(identity);
  BOOST_CHECK_EQUAL(info.getSignerType(), SigningInfo::SIGNER_TYPE_ID);
  BOOST_CHECK_EQUAL(info.getSignerName(), identity);
}

BOOST_AUTO_TEST_CASE(Key)
{
  Name key("/key");
  SigningInfo info = signingByKey(key);
  BOOST_CHECK_EQUAL(info.getSignerType(), SigningInfo::SIGNER_TYPE_KEY);
  BOOST_CHECK_EQUAL(info.getSignerName(), key);
}

BOOST_AUTO_TEST_CASE(Certificate)
{
  Name cert("/cert");
  SigningInfo info = signingByCertificate(cert);
  BOOST_CHECK_EQUAL(info.getSignerType(), SigningInfo::SIGNER_TYPE_CERT);
  BOOST_CHECK_EQUAL(info.getSignerName(), cert);
}

BOOST_AUTO_TEST_CASE(Sha256)
{
  SigningInfo info = signingWithSha256();
  BOOST_CHECK_EQUAL(info.getSignerType(), SigningInfo::SIGNER_TYPE_SHA256);
  BOOST_CHECK_EQUAL(info.getSignerName(), Name());
}

BOOST_AUTO_TEST_SUITE_END() // TestSigningHelpers
BOOST_AUTO_TEST_SUITE_END() // Security

} // namespace ndn::tests
