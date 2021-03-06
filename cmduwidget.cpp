#include "cmduwidget.h"
#include "dde-dock/constants.h"
#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QSvgRenderer>
#include <QMouseEvent>

#define PLUGIN_STATE_KEY    "enable"

CMDUWidget::CMDUWidget(QWidget *parent)
    : QWidget(parent),
      m_settings("deepin", "dde-dock-cmdu")
{
    font.setFamily("Noto Mono");
    text = " ↑    0KB/s \n ↓    0KB/s ";
    mp = 0;
    cp = 0;
}

bool CMDUWidget::enabled()
{
    return m_settings.value(PLUGIN_STATE_KEY, true).toBool();
}

void CMDUWidget::setEnabled(const bool b)
{
    m_settings.setValue(PLUGIN_STATE_KEY, b);
}

QSize CMDUWidget::sizeHint() const
{
    QFontMetrics FM(font);
    return FM.boundingRect(" ↑    0KB/s ").size() + QSize(0, FM.boundingRect(" ↓    0KB/s ").height());
}

void CMDUWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void CMDUWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::white);
    painter.setFont(font);
    painter.drawText(rect().adjusted(2,0,0,0), Qt::AlignLeft | Qt::AlignVCenter, text);
    if(mp < 90){
        painter.fillRect(0, height()*(100-mp)/100, 2, height()*mp/100, Qt::white);
    }else{
        painter.fillRect(0, height()*(100-mp)/100, 2, height()*mp/100, Qt::red);
    }
    painter.fillRect(width()-2, height()*(100-cp)/100, 2, height()*cp/100, Qt::white);
}

void CMDUWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::RightButton)
        return QWidget::mousePressEvent(e);

    const QPoint p(e->pos() - rect().center());
    if (p.manhattanLength() < std::min(width(), height()) * 0.8 * 0.5)
    {
        emit requestContextMenu();
        return;
    }

    QWidget::mousePressEvent(e);
}