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

#ifndef NDN_CXX_SECURITY_SIGNING_HELPERS_HPP
#define NDN_CXX_SECURITY_SIGNING_HELPERS_HPP

#include "ndn-cxx/security/signing-info.hpp"

namespace ndn {
namespace security {

/**
 * \brief Return a SigningInfo for signing with an identity.
 */
SigningInfo
signingByIdentity(const Name& identityName);

/**
 * \brief Return a SigningInfo for signing with an identity.
 */
SigningInfo
signingByIdentity(const Identity& identity);

/**
 * \brief Return a SigningInfo for signing with a key.
 */
SigningInfo
signingByKey(const Name& keyName);

/**
 * \brief Return a SigningInfo for signing with a key.
 */
SigningInfo
signingByKey(const Key& key);

/**
 * \brief Return a SigningInfo for signing with a certificate.
 */
SigningInfo
signingByCertificate(const Name& certName);

/**
 * \brief Return a SigningInfo for signing with a certificate.
 */
SigningInfo
signingByCertificate(const Certificate& cert);

/**
 * \brief Return a SigningInfo for signing with a SHA-256 digest.
 */
SigningInfo
signingWithSha256();

} // namespace security

using security::signingByIdentity;
using security::signingByKey;
using security::signingByCertificate;
using security::signingWithSha256;

} // namespace ndn

#endif // NDN_CXX_SECURITY_SIGNING_HELPERS_HPP
