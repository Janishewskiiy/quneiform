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

#include <boost/bind.hpp>

#include "crtfhorizontalcolumn.h"
#include "crtfverticalcolumn.h"
#include "crtffragment.h"
#include "crtffunc.h"
#include "crtfpage.h"
#include "rfrmtoptions.h"

#include "common/debug.h"
#include "ced/ced.h"
#include "minmax.h"

namespace CIF
{

struct VColumnBigTextAndFrame
{
        bool operator()(const CRtfVerticalColumn * col) {
            return !col->isSmall() && (col->type() == FT_TEXT || col->type() == FT_FRAME);
        }
};

struct VColumnText
{
        bool operator()(const CRtfVerticalColumn * col) {
            return col->type() == FT_TEXT;
        }
};

struct VColumnFrame
{
        bool operator()(const CRtfVerticalColumn * col) {
            return col->type() == FT_FRAME;
        }
};

template<class Iterator, class Pred>
int columnMaxRightBorder(Iterator begin, Iterator end, Pred condition) {
    int res = (begin != end) ? (*begin)->m_rectReal.right : 0;

    for (Iterator it = begin; it != end; ++it) {
        if (!condition(*it))
            continue;

        res = std::max(res, static_cast<int> ((*it)->m_rectReal.right));
    }

    return res;
}

template<class Iterator, class Pred>
int columnMinLeftBorder(Iterator begin, Iterator end, Pred condition) {
    int res = (begin != end) ? (*begin)->m_rectReal.left : 0;

    for (Iterator it = begin; it != end; ++it) {
        if (!condition(*it))
            continue;

        res = std::min(res, static_cast<int> ((*it)->m_rectReal.left));
    }

    return res;
}

template<class Pred>
void addVColumnToHistogram(const CRtfVerticalColumn * vcol, Histogram& hist, int histogram_offset,
        Pred condition) {
    assert(vcol);

    if (!condition(vcol))
        return;

    const int col_left = vcol->m_rectReal.left - histogram_offset;
    const int col_right = vcol->m_rectReal.right - histogram_offset;

    assert((int) hist.size() < col_right);

    for (int i = col_left; i < col_right; i++)
        hist[i]++;
}

template<class Iterator, class Pred>
void accumulateHistogram(Iterator begin, Iterator end, Histogram& hist, int histogram_offset,
        Pred condition) {
    std::for_each(begin, end, boost::bind(addVColumnToHistogram<Pred> , _1, hist, histogram_offset,
            condition));
}

template<class Iterator, class Pred>
int maxColumnWidth(Iterator start, Iterator end, Pred condition) {
    int res = 0;
    while (start != end) {
        if (condition(*start)) {
            res = std::max(res, (*start)->realWidth());
        }
        ++start;
    }
    return res;
}

template<class Iterator, class Pred>
Iterator findColumnWithMaxWidth(Iterator start, Iterator end, Pred condition) {
    int width = 0;
    Iterator res = end;
    while (start != end) {
        if (condition(*start) && (*start)->realWidth() > width) {
            width = (*start)->realWidth();
            res = start;
        }

        ++start;
    }

    return res;
}

CRtfHorizontalColumn::CRtfHorizontalColumn() :
    page_(NULL), type_(SINGLE_TERMINAL) {
    SetRect(&m_rect, 32000, 32000, 0, 0);
    SetRect(&m_rectReal, 32000, 32000, 0, 0);
}

CRtfHorizontalColumn::~CRtfHorizontalColumn() {
    clearColumns();
}

void CRtfHorizontalColumn::addColumn(CRtfVerticalColumn * col) {
    vcols_.push_back(col);
}

void CRtfHorizontalColumn::calcHorizontalColumn() {
    if (type_ == FRAME_AND_COLUMN || type_ == ALL_FRAME) {
        if (checkTerminalColumn()) {
            type_ = ALL_TERMINAL;
        } else {//~проверка вертикального затенения колонок ("жертва" будет frame)
            findHeadingAndSetFrameFlag(); //это проверка, что после удаления жертвы все стало лучше
            defineTerminalProperty(); //присвоение признака терминальности колонкам
        }
    }

    if (type_ <= FRAME_AND_COLUMN)
        fillTerminalColumnsIndex(); //есть хорошие колонки
}

void addBigColToHist(const CRtfVerticalColumn * col, Histogram& hist, int histogram_offset) {
    assert(col);

    if ((col->type() == FT_TEXT || col->type() == FT_FRAME) && !col->isSmall()) {
        const int col_left = col->m_rectReal.left - histogram_offset;
        const int col_right = col->m_rectReal.right - histogram_offset;

        assert((int) hist.size() < col_right);

        for (int i = col_left; i < col_right; i++)
            hist[i]++;
    }
}

bool CRtfHorizontalColumn::checkTerminalColumn() const {
    for (size_t i = 1; i < vcols_.size(); i++) {
        const CRtfVerticalColumn * vcol = vcols_[i];
        const CRtfVerticalColumn * vcol_prev = vcols_[i - 1];

        if (vcol->m_rectReal.top < vcol_prev->m_rectReal.bottom)
            return false;
    }

    return true;
}

void CRtfHorizontalColumn::clearColumns() {
    for (VColumnIterator it = vcols_.begin(), end = vcols_.end(); it != end; ++it)
        delete *it;

    vcols_.clear();
    terminal_col_group_.clear();
    terminal_col_idx_.clear();
}

CRtfVerticalColumn * CRtfHorizontalColumn::columnAt(size_t pos) {
    return vcols_.at(pos);
}

const CRtfVerticalColumn * CRtfHorizontalColumn::columnAt(size_t pos) const {
    return vcols_.at(pos);
}

size_t CRtfHorizontalColumn::columnCount() const {
    return vcols_.size();
}

void CRtfHorizontalColumn::setPage(CRtfPage * page) {
    page_ = page;
}

void CRtfHorizontalColumn::setType(column_t type) {
    type_ = type;
}

CRtfHorizontalColumn::column_t CRtfHorizontalColumn::type() const {
    return type_;
}

void determineColSize(CRtfVerticalColumn * col, int threshold_width, int threshold_height) {
    if (col->type() == FT_TEXT || col->type() == FT_FRAME) {
        if ((col->realWidth() < threshold_width) && (col->realHeight() < threshold_height)) {
            col->setSmall(true);
            col->setType(FT_FRAME);
        } else
            col->setSmall(false);
    }
}

void CRtfHorizontalColumn::markSmallColumns() {
    const int threshold_width = maxVColumnWidth() / 2;
    const int threshold_height = maxVColumnHeight() / 2;

    // all small column became frames
    std::for_each(vcols_.begin(), vcols_.end(), boost::bind(determineColSize, _1, threshold_width,
            threshold_height));
}

int CRtfHorizontalColumn::maxVColumnHeight() const {
    int max_height = 0;

    for (VColumnIteratorConst it = vcols_.begin(), end = vcols_.end(); it != end; ++it) {
        if ((*it)->type() == FT_TEXT || (*it)->type() == FT_FRAME)
            max_height = std::max(max_height, (*it)->realHeight());
    }

    return max_height;
}

int CRtfHorizontalColumn::maxVColumnWidth() const {
    int max_width = 0;

    for (VColumnIteratorConst it = vcols_.begin(), end = vcols_.end(); it != end; ++it) {
        if ((*it)->type() == FT_TEXT || (*it)->type() == FT_FRAME)
            max_width = std::max(max_width, (*it)->realWidth());
    }

    return max_width;
}

void CRtfHorizontalColumn::mergeColumnsToGroup(int minLeft, int maxRight, IndexList * group) {
    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfVerticalColumn * vcol = vcols_[i];

        //слияние секторов в одну колонку по вертикали
        if (vcol->type() == FT_TEXT && vcol->m_rectReal.left >= minLeft && vcol->m_rectReal.right
                <= maxRight) {
            group->push_back(i);
        }
    }
}

void CRtfHorizontalColumn::processColsByHist(const Histogram& hist, int left_offset) {
    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfVerticalColumn * vcol = vcols_[i];

        if ((vcol->type() == FT_TEXT || vcol->type() == FT_FRAME) && !vcol->isSmall()) {
            const int left = vcol->m_rectReal.left - left_offset;
            const int right = vcol->m_rectReal.right - left_offset;

            if (hist.findU(left, right))
                vcol->setType(FT_FRAME);
            else
                vcol->setType(FT_TEXT);
        }
    }
}

void CRtfHorizontalColumn::processSpaceByHist(const Histogram& hist) {
    hist_spaces_.clear();
    hist.spacePosition(std::back_inserter(hist_spaces_));
}

void CRtfHorizontalColumn::allTextToFrames() {
    for (VColumnIterator it = vcols_.begin(), end = vcols_.end(); it != end; ++it) {
        if ((*it)->type() == FT_TEXT)
            (*it)->setType(FT_FRAME);
    }
}

//после удаления жертвы по гистограммам проверяем разделимость остальных колонок и если да,
// все они будут терминал.
void CRtfHorizontalColumn::findHeadingAndSetFrameFlag() {
    markSmallColumns();
    const int left = columnMinLeftBorder(vcols_.begin(), vcols_.end(), VColumnBigTextAndFrame());
    const int right = columnMaxRightBorder(vcols_.begin(), vcols_.end(), VColumnBigTextAndFrame());
    assert(right - left >= 0);
    Histogram hist(right - left);

    accumulateHistogram(vcols_.begin(), vcols_.end(), hist, left, VColumnBigTextAndFrame());
    processColsByHist(hist, left);
}

int CRtfHorizontalColumn::findHighestUnsortedColumn() const {
    int res = -1;
    int top = std::numeric_limits<int>::max();

    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfVerticalColumn * col = vcols_[i];
        if (col->type() == FT_FRAME || col->isSorted())
            continue;

        if (col->m_rectReal.top < top) {
            top = col->m_rectReal.top;
            res = static_cast<int> (i);
        }

    }

    return res;
}

int CRtfHorizontalColumn::findHighestUnsortedColumnInGroup(const IndexList * group) const {
    assert(group);
    int res = -1;
    int top = std::numeric_limits<int>::max();

    for (size_t i = 0; i < group->size(); i++) {
        const int number = group->at(i);
        CRtfVerticalColumn * col = vcols_[number];

        if (col->type() == FT_FRAME || col->isSorted())
            continue;

        if (col->m_rectReal.top < top) {
            top = col->m_rectReal.top;
            res = number;
        }
    }

    return res;
}

void CRtfHorizontalColumn::defineTerminalProperty() {
    int left = columnMinLeftBorder(vcols_.begin(), vcols_.end(), VColumnText());
    int right = columnMaxRightBorder(vcols_.begin(), vcols_.end(), VColumnText());

    assert((right - left) > 0);
    Histogram hist(right - left);

    accumulateHistogram(vcols_.begin(), vcols_.end(), hist, left, VColumnText());
    processSpaceByHist(hist);

    int CountColumn = hist_spaces_.size();

    if (CountColumn) {
        fillTerminalGroups(left, right);
    }

    if (hist_spaces_.size()) {
        type_ = FRAME_AND_COLUMN;
    } else {
        divisionFailed();
    }
}

void CRtfHorizontalColumn::divisionFailed() {
    // all columns become frames
    allTextToFrames();

    VColumnIterator it = findColumnWithMaxWidth(vcols_.begin(), vcols_.end(), VColumnFrame());
    assert(it != vcols_.end());
    (*it)->setType(FT_TEXT);
    int max_col_idx = std::distance(vcols_.begin(), it);
    terminal_col_group_.push_back(IndexListPtr(new IndexList));
    terminal_col_group_.front()->push_back(max_col_idx);
    type_ = FRAME_AND_COLUMN;
}

void CRtfHorizontalColumn::fillTerminalGroups(int minLeft, int maxRight) {
    int left = 0, right = 0;
    const int column_count = hist_spaces_.size();

    for (int i = 0; i <= column_count; i++) {
        IndexList * group = new IndexList;
        terminal_col_group_.push_back(IndexListPtr(group));

        if (i == 0) {
            left = minLeft;
            right = minLeft + hist_spaces_[i];
        } else if (i == column_count) {
            left = minLeft + hist_spaces_[i - 1];
            right = maxRight;
        } else {
            left = minLeft + hist_spaces_[i - 1];
            right = minLeft + hist_spaces_[i];
        }

        mergeColumnsToGroup(left, right, group);
    }
}

void CRtfHorizontalColumn::fillAllTerminalColumnIndex() {
    terminal_col_idx_.clear();

    IndexList * group_idx = new IndexList;
    terminal_col_idx_.push_back(IndexListPtr(group_idx));

    sortColumns(group_idx);
}

void CRtfHorizontalColumn::fillSingleTerminalColumnIndex() {
    terminal_col_idx_.clear();
    terminal_col_idx_.push_back(IndexListPtr(new IndexList(1, 0)));
}

void CRtfHorizontalColumn::fillTerminalFrameColumnIndex() {
    for (size_t j = 0, col_count = terminal_col_group_.size(); j < col_count; j++) {
        IndexList * col_group = terminal_col_group_[j].get();
        IndexList * group_idx = new IndexList;
        terminal_col_idx_.push_back(IndexListPtr(group_idx));

        sortColumnsInGroup(col_group, group_idx);
    }
}

void CRtfHorizontalColumn::fillTerminalColumnsIndex() {
    switch (type_) {
    case SINGLE_TERMINAL:
        fillSingleTerminalColumnIndex();
        break;
        //section includes only terminal columns (! NO frames)
    case ALL_TERMINAL:
        fillAllTerminalColumnIndex();
        break;
    case FRAME_AND_COLUMN:
        fillTerminalFrameColumnIndex();
        break;
    default:
        Debug() << "[CRtfHorizontalColumn::fillVTerminalColumnsIndex] unhandled column type: "
                << type_ << "\n";
        break;
    }
}

int32_t CRtfHorizontalColumn::GetCountAndRightBoundVTerminalColumns(
        VectorWord* arRightBoundTerminalColumns, VectorWord* arWidthTerminalColumns) {
    int32_t CountVTerminalColumns = 0;
    CRtfVerticalColumn *pRtfVerticalColumn;
    uint16_t RightBound, index, WidthColumn, tmp;

    if (type_ == SINGLE_TERMINAL || type_ == ALL_TERMINAL) {
        RightBound = (uint16_t) MAX(m_rectReal.left, 0);
        WidthColumn = (uint16_t) (m_rectReal.right - m_rectReal.left);
        arRightBoundTerminalColumns->push_back(RightBound);
        arWidthTerminalColumns->push_back(WidthColumn);
        CountVTerminalColumns = 1;
        return CountVTerminalColumns;
    }

    if (type_ == FRAME_AND_COLUMN) {
        CountVTerminalColumns = terminal_col_idx_.size();

        for (int i = 0; i < CountVTerminalColumns; i++) {
            IndexList * pGroup = terminal_col_idx_[i].get();
            int CountInGroup = pGroup->size();
            RightBound = 32000;
            WidthColumn = 0;

            for (int j = 0; j < CountInGroup; j++) {
                index = (*pGroup)[j];
                pRtfVerticalColumn = vcols_[index];
                tmp = MAX(pRtfVerticalColumn->m_rectReal.left, 0);
                RightBound = MIN(RightBound, tmp);
                WidthColumn = MAX(WidthColumn, (uint16_t)(pRtfVerticalColumn->m_rectReal.right
                                - pRtfVerticalColumn->m_rectReal.left));
            }

            arRightBoundTerminalColumns->push_back(RightBound);
            arWidthTerminalColumns->push_back(WidthColumn);
        }
    }

    return CountVTerminalColumns;
}

void CRtfHorizontalColumn::WriteTerminalColumnsTablesAndPictures(RtfSectorInfo *SectorInfo) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    CRtfFragment* pRtfFragment;
    int CountFrameInTerminalColumn = vcols_.size();

    for (int i = 0; i < CountFrameInTerminalColumn; i++) {
        pRtfVerticalColumn = vcols_[i];
        pRtfFragment = pRtfVerticalColumn->firstFragment();

        if (pRtfFragment->type() == FT_TABLE || pRtfFragment->type() == FT_PICTURE) {
            if (type_ <= ALL_TERMINAL) {
                pRtfFragment->setInColumn(true);
                page_->setFragmentsInColumn(pRtfFragment);
            } else {
                SectorInfo->FlagInColumn = FALSE;
                pRtfFragment->setInColumn(false);
                pRtfVerticalColumn->write(SectorInfo, FOT_SINGLE);
            }
        }
    }
}

void CRtfHorizontalColumn::WriteTerminalColumns(VectorWord* arRightBoundTerminalColumns,
        int32_t* VTerminalColumnNumber, int32_t CountVTerminalColumns, RtfSectorInfo *SectorInfo) {
    int colsr(0), i(0), j(0), colw(0), CountInGroup(0);
    int32_t CountTerminalColumns, NextColumnsLeft, CountFrameInTerminalColumn, Left, Right;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment;
    uint16_t FreeSpace;
    int number;
    Bool FlagFirstTerminalFragment = FALSE;
    int32_t TopPositionFirstTerminalFragment;
    RECT Rect;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //***********************        Tерминальная колонка из одного или нескольких фрагментов  ******************************************
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (type_ == SINGLE_TERMINAL || type_ == ALL_TERMINAL) {
        *VTerminalColumnNumber += 1;

        if (RfrmtOptions::useFramesAndColumns() && *VTerminalColumnNumber == 1
                && CountVTerminalColumns > 1) {
            Rect.top = m_rectReal.top;
            Rect.bottom = m_rectReal.bottom;
            Rect.left = m_rectReal.right;
            Rect.right = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];

            if (CheckLines(&Rect, TRUE, SectorInfo)) {
                CED_SetSectLineBetCol(SectorInfo->hEDSector, TRUE);
            }
        }

        if (RfrmtOptions::useFramesAndColumns() && SectorInfo->FlagOneString == TRUE)
            colw = MAX(0, SectorInfo->PaperW - (SectorInfo->MargL + SectorInfo->MargR));
        else
            colw = m_rectReal.right - m_rectReal.left;

        colsr = -1;

        if (CountVTerminalColumns && *VTerminalColumnNumber < CountVTerminalColumns) {
            NextColumnsLeft = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
            colsr = NextColumnsLeft - m_rectReal.right;
        }

        if (*VTerminalColumnNumber == 1)
            SectorInfo->hColumn = SectorInfo->hFirstColumn;
        else
            SectorInfo->hColumn = CED_CreateColumn(SectorInfo->hEDSector);

        SectorInfo->hObject = SectorInfo->hColumn;

        CountFrameInTerminalColumn = vcols_.size();
        sortFragments();

        for (i = 0; i < CountFrameInTerminalColumn; i++) {
            number = i;

            if (m_arOrderingNumber.size())
                number = m_arOrderingNumber[i];

            pRtfVerticalColumn = vcols_[number];
            pRtfFragment = pRtfVerticalColumn->firstFragment();
            FreeSpace = GetFreeSpaceBetweenPrevAndCurrentFragments(pRtfFragment->m_rect.top,
                    SectorInfo);
            SectorInfo->VerticalOffsetFragmentInColumn = FreeSpace;

            if (pRtfFragment->type() != FT_TABLE && pRtfFragment->type() != FT_PICTURE) {//Text
                pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn = pRtfFragment->m_rect.left
                        - m_rect.left;
                pRtfFragment->m_RightOffsetFragmentFromVerticalColumn = m_rect.right
                        - pRtfFragment->m_rect.right;
                pRtfFragment->m_WidthVerticalColumn = int16_t(m_rect.right - m_rect.left);
                SectorInfo->hObject = SectorInfo->hColumn;
                SectorInfo->FlagOverLayed = getOverLayedFlag(i);
                pRtfVerticalColumn->write(SectorInfo, FOT_SINGLE);
            } else {
                if ((pRtfFragment->type() == FT_TABLE || pRtfFragment->type() == FT_PICTURE)
                        && pRtfFragment->inColumn()) {//Picture,Table
                    SectorInfo->FlagInColumn = TRUE;

                    if (!SectorInfo->FlagOneString)//!!!Art
                        SectorInfo->OffsetFromColumn.rx() = pRtfFragment->m_rect.left - m_rect.left;
                    else
                        //!!!Art
                        SectorInfo->OffsetFromColumn.rx() = pRtfFragment->m_rect.left
                                - SectorInfo->MargL;//!!!Art

                    SectorInfo->OffsetFromColumn.ry() = pRtfFragment->m_wOffsetFromPrevTextFragment;
                    SectorInfo->hObject = SectorInfo->hColumn;
                    pRtfVerticalColumn->write(SectorInfo, FOT_SINGLE);
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //***********************************                    Фреймы и колонки            ************************************************
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else {
        CountTerminalColumns = terminal_col_idx_.size();

        for (i = 0; i < CountTerminalColumns; i++) {
            *VTerminalColumnNumber += 1;
            Left = 32000;
            Right = 0;
            IndexList * pGroup = terminal_col_idx_[i].get();
            CountInGroup = pGroup->size();

            for (j = 0; j < CountInGroup; j++) { //~ V-columns in one H-col
                int index = (*pGroup)[j];
                pRtfVerticalColumn = vcols_[index];

                if (i == 0 && j == 0) {
                    FlagFirstTerminalFragment = TRUE;
                    TopPositionFirstTerminalFragment = pRtfVerticalColumn->m_rectReal.top;
                }

                Left = MIN(Left, pRtfVerticalColumn->m_rectReal.left);
                Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right);
            }

            colw = Right - Left;
            colsr = -1;

            if (*VTerminalColumnNumber < CountVTerminalColumns) {
                NextColumnsLeft = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
                colsr = NextColumnsLeft - Right;
            }

            if (*VTerminalColumnNumber == 1)
                SectorInfo->hColumn = SectorInfo->hFirstColumn;

            else
                SectorInfo->hColumn = CED_CreateColumn(SectorInfo->hEDSector);

            SectorInfo->hObject = SectorInfo->hColumn;

            if (!i) //noisy fragment or picture are made as frame,frames привязаны к первой терминал. колонке сектора
                writeFramesInTerminalColumn(SectorInfo, FlagFirstTerminalFragment);

            for (j = 0; j < CountInGroup; j++) {
                int index = (*pGroup)[j];
                pRtfVerticalColumn = vcols_[index];
                FreeSpace = GetFreeSpaceBetweenPrevAndCurrentFragments(
                        pRtfVerticalColumn->m_rect.top, SectorInfo);
                SectorInfo->VerticalOffsetFragmentInColumn = FreeSpace;
                SectorInfo->hObject = SectorInfo->hColumn;
                SectorInfo->FlagOverLayed = getOverLayedFlag(index);
                pRtfVerticalColumn->write(SectorInfo, FOT_SINGLE);
            }
        }
    }
}

uint16_t CRtfHorizontalColumn::GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFragment,
        RtfSectorInfo *SectorInfo) {
    CRtfFragment *pRtfFragment;
    RECT FreePlace;
    int i, CountFragments;
    int32_t FreePlaceHeight;
    FreePlace.left = m_rect.left;
    FreePlace.right = m_rect.right;
    FreePlace.bottom = TopPosCurFragment - 1;
    FreePlace.top = MAX(0, m_rect.top - SectorInfo->VerticalOffsetColumnFromSector);

    if (FreePlace.top >= FreePlace.bottom)
        return 0;

    CountFragments = page_->m_arFragments.size();

    for (i = 0; i < CountFragments; i++) {
        pRtfFragment = page_->m_arFragments[i];

        if (pRtfFragment->m_rect.bottom <= FreePlace.top || pRtfFragment->m_rect.right
                <= FreePlace.left || pRtfFragment->m_rect.top >= FreePlace.bottom
                || pRtfFragment->m_rect.left >= FreePlace.right)
            continue;

        if (pRtfFragment->m_rect.bottom >= FreePlace.top && pRtfFragment->m_rect.bottom
                <= FreePlace.bottom)
            FreePlace.top = pRtfFragment->m_rect.bottom;
    }

    FreePlaceHeight = MAX(0, FreePlace.bottom - FreePlace.top);
    return (uint16_t) FreePlaceHeight;
}

Bool CRtfHorizontalColumn::getOverLayedFlag(int CurFragmentNumber) {
    CIF::Point pt;
    int number = CurFragmentNumber;

    if (m_arOrderingNumber.size())
        number = m_arOrderingNumber[CurFragmentNumber];

    CRtfFragment * first_frag = vcols_.at(number)->firstFragment();
    Rect CurFragmentRect = first_frag->rect();
    int CountFragments = page_->m_arFragments.size();

    for (int i = 0; i < CountFragments; i++) {
        CRtfFragment * frag = page_->m_arFragments[i];

        if (frag->type() == FT_TEXT)
            continue;

        pt.rx() = frag->m_rect.left;
        pt.ry() = frag->m_rect.top;

        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt.rx() = frag->m_rect.right;

        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt.rx() = frag->m_rect.left;
        pt.ry() = frag->m_rect.bottom;

        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt.rx() = frag->m_rect.right;

        if (CurFragmentRect.contains(pt))
            return TRUE;
    }

    return FALSE;
}

void CRtfHorizontalColumn::sortColumns(IndexList * dest_idx) {
    for (size_t i = 0; i < vcols_.size(); i++) {
        int pos = findHighestUnsortedColumn();
        if (pos < 0)
            break;

        vcols_[pos]->setSorted(true);
        dest_idx->push_back(pos);
    }
}

void CRtfHorizontalColumn::sortColumnsInGroup(const IndexList* col_group, IndexList * dest_idx) {
    assert(col_group);
    assert(dest_idx);

    for (size_t i = 0; i < col_group->size(); i++) {
        int highest_col_idx = findHighestUnsortedColumnInGroup(col_group);

        if (highest_col_idx == -1)
            break;

        dest_idx->push_back(highest_col_idx);
        vcols_.at(highest_col_idx)->setSorted(true);
    }
}

void CRtfHorizontalColumn::sortFragments() {
    int32_t CountFrameInTerminalColumn;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment, *pRtfFragmentFirst;
    int /*NextTop=-32000,*/size, number;
    Bool FlagInserted = FALSE;
    CountFrameInTerminalColumn = vcols_.size();

    for (int i = 0; i < CountFrameInTerminalColumn; i++) {
        pRtfVerticalColumn = vcols_[i];
        pRtfFragment = pRtfVerticalColumn->firstFragment();
        FlagInserted = FALSE;

        if (i == 0)
            m_arOrderingNumber.push_back(i);

        else {
            size = m_arOrderingNumber.size();
            int m;

            for (m = 0; m < size; m++) {
                number = m_arOrderingNumber[m];
                pRtfVerticalColumn = vcols_[number];
                pRtfFragmentFirst = pRtfVerticalColumn->firstFragment();

                //Если фрагмент выше другого
                if (pRtfFragment->m_rect.top < pRtfFragmentFirst->m_rect.top) {
                    m_arOrderingNumber.insert(m_arOrderingNumber.begin() + m, i);

                    if (pRtfFragment->type() == FT_TABLE || pRtfFragment->type() == FT_PICTURE)
                        pRtfFragment->m_wOffsetFromPrevTextFragment
                                = getOffsetFromPrevTextFragment(pRtfFragment);

                    FlagInserted = TRUE;
                    break;
                }

                else

                //Если таблица/картинка покрывается текстовым фрагментом
                if ((pRtfFragment->type() == FT_TABLE || pRtfFragment->type() == FT_PICTURE)
                        && pRtfFragmentFirst->type() == FT_TEXT && pRtfFragment->m_rect.top
                        >= pRtfFragmentFirst->m_rect.top && pRtfFragment->m_rect.top
                        < pRtfFragmentFirst->m_rect.bottom) {
                    m_arOrderingNumber.insert(m_arOrderingNumber.begin() + m, i);
                    pRtfFragment->m_wOffsetFromPrevTextFragment
                            = (uint16_t) (pRtfFragment->m_rect.top - pRtfFragmentFirst->m_rect.top);
                    FlagInserted = TRUE;
                    break;
                }
            }

            if (FlagInserted == FALSE) {
                m_arOrderingNumber.push_back(m);
            }
        }
    }

    //Надо оттестировать
    /*  //Выставляем расстояние для картинок и таблиц, которые пойдут
     //во фреймы после последнго текстового фрагмента
     for( int m=m_arOrderingNumber.size()-1; m>=0; m-- )
     {
     pRtfVerticalColumn = vcols_[m_arOrderingNumber[m]];
     pRtfFragmentFirst   = pRtfVerticalColumn->m_arFragments[0];
     if (pRtfFragmentFirst->type_==FT_TEXT)
     break;
     }
     if (m<0)
     return;
     //нашли последний текстовый фрагмент - выставляем до него расстояние
     int lastTxtNum=m;
     //Ищем высоту фиктивного абзаца+разрыва колонки/раздела
     int16_t parHeight=0,brkHeight=0;
     GetRealSize("A",0,6,0,&parHeight);
     GetRealSize("A",0,24,0,&brkHeight);
     parHeight/=Twips;
     brkHeight/=Twips;
     for( m=m_arOrderingNumber.size()-1; m>lastTxtNum; m-- )
     {
     pRtfVerticalColumn = vcols_[m_arOrderingNumber[m]];
     pRtfFragment   = pRtfVerticalColumn->m_arFragments[0];
     if( (pRtfFragment->type_==FT_TABLE || pRtfFragment->type_==FT_PICTURE))
     {
     //вычитаем 2 высоты - фикт. абзаца и разрыва
     //если меньше, чем высота разрава, то чтобы он не упрыгнул вниз
     pRtfFragment->m_wOffsetFromPrevTextFragment = (uint16_t)MAX(brkHeight,pRtfFragment->m_rect.top - pRtfFragmentFirst->m_rect.bottom-brkHeight-parHeight);
     }
     }
     */
}

int CRtfHorizontalColumn::getOffsetFromPrevTextFragment(const CRtfFragment * frag) {
    int vert_offset = 0;

    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfFragment * cur_frag = vcols_[i]->firstFragment();

        if (cur_frag->type() == FT_TEXT && frag->m_rect.top >= cur_frag->m_rect.top
                && frag->m_rect.top < cur_frag->m_rect.bottom)
            vert_offset = frag->m_rect.top - cur_frag->m_rect.top;
    }

    return vert_offset;
}

void CRtfHorizontalColumn::writeFramesInTerminalColumn(RtfSectorInfo* SectorInfo,
        Bool FlagFirstTerminalFragment) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    int32_t shpleft, shptop, shpright, shpbottom, fri = 0;
    EDBOX EdFragmRect;
    Handle hParagraph = NULL;
    Handle hString = NULL;
    int align;
    CIF::Rect indent;
    EDSIZE interval;
    EDBOX playout;

    for (size_t i = 0; i < vcols_.size(); i++) {
        if (!fri) {
            fri = 1;
            indent = CIF::Rect();
            interval.cx = 0;
            interval.cy = 0;
            playout.x = -1;
            playout.w = -1;
            playout.y = -1;
            playout.h = -1;
            align = TP_LEFT_ALLIGN;
            hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, align,
                    indent, SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
            hString = CED_CreateLine(hParagraph, FALSE, 6);
        }

        pRtfVerticalColumn = vcols_[i];

        if (pRtfVerticalColumn->type() == FT_FRAME) {
            shpleft = pRtfVerticalColumn->m_rectReal.left - m_rectReal.left;
            shptop = pRtfVerticalColumn->m_rectReal.top - m_rectReal.top;
            shpbottom = pRtfVerticalColumn->m_rectReal.bottom - pRtfVerticalColumn->m_rectReal.top;
            shpright = pRtfVerticalColumn->m_rectReal.right - pRtfVerticalColumn->m_rectReal.left;

            EdFragmRect.x = pRtfVerticalColumn->m_rectReal.left - m_rectReal.left;
            EdFragmRect.w = pRtfVerticalColumn->m_rectReal.right
                    - pRtfVerticalColumn->m_rectReal.left;
            EdFragmRect.y = pRtfVerticalColumn->m_rectReal.top - m_rectReal.top;
            EdFragmRect.h = pRtfVerticalColumn->m_rectReal.bottom
                    - pRtfVerticalColumn->m_rectReal.top;
            SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn,
                    EdFragmRect, 0x22, -1, 86, 43);

            SectorInfo->FlagOverLayed = FALSE;
            pRtfVerticalColumn->write(SectorInfo, FOT_FRAME);
        }
    }
}

void CRtfHorizontalColumn::writeNonTerminalColumns(RtfSectorInfo* SectorInfo) {
    for (size_t i = 0; i < vcols_.size(); i++) {
        CRtfVerticalColumn* vcol = vcols_[i];

        if (vcol->type() > FT_FRAME) {
            SectorInfo->FlagInColumn = FALSE;
            vcol->write(SectorInfo, FOT_FRAME);
        }
    }
}

void CRtfHorizontalColumn::ToPlacePicturesAndTables(CRtfFragment* pRtfFragment) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    CRtfFragment* pRtfFragmentNew;
    vcols_.push_back(new CRtfVerticalColumn());

    if (vcols_.size() == 1)
        type_ = ONLY_PICTURE_TABLE;

    pRtfVerticalColumn = vcols_.back();
    pRtfVerticalColumn->setType(pRtfFragment->type());
    pRtfFragmentNew = new CRtfFragment;
    pRtfVerticalColumn->addFragment(pRtfFragmentNew);
    pRtfFragmentNew->setType(pRtfFragment->type());
    pRtfFragmentNew->m_wUserNumber = pRtfFragment->m_wUserNumber;
    pRtfFragmentNew->m_wUserNumberForFormattedMode = pRtfFragment->m_wUserNumberForFormattedMode;
    RtfUnionRect_CRect_CRect(&pRtfFragmentNew->m_rect, &pRtfFragment->m_rect);
}

}

