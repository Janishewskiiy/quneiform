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

#include "page.h"
#include "pagepropertiesdialog.h"
#include "ui_pagepropertiesdialog.h"

static const char * PIXMAP_OK = ":/img/oxygen/22x22/dialog_ok.png";
static const char * PIXMAP_NO = ":/img/oxygen/22x22/button_cancel.png";

PagePropertiesDialog::PagePropertiesDialog(Page * p, QWidget * parent) :
    QDialog(parent),
    ui_(new Ui::PagePropertiesDialog)
{
    ui_->setupUi(this);
    ui_->file->setText(p->imagePath());

    QSize sz = p->imageSize();
    ui_->imageSize->setText(QString("%1x%2").arg(sz.width()).arg(sz.height()));
    ui_->rotation->setNum(p->angle());
    ui_->recognized->setText(p->isRecognized() ? tr("Yes") : tr("No"));
    ui_->saved->setText(p->isExported() ? tr("Yes") : tr("No"));
    ui_->language->setText(p->language().trName());
    ui_->sections->setNum(p->blocksCount(Page::SECTION));
    ui_->paragraphs->setNum(p->blocksCount(Page::PARAGRAPH));
    ui_->characters->setNum(p->blocksCount(Page::CHAR));
    ui_->images->setNum(p->blocksCount(Page::PICTURE));
}

PagePropertiesDialog::~PagePropertiesDialog()
{
    delete ui_;
}
