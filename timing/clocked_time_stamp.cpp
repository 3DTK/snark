// This file is part of snark, a generic and flexible library for robotics research
// Copyright (c) 2011 The University of Sydney
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the University of Sydney nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <cassert>
#include "clocked_time_stamp.h"

namespace snark{ namespace timing {

/// assumptions:
/// - the system performance is enough to catch up at least once in a while
/// - here we discount the lower latency boundary below which the system cannot go

clocked_time_stamp::clocked_time_stamp( boost::posix_time::time_duration period )
: m_first( true )
, m_period( period )
, m_totalDeviation( boost::posix_time::seconds( 0 ) )
{
}

boost::posix_time::ptime clocked_time_stamp::adjusted( const boost::posix_time::ptime& t, std::size_t ticks )
{
    assert( ticks );
    if( m_first ) { m_first = false; }
    else
    {
        // nothing more than t - t0 - sum( periods )
        m_totalDeviation += ( t - m_last - m_period * ticks );
        // whenever we get a better approximation, we take it as a better approximation
        if( m_totalDeviation.is_negative() ) { m_totalDeviation = boost::posix_time::seconds( 0 ); }
    }
    m_last = t;
    // first m_totalDeviation = 0, i.e. we consider t the best approximation available
    return t - m_totalDeviation;
}

boost::posix_time::ptime clocked_time_stamp::adjusted( const boost::posix_time::ptime& t, boost::posix_time::time_duration period, std::size_t ticks )
{
    m_period = period;
    return adjusted( t, ticks );
}

void clocked_time_stamp::reset( void )
{
    m_first = true;
    m_totalDeviation = boost::posix_time::seconds( 0 );
}

/***************************periodic_time_stamp*****************************************************/

periodic_time_stamp::periodic_time_stamp( boost::posix_time::time_duration const& i_adjusted_period
                                        , boost::posix_time::time_duration const& i_adjusted_threshold
                                        , boost::posix_time::time_duration const& i_adjusted_reset )
    : adjusted_period_( i_adjusted_period )
    , adjusted_threshold_( i_adjusted_threshold )
    , adjusted_reset_( i_adjusted_reset )
{
}

boost::posix_time::ptime periodic_time_stamp::adjusted( const boost::posix_time::ptime& timestamp,std::size_t ticks )
{
    boost::posix_time::ptime newt = timestamp;

    if( !last_.is_not_a_date_time() && ( timestamp - last_reset_ ) < adjusted_reset_ )
    {
        boost::posix_time::time_duration delta = timestamp - last_;

        if( delta > adjusted_period_ * ticks && delta < adjusted_period_ * ticks + adjusted_threshold_ )
        { newt = last_ + adjusted_period_ * ticks; }
        else
        { last_reset_ = timestamp; }
    }
    else { last_reset_ = timestamp; }

    last_ = newt;
    return newt;
}

} } // namespace snark{ namespace timing

