#include "drawwidget.h"

QFont getFont()
{
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(defaultFontSize);
    return font;
}

void drawWidget::drawText(QPoint pos, QString str, QPainter& painter)
{
    painter.setFont(getFont());
    painter.setPen(QPen(Qt::black));
    QRect x(pos.x() - 24, pos.y() - 24, 48, 48);
    painter.drawText(x, Qt::AlignCenter, str);
}

void drawWidget::drawLineText(QPoint beg, QPoint end, QString str, QPainter& painter)
{
    if (!isShowEdgeValue) return;
    painter.setFont(getFont());
    painter.setPen(QPen(Qt::black));
    QPoint pos= (beg + end) / 2;
    painter.drawText(pos, str);
}

void drawWidget::drawCircle(Circle x, QColor color)
{
    QPainter *painter = new QPainter(this);
    painter->begin(pixCircle);
    painter->setPen(QPen(color));
    painter->setBrush(QBrush(color));
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawEllipse(x.pos(), x.size(), x.size());

    drawText(x.pos(), QString::number(x.id()), *painter);

    painter->end();
    update();
}

void drawWidget::drawArrowLine(QPoint begin, QPoint end, QPainter& painter, QColor color, double size)
{
    double par = size;
    double slopy = atan2((end.y() - begin.y()), (end.x() - begin.x()));
    double cosy = cos(slopy);
    double siny = sin(slopy);

    QPoint point1 = QPoint(end.x() + int(-par*cosy - (par / 2.0*siny)), end.y() + int(-par*siny + (par / 2.0*cosy)));
    QPoint point2 = QPoint(end.x() + int(-par*cosy + (par / 2.0*siny)), end.y() - int(par / 2.0*cosy + par*siny));
    QPoint points[3] = { end, point1, point2 };

    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen drawTrianglePen;
    drawTrianglePen.setColor(color);
    drawTrianglePen.setWidth(3);
    painter.setPen(drawTrianglePen);
    painter.setBrush(QBrush(color));

    painter.drawPolygon(points, 3);
}

void drawWidget::drawLine(int id, QPoint beg, QPoint end, QColor color, int width)
{
    QPainter *painter = new QPainter(this);

    pixTemp->fill(QColor(0, 0, 0, 0));

    if (id == -1){
        painter->begin(pixTemp);
    }
    else{
        painter->begin(pixLine);
    }


    painter->setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawLine(beg, end);


    if (id == -1){
        drawArrowLine(beg, end, *painter, color);
    }
    else{
        QPoint v = end - beg;
        double x = v.x(), y = v.y();
        double t = sqrt(x * x + y * y);
        int i = graph.getCircleIndex(end);
        double k = (t - graph.circle(i).size()) / t;

        QPoint tag = beg + v * k;

        drawArrowLine(beg, tag, *painter, color);

        drawLineText(beg, end, QString::number(graph.val(id)), *painter);
    }

    painter->end();
    update();
}

void drawWidget::drawLine(int beg, int end, QColor color, int width)
{
    if (!graph.edge(beg, end)) return;
    QPoint a = graph.circle(beg).pos(), b = graph.circle(end).pos();
    drawLine(graph.edgeID(beg, end), a, b, color, width);
}

void drawWidget::drawLineAnimation(Line x, QColor color)
{
    QPainter *painter = new QPainter(this);

    painter->begin(pixTemp);

    painter->setPen(QPen(color, defaultLineSize + 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPoint v = x.v(), beg = x.begin();
    double tx = v.x(), ty = v.y();
    double t = sqrt(tx * tx + ty * ty);
    int i = graph.getCircleIndex(x.end());
    double k = (t - graph.circle(i).size() + 2) / t;
    double ed = (t - graph.circle(i).size() - 2) / t;
    QPoint tag = beg + v * k;

    int time = speed / 25;

    for (double i = 0; i < ed; i += 0.01){
        sleep(time);
        painter->drawLine(beg, beg + v * i);
        update();
    }

    drawArrowLine(beg, tag, *painter, color, 20.0);

    painter->end();
}

void drawWidget::drawLineAnimation(int beg, int end, QColor color)
{
    if (beg >= graph.size() || end >= graph.size()) return;
    int id = graph.edgeID(beg, end);
    if (id == -1) return;
    drawLineAnimation(graph.line(id), color);
}
