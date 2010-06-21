/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 Разрешается повторное распространение и использование как в виде исходного кода,
 так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

 * При повторном распространении исходного кода должны оставаться указанное
 выше уведомление об авторском праве, этот список условий и последующий
 отказ от гарантий.
 * При повторном распространении двоичного кода в документации и/или в
 других материалах, поставляемых при распространении, должны сохраняться
 указанная выше информация об авторском праве, этот список условий и
 последующий отказ от гарантий.
 * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
 быть использованы в качестве средства поддержки и/или продвижения
 продуктов, основанных на этом ПО, без предварительного письменного
 разрешения.

 ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
 ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
 ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
 ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
 МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
 НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
 ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
 НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
 СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
 ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
 НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
 ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of the Cognitive Technologies nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __CEDINT_H
#define __CEDINT_H

#include <cstdio>
#include <climits>
#include "globus.h"

#pragma pack (push,8)

#define CFIOHightCode 0x0001

#include "ced.h"
#include "cedchar.h"
#include "cedparagraph.h"
#include "cedsection.h"
#include "cedpage.h"
#include "ced_struct.h"
#include "compat_defs.h"
#include "compat/filefunc.h"

void SetReturnCode_ced(uint32_t rc);//set error code
uint32_t GetReturnCode_ced();

#pragma pack(1)
struct pageDescr
{
        uint32_t paperw;
        uint32_t paperh;
        uint32_t margl;
        uint32_t margr;
        uint32_t margt;
        uint32_t margb;
        uchar resizeToFit;
        uchar recogLang;
};

#pragma pack(1)
struct fontDiscr
{
        uint16_t size;
        uchar fontNumber;
        uchar fontPitchAndFamily;
        uchar fontCharset;
};

#pragma pack(1)
struct pictDescr
{
        uint32_t size;
        uint16_t pictNumber;
        CIF::Size pictSize;
        EDSIZE pictGoal;
        uchar pictAlign;
        uchar type;
        uint32_t len;
};

#pragma pack(1)
struct originalImageDesc
{
        uint16_t resolutionX;
        uint16_t resolutionY;
        uint16_t inclune;
        uint16_t pageNum;
        uint32_t width;
        uint32_t height;
        uchar unrecogSymbol;
};

#pragma pack(1)
struct sectParams1
{
        uint32_t topMargin;
        uint32_t bottomMargin;
        uint32_t leftMargin;
        uint32_t rightMargin;
        uchar columns;
        uchar numSnakeCols;
        uint32_t colInterval;
};
#pragma pack(1)
struct sectParams2
{
        uchar sectionBreak;
        uint32_t width;
        uint32_t height;
        uchar orientation;
        uint32_t headerY;
        uint32_t footerY;
        uchar lineBetCol;
};

#pragma pack(1)
struct frameParam
{
        uchar position;
        uint32_t posx;
        uint32_t posy;
        uint32_t absw;
        uint32_t absh;
        uint32_t borderSpace;
        uint32_t dxfrtextx;
        uint32_t dxfrtexty;
        uchar flag;
};

#pragma pack(1)
struct rowParam
{
        uint32_t left;
        uint32_t rowHeight;
        uchar leftBrdrType;
        uint32_t leftBrdrWidth;
        uchar rightBrdrType;
        uint32_t rightBrdrWidth;
        uchar topBrdrType;
        uint32_t topBrdrWidth;
        uchar bottomBrdrType;
        uint32_t bottomBrdrWidth;
        uint32_t gaph;
        uchar position;
        uchar header;
};

#pragma pack(1)
struct cellParam
{
        uint32_t cellX;
        uchar merging;
        uchar vertTextAlign;
        uchar leftBrdrType;
        uint32_t leftBrdrWidth;
        uchar rightBrdrType;
        uint32_t rightBrdrWidth;
        uchar topBrdrType;
        uint32_t topBrdrWidth;
        uchar bottomBrdrType;
        uint32_t bottomBrdrWidth;
        EDBOX layout;
        uint16_t shading;
        uint16_t color;
        uchar flag;
};

#pragma pack(1)
struct paraParams
{
        uint32_t firstIndent;
        uint32_t leftIndent;
        uint32_t rightIndent;
        uchar alignment;
        uint16_t userNum;
        uint16_t color;
        uint16_t shading;
        uint32_t spaceBefore;
        uint32_t spaceAfter;
        uint32_t spaceBetweenLines;
        uchar spcBtwLnsMult;
        uchar keep;
        uchar leftBrdrType;
        uint32_t leftBrdrWidth;
        uchar rightBrdrType;
        uint32_t rightBrdrWidth;
        uchar topBrdrType;
        uint32_t topBrdrWidth;
        uchar bottomBrdrType;
        uint32_t bottomBrdrWidth;
        uchar brdrBtw;
};

#pragma pack(1)
struct charParams
{
        uint32_t fontNumber;
        uint32_t foregroundColor;
        uint32_t backgroundColor;
};
#pragma pack()

#define MAX_WIDTH 300
#define MAX_RTF_GROUPS     50

struct StrRtfColor
{ // color table in an rtf file
        CIF::Color color; // color
};

struct StrRtfOut
{ // Rtf output file processing block
        int output; // rtr output type: RTF_FILE,RTF_BUF,RTF_CB
        FILE * hFile; // stream if file is used
        Handle hBuf; // output buffer handle if using buffer output
        uchar *buf; // output buffer pointer if using buffer
        long BufLen; // allcated length of buffer
        long BufIndex; // index of next character in the buffer
        char text[MAX_WIDTH + 1]; // temporary buffer to write text
        int TextLen; // length of the text in the 'text' buffer
        Bool SpacePending; // TRUE if space needs to be written out after the last control
        Bool WritingControl; // TRUE when writing a control word
        struct StrRtfColor *color; // rtf color table pointer
        int TotalColors; // total colors in the color table
        int GroupLevel;
        uint flags; // ROFLAG_ flags
        int RtfInHdrFtr;
        Bool RtfInTable;
        Bool RtfInFrame;
        Bool wrtFrmSz;
        CIF::CEDPage * page;
        Bool reset;
        CIF::CEDParagraph* PrevPfmt;
        CIF::CEDParagraph* frm;
        CIF::CEDChar PrevChar;
        char * oldFile; //data from file we write
        int oldFileLen; //their length
        int * table; //table of correspondence between fonts of old and new rtf
        int maxFntNum;
        Bool GroupBegin; // TRUE if the group begins
        Bool GroupEnd; // TRUE if the group ends
        Bool IsControlWord; // TRUE if control word
        long IntParam; // rtf control word parameter in integer numeric format
        char CurWord[MAX_WIDTH + 1]; // current word
        int WordLen; // length of the current word
        int TextIndex; // index if current uchar in the 'text' buffer
        char CurChar; // last character read
        Bool eof; // end of file
        char param[MAX_WIDTH / 3]; // rtf control word parameter
        uchar stack[MAX_WIDTH + 1]; // characters returned from previous operations
        int StackLen; // number of characters in the stack

};

struct StrRtfFont
{ // font table in an rtf file
//      Bool InUse;                         // TRUE when the slot is in use
        int FontId; // font id
        char family[32]; // font family
        char name[32]; // font name
        int CharSet; // Piter A
};

#define RTF_FILE_INCOMPLETE 1
#define RTF_SYNTAX_ERROR    2
#define RTF_END_FILE        3

#define PARAM_NONE          0
#define PARAM_INT           1
#define PARAM_DBL           2

#define DEFAULT_CFMT       0x0
#define DEFAULT_PFMT        0x0

#define ROFLAG_IGNORE_TABLE   0x1      // ignore table information
#define ROFLAG_IGNORE_SLASH   0x2      // do not treat backslash as a special character
#define ROFLAG_IN_SUPSCR      0x4      // in superscript group
#define ROFLAG_IN_SUBSCR      0x8      // in subscript group
Bool BeginRtfGroup(StrRtfOut *rtf);
Bool WriteRtfControl(StrRtfOut *rtf, const char* control, int type, double val);
Bool WriteRtfFont(StrRtfOut *rtf, Bool head);
Bool WriteRtfMargin(StrRtfOut *rtf);
Bool WriteRtfSection(StrRtfOut *rtf, CIF::CEDSection* sect);
Bool WriteRtfCharFmt(StrRtfOut *rtf, CIF::CEDChar* curChar);
Bool EndRtfGroup(StrRtfOut *rtf);
Bool WriteRtfParaFmt(StrRtfOut *rtf, CIF::CEDParagraph* NewPfmt, CIF::CEDParagraph* PrevPfmt);
Bool FlushRtfLine(StrRtfOut *rtf);
Bool PutRtfChar(StrRtfOut *rtf, uchar CurChar);
Bool WriteRtfText(StrRtfOut *rtf, const char* text, int TextLen);
Bool WriteRtfRow(StrRtfOut *rtf, CIF::CEDParagraph* NewCell, CIF::CEDParagraph * prevRow);
Bool WriteRtfCell(StrRtfOut *rtf, CIF::CEDParagraph* NewCell);
Bool WriteRtfPara(StrRtfOut *rtf, CIF::CEDParagraph* p, Bool brk);
Bool WriteFrmPos(StrRtfOut *rtf, CIF::CEDParagraph* frm, Bool writeWidth);
Bool WriteRtfDIB(StrRtfOut *rtf, int pict);
Bool PutRtfHexChar(StrRtfOut *rtf, uchar CurChar);
Bool WriteRtfMetafile(StrRtfOut *rtf, int pict);
Bool WriteRtfMergedHeader(StrRtfOut *rtf, const char * name);
Bool WriteRtfParaBorder(StrRtfOut *rtf, CIF::CEDParagraph * para);

extern FILE *logStream;

// error code

#pragma pack (pop)

#endif
