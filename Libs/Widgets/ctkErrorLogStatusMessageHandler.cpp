/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.commontk.org/LICENSE

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

// Qt includes
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QStatusBar>

// CTK includes
#include "ctkErrorLogStatusMessageHandler.h"

// --------------------------------------------------------------------------
QString ctkErrorLogStatusMessageHandler::HandlerName = QLatin1String("Status");

// --------------------------------------------------------------------------
ctkErrorLogStatusMessageHandler::ctkErrorLogStatusMessageHandler(QMainWindow * mainWindow) :
  QObject(), ctkErrorLogAbstractMessageHandler()
{
  this->MainWindow = mainWindow;
}

// --------------------------------------------------------------------------
QString ctkErrorLogStatusMessageHandler::handlerName()const
{
  return ctkErrorLogStatusMessageHandler::HandlerName;
}

// --------------------------------------------------------------------------
void ctkErrorLogStatusMessageHandler::setEnabledInternal(bool value)
{
  if (!this->MainWindow)
    {
    qCritical() << "ctkErrorLogStatusMessageHandler - "
                   " Failed to enable ctkErrorLogStatusMessageHandler - "
                   "QMainWindow passed to the constructor is Null !";
    return;
    }
  if (!this->MainWindow->statusBar())
    {
    qCritical() << "ctkErrorLogStatusMessageHandler - Failed to enable the message handler: "
                   "There is no StatusBar associated with QMainWindow" << this->MainWindow;
    return;
    }
  if (value)
    {
    connect(this->MainWindow->statusBar(), SIGNAL(messageChanged(QString)),
            this, SLOT(statusBarMessageChanged(QString)));
    }
  else
    {
    disconnect(this->MainWindow->statusBar(), SIGNAL(messageChanged(QString)),
               this, SLOT(statusBarMessageChanged(QString)));
    }
}

//------------------------------------------------------------------------------
void ctkErrorLogStatusMessageHandler::statusBarMessageChanged(const QString& text)
{
  if (text.isEmpty())
    {
    return;
    }
  this->errorLogModel()->addEntry(
        ctkErrorLogModel::Status, this->handlerPrettyName(), text.toLatin1());
}
