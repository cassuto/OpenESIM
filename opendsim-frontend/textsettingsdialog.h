/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef TEXTSETTINGSDIALOG_H_
#define TEXTSETTINGSDIALOG_H_

#include <QtWidgets>

namespace dsim
{

class TextSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  TextSettingsDialog( const QString &text, QWidget *parent = 0l );
  ~TextSettingsDialog();

  QString text() const;

private:
  QLineEdit *m_edit;
};

}

#endif
