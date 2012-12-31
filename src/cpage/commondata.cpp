/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include "commondata.h"

namespace cf {

namespace cpage {

CommonData::CommonData() :
    type_(0),
    number_(0),
    count(0),
    flags_(0)
{
}

void CommonData::addVertex(const Point &pt)
{
    Vertex[count] = pt;
    count++;
}

int CommonData::number() const
{
    return number_;
}

void CommonData::setNumber(int n)
{
    number_ = n;
}

void CommonData::setType(CDataType type)
{
    type_ = type;
}

CDataType CommonData::type() const
{
    return type_;
}

Point CommonData::vertexAt(size_t pos) const
{
    assert(pos < CPAGE_MAXCORNER);
    return Vertex[pos];
}

size_t CommonData::vertexCount() const
{
    return count;
}

}
}
