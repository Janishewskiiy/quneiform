/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include <QMessageBox>
#include <QDebug>

#include "dummyscannerdialog.h"

namespace {

AbstractScannerDialog * createDummyScannerDialog()
{
    DummyScannerDialog * d = new DummyScannerDialog();
    return d;
}

}

DummyScannerDialog::DummyScannerDialog(QObject *parent) :
    AbstractScannerDialog(parent)
{
}

void DummyScannerDialog::exec()
{
    QMessageBox::information(qobject_cast<QWidget*>(parent()),
                             tr("Information"),
                             tr("No scanner support"));
}

void DummyScannerDialog::registerDialog(int order)
{
    AbstractScannerDialog::registerDialogFunc(&createDummyScannerDialog, order);
}
