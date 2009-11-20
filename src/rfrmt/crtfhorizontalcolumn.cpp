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

#include "crtfhorizontalcolumn.h"
#include "crtfverticalcolumn.h"
#include "crtffragment.h"
#include "crtfpage.h"
#include "ced/ced.h"

using namespace CIF;

CRtfHorizontalColumn::CRtfHorizontalColumn() :
    m_rect(Point(32000, 32000), Point()), m_rectReal(Point(32000, 32000), Point()) {
    m_wVerticalColumnsCount = 0;
    m_wType = HC_SingleTerminal;
}

CRtfHorizontalColumn::~CRtfHorizontalColumn() {
    m_wVerticalColumnsCount = m_arVerticalColumns.size();
    for (int i = 0; i < m_wVerticalColumnsCount; i++) {
        CRtfVerticalColumn* cVerticalColumn = m_arVerticalColumns[i];
        delete cVerticalColumn;
    }

    int Count = m_arVTerminalColumnsGroup.size();
    for (int i = 0; i < Count; i++) {
        vectorWord* pGroup = m_arVTerminalColumnsGroup[i];
        delete pGroup;
    }

    Count = m_arVTerminalColumnsIndex.size();
    for (int i = 0; i < Count; i++) {
        vectorWord* pGroupIndex = m_arVTerminalColumnsIndex[i];
        delete pGroupIndex;
    }
}

//~ здесь попытка деления на колонки
void CRtfHorizontalColumn::CalcHorizontalColumn(void) {

    if (m_wType == HC_FrameAndColumn || m_wType == HC_AllFrame) {
        if (CheckTermColumn() == FALSE) //~проверка вертикал. затенения колонок ("жертва" будет frame)
        {
            FindHeadingAndSetFrameFlag(); //это проверка, что после удаления жертвы все стало лучше
            DefineTerminalProperty(); //присвоение признака терминальности колонкам
        }
        else
            m_wType = HC_AllTerminal;
    }

    if (m_wType <= HC_FrameAndColumn)
        FillingVTerminalColumnsIndex(); //есть хорошие колонки
}

Bool CRtfHorizontalColumn::CheckTermColumn(void) {
    CRtfVerticalColumn *pRtfVerticalColumn, *pRtfPrevVerticalColumn;
    m_wVerticalColumnsCount = m_arVerticalColumns.size();
    for (int i = 0; i < m_wVerticalColumnsCount; i++) {
        if (i) {
            pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
            pRtfPrevVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i - 1];
            if (pRtfVerticalColumn->m_rectReal.top() < pRtfPrevVerticalColumn->m_rectReal.bottom())
                return FALSE;
        }
    }

    return TRUE;
}

//после удаления жертвы по гистограммам проверяем разделимость остальных колонок и если да,
// все они будут терминал.
void CRtfHorizontalColumn::FindHeadingAndSetFrameFlag(void) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    std::vector<uchar> Hist;
    int Left = 32000, Right = 0, Length, Left1, Right1;
    int j, FlagLeft, FlagRight, tmp;
    int MaxWidth = 1, MaxHeight = 1;

    m_wVerticalColumnsCount = m_arVerticalColumns.size();

    for (int i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME) {
            MaxWidth = MAX(MaxWidth, pRtfVerticalColumn->m_rectReal.right()
                    - pRtfVerticalColumn->m_rectReal.left());
            MaxHeight = MAX(MaxHeight, pRtfVerticalColumn->m_rectReal.bottom()
                    - pRtfVerticalColumn->m_rectReal.top());
            pRtfVerticalColumn->m_bFlagSmall = FALSE; //в первую очередь жертвы ищутся среди малых
        }
    }

    for (int i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME) {
            if (((pRtfVerticalColumn->m_rectReal.right() - pRtfVerticalColumn->m_rectReal.left())
                    * 2 < MaxWidth) && ((pRtfVerticalColumn->m_rectReal.bottom()
                    - pRtfVerticalColumn->m_rectReal.top()) * 2 < MaxHeight))
                pRtfVerticalColumn->m_bFlagSmall = TRUE;
        }
    }

    for (int i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if ((pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME)
                && pRtfVerticalColumn->m_bFlagSmall == FALSE) {
            Left = MIN(Left, pRtfVerticalColumn->m_rectReal.left());
            Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right());
        }
    }
    Length = Right - Left + 2;

    for (int i = 0; i < Length; i++)
        Hist.push_back(0);

    for (int i = 0; i < m_wVerticalColumnsCount; i++)//~накопление гистограммы
    {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if ((pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME)
                && pRtfVerticalColumn->m_bFlagSmall == FALSE) {
            Left1 = pRtfVerticalColumn->m_rectReal.left() - Left;
            Right1 = pRtfVerticalColumn->m_rectReal.right() - Left;
            for (j = Left1; j < Right1; j++)
                Hist[j] += 1;
        }
    }

    for (int i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_bFlagSmall) {
            pRtfVerticalColumn->m_wType = FT_FRAME;
            continue;
        }
        if (pRtfVerticalColumn->m_wType == FT_TEXT || pRtfVerticalColumn->m_wType == FT_FRAME) {

            Left1 = pRtfVerticalColumn->m_rectReal.left() - Left;
            Right1 = pRtfVerticalColumn->m_rectReal.right() - Left;

            FlagLeft = 0;
            FlagRight = 0;
            tmp = Hist[Left1];

            for (j = Left1; j < Right1; j++) {
                if (tmp < Hist[j]) {
                    if (FlagLeft == 0)
                        tmp = Hist[j];
                    else if (FlagLeft == 1 && FlagRight == 0) {
                        FlagRight = 1;
                        break;
                    }
                }
                else if (tmp > Hist[j]) {
                    if (FlagLeft == 0) {
                        tmp = Hist[j];
                        FlagLeft = 1;
                    }
                    else if (FlagLeft == 1 && FlagRight == 0)
                        tmp = Hist[j];
                }
            }

            if (FlagRight)
                pRtfVerticalColumn->m_wType = FT_FRAME;
            else
                pRtfVerticalColumn->m_wType = FT_TEXT;

        }
    }
}

void CRtfHorizontalColumn::DefineTerminalProperty(void) { //~ recalculation of histogram after victim deletion
    std::vector<uchar> Hist;
    vectorWord* pGroup;
    CRtfVerticalColumn* pRtfVerticalColumn;
    int32_t Left = 32000, Right = 0, Length, Left1, Right1;
    int i, j, FlagSpace = 0, CountColumn, MinLeft, MaxRight;

    m_wVerticalColumnsCount = m_arVerticalColumns.size();
    for (i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_wType == FT_TEXT) {
            Left = MIN(Left, pRtfVerticalColumn->m_rectReal.left());
            Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right());
        }
    }

    MinLeft = Left;
    MaxRight = Right;
    Length = Right - Left;

    for (i = 0; i < Length; i++)
        Hist.push_back(0);

    for (i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_wType == FT_TEXT) {
            Left1 = pRtfVerticalColumn->m_rectReal.left() - Left;
            Right1 = pRtfVerticalColumn->m_rectReal.right() - Left;
            for (j = Left1; j < Right1; j++)
                Hist[j] += 1;
        }
    }

    for (j = 0; j < Length; j++) {
        if (Hist[j] == 0) {
            if (!FlagSpace) {
                FlagSpace = 1;
                m_arSpacePlace.push_back(j);
            }
        }
        else {
            if (FlagSpace)
                FlagSpace = 0;
        }
    }

    CountColumn = m_arSpacePlace.size();
    if (CountColumn) {
        for (i = 0; i <= CountColumn; i++) {
            m_arVTerminalColumnsGroup.push_back(new vectorWord());
            pGroup = (vectorWord*) m_arVTerminalColumnsGroup[i];
            if (i == 0) {
                Left = MinLeft;
                Right = MinLeft + m_arSpacePlace[i];
            }
            else if (i == CountColumn) {
                Left = MinLeft + m_arSpacePlace[i - 1];
                Right = MaxRight;
            }
            else {
                Left = MinLeft + m_arSpacePlace[i - 1];
                Right = MinLeft + m_arSpacePlace[i];
            }

            for (int j = 0; j < m_wVerticalColumnsCount; j++) {
                pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[j];
                if (pRtfVerticalColumn->m_wType == FT_TEXT && pRtfVerticalColumn->m_rectReal.left()
                        >= Left && pRtfVerticalColumn->m_rectReal.right() <= Right) {
                    pGroup->push_back(j); //~слияние секторов в одну колонку по вертикали
                }
            }
        }
    }

    if (m_arSpacePlace.size()) {
        m_wType = HC_FrameAndColumn;
        return;
    }
    //~неудача деления => all columns become frames
    for (i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_wType == FT_TEXT)
            pRtfVerticalColumn->m_wType = FT_FRAME;
    }

    int IndexMaxWidthFragment = 0, MaxWidth = 0;

    for (i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_wType == FT_FRAME) {
            if ((pRtfVerticalColumn->m_rectReal.right() - pRtfVerticalColumn->m_rectReal.left())
                    > MaxWidth) {
                MaxWidth = pRtfVerticalColumn->m_rectReal.right()
                        - pRtfVerticalColumn->m_rectReal.left();
                IndexMaxWidthFragment = i;
            }
        }
    }

    pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[IndexMaxWidthFragment];
    pRtfVerticalColumn->m_wType = FT_TEXT;
    m_arVTerminalColumnsGroup.push_back(new vectorWord());
    pGroup = m_arVTerminalColumnsGroup[0];
    pGroup->push_back(IndexMaxWidthFragment);

    m_wType = HC_FrameAndColumn;
}

//resorting of the fragment array (at first position should be located Frames, next ones are terminal columns)
void CRtfHorizontalColumn::FillingVTerminalColumnsIndex(void) {
    int i, j, FlagChange;
    uint16_t index, Number;
    int32_t Top;
    CRtfVerticalColumn *pRtfVerticalColumn;
    vectorWord *pGroup, *pGroupNew;

    if (m_wType == HC_SingleTerminal) {
        m_arVTerminalColumnsIndex.push_back(new vectorWord());
        pGroup = m_arVTerminalColumnsIndex[0];
        pGroup->push_back(0);
    }

    if (m_wType == HC_AllTerminal) //section includes only terminal columns (! NO frames)
    {
        m_arVTerminalColumnsIndex.push_back(new vectorWord());
        pGroup = m_arVTerminalColumnsIndex[0];

        m_wVerticalColumnsCount = m_arVerticalColumns.size();
        for (j = 0; j < m_wVerticalColumnsCount; j++) {
            FlagChange = 0;
            Top = 320000;
            for (i = 0; i < m_wVerticalColumnsCount; i++) {
                pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
                if (pRtfVerticalColumn->m_wType == FT_FRAME || pRtfVerticalColumn->m_bSortFlag == 1)
                    continue;
                if (pRtfVerticalColumn->m_rectReal.top() < Top) {
                    Top = pRtfVerticalColumn->m_rectReal.top();
                    index = i;
                    FlagChange = 1;
                }
            }
            if (FlagChange) {
                pGroup->push_back(index);
                pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[index];
                pRtfVerticalColumn->m_bSortFlag = 1;
            }
        }
    }

    if (m_wType == HC_FrameAndColumn) {
        int CountColumn = m_arVTerminalColumnsGroup.size();

        for (j = 0; j < CountColumn; j++) {
            m_arVTerminalColumnsIndex.push_back(new vectorWord());
            pGroupNew = m_arVTerminalColumnsIndex[j];

            pGroup = m_arVTerminalColumnsGroup[j];
            int CountInGroup = pGroup->size();
            for (i = 0; i < CountInGroup; i++) {
                FlagChange = 0;
                Top = 320000;
                for (int i1 = 0; i1 < CountInGroup; i1++) {
                    Number = (*pGroup)[i1];
                    pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[Number];
                    if (pRtfVerticalColumn->m_wType == FT_FRAME || pRtfVerticalColumn->m_bSortFlag
                            == 1)
                        continue;
                    if (pRtfVerticalColumn->m_rectReal.top() < Top) {
                        Top = pRtfVerticalColumn->m_rectReal.top();
                        index = Number;
                        FlagChange = 1;
                    }
                }
                if (FlagChange) {
                    pGroupNew->push_back(index);
                    pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[index];
                    pRtfVerticalColumn->m_bSortFlag = 1;
                }
            }
        }
    }
}

int32_t CRtfHorizontalColumn::GetCountAndRightBoundVTerminalColumns(
        vectorWord* arRightBoundTerminalColumns, vectorWord* arWidthTerminalColumns) {
    int32_t CountVTerminalColumns = 0;
    CRtfVerticalColumn *pRtfVerticalColumn;
    uint16_t RightBound, index, WidthColumn, tmp;
    vectorWord *pGroup;

    if (m_wType == HC_SingleTerminal || m_wType == HC_AllTerminal) {
        RightBound = (uint16_t) MAX(m_rectReal.left(), 0);
        WidthColumn = (uint16_t) (m_rectReal.right() - m_rectReal.left());
        arRightBoundTerminalColumns->push_back(RightBound);
        arWidthTerminalColumns->push_back(WidthColumn);
        CountVTerminalColumns = 1;
        return CountVTerminalColumns;
    }

    if (m_wType == HC_FrameAndColumn) {
        CountVTerminalColumns = m_arVTerminalColumnsIndex.size();
        for (int i = 0; i < CountVTerminalColumns; i++) {
            pGroup = m_arVTerminalColumnsIndex[i];
            int CountInGroup = pGroup->size();
            RightBound = 32000;
            WidthColumn = 0;
            for (int j = 0; j < CountInGroup; j++) {
                index = (*pGroup)[j];
                pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[index];
                tmp = MAX(pRtfVerticalColumn->m_rectReal.left(), 0);
                RightBound = MIN(RightBound, tmp);
                WidthColumn = MAX(WidthColumn, (uint16_t) (pRtfVerticalColumn->m_rectReal.right()
                                - pRtfVerticalColumn->m_rectReal.left()));
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

    int CountFrameInTerminalColumn = m_arVerticalColumns.size();
    for (int i = 0; i < CountFrameInTerminalColumn; i++) {
        pRtfVerticalColumn = m_arVerticalColumns[i];
        pRtfFragment = pRtfVerticalColumn->m_arFragments[0];
        if (pRtfFragment->m_wType == FT_TABLE || pRtfFragment->m_wType == FT_PICTURE) {
            if (m_wType <= HC_AllTerminal) {
                pRtfFragment->m_bFlagObjectInColumn = TRUE;
                pRtfVerticalColumn->m_bFlagObjectInColumn = TRUE;
                SetFlagObjectInColumnForPageFragment(pRtfFragment);
            }
            else {
                SectorInfo->FlagInColumn = FALSE;
                pRtfFragment->m_bFlagObjectInColumn = FALSE;
                pRtfVerticalColumn->m_bFlagObjectInColumn = FALSE;
                pRtfVerticalColumn->Write(FOT_SINGLE, SectorInfo);
            }
        }
    }
}

void CRtfHorizontalColumn::SetFlagObjectInColumnForPageFragment(CRtfFragment* CurFragment) {
    CRtfFragment* pRtfFragment;

    int CountFragments = m_PagePtr->m_arFragments.size();
    for (int i = 0; i < CountFragments; i++) {
        pRtfFragment = m_PagePtr->m_arFragments[i];

        if (pRtfFragment->m_wType == CurFragment->m_wType && pRtfFragment->m_rect.left()
                == CurFragment->m_rect.left() && pRtfFragment->m_rect.right()
                == CurFragment->m_rect.right() && pRtfFragment->m_rect.top()
                == CurFragment->m_rect.top() && pRtfFragment->m_rect.bottom()
                == CurFragment->m_rect.bottom())
            pRtfFragment->m_bFlagObjectInColumn = TRUE;
    }
}

void CRtfHorizontalColumn::WriteTerminalColumns(vectorWord* arRightBoundTerminalColumns,
        int32_t* VTerminalColumnNumber, int32_t CountVTerminalColumns, RtfSectorInfo *SectorInfo) {
    int colsr(0), i(0), j(0), colw(0), CountInGroup(0);
    int32_t CountTerminalColumns, NextColumnsLeft, CountFrameInTerminalColumn, Left, Right;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment;
    vectorWord *pGroup;
    uint16_t FreeSpace;
    int number;
    Bool FlagFirstTerminalFragment = FALSE;
    int32_t TopPositionFirstTerminalFragment;
    Rect Rect;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //***********************        Tерминальная колонка из одного или нескольких фрагментов  ******************************************
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (m_wType == HC_SingleTerminal || m_wType == HC_AllTerminal) {
        *VTerminalColumnNumber += 1;

        if (FlagMode & USE_FRAME_AND_COLUMN && *VTerminalColumnNumber == 1 && CountVTerminalColumns
                > 1) {
            Rect.rtop() = m_rectReal.top();
            Rect.rbottom() = m_rectReal.bottom();
            Rect.rleft() = m_rectReal.right();
            Rect.rright() = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
            if (CheckLines(&Rect, TRUE, SectorInfo)) {
                if (RtfWriteMode)
                    Put("\\linebetcol");
                else
                    CED_SetSectLineBetCol(SectorInfo->hEDSector, TRUE);
            }
        }

        PutCom("\\colno", *VTerminalColumnNumber, 1);

        if (FlagMode & USE_FRAME_AND_COLUMN && SectorInfo->FlagOneString == TRUE)
            colw = MAX(0, SectorInfo->PaperW - (SectorInfo->MargL + SectorInfo->MargR));
        else
            colw = m_rectReal.right() - m_rectReal.left();

        PutCom("\\colw", colw, 1);
        colsr = -1;

        if (CountVTerminalColumns && *VTerminalColumnNumber < CountVTerminalColumns) {
            NextColumnsLeft = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
            colsr = NextColumnsLeft - m_rectReal.right();
            PutCom("\\colsr", colsr, 1);
        }

#ifdef EdWrite
        if (!RtfWriteMode) {
            if (*VTerminalColumnNumber == 1)
                SectorInfo->hColumn = SectorInfo->hFirstColumn;
            else
                SectorInfo->hColumn = CED_CreateColumn(SectorInfo->hEDSector);

            SectorInfo->hObject = SectorInfo->hColumn;
        }
#endif

        CountFrameInTerminalColumn = m_arVerticalColumns.size();
        SortFragments();

        for (i = 0; i < CountFrameInTerminalColumn; i++) {
            number = i;
            if (m_arOrderingNumber.size())
                number = m_arOrderingNumber[i];
            pRtfVerticalColumn = m_arVerticalColumns[number];
            pRtfFragment = pRtfVerticalColumn->m_arFragments[0];

            FreeSpace = GetFreeSpaceBetweenPrevAndCurrentFragments(pRtfFragment->m_rect.top(),
                    SectorInfo);
            SectorInfo->VerticalOffsetFragmentInColumn = FreeSpace;

            if (pRtfFragment->m_wType != FT_TABLE && pRtfFragment->m_wType != FT_PICTURE) {//Text
                pRtfFragment->m_LeftOffsetFragmentFromVerticalColumn = pRtfFragment->m_rect.left()
                        - m_rect.left();

                pRtfFragment->m_RightOffsetFragmentFromVerticalColumn = m_rect.right()
                        - pRtfFragment->m_rect.right();

                pRtfFragment->m_WidthVerticalColumn = m_rect.width();
#ifdef EdWrite
                if (!RtfWriteMode)
                    SectorInfo->hObject = SectorInfo->hColumn;
#endif

                SectorInfo->FlagOverLayed = GetOverLayedFlag(i);
                pRtfVerticalColumn->Write(FOT_SINGLE, SectorInfo);
            }
            else {
                if ((pRtfFragment->m_wType == FT_TABLE || pRtfFragment->m_wType == FT_PICTURE)
                        && pRtfFragment->m_bFlagObjectInColumn) {//Picture,Table
                    SectorInfo->FlagInColumn = TRUE;
                    if (!SectorInfo->FlagOneString)//!!!Art
                        SectorInfo->OffsetFromColumn.rx() = pRtfFragment->m_rect.left()
                                - m_rect.left();
                    else
                        //!!!Art
                        SectorInfo->OffsetFromColumn.rx() = pRtfFragment->m_rect.left()
                                - SectorInfo->MargL;//!!!Art

                    SectorInfo->OffsetFromColumn.ry() = pRtfFragment->m_wOffsetFromPrevTextFragment;
#ifdef EdWrite
                    if (!RtfWriteMode)
                        SectorInfo->hObject = SectorInfo->hColumn;
#endif
                    pRtfVerticalColumn->Write(FOT_SINGLE, SectorInfo);
                }
            }
        }

        if (*VTerminalColumnNumber < CountVTerminalColumns)
            Put("\\column");
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //***********************************                    Фреймы и колонки            ************************************************
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else {
        CountTerminalColumns = m_arVTerminalColumnsIndex.size();

        for (i = 0; i < CountTerminalColumns; i++) {
            *VTerminalColumnNumber += 1;
            PutCom("\\colno", *VTerminalColumnNumber, 1);
            Left = 32000;
            Right = 0;

            pGroup = m_arVTerminalColumnsIndex[i];
            CountInGroup = pGroup->size();
            for (j = 0; j < CountInGroup; j++) //~ V-columns in one H-col
            {
                int index = (*pGroup)[j];
                pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[index];

                if (i == 0 && j == 0) {
                    FlagFirstTerminalFragment = TRUE;
                    TopPositionFirstTerminalFragment = pRtfVerticalColumn->m_rectReal.top();
                }
                Left = MIN(Left, pRtfVerticalColumn->m_rectReal.left());
                Right = MAX(Right, pRtfVerticalColumn->m_rectReal.right());
            }

            colw = Right - Left;
            PutCom("\\colw", colw, 1);
            colsr = -1;
            if (*VTerminalColumnNumber < CountVTerminalColumns) {
                NextColumnsLeft = (*arRightBoundTerminalColumns)[*VTerminalColumnNumber];
                colsr = NextColumnsLeft - Right;
                PutCom("\\colsr", colsr, 1);
            }

#ifdef EdWrite
            if (!RtfWriteMode) {
                if (*VTerminalColumnNumber == 1)
                    SectorInfo->hColumn = SectorInfo->hFirstColumn;
                else
                    SectorInfo->hColumn = CED_CreateColumn(SectorInfo->hEDSector);

                SectorInfo->hObject = SectorInfo->hColumn;
            }
#endif

            if (!i) //noisy fragment or picture are made as frame,frames привязаны к первой терминал. колонке сектора
                WriteFramesInTerminalColumn(SectorInfo);

            for (j = 0; j < CountInGroup; j++) {
                int index = (*pGroup)[j];
                pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[index];
                FreeSpace = GetFreeSpaceBetweenPrevAndCurrentFragments(
                        pRtfVerticalColumn->m_rect.top(), SectorInfo);
                SectorInfo->VerticalOffsetFragmentInColumn = FreeSpace;

#ifdef EdWrite
                if (!RtfWriteMode)
                    SectorInfo->hObject = SectorInfo->hColumn;
#endif

                SectorInfo->FlagOverLayed = GetOverLayedFlag(index);
                pRtfVerticalColumn->Write(FOT_SINGLE, SectorInfo);
            }

            if (*VTerminalColumnNumber < CountVTerminalColumns)
                Put("\\column");
        }
    }
}

uint16_t CRtfHorizontalColumn::GetFreeSpaceBetweenPrevAndCurrentFragments(int TopPosCurFragment,
        RtfSectorInfo *SectorInfo) {
    CRtfFragment *pRtfFragment;
    Rect FreePlace;
    int i, CountFragments;
    int32_t FreePlaceHeight;

    FreePlace.rleft() = m_rect.left();
    FreePlace.rright() = m_rect.right();
    FreePlace.rbottom() = TopPosCurFragment - 1;
    FreePlace.rtop() = MAX(0, m_rect.top() - SectorInfo->VerticalOffsetColumnFromSector);
    if (FreePlace.top() >= FreePlace.bottom())
        return 0;

    CountFragments = m_PagePtr->m_arFragments.size();
    for (i = 0; i < CountFragments; i++) {
        pRtfFragment = m_PagePtr->m_arFragments[i];

        if (pRtfFragment->m_rect.bottom() <= FreePlace.top() || pRtfFragment->m_rect.right()
                <= FreePlace.left() || pRtfFragment->m_rect.top() >= FreePlace.bottom()
                || pRtfFragment->m_rect.left() >= FreePlace.right())
            continue;

        if (pRtfFragment->m_rect.bottom() >= FreePlace.top() && pRtfFragment->m_rect.bottom()
                <= FreePlace.bottom())
            FreePlace.rtop() = pRtfFragment->m_rect.bottom();
    }
    FreePlaceHeight = MAX(0, FreePlace.bottom() - FreePlace.top());

    return (uint16_t) FreePlaceHeight;
}

Bool CRtfHorizontalColumn::GetOverLayedFlag(int CurFragmentNumber) {
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment;
    Rect CurFragmentRect;
    int number, CountFragments;
    Point pt;

    number = CurFragmentNumber;
    if (m_arOrderingNumber.size())
        number = m_arOrderingNumber[CurFragmentNumber];
    pRtfVerticalColumn = m_arVerticalColumns[number];
    pRtfFragment = pRtfVerticalColumn->m_arFragments[0];

    CurFragmentRect.rleft() = pRtfFragment->m_rect.left();
    CurFragmentRect.rright() = pRtfFragment->m_rect.right();
    CurFragmentRect.rbottom() = pRtfFragment->m_rect.bottom();
    CurFragmentRect.rtop() = pRtfFragment->m_rect.top();

    CountFragments = m_PagePtr->m_arFragments.size();
    for (int i = 0; i < CountFragments; i++) {
        pRtfFragment = m_PagePtr->m_arFragments[i];

        if (!pRtfFragment->m_wType)
            continue;
        pt = pRtfFragment->m_rect.leftTop();
        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt = pRtfFragment->m_rect.rightTop();
        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt = pRtfFragment->m_rect.leftBottom();
        if (CurFragmentRect.contains(pt))
            return TRUE;

        pt = pRtfFragment->m_rect.rightBottom();
        if (CurFragmentRect.contains(pt))
            return TRUE;
    }

    return FALSE;
}

void CRtfHorizontalColumn::SortFragments() {
    int32_t CountFrameInTerminalColumn;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragment, *pRtfFragmentFirst;
    int size, number;
    Bool FlagInserted = FALSE;

    CountFrameInTerminalColumn = m_arVerticalColumns.size();
    for (int i = 0; i < CountFrameInTerminalColumn; i++) {
        pRtfVerticalColumn = m_arVerticalColumns[i];
        pRtfFragment = pRtfVerticalColumn->m_arFragments[0];
        FlagInserted = FALSE;

        if (i == 0)
            m_arOrderingNumber.push_back(i);
        else {
            size = m_arOrderingNumber.size();
            int m;
            for (m = 0; m < size; m++) {
                number = m_arOrderingNumber[m];
                pRtfVerticalColumn = m_arVerticalColumns[number];
                pRtfFragmentFirst = pRtfVerticalColumn->m_arFragments[0];

                //Если фрагмент выше другого
                if (pRtfFragment->m_rect.top() < pRtfFragmentFirst->m_rect.top()) {
                    m_arOrderingNumber.insert(m_arOrderingNumber.begin() + m, i);
                    if (pRtfFragment->m_wType == FT_TABLE || pRtfFragment->m_wType == FT_PICTURE)
                        pRtfFragment->m_wOffsetFromPrevTextFragment
                                = GetOffsetFromPrevTextFragment(pRtfFragment);
                    FlagInserted = TRUE;
                    break;
                }
                else
                //Если таблица/картинка покрывается текстовым фрагментом
                if ((pRtfFragment->m_wType == FT_TABLE || pRtfFragment->m_wType == FT_PICTURE)
                        && pRtfFragmentFirst->m_wType == FT_TEXT && pRtfFragment->m_rect.top()
                        >= pRtfFragmentFirst->m_rect.top() && pRtfFragment->m_rect.top()
                        < pRtfFragmentFirst->m_rect.bottom()) {
                    m_arOrderingNumber.insert(m_arOrderingNumber.begin() + m, i);
                    pRtfFragment->m_wOffsetFromPrevTextFragment
                            = (uint16_t) (pRtfFragment->m_rect.top()
                                    - pRtfFragmentFirst->m_rect.top());
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
     pRtfVerticalColumn = m_arVerticalColumns[m_arOrderingNumber[m]];
     pRtfFragmentFirst   = pRtfVerticalColumn->m_arFragments[0];
     if (pRtfFragmentFirst->m_wType==FT_TEXT)
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
     pRtfVerticalColumn = m_arVerticalColumns[m_arOrderingNumber[m]];
     pRtfFragment   = pRtfVerticalColumn->m_arFragments[0];
     if( (pRtfFragment->m_wType==FT_TABLE || pRtfFragment->m_wType==FT_PICTURE))
     {
     //вычитаем 2 высоты - фикт. абзаца и разрыва
     //если меньше, чем высота разрава, то чтобы он не упрыгнул вниз
     pRtfFragment->m_wOffsetFromPrevTextFragment = (uint16_t)MAX(brkHeight,pRtfFragment->rect.top() - pRtfFragmentFirst->rect.bottom-brkHeight-parHeight);
     }
     }
     */
}

uint16_t CRtfHorizontalColumn::GetOffsetFromPrevTextFragment(CRtfFragment *pRtfFragment) {
    int32_t CountFrameInTerminalColumn;
    CRtfVerticalColumn *pRtfVerticalColumn;
    CRtfFragment *pRtfFragmentNext;
    uint16_t VerOffset = 0;
    CountFrameInTerminalColumn = m_arVerticalColumns.size();
    for (int i = 0; i < CountFrameInTerminalColumn; i++) {
        pRtfVerticalColumn = m_arVerticalColumns[i];
        pRtfFragmentNext = pRtfVerticalColumn->m_arFragments[0];

        if (pRtfFragmentNext->m_wType == FT_TEXT && pRtfFragment->m_rect.top()
                >= pRtfFragmentNext->m_rect.top() && pRtfFragment->m_rect.top()
                < pRtfFragmentNext->m_rect.bottom())
            VerOffset = (uint16_t) (pRtfFragment->m_rect.top() - pRtfFragmentNext->m_rect.top());
    }
    return VerOffset;
}

void CRtfHorizontalColumn::WriteFramesInTerminalColumn(RtfSectorInfo* SectorInfo) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    int32_t shpleft, shptop, shpright, shpbottom, fri = 0;

#ifdef EdWrite
    EDBOX EdFragmRect;

    Handle hParagraph = NULL;
    Handle hString = NULL;
    int align;
    Rect indent;
    Size interval;
    EDBOX playout;
#endif

    m_wVerticalColumnsCount = m_arVerticalColumns.size();
    for (int i = 0; i < m_wVerticalColumnsCount; i++) {
        if (!fri) {
            Put("\\pard\\fs6\\par");
            fri = 1;
#ifdef EdWrite
            if (!RtfWriteMode) {
                indent.rleft() = 0;
                indent.rright() = 0;
                indent.rtop() = 0;
                indent.rbottom() = 0;
                interval.rwidth() = 0;
                interval.rheight() = 0;
                playout.x = -1;
                playout.w = -1;
                playout.y = -1;
                playout.h = -1;

                align = TP_LEFT_ALLIGN;
                hParagraph = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, align,
                        indent, SectorInfo->userNum, -1, interval, playout, -1, -1, -1, -1, FALSE);
                hString = CED_CreateLine(hParagraph, FALSE, 6);
            }
#endif
        }

        pRtfVerticalColumn = (CRtfVerticalColumn*) m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_wType == FT_FRAME) {
            Put("{\\pard\\plain\\slmult1\\nowidctlpar\\pvpara");
            shpleft = pRtfVerticalColumn->m_rectReal.left() - m_rectReal.left();
            PutCom("\\posx", shpleft, 0);
            shptop = pRtfVerticalColumn->m_rectReal.top() - m_rectReal.top();
            PutCom("\\posy", shptop, 0);
            shpbottom = pRtfVerticalColumn->m_rectReal.bottom()
                    - pRtfVerticalColumn->m_rectReal.top();
            PutCom("\\absh", shpbottom, 0);
            shpright = pRtfVerticalColumn->m_rectReal.right()
                    - pRtfVerticalColumn->m_rectReal.left();
            PutCom("\\absw", shpright, 0);
            Put("\\abslock1");
            Put("\\dxfrtext86\\dfrmtxtx86\\dfrmtxty43\\adjustright");

#ifdef EdWrite
            if (!RtfWriteMode) {
                EdFragmRect.x = pRtfVerticalColumn->m_rectReal.left() - m_rectReal.left();
                EdFragmRect.w = pRtfVerticalColumn->m_rectReal.right()
                        - pRtfVerticalColumn->m_rectReal.left();
                EdFragmRect.y = pRtfVerticalColumn->m_rectReal.top() - m_rectReal.top();
                EdFragmRect.h = pRtfVerticalColumn->m_rectReal.bottom()
                        - pRtfVerticalColumn->m_rectReal.top();
                SectorInfo->hObject = CED_CreateFrame(SectorInfo->hEDSector, SectorInfo->hColumn,
                        EdFragmRect, 0x22, -1, 86, 43);
            }
#endif

            SectorInfo->FlagOverLayed = FALSE;
            pRtfVerticalColumn->Write(FOT_FRAME, SectorInfo);
            Put("}");
        }
    }
}

void CRtfHorizontalColumn::WriteNonTerminalColumns(RtfSectorInfo* SectorInfo) {
    CRtfVerticalColumn* pRtfVerticalColumn;

    m_wVerticalColumnsCount = m_arVerticalColumns.size();
    for (int i = 0; i < m_wVerticalColumnsCount; i++) {
        pRtfVerticalColumn = m_arVerticalColumns[i];
        if (pRtfVerticalColumn->m_wType > FT_FRAME) {
            SectorInfo->FlagInColumn = FALSE;
            pRtfVerticalColumn->Write(FOT_FRAME, SectorInfo);
        }
    }
}

void CRtfHorizontalColumn::ToPlacePicturesAndTables(CRtfFragment* pRtfFragment) {
    CRtfVerticalColumn* pRtfVerticalColumn;
    CRtfFragment* pRtfFragmentNew;

    m_arVerticalColumns.push_back(new CRtfVerticalColumn());
    m_wVerticalColumnsCount = m_arVerticalColumns.size();
    if (m_wVerticalColumnsCount == 1)
        m_wType = HC_ONLY_PICTURE_TABLE;

    pRtfVerticalColumn = m_arVerticalColumns.back();
    pRtfVerticalColumn->m_wFragmentsCount = 1;
    pRtfVerticalColumn->m_wType = pRtfFragment->m_wType;
    pRtfVerticalColumn->m_arFragments.push_back(new CRtfFragment());

    pRtfFragmentNew = pRtfVerticalColumn->m_arFragments.back();
    pRtfFragmentNew->m_wType = pRtfFragment->m_wType;
    pRtfFragmentNew->m_wUserNumber = pRtfFragment->m_wUserNumber;
    pRtfFragmentNew->m_wUserNumberForFormattedMode = pRtfFragment->m_wUserNumberForFormattedMode;

    pRtfFragmentNew->m_rect |= pRtfFragment->m_rect;
}