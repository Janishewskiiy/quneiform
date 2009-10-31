/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef PAGE_H_
#define PAGE_H_

#include "ced_struct.h"

namespace CIF {

class CEDSection;

class CEDPage
{
public:
    //picture data
    EDSIZE sizeOfImage; // The size of the original image in pixels
    EDSIZE dpi; //scanner resolution for this picture
    int turn; // Tangent angle on the vertical images * 2048
    char* imageName; // Filename image. If the path is not specified, is searched in one
    // Directory with the file ed

    int pageNumber; // Number of Pages (= 0 not in batch mode)
    EDSIZE pageSizeInTwips; // The width of the page in twip (1dyuym = 1440tvipov) for text editor
    EDRECT pageBordersInTwips;
    char unrecogChar;
    char recogLang;
    Bool resizeToFit;

    int fontsUsed; //РљThe number of fonts used in table
    int fontsCreated; //РљThe number of fonts created in the table
    fontEntry* fontTable; // Pointer to the table fonts
    int picsUsed; //РљNumber of images used in table
    int picsCreated; //РљNumber of images created in the table
    pictEntry* picsTable; // pointer to a table of images

    char * extData; // Data cat. will be recorded in the file after the title
    int extDataLen; // Its size

    CEDPage();
    ~CEDPage();

    Bool FormattedWriteRtf(const char * fileName);

    CEDSection * GetSection(int _num);
    CEDParagraph * GetParagraph(int _num);
    CEDLine * GetLine(int _num);
    CEDChar * GetChar(int _num);

    Bool GoToNextSection();
    Bool GoToNextParagraph(Bool32 NonFictiveOnly);
    Bool GoToNextLine();
    Bool GoToNextChar();

    int GetNumberOfSections();
    int GetNumberOfParagraphs();
    int GetNumberOfLines();
    int GetNumberOfChars();

    Bool CreateFont(uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset, char* fontName);
    Bool GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily, uchar* fontCharset,
            char** fontName);

    int GetFontByNum(uchar fontNumber);

    Bool CreatePicture(int pictNumber, EDSIZE pictSize, EDSIZE pictGoal, int pictAlign, int type,
            void * data, int len);

    CEDSection * InsertSection(); //inserts new section after current one. inserted one becomes current
    //sets pointer to the inserted one
    //_deleteSubItems - either delete all daughter elements or attach it to previous object
    CEDSection * SetCurSection(CEDSection* _sect);//sets new value of current section
    CEDSection * SetCurSection(int _number);//sets new value of current section

    CEDSection * GetCurSection(); //returns current section
    int GetNumOfCurSection(); //returns current section

    CEDSection * NextSection(); //returns next section, 0 if last
    CEDSection * PrevSection(); //return previous section, 0 if first

    int NumberOfSections, NumberOfParagraphs, NumberOfLines, NumberOfChars;

    CEDSection * sections; //connected list of sections
    CEDSection * curSect; //current section
};

}

#endif /* PAGE_H_ */
