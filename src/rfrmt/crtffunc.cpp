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

#include <cstring>
#include "crtffunc.h"
#include "lst3_win.h"

void Put(const char *Data) {
    int16_t i, len;

    if (RtfWriteMode == FALSE)
        return;

    len = strlen(Data);
    do0(i, 0, len - 1)
        PutC(Data[i]);
}

#define SIZE_BLOC 1024
//==Return: >0 - код символа, 0 - END_OF_FILE

void PutC(char sym) {
    static char *b;
    static int16_t pos = -1;

    if (RtfWriteMode == FALSE)
        return;

    if (sym == 0) {
        b = (char*) malloc(SIZE_BLOC);
        pos = -1;
        return;
    } //Init

    if (sym == 1) { //Вывод буферов
        if (pos >= 0)
            fwrite(b, pos + 1, 1, out);

        free(b);
        return;
    }

    if (pos == SIZE_BLOC - 1) {
        fwrite(b, SIZE_BLOC, 1, out);
        pos = -1;
    }

    b[++pos] = sym;
}

//==Command - сама команда, value - числовой аргумент (-1 - нет)
void PutCom(const char *Command, int32_t value, int16_t space) {
    char Num[10];
    int16_t i, len;

    if (RtfWriteMode == FALSE)
        return;

    len = strlen(Command);
    do0(i, 0, len - 1)
        PutC(Command[i]);

    if (value >= 0) {
        sprintf(Num, "%d", value);
        len = strlen(Num);
        do0 (i, 0, len - 1)
            PutChar(Num[i]);
    }

    if (space)
        PutChar(' ');
}

void PutChar(uchar sym) {
    char s[10];
    int16_t i, len;

    if (RtfWriteMode == FALSE)
        return;

    if (sym == '{' || sym == '}')
        return;

    if ((uint) sym < (uint) 0xC0 || sym == '}') {
        if (sym == '\\')
            PutC(sym);

        PutC(sym);
    }

    else { //Cyrillic
        sprintf(s, "%x", (uint) sym);
        PutC('\\');
        PutC('\'');
        len = strlen(s);
        do0 (i, 0, len - 1)
            PutC(s[i]);
    }
}
