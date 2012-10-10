// This file is part of Ark, a generic and flexible library 
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// Ark is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Ark is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
// for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with Ark. If not, see <http://www.gnu.org/licenses/>.

#ifndef SNARK_TIMING_NTP_H_
#define SNARK_TIMING_NTP_H_

#include <comma/base/types.h>
#include <snark/timing/time.h>

namespace snark{ namespace timing {

std::pair< comma::uint32, comma::uint32 > to_ntp_time( boost::posix_time::ptime t );

boost::posix_time::ptime from_ntp_time( std::pair< comma::uint32, comma::uint32 > ntp );

boost::posix_time::ptime from_ntp_time( comma::uint32 seconds, comma::uint32 fractions );

} } // namespace snark{ namespace timing

#endif /*SNARK_TIMING_NTP_H_*/
