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

#ifndef CRTFCHAR_H_
#define CRTFCHAR_H_

#include <vector>

#include "globus.h"
#include "cfcompat.h"
#include "lang_def.h"
#include "crtfstruct.h"
#include "common/rect.h"
#include "common/letter.h"

namespace CIF
{

class CLA_EXPO CRtfChar
{
    public:
        CRtfChar();

        /**
         * Adds version
         */
        void addVersion(const Letter& version);

        /**
         * Returns first alternative
         */
        const Letter& first() const;

        /**
         * Returns first alternative
         */
        Letter& first();

        /**
         * Returns font number
         */
        font_number font() const;

        /**
         * Returns font size
         */
        short fontSize() const;

        /**
         * Returns ideal char bounding rectangle
         */
        Rect idealRect() const;

        /**
         * Returns character language
         */
        language_t language() const;

        /**
         * Returns bounding rectangle
         */
        Rect realRect() const;

        /**
         * Sets font number
         */
        void setFont(font_number fontNumber);

        /**
         * Sets font size
         */
        void setFontSize(short size);

        /**
         * Sets ideal rectangle
         * @param rect
         */
        void setIdealRect(const Rect& rect);

        /**
         * Sets character language
         * @param lang - new language
         */
        void setLanguage(language_t lang);

        /**
         * Sets real rectangle
         */
        void setRealRect(const Rect& rect);

        /**
         * Returns reference to character alternative
         * @param pos
         * @return const reference
         */
        const Letter& versionAt(size_t pos) const;

        /**
         * Returns reference to character alternative
         * @param pos
         * @return reference
         */
        Letter& versionAt(size_t pos);

        /**
         * Returns number of versions
         */
        size_t versionCount() const;

        uchar m_bFlg_spell;
        uchar m_bFlg_spell_nocarrying;
        uchar m_bFlg_cup_drop;
    private:
        language_t language_;
        font_number font_number_;
        uint16_t font_size_;
        Rect ideal_rect_;
        Rect real_rect_;
        typedef std::vector<Letter> LetterList;
        LetterList versions_;
};

}

#endif /* CRTFCHAR_H_ */