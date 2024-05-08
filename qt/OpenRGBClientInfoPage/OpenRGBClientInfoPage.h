/*---------------------------------------------------------*\
| OpenRGBClientInfoPage.h                                   |
|                                                           |
|   User interface for OpenRGB client information page      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QFrame>
#include "RGBController.h"
#include "ui_OpenRGBClientInfoPage.h"
#include "NetworkClient.h"

namespace Ui
{
    class OpenRGBClientInfoPage;
}

class Ui::OpenRGBClientInfoPage : public QFrame
{
    Q_OBJECT

public:
    explicit OpenRGBClientInfoPage(QWidget *parent = nullptr);
    ~OpenRGBClientInfoPage();

    void AddClient(NetworkClient* new_client);

public slots:
    void UpdateInfo();

private slots:
    void changeEvent(QEvent *event);
    void on_ClientConnectButton_clicked();
    void onClientDisconnectButton_clicked(QObject * arg);
    void onClientSaveCheckBox_clicked(QObject * arg);

private:
    Ui::OpenRGBClientInfoPageUi *ui;
};
