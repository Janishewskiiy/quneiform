/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef PROCESSRECOGNITIONSERVER_H
#define PROCESSRECOGNITIONSERVER_H

#include "globus.h"
#include "abstractrecognitionserver.h"

namespace cf {

class CLA_EXPO ProcessRecognitionServer : public AbstractRecognitionServer
{
public:
    ProcessRecognitionServer();

    /**
      * Recognizes given image
      * @param image - image pointer
      * @param ropts - recognition options
      * @param fopts - format options
      * @throw RecognitionException on error
      */
    CEDPagePtr recognize(ImagePtr image,
                         const RecognizeOptions& ropts,
                         const FormatOptions& fopts);
private:
    bool processWorkerReturnCode(int code);
    bool startWorker(ImagePtr image, const std::string& key);
    std::string workerPath() const;
};

}

#endif // PROCESSRECOGNITIONSERVER_H