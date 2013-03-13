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

#ifndef TESTLANGUAGE_H_
#define TESTLANGUAGE_H_

#include <cppunit/extensions/HelperMacros.h>

class TestLanguage : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestLanguage);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testIsoNames);
    CPPUNIT_TEST(testNamesSort);
    CPPUNIT_TEST(testIsValid);
    CPPUNIT_TEST(testByCode);
    CPPUNIT_TEST(testByName);
    CPPUNIT_TEST(testOutput);
    CPPUNIT_TEST(testEncodings);
    CPPUNIT_TEST(testAll);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testIsoNames();
    void testNamesSort();
    void testIsValid();
    void testByCode();
    void testByName();
    void testOutput();
    void testEncodings();
    void testAll();
};

#endif /* TESTLANGUAGE_H_ */
