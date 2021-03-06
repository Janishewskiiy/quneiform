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
#include "rfrmtoptions.h"
#include "rfrmtfile.h"

// ced
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"

#include "cstr/cstrdefs.h"
#include "minmax.h"

namespace cf
{

const int SMALLEST_FONT_SIZE = 3;
const char SPACE = ' ';
const unsigned char HYPHEN = '-';

RfrmtDrawStringFunction CRtfString::draw_func_;

CRtfString::CRtfString() :
    line_break_(false), paragraph_begin_(false), has_attributes_(false), equal_center_(false),
            equal_left_(false), equal_right_(false), first_indent_(0), left_indent_(0),
            right_indent_(0), margin_top_(0), real_length_(0), align_(FORMAT_ALIGN_LEFT), flags_(0) {

}

CRtfString::~CRtfString() {
    clearWords();
}

void CRtfString::addWord(CRtfWord * word) {
    words_.push_back(word);
}

void CRtfString::addWord(const std::string& word) {
    CRtfWord * w = new CRtfWord;
    w->set(word.substr(0, word.find(' ')));
    addWord(w);
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

format_align_t CRtfString::align() const {
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

void CRtfString::drawLayout() const {
    if (!draw_func_.empty())
        draw_func_(this);
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
    for (size_t i = 1; i < words_.size(); i++) {
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

bool CRtfString::isEndOfSentence() const {
    return endsWith('.') || endsWith('?') || endsWith('!');
}

bool CRtfString::isEndOfSentencePart() const {
    return endsWith(':') || endsWith(';');
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
    return endsWith(HYPHEN) && lastChar()->isSpelledNoCarrying();
}

bool CRtfString::isNegative() const {
    return hasFlag(CSTR_STR_NEGATIVE);
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

bool CRtfString::lineBreak() const {
    return line_break_;
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
        "line break:          " << lineBreak() << "\n"
        "line carry:          " << isLineCarryNeeded() << "\n";
}

CRtfString * CRtfString::read(FILE * in) {
    assert(in);
    InternalRect SRect;

    CRtfString * str = new CRtfString;

    fread(&SRect, sizeof(InternalRect), 1, in);
    //Реальные коор. строки!
    fread(&SRect, sizeof(InternalRect), 1, in);
    uint16_t word_count;
    fread(&word_count, sizeof(uint16_t), 1, in);
    uint32_t tmp;
    fread(&tmp, sizeof(uint32_t), 1, in);//NEGA_STR

    for (uint16_t nw = 0; nw < word_count; ++nw) {
        CRtfWord * word = CRtfWord::read(in);
        str->addWord(word);
    }

    return str;
}

uint CRtfString::realLength() const {
    return real_length_;
}

void CRtfString::setAlign(format_align_t align) {
    align_ = align;
}

void CRtfString::setAttributes(bool value) {
    has_attributes_ = value;
}

void CRtfString::setDrawCallback(RfrmtDrawStringFunction f) {
    draw_func_ = f;
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

void CRtfString::setLineBreak(bool value) {
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

CEDLine * CRtfString::toCedLine() const {
    CEDLine * line = new CEDLine;
    line->setHardBreak(line_break_);

    if (empty())
        line->setDefaultFontHeight(DefFontSize);
    else
        line->setDefaultFontHeight(firstWord()->realFontSize() * 2);

    return line;
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

void CRtfString::write(CEDParagraph * par) const {
    assert(par);

    CEDLine * line = toCedLine();
    line->setHardBreak(line_break_);

    for (WordIteratorConst it = words_.begin(), end = words_.end(); it != end; ++it) {
        // insert space before if not first word in string
        if (it != words_.begin()) {
            CEDChar * space = CRtfChar::makeCedSpace((*it)->fontNumber(), -1, (*it)->fontAttrs());
            if (!(*it)->empty()) {
                space->setFontHeight((*it)->lastChar()->fontSize());
            }
            line->insertChar(space);
        }

        (*it)->write(line);
    }

    par->addLine(line);
}

}

