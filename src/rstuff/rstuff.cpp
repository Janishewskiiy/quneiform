/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <iostream>
#include <boost/current_function.hpp>

#include "rstuff.h"
#include "rstuff_local.h"
#include "rsmemory.h"
#include "rsfunc.h"
#include "common/recognizeoptions.h"
#include "puma/pumadef.h"
#include "smetric/smetric.h"
#include "rline/rline.h"
#include "cpage/cpage.h"
#include "dpuma.h"

Bool32 gbRSLT = FALSE;

namespace cf {

static const int MAIN_BUF_SIZE = 500000;
static const int WORK_BUF_SIZE = 180000;

RStuff::RStuff() :
    image_data_(NULL),
    buffer_main_(NULL),
    buffer_work_(NULL)
{
    LDPUMA_Init(0, NULL);

    bool rc = true;

    if(!SMetric_Init (PUMA_MODULE_RSTUFF, NULL))
        rc = false;

    rc = RLINE_Init(PUMA_MODULE_RLINE, NULL);

    static const int RESULT = 2;
    if (rc == RESULT) {
        gbRSLT = TRUE;
        DebugInit();
    }
    else if (!rc) {
        std::cerr << BOOST_CURRENT_FUNCTION << " failed." << std::endl;
    }

    buffer_main_ = new uchar[MAIN_BUF_SIZE];
    buffer_work_ = new uchar[WORK_BUF_SIZE];
}

RStuff::~RStuff() {
    SMetric_Done();
    RLINE_Done();
    LDPUMA_Done();

    delete []buffer_main_;
    delete []buffer_work_;
}

void RStuff::binarise()
{
    Binarise();
}

void RStuff::calculateIncline()
{
    CalcIncline(image_data_);
}

void RStuff::createContainerBigComp()
{
    CreateContainerBigComp(image_data_);
}

void RStuff::killLines()
{
    KillLinesN(image_data_);

    // убиваем остатки линии после сняти
    if (LDPUMA_Skip(image_data_->hDebugCancelRemoveLines))
        LineKiller(image_data_);
}

void RStuff::normalize()
{
    SetReturnCode_rstuff(0);
    SetMainBuff(buffer_main_, MAIN_BUF_SIZE);
    SetWorkBuff(buffer_work_, WORK_BUF_SIZE);

    preProcessImage();
    searchLines();
    calculateIncline();
    ortoMove();
    createContainerBigComp();
    searchNewLines();
    killLines();

    ReSetMem();
}

void RStuff::ortoMove()
{
    OrtoMove(image_data_);
}

void RStuff::preProcessImage()
{
    PreProcessImage(image_data_);
}

void RStuff::searchNewLines()
{
    SearchNewLines(image_data_);
}

void RStuff::searchLines()
{
    SearchLines(image_data_);
}

void RStuff::setCallbacks(RSCBProgressPoints * cb)
{
    SetCBProgressPoints(cb);
}

void RStuff::setImageData(RSPreProcessImage * imageData)
{
    image_data_ = imageData;
}

void RStuff::setRecognizeOptions(const cf::RecognizeOptions& ropts)
{
    if(!image_data_)
        return;

    image_data_->gbAutoRotate = ropts.autoRotate();
    image_data_->gnPictures = ropts.pictureSearch();
    image_data_->gnLanguage = ropts.language();
    image_data_->gbDotMatrix = ropts.dotMatrix();
    image_data_->gbFax100 = ropts.fax();
    image_data_->gnTables = ropts.tableMode();
}

}
