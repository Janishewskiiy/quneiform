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

#include <iostream>
#include "crtfchar.h"
#include "crtfstring.h"
#include "crtfword.h"
#include "creatertf.h"
#include "crtfstruct.h"
#include "crtffunc.h"
#include "minmax.h"

namespace CIF
{

const int SMALLEST_FONT_SIZE = 3;
const char SPACE = ' ';
const unsigned char HYPHEN = '-';

CRtfString::CRtfString() :
    line_break_(false), paragraph_begin_(false), carry_(false), has_attributes_(false),
            equal_center_(false), equal_left_(false), equal_right_(false), first_indent_(0),
            left_indent_(0), right_indent_(0), margin_top_(0), real_length_(0), align_(
                    RTF_TP_LEFT_ALLIGN), flags_(0) {

}

CRtfString::~CRtfString() {
    clearWords();
}

void CRtfString::addWord(CRtfWord * word) {
    words_.push_back(word);
}

int CRtfString::leftBorder() const {
    return firstChar()->idealRect().left();
}

int CRtfString::rightBorder() const {
    return lastChar()->idealRect().right();
}

int CRtfString::rightIndent() const {
    return right_indent_;
}

rtf_align_t CRtfString::align() const {
    return align_;
}

void CRtfString::calcRealLength() {
    if (words_.empty() || words_.front()->charCount() == 0) {
        real_length_ = 0;
        return;
    }

    int16_t strHeight;
    std::string str = toString();
    const CRtfChar *pRtfChar = firstChar();
    real_length_ = GetRealSize(str.c_str(), str.length(), firstWord()->realFontSize(),
            pRtfChar->font(), &strHeight);
}

int CRtfString::center() const {
    return (leftBorder() + rightBorder()) / 2;
}

inline int charCount(const CRtfWord * word) {
    return word->charCount();
}

size_t CRtfString::charCount() const {
    size_t result = 0;
    for (WordIteratorConst it = words_.begin(), end = words_.end(); it != end; ++it)
        result += (*it)->charCount();
    return result;
}

int CRtfString::charTotalLength() const {
    int result = 0;
    for (WordIteratorConst it = words_.begin(), end = words_.end(); it != end; ++it)
        result += (*it)->charTotalLength();
    return result;
}

void CRtfString::clearWords() {
    for (WordIterator it = words_.begin(), e = words_.end(); it != e; ++it)
        delete (*it);
    words_.clear();
}

bool CRtfString::empty() const {
    return words_.empty();
}

bool CRtfString::endsWith(int c) const {
    return words_.empty() ? false : words_.back()->endsWith(c);
}

CRtfChar * CRtfString::firstChar() {
    return const_cast<CRtfChar*> (const_cast<const CRtfString*> (this)->firstChar());
}

const CRtfChar * CRtfString::firstChar() const {
    if (words_.empty())
        throw std::out_of_range("[CRtfString::firstChar] string is empty");
    return words_.front()->firstChar();
}

int CRtfString::firstIndent() const {
    return first_indent_;
}

CRtfWord * CRtfString::firstWord() {
    return const_cast<CRtfWord*> (const_cast<const CRtfString*> (this)->firstWord());
}

const CRtfWord * CRtfString::firstWord() const {
    if (words_.empty())
        throw std::out_of_range("[CRtfString::firstWord] string is empty");
    return words_.front();
}

uint CRtfString::flags() const {
    return flags_;
}

bool CRtfString::hasAttributes() const {
    return has_attributes_;
}

bool CRtfString::hasBigSpace(int max_space) const {
    for (size_t i = 1; i < wordCount(); i++) {
        CRtfChar * current = words_[i]->firstChar();
        CRtfChar * prev = words_[i - 1]->lastChar();

        if ((current->idealRect().left() - prev->idealRect().right()) > max_space)
            return true;
    }

    return false;
}

bool CRtfString::hasChars() const {
    for (WordIteratorConst it = words_.begin(), e = words_.end(); it != e; ++it) {
        if (!(*it)->empty())
            return true;
    }
    return false;
}

bool CRtfString::hasFlag(uint flag) const {
    return flags_ & flag;
}

bool CRtfString::isEqualCenter() const {
    return equal_center_;
}

bool CRtfString::isEqualLeft() const {
    return equal_left_;
}

bool CRtfString::isEqualRight() const {
    return equal_right_;
}

bool CRtfString::isLineCarryNeeded() const {
    try {
        const CRtfChar * chr = lastChar();
        return endsWith(HYPHEN) && chr->m_bFlg_spell_nocarrying;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool CRtfString::isParagraphBegin() const {
    return paragraph_begin_;
}

const CRtfChar * CRtfString::lastChar() const {
    if (words_.empty())
        throw std::out_of_range("[CRtfString::lastChar] string is empty");
    return words_.back()->lastChar();
}

CRtfChar * CRtfString::lastChar() {
    return const_cast<CRtfChar*> (const_cast<const CRtfString*> (this)->lastChar());
}

CRtfWord * CRtfString::lastWord() {
    return const_cast<CRtfWord*> (const_cast<const CRtfString*> (this)->lastWord());
}

const CRtfWord * CRtfString::lastWord() const {
    if (words_.empty())
        throw std::out_of_range("[CRtfString::lasttWord] string is empty");
    return words_.back();
}

int CRtfString::marginTop() const {
    return margin_top_;
}

int CRtfString::maxWordFontSize() const {
    int str_max_font = SMALLEST_FONT_SIZE;

    for (size_t i = 0; i < words_.size(); i++)
        str_max_font = MAX(str_max_font, words_[i]->realFontSize());

    return str_max_font;
}

int CRtfString::width() const {
    if (empty())
        return 0;
    return rightBorder() - leftBorder();
}

CRtfWord * CRtfString::wordAt(size_t pos) {
    return words_.at(pos);
}

const CRtfWord * CRtfString::wordAt(size_t pos) const {
    return words_.at(pos);
}

size_t CRtfString::wordCount() const {
    return words_.size();
}

int CRtfString::leftIndent() const {
    return left_indent_;
}

int CRtfString::lengthInTwips() const {
    return width() * getTwips();
}

bool CRtfString::lineTransfer() const {
    return line_break_;
}

bool CRtfString::lineCarry() const {
    return carry_;
}

void CRtfString::print(std::ostream& os) const {
    os << "\"" << toString() << "\"" << "\n";
    os << std::boolalpha;
    os << "align:               " << align_ << "\n"
        "begin paragraph:     " << isParagraphBegin() << "\n"
        "first indent:        " << firstIndent() << "\n"
        "left indent:         " << leftIndent() << "\n"
        "right indent:        " << rightIndent() << "\n"
        "left border equal:   " << isEqualLeft() << "\n"
        "right border equal:  " << isEqualRight() << "\n"
        "center equal:        " << isEqualCenter() << "\n"
        "line break:          " << lineTransfer() << "\n"
        "line carry:          " << lineCarry() << "\n";
}

uint CRtfString::realLength() const {
    return real_length_;
}

void CRtfString::setAlign(rtf_align_t align) {
    align_ = align;
}

void CRtfString::setAttributes(bool value) {
    has_attributes_ = value;
}

void CRtfString::setEqualCenter(bool value) {
    equal_center_ = value;
}

void CRtfString::setEqualLeft(bool value) {
    equal_left_ = value;
}

void CRtfString::setEqualRight(bool value) {
    equal_right_ = value;
}

void CRtfString::setFlags(uint flag) {
    flags_ = flag;
}

void CRtfString::setFirstIndent(int indent) {
    first_indent_ = indent;
}

void CRtfString::setFontSizePenalty(int min_size, int penalty) {
    for (WordIterator it = words_.begin(), e = words_.end(); it != e; ++it) {
        if ((*it)->realFontSize() > min_size)
            (*it)->setRealFontSize((*it)->realFontSize() - penalty);
    }
}

void CRtfString::setLeftIndent(int indent) {
    left_indent_ = indent;
}

void CRtfString::setLineCarry(bool value) {
    carry_ = value;
}

void CRtfString::setLineTransfer(bool value) {
    line_break_ = value;
}

void CRtfString::setTopMargin(int value) {
    margin_top_ = value;
}

void CRtfString::setParagraphBegin(bool value) {
    paragraph_begin_ = value;
}

void CRtfString::setRightIndent(int indent) {
    right_indent_ = indent;
}

bool CRtfString::startsWith(int c) const {
    return words_.empty() ? false : words_.front()->startsWith(c);
}

bool CRtfString::startsWithDash() const {
    return startsWith(HYPHEN) || startsWith(TIRE);
}

bool CRtfString::startsWithDigit() const {
    if (words_.empty() || words_.front()->empty())
        return false;
    return isdigit(firstChar()->getChar());
}

std::string CRtfString::toString() const {
    std::string result;

    for (WordIteratorConst it = words_.begin(), e = words_.end(); it != e; ++it) {
        if (it != words_.begin())
            result += SPACE;

        result += (*it)->toString();
    }

    return result;
}

}

