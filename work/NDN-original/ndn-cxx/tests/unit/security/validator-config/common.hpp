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

#ifndef NDN_CXX_TESTS_UNIT_SECURITY_VALIDATOR_CONFIG_COMMON_HPP
#define NDN_CXX_TESTS_UNIT_SECURITY_VALIDATOR_CONFIG_COMMON_HPP

#include "ndn-cxx/security/validator-config/common.hpp"

#include <boost/property_tree/info_parser.hpp>

namespace ndn::tests {

using ndn::security::validator_config::ConfigSection;

inline ConfigSection
makeSection(const std::string& config)
{
  std::istringstream inputStream(config);
  ConfigSection section;
  boost::property_tree::read_info(inputStream, section);
  return section;
}

} // namespace ndn::tests

#endif // NDN_CXX_TESTS_UNIT_SECURITY_VALIDATOR_CONFIG_COMMON_HPP
