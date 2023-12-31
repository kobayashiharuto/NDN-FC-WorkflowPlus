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

#include "ndn-cxx/security/transform/strip-space.hpp"

namespace ndn::security::transform {

const char* const StripSpace::DEFAULT_WHITESPACES = " \f\n\r\t\v";

StripSpace::StripSpace(const char* whitespaces)
{
  for (const char* i = whitespaces; *i != '\0'; ++i) {
    m_isWhitespace.set(*i);
  }
}

size_t
StripSpace::convert(span<const uint8_t> data)
{
  auto buffer = make_unique<OBuffer>();
  buffer->reserve(data.size());

  for (auto ch : data) {
    if (!m_isWhitespace[ch]) {
      buffer->push_back(ch);
    }
  }

  setOutputBuffer(std::move(buffer));
  return data.size();
}

unique_ptr<Transform>
stripSpace(const char* whitespaces)
{
  return make_unique<StripSpace>(whitespaces);
}

} // namespace ndn::security::transform
