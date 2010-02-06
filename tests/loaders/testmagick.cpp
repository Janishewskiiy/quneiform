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
#include "testmagick.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestMagickLoader);
#include <rdib/magickimageloader.h>
#include <memory>
using namespace CIF;
void TestMagickLoader::testInit() {
    std::auto_ptr<MagickImageLoader> loader(new MagickImageLoader);
}

void TestMagickLoader::testLoad() {
    std::string path = LOADER_TEST_IMAGE_DIR;
    std::auto_ptr<MagickImageLoader> loader(new MagickImageLoader);
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.xpm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.bmp"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.gif"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.tif"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.jpg"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.png"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.pbm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.pgm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.ppm"));

}