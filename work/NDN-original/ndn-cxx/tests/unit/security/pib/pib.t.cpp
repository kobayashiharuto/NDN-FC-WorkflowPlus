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

#include "ndn-cxx/security/pib/pib.hpp"
#include "ndn-cxx/security/pib/impl/pib-memory.hpp"

#include "tests/boost-test.hpp"
#include "tests/unit/security/pib/pib-data-fixture.hpp"

namespace ndn::tests {

using namespace ndn::security::pib;

BOOST_AUTO_TEST_SUITE(Security)
BOOST_FIXTURE_TEST_SUITE(TestPib, PibDataFixture)

BOOST_AUTO_TEST_CASE(TpmLocator)
{
  Pib pib("pib-memory:", make_shared<PibMemory>());

  BOOST_CHECK_EQUAL(pib.getPibLocator(), "pib-memory:");
  BOOST_CHECK_EQUAL(pib.getTpmLocator(), "");

  pib.setTpmLocator("test-tpm-locator");
  BOOST_CHECK_EQUAL(pib.getTpmLocator(), "test-tpm-locator");

  BOOST_CHECK_THROW(pib.getIdentity(id1), Pib::Error);
  pib.addIdentity(id1);
  BOOST_CHECK_NO_THROW(pib.getIdentity(id1));

  pib.setTpmLocator("another-tpm-locator");
  BOOST_CHECK_THROW(pib.getIdentity(id1), Pib::Error);
  BOOST_CHECK_EQUAL(pib.getTpmLocator(), "another-tpm-locator");

  pib.addIdentity(id1);
  BOOST_CHECK_NO_THROW(pib.getIdentity(id1));
  pib.reset();
  BOOST_CHECK_THROW(pib.getIdentity(id1), Pib::Error);
  BOOST_CHECK_EQUAL(pib.getTpmLocator(), "");
}

BOOST_AUTO_TEST_CASE(IdentityOperations)
{
  Pib pib("pib-memory:", make_shared<PibMemory>());

  // PIB starts with no identities
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 0);
  BOOST_CHECK_THROW(pib.getIdentity(id1), Pib::Error);
  // get default identity when it is not set yet, throw Pib::Error
  BOOST_CHECK_THROW(pib.getDefaultIdentity(), Pib::Error);

  // add identity
  pib.addIdentity(id1);
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 1);
  BOOST_CHECK_EQUAL(pib.getIdentity(id1).getName(), id1);
  // add another
  pib.addIdentity(id2);
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 2);
  BOOST_CHECK_EQUAL(pib.getIdentity(id2).getName(), id2);

  // first identity implicitly becomes the default when there was no default identity
  BOOST_CHECK_EQUAL(pib.getDefaultIdentity().getName(), id1);

  // remove both identities
  pib.removeIdentity(id2);
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 1);
  BOOST_CHECK_EQUAL(pib.getIdentity(id1).getName(), id1);
  BOOST_CHECK_THROW(pib.getIdentity(id2), Pib::Error);
  BOOST_CHECK_EQUAL(pib.getDefaultIdentity().getName(), id1);
  pib.removeIdentity(id1);
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 0);
  BOOST_CHECK_THROW(pib.getIdentity(id1), Pib::Error);
  BOOST_CHECK_THROW(pib.getDefaultIdentity(), Pib::Error);

  // set default identity (and implicitly create it)
  pib.setDefaultIdentity(id1);
  BOOST_CHECK_EQUAL(pib.getDefaultIdentity().getName(), id1);
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 1);
  pib.setDefaultIdentity(id2);
  BOOST_CHECK_EQUAL(pib.getDefaultIdentity().getName(), id2);
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 2);

  // remove default identity
  pib.removeIdentity(id2);
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 1);
  BOOST_CHECK_EQUAL(pib.getIdentity(id1).getName(), id1);
  BOOST_CHECK_THROW(pib.getIdentity(id2), Pib::Error);
  BOOST_CHECK_THROW(pib.getDefaultIdentity(), Pib::Error);

  // adding an identity now makes it the default
  pib.addIdentity(id2);
  BOOST_CHECK_EQUAL(pib.getIdentities().size(), 2);
  BOOST_CHECK_EQUAL(pib.getDefaultIdentity().getName(), id2);
}

BOOST_AUTO_TEST_SUITE_END() // TestPib
BOOST_AUTO_TEST_SUITE_END() // Security

} // namespace ndn::tests
