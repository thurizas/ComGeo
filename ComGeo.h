#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ComGeo.h"

class ComGeo : public QMainWindow
{
    Q_OBJECT

public:
    ComGeo(QWidget *parent = Q_NULLPTR);

private:
    Ui::ComGeoClass ui;
};
