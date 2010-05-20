/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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
#include <stdexcept>
#include "testcrtfhorizontalcolumn.h"
#include "common/tostring.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCRtfHorizontalColumn);
#define private public
#include "rfrmt/crtfhorizontalcolumn.h"
#include "rfrmt/crtfverticalcolumn.h"
using namespace CIF;

void TestCRtfHorizontalColumn::testInit() {
    CRtfHorizontalColumn col;
    CPPUNIT_ASSERT_EQUAL(col.type(), CRtfHorizontalColumn::SINGLE_TERMINAL);
    CPPUNIT_ASSERT(col.columnCount() == 0);
    col.clearColumns();
    col.clearTerminalColumnsGroup();
    col.clearTerminalColumnsIndexes();
}

int numSmall(const CRtfHorizontalColumn& col) {
    int res = 0;

    for (size_t i = 0; i < col.vcols_.size(); i++) {
        if (col.vcols_[i]->isSmall())
            res++;
    }

    return res;
}

void ADD_VCOL(CRtfHorizontalColumn& col, const Rect& r) {
    CRtfVerticalColumn * vcol = new CRtfVerticalColumn;
    vcol->setRealRect(r);
    col.addColumn(vcol);
}

void ADD_VCOL(CRtfHorizontalColumn& col, int width, int height, int xoffset) {
    ADD_VCOL(col, Rect(Point(xoffset, 0), width, height));
}

void TestCRtfHorizontalColumn::testMarkSmallColumns() {
    CRtfHorizontalColumn col;
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(0, numSmall(col));

    ADD_VCOL(col, 10, 10, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(0, numSmall(col));

    ADD_VCOL(col, 14, 11, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(0, numSmall(col));

    ADD_VCOL(col, 17, 9, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(0, numSmall(col));

    ADD_VCOL(col, 4, 4, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL(1, numSmall(col));

    ADD_VCOL(col, 50, 80, 0);
    col.markSmallColumns();
    CPPUNIT_ASSERT_EQUAL((int) col.columnCount() - 1, numSmall(col));
}

void TestCRtfHorizontalColumn::testMaxVColumnHeight() {
    CRtfHorizontalColumn col;
    CPPUNIT_ASSERT_EQUAL(0, col.maxVColumnHeight());
    ADD_VCOL(col, 0, 1, 0);
    ADD_VCOL(col, 0, 5, 0);
    ADD_VCOL(col, 0, 2, 0);
    CPPUNIT_ASSERT_EQUAL(5, col.maxVColumnHeight());
    ADD_VCOL(col, 0, 111, 0);
    ADD_VCOL(col, 0, 0, 0);
    CPPUNIT_ASSERT_EQUAL(111, col.maxVColumnHeight());
}

void TestCRtfHorizontalColumn::testMaxVColumnWidth() {
    CRtfHorizontalColumn col;
    CPPUNIT_ASSERT_EQUAL(0, col.maxVColumnWidth());
    ADD_VCOL(col, 0, 0, 0);
    ADD_VCOL(col, 10, 0, 0);
    ADD_VCOL(col, 1, 0, 0);
    CPPUNIT_ASSERT_EQUAL(10, col.maxVColumnWidth());
    ADD_VCOL(col, 120, 0, 0);
    ADD_VCOL(col, 4, 0, 0);
    CPPUNIT_ASSERT_EQUAL(120, col.maxVColumnWidth());
}

void TestCRtfHorizontalColumn::testProcessSpaceByHist() {
    CRtfHorizontalColumn col;
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 0);
    Histogram hist(0);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 0);
    hist.push_back(1);
    // 1
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 0);

    // 0
    hist[0] = 0;
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 1);
    CPPUNIT_ASSERT(col.hist_spaces_.at(0) == 0);

    // 00
    hist.push_back(0);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 1);
    CPPUNIT_ASSERT(col.hist_spaces_.at(0) == 0);

    // 001
    hist.push_back(1);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 1);

    // 0011
    hist.push_back(1);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 1);

    // 00110
    hist.push_back(0);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 2);
    CPPUNIT_ASSERT(col.hist_spaces_.at(0) == 0);
    CPPUNIT_ASSERT(col.hist_spaces_.at(1) == 4);

    // 001101
    hist.push_back(1);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 2);

    // 0011010
    hist.push_back(0);
    col.processSpaceByHist(hist);
    CPPUNIT_ASSERT(col.hist_spaces_.size() == 3);
    CPPUNIT_ASSERT(col.hist_spaces_.at(0) == 0);
    CPPUNIT_ASSERT(col.hist_spaces_.at(1) == 4);
    CPPUNIT_ASSERT(col.hist_spaces_.at(2) == 6);
}
