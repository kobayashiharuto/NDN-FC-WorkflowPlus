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

#include "ndn-cxx/security/validity-period.hpp"
#include "ndn-cxx/encoding/block-helpers.hpp"

namespace ndn::security {

using boost::chrono::time_point_cast;

constexpr size_t ISO_DATETIME_SIZE = 15;
constexpr size_t NOT_BEFORE_OFFSET = 0;
constexpr size_t NOT_AFTER_OFFSET = 1;

ValidityPeriod
ValidityPeriod::makeRelative(time::seconds validFrom, time::seconds validUntil,
                             const time::system_clock::time_point& now)
{
  return ValidityPeriod(now + validFrom, now + validUntil);
}

ValidityPeriod::ValidityPeriod()
  : ValidityPeriod(time::system_clock::time_point() + 1_ns,
                   time::system_clock::time_point())
{
}

ValidityPeriod::ValidityPeriod(const time::system_clock::time_point& notBefore,
                               const time::system_clock::time_point& notAfter)
  : m_notBefore(time_point_cast<TimePoint::duration>(notBefore + TimePoint::duration(1) -
                                                     time::system_clock::time_point::duration(1)))
  , m_notAfter(time_point_cast<TimePoint::duration>(notAfter))
{
}

ValidityPeriod::ValidityPeriod(const Block& block)
{
  wireDecode(block);
}

template<encoding::Tag TAG>
size_t
ValidityPeriod::wireEncode(EncodingImpl<TAG>& encoder) const
{
  size_t totalLength = 0;

  totalLength += prependStringBlock(encoder, tlv::NotAfter, time::toIsoString(m_notAfter));
  totalLength += prependStringBlock(encoder, tlv::NotBefore, time::toIsoString(m_notBefore));

  totalLength += encoder.prependVarNumber(totalLength);
  totalLength += encoder.prependVarNumber(tlv::ValidityPeriod);
  return totalLength;
}

NDN_CXX_DEFINE_WIRE_ENCODE_INSTANTIATIONS(ValidityPeriod);

const Block&
ValidityPeriod::wireEncode() const
{
  if (m_wire.hasWire())
    return m_wire;

  EncodingEstimator estimator;
  size_t estimatedSize = wireEncode(estimator);

  EncodingBuffer buffer(estimatedSize, 0);
  wireEncode(buffer);

  m_wire = buffer.block();
  m_wire.parse();

  return m_wire;
}

void
ValidityPeriod::wireDecode(const Block& wire)
{
  if (!wire.hasWire()) {
    NDN_THROW(Error("The supplied block does not contain wire format"));
  }

  m_wire = wire;
  m_wire.parse();

  if (m_wire.type() != tlv::ValidityPeriod)
    NDN_THROW(Error("ValidityPeriod", m_wire.type()));

  if (m_wire.elements_size() != 2)
    NDN_THROW(Error("ValidityPeriod does not have two sub-TLVs"));

  if (m_wire.elements()[NOT_BEFORE_OFFSET].type() != tlv::NotBefore ||
      m_wire.elements()[NOT_BEFORE_OFFSET].value_size() != ISO_DATETIME_SIZE ||
      m_wire.elements()[NOT_AFTER_OFFSET].type() != tlv::NotAfter ||
      m_wire.elements()[NOT_AFTER_OFFSET].value_size() != ISO_DATETIME_SIZE) {
    NDN_THROW(Error("Invalid NotBefore or NotAfter field"));
  }

  try {
    m_notBefore = time_point_cast<TimePoint::duration>(
                    time::fromIsoString(readString(m_wire.elements()[NOT_BEFORE_OFFSET])));
    m_notAfter = time_point_cast<TimePoint::duration>(
                   time::fromIsoString(readString(m_wire.elements()[NOT_AFTER_OFFSET])));
  }
  catch (const std::bad_cast&) {
    NDN_THROW(Error("Invalid date format in NOT-BEFORE or NOT-AFTER field"));
  }
}

ValidityPeriod&
ValidityPeriod::setPeriod(const time::system_clock::time_point& notBefore,
                          const time::system_clock::time_point& notAfter)
{
  m_wire.reset();
  m_notBefore = time_point_cast<TimePoint::duration>(notBefore + TimePoint::duration(1) -
                                                     time::system_clock::time_point::duration(1));
  m_notAfter = time_point_cast<TimePoint::duration>(notAfter);
  return *this;
}

std::pair<time::system_clock::time_point, time::system_clock::time_point>
ValidityPeriod::getPeriod() const
{
  return {m_notBefore, m_notAfter};
}

bool
ValidityPeriod::isValid(const time::system_clock::time_point& now) const
{
  return m_notBefore <= now && now <= m_notAfter;
}

std::ostream&
operator<<(std::ostream& os, const ValidityPeriod& period)
{
  os << "(" << time::toIsoString(period.getPeriod().first)
     << ", " << time::toIsoString(period.getPeriod().second) << ")";
  return os;
}

} // namespace ndn::security
