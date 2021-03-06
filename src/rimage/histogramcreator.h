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

#ifndef HISTOGRAMCREATOR_H
#define HISTOGRAMCREATOR_H

#include "common/histogram.h"
#include "common/ctdib.h"

namespace cf
{

class HistogramCreator
{
public:
    static bool grayBrighnessFromRGB24(HistogramInt& hist, const void * data, size_t w, size_t h);
    static bool grayBrighnessFromRGB32(HistogramInt& hist, const void * data, size_t w, size_t h);
    static bool grayBrighness(HistogramInt& hist, const CTDIB& dib);
    static bool save(const HistogramInt& hist, const std::string& fileName, int threshold = -1);
};

}

#endif // HISTOGRAMCREATOR_H
