// This file is part of snark, a generic and flexible library
// for robotics research.
//
// Copyright (C) 2011 The University of Sydney
//
// snark is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// snark is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with snark. If not, see <http://www.gnu.org/licenses/>.

#ifndef SNARK_SENSORS_IMPL_ANGLE_H_
#define SNARK_SENSORS_IMPL_ANGLE_H_

namespace snark {  namespace velodyne { namespace impl {

struct angle
{
    static double sin( unsigned int angle );
    static double cos( unsigned int angle );
};

} } } // namespace snark {  namespace velodyne { namespace impl {

#endif // #ifndef SNARK_SENSORS_IMPL_ANGLE_H_
