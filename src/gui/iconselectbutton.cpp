/*
    Copyright (c) 2016, Lukas Holecek <hluk@email.cz>

    This file is part of CopyQ.

    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "iconselectbutton.h"

#include "common/common.h"
#include "gui/iconfont.h"
#include "gui/iconselectdialog.h"
#include "gui/icons.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>
#include <QScopedPointer>

IconSelectButton::IconSelectButton(QWidget *parent)
    : QPushButton(parent)
    , m_currentIcon()
{
    setToolTip(tr("Select Icon..."));

    connect( this, SIGNAL(clicked()), SLOT(onClicked()) );

    // reset button text to "..."
    m_currentIcon = "X";
    setCurrentIcon(QString());
}

void IconSelectButton::setCurrentIcon(const QString &iconString)
{
    if ( m_currentIcon == iconString )
        return;

    m_currentIcon = iconString;

    setText(QString());
    setIcon(QIcon());

    if ( iconString.size() == 1 ) {
        const QChar c = iconString[0];
        if ( c.unicode() >= IconFirst && c.unicode() <= IconLast && QFontMetrics(iconFont()).inFont(c) ) {
            setFont(iconFont());
            setText(iconString);
        } else {
            m_currentIcon = QString();
        }
    } else if ( !iconString.isEmpty() ) {
        const QIcon icon(iconString);
        if ( icon.isNull() )
            m_currentIcon = QString();
        else
            setIcon(icon);
    }

    if (m_currentIcon.isEmpty()) {
        setFont(QFont());
        setText( tr("...", "Select/browse icon.") );
    }

    emit currentIconChanged(m_currentIcon);
}

QSize IconSelectButton::sizeHint() const
{
    const int h = QPushButton::sizeHint().height();
    return QSize(h, h);
}

void IconSelectButton::onClicked()
{
    QScopedPointer<IconSelectDialog> dialog( new IconSelectDialog(m_currentIcon, this) );

    // Set position under button.
    const QPoint dialogPosition = mapToGlobal(QPoint(0, height()));
    moveWindowOnScreen(dialog.data(), dialogPosition);

    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(dialog.data(), SIGNAL(iconSelected(QString)), this, SLOT(setCurrentIcon(QString)));
    dialog->open();
    dialog.take();
}

