
#include <QLayout>
#include <QMessageBox>
#include <QPen>
#include <QScreen>
#include <QtMath>
#include <QWidget>
#include <QtDataVisualization/Q3DSurface>

#include "graph3d.h"
#include "plot.h"
#include <qcustomplot.h>

using namespace QtDataVisualization;

//=============================================================
Plot::Plot(const QString &title, double sX, double eX, int dX, double sY, double eY, int dY, double min, double max) : QObject(),
    mBegX(sX), mBegY(sY), mCanUsed(false), mDimX(dX), mDimY(dY), mEndX(eX), mEndY(eY),
    mMax(max), mMaxArg(3), mMin(min),
    mTitle(title), mXTitle("X"), mYTitle("Y"), mZTitle("Z")
{
    // ctor
    setObjectName("3D");
    init();
}

//=============================================================
Plot::Plot(const QString &title, int dim, double sX, double eX, double sY, double eY) : QObject(),
    mBegX(sX), mBegY(sY), mDimX(dim), mDimY(dim), mEndX(eX), mEndY(eY),
    mMax(0.), mMaxArg(3), mMin(0.),
    mTitle(title), mXTitle("X"), mYTitle(""), mZTitle("Y")
{
    // ctor
    setObjectName("Graph");
    init();
}

//=============================================================
Plot::Plot(const QString &title, double sX, double eX, int dX) : QObject(),
    mBegX(sX), mBegY(0.), mDimX(dX), mDimY(0.), mEndX(eX), mEndY(0.),
    mMax(0), mMaxArg(3), mMin(0),
    mTitle(title), mXTitle("X"), mYTitle("Y"), mZTitle("")
{
    // ctor
    setObjectName("2D");
    init();
}

//=============================================================
Plot::Plot(const QString &title) : QObject(),
    mBegX(1), mBegY(0.), mDimX(0), mDimY(0.), mEndX(0), mEndY(0.),
    mMax(0), mMaxArg(0), mMin(0),
    mTitle(title), mXTitle("X"), mYTitle(""), mZTitle("Y")
{
    // ctor
    setObjectName("Chart");
    init();
}

//=============================================================
Plot::Plot()
{
    // a test
   QWidget *canvas = new QWidget;

    QCustomPlot *customPlot = new QCustomPlot();

    customPlot->legend->setVisible(true);

    // generate two sets of random walk data (one for candlestick and one for ohlc chart):
    int n = 500;
    QVector<double> time(n), value1(n), value2(n);
    QDateTime start = QDateTime(QDate(2014, 6, 11));
    start.setTimeSpec(Qt::UTC);
    double startTime = start.toTime_t();
    double binSize = 3600*24; // bin data in 1 day intervals
    time[0] = startTime;
    value1[0] = 60;
    value2[0] = 20;
    qsrand(9);
    for (int i=1; i<n; ++i)
    {
      time[i] = startTime + 3600*i;
      value1[i] = value1[i-1] + (qrand()/(double)RAND_MAX-0.5)*10;
      value2[i] = value2[i-1] + (qrand()/(double)RAND_MAX-0.5)*3;
    }

    // create candlestick chart:
    QCPFinancial *candlesticks = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
    candlesticks->setName("Candlestick");
    candlesticks->setChartStyle(QCPFinancial::csCandlestick);
    candlesticks->data()->set(QCPFinancial::timeSeriesToOhlc(time, value1, binSize, startTime));
    candlesticks->setWidth(binSize*0.9);
    candlesticks->setTwoColored(true);
    candlesticks->setBrushPositive(QColor(245, 245, 245));
    candlesticks->setBrushNegative(QColor(40, 40, 40));
    candlesticks->setPenPositive(QPen(QColor(0, 0, 0)));
    candlesticks->setPenNegative(QPen(QColor(0, 0, 0)));

    // create ohlc chart:
    QCPFinancial *ohlc = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
    ohlc->setName("OHLC");
    ohlc->setChartStyle(QCPFinancial::csOhlc);
    ohlc->data()->set(QCPFinancial::timeSeriesToOhlc(time, value2, binSize/3.0, startTime)); // divide binSize by 3 just to make the ohlc bars a bit denser
    ohlc->setWidth(binSize*0.2);
    ohlc->setTwoColored(true);

    // create bottom axis rect for volume bar chart:
    QCPAxisRect *volumeAxisRect = new QCPAxisRect(customPlot);
    customPlot->plotLayout()->addElement(1, 0, volumeAxisRect);

//    volumeAxisRect->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));

    volumeAxisRect->axis(QCPAxis::atBottom)->setLayer("axes");
    volumeAxisRect->axis(QCPAxis::atBottom)->grid()->setLayer("grid");
    // bring bottom and main axis rect closer together:
    customPlot->plotLayout()->setRowSpacing(0);
    volumeAxisRect->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom);
    volumeAxisRect->setMargins(QMargins(0, 0, 0, 0));
    // create two bar plottables, for positive (green) and negative (red) volume bars:
    customPlot->setAutoAddPlottableToLegend(false);
    QCPBars *volumePos = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
    QCPBars *volumeNeg = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
    for (int i=0; i<n/5; ++i)
    {
      int v = qrand()%20000+qrand()%20000+qrand()%20000-10000*3;
      (v < 0 ? volumeNeg : volumePos)->addData(startTime+3600*5.0*i, qAbs(v)); // add data to either volumeNeg or volumePos, depending on sign of v
    }
    volumePos->setWidth(3600*4);
    volumePos->setPen(Qt::NoPen);
    volumePos->setBrush(QColor(100, 180, 110));
    volumeNeg->setWidth(3600*4);
    volumeNeg->setPen(Qt::NoPen);
    volumeNeg->setBrush(QColor(180, 90, 90));

    // interconnect x axis ranges of main and bottom axis rects:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), volumeAxisRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
    connect(volumeAxisRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis, SLOT(setRange(QCPRange)));
    // configure axes of both main and bottom axis rect:
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeSpec(Qt::UTC);
    dateTimeTicker->setDateTimeFormat("dd. MMMM");
    volumeAxisRect->axis(QCPAxis::atBottom)->setTicker(dateTimeTicker);
    volumeAxisRect->axis(QCPAxis::atBottom)->setTickLabelRotation(15);
    customPlot->xAxis->setBasePen(Qt::NoPen);
    customPlot->xAxis->setTickLabels(false);
    customPlot->xAxis->setTicks(false); // only want vertical grid in main axis rect, so hide xAxis backbone, ticks, and labels
    customPlot->xAxis->setTicker(dateTimeTicker);
    customPlot->rescaleAxes();
    customPlot->xAxis->scaleRange(1.025, customPlot->xAxis->range().center());
    customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());

    // make axis rects' left side line up:
    QCPMarginGroup *group = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msLeft|QCP::msRight, group);
    volumeAxisRect->setMarginGroup(QCP::msLeft|QCP::msRight, group);

    customPlot->replot();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(customPlot);
    canvas->setLayout(layout);

    canvas->show();

}

//=============================================================
Plot::~Plot()
{
    // dtor
    clear();
}

//=============================================================
void Plot::addEntry(const QString &key, double the, double exp, double err, double comp1, double errcomp1, double comp2, double errcomp2)
{
    // adds data to hash table for chart
    int count = 3;
    static int kIndex = 0;
    QString kkey = QString("%1.%2").arg(kIndex++).arg(key);
    mDataC.insertMulti(kkey, the);
    mDataC.insertMulti(kkey, exp);
    mDataC.insertMulti(kkey, err);

    if (comp1 != 999) {
        mDataC.insertMulti(kkey, comp1);
        count++;
        mDataC.insertMulti(kkey, errcomp1);
        count++;
        if (comp2 != 999) {
            mDataC.insertMulti(kkey, comp2);
            count++;
            mDataC.insertMulti(kkey, errcomp2);
            count++;
        }
        if (mMaxArg == 0)
            mMaxArg = count;
        else if (count != mMaxArg) {
            QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
            if (count < mMaxArg)
                msg.setInformativeText("not enough parameters passed to addEntry");
            else
                msg.setInformativeText("too many parameters passed to addEntry");
            msg.exec();
        }
    }
    if (getCP()->graphCount() == 0) {
        getCP()->addGraph(getCP()->xAxis, getCP()->yAxis);
        getCP()->addGraph();
        getCP()->graph(0)->setName("Graph 1");
        getCP()->graph(1)->setName("Graph 2");
    }

    QMapIterator<QString, double> i(mDataC);
    while (i.hasNext()) {
        i.next();
        qDebug() << i.key() << ": " << i.value();
    }
}

//=============================================================
void Plot::addGraph(const QString &name, const QVector<double> &x, const QVector<double> &y)
{
    // fill data from arrays of double

    if (x.size() != y.size()) {
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("x and y must have the same number of elements");
        msg.exec();
        return;
    }
    for (int i = 0; i < x.size(); i++) {
        QVector2D * vec = new QVector2D(x.at(i), y.at(i));
        mData2.append(vec);
    }
    mCurves.append(x.size());
    mNames.append(name);
}

//=============================================================
void Plot::clear()
{
    qDeleteAll(mData2.begin(), mData2.end());
    mData2.clear();
    qDeleteAll(mData3.begin(), mData3.end());
    mData3.clear();
    mMaxArg = 0;
    setLin(getCP()->xAxis, getCP()->xAxis2);
    setLin(getCP()->yAxis, getCP()->yAxis2);
}

//=============================================================
void Plot::contour()
{
    // make a contour plot

    QWidget *localCanvas = getCanvas();
    getCP()->setParent(localCanvas);

    setCosmetics();

    localCanvas->setWindowTitle(mTitle + ": Contour X, Y");

    // set up the QCPColorMap:
    QCPColorMap *colorMap = new QCPColorMap(getCP()->xAxis, getCP()->yAxis);
    colorMap->data()->setSize(mDimX, mDimY);
    colorMap->data()->setRange(QCPRange(mBegX, mEndX), QCPRange(mBegX, mEndY));
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    for (int ix = 0; ix < mDimX; ix++) {
      for (int iy = 0; iy < mDimY; iy++) {
        double z = mData3.at(ix * mDimY + iy)->y();
        colorMap->data()->setCell(ix, iy, z);
      }
    }
    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(getCP());
    getCP()->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel(mZTitle);

    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpJet);

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();
    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(getCP());
    getCP()->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    getCP()->rescaleAxes();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(getCP());
    localCanvas->setLayout(layout);

    localCanvas->show();
}

//=============================================================
void Plot::contextMenuRequest(QPoint pos)
{
    // pop menu on right click in legend/axis/graph
    QMenu *menu = new QMenu(nullptr);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    QCPAxis *axis = nullptr;
    QCPAxis *axis2 = nullptr;

    // legend is selected
    if (getCP()->legend->selectTest(pos, false) >= 0) { // context menu on legend requested
      menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
      menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
      menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
      menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
      menu->addAction("Move to bottom center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignHCenter));
      menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
    }
    // an axis is selected
    else if (getCP()->xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
               getCP()->yAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
        if (getCP()->xAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
            axis  = getCP()->xAxis;
            axis2 = getCP()->xAxis2;
        } else {
            axis = getCP()->yAxis;
            axis2 = getCP()->yAxis2;
        }
        menu->addAction("Label", this, [this, axis]{ setLabelSignal(axis); });
        menu->addAction("Lin/Log", this, [this, axis, axis2]{ toggleLinLog(axis, axis2); });
    }
    // a least one graph is selected
    else if (getCP()->selectedGraphs().size() > 0 ) {
        //      menu->addAction("Add graph", this, SLOT(addGraph()));
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    }
    // any where
    else {
        QList<QCPLayoutElement*> elementsList = getCP()->plotLayout()->elements(false);
        for (QCPLayoutElement* element : elementsList) {
            if (element->selectTest(pos, false) >= 0.) {
                QCPAxisRect *axisElement = qobject_cast<QCPAxisRect*>(element);
                bool plus = true;
                menu->addAction("Resize++", this, [this, axisElement, plus]{ resizeGraph(axisElement, plus);});
                plus = false;
                menu->addAction("Resize--", this, [this, axisElement, plus]{ resizeGraph(axisElement, plus);});
            }
        }
    }
    menu->popup(getCP()->mapToGlobal(pos));
}

//=============================================================
void Plot::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
    // Rename a graph by double clicking on its legend item
    Q_UNUSED(legend)
    if (item) { // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
      QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
      bool ok;
      QString newName = QInputDialog::getText(nullptr, Q_FUNC_INFO, "Change name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
      if (ok) {
        plItem->plottable()->setName(newName);
        getCP()->replot();
      }
    }
}

//=============================================================
void Plot::mouseMoveSignal(QMouseEvent *event)
{
    // do something when mouse is moving
    double xm = event->x();
    double ym = event->y();

    int indexgraph = -1;
    int indexmin   = -1;
    double distmin = 99;
    if (getCP()->selectedGraphs().size() > 0 ) {
        int index = 0;
        for (index = 0; index < getCP()->graphCount(); index++) {
            if (getCP()->graph(index)->name() == getCP()->selectedGraphs().first()->name()) {
                indexgraph = index;
                break;
            }
        }
        if (indexgraph == -1)
            return;
        QCPGraph  *graph = getCP()->selectedGraphs().first();
        index = 0;
        for (index = 0; index < graph->dataCount(); index++) {
            double x = graph->dataPixelPosition(index).x();
            double y = graph->dataPixelPosition(index).y();
            double dist = qSqrt( (x - xm) * (x - xm) + (y - ym) * (y - ym));
            if (dist < distmin) {
                distmin = dist;
                indexmin = index;
            }
        }
        if (indexmin == -1)
            return;
        double key = 0;
        double valueT = 0;
        double valueD = 0.;
        graph->pixelsToCoords(graph->dataPixelPosition(indexmin).x(),
                              graph->dataPixelPosition(indexmin).y(),
                              key, valueT);
        if (indexgraph == 1) {
            QCPGraph *graphD = getCP()->graph(0);
            graphD->pixelsToCoords(graphD->dataPixelPosition(indexmin).x(),
                                   graphD->dataPixelPosition(indexmin).y(),
                                   key, valueD);
            mText->setText(QString("D:%1, T:%2").arg(valueD).arg(valueT));
            mText->position->setCoords(QPoint(getCP()->xAxis->pixelToCoord(xm), getCP()->yAxis->pixelToCoord(ym)));

        } else {
            mText->setText(QString("%1").arg(valueT));
            mText->position->setCoords(QPoint(graph->keyAxis()->pixelToCoord(xm), graph->valueAxis()->pixelToCoord(ym)));
        }

       getCP()->replot();
    }
}
//=============================================================
void Plot::mousePressSignal(QMouseEvent *event)
{
    // do something when mouse is clicked
    Q_UNUSED(event)
}

//=============================================================
void Plot::moveLegend()
{
    // move the legend as specified in the context menu
    if (QAction* contextAction = qobject_cast<QAction*>(sender())) { // make sure this slot is really called by a context menu action, so it carries the data we need
      bool ok;
      int dataInt = contextAction->data().toInt(&ok);
      if (ok) {
        getCP()->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
        getCP()->replot();
      }
    }
}

//=============================================================
void Plot::draw()
{
    // draw any kind of plot
    if (objectName() == "3D")
        draw3D();
    else if (objectName() == "2D")
        draw2D();
    else if (objectName() == "Graph")
        drawGraph();
    else if (objectName() == "Chart")
        drawChart();
}

//=============================================================
void Plot::fill(double x, double y, double z)
{
    // populate the 3D data array
    QVector3D *vec = new QVector3D(x, z, y);
    mData3.append(vec);
}

//=============================================================
void Plot::fill(double x, double y)
{
    // populate the 2D data array
    QVector2D * vec = new QVector2D(x, y);
    mData2.append(vec);
}

//=============================================================
void Plot::proX()
{
    // draw projection on X axis
    QVector<double> xx(mDimX), zz(mDimX);
    for (int ix = 0; ix < mDimX; ix++) {
        xx.replace(ix, mData3.at(ix)->x());
        zz.replace(ix, 0.);
    }

    double zmax = -9999999;
    double zmin =  9999999;
    for (int ix = 0; ix < mDimX; ix++) {
        double z = 0.;
        for (int iy = 0; iy < mDimY; iy++) {
            z = z + mData3.at(ix * mDimX + iy)->y();   // carefull the vector is v(x, z, y)
        }
        if (z > zmax)
            zmax = z;
        if (z < zmin)
            zmin = z;
        zz.replace(ix, z);
    }
    projection('X', xx, zz, zmin, zmax);
}

//=============================================================
void Plot::proY()
{
    // draw projection on Y axis
    QVector<double> yy(mDimY), zz(mDimY);
    for (int iy = 0; iy < mDimY; iy++) {
        yy.replace(iy, mData3.at(iy * mDimX)->z());
        zz.replace(iy, 0.);
    }
    double zmax = -9999999;
    double zmin =  9999999;
    for (int iy = 0; iy < mDimY; iy++) {
        double z = 0.;
        for (int ix = 0; ix < mDimX; ix++) {
            z = z + mData3.at(iy * mDimX + ix)->y();   // carefull the vector is v(x, z, y)
        }
        if (z > zmax)
            zmax = z;
        if (z < zmin)
            zmin = z;
        zz.replace(iy, z);
    }
    projection('Y', yy, zz, zmin, zmax);
}

//=============================================================
void Plot::removeSelectedGraph()
{
    // removes a graph selected in the context menu
    if (getCP()->selectedGraphs().size() > 0) {
        getCP()->removeGraph(getCP()->selectedGraphs().first());
        getCP()->replot();
    }
}

//=============================================================
void Plot::resizeGraph(QCPAxisRect *ar, bool plus)
{
    // change the size of a graph
    QCPAxis *yAxis  = ar->axis(QCPAxis::atLeft);

    int newPointSize     =  yAxis->labelFont().pointSize();
    int newMaximumHeight = ar->maximumSize().height();

    if (plus) {
        newMaximumHeight *=2;
        newPointSize     = qMin(getCP()->yAxis->labelFont().pointSize(), newPointSize * 2);
    } else {
        ar->setMaximumSize(QSize(QWIDGETSIZE_MAX, ar->maximumSize().height() / 2));
        newMaximumHeight /=2;
        newPointSize     = qMin(getCP()->yAxis->labelFont().pointSize(),newMaximumHeight / 10);
    }
    ar->setMaximumSize(QSize(QWIDGETSIZE_MAX, newMaximumHeight));
    yAxis->setLabelFont(QFont(
                            yAxis->labelFont().family(),
                            newPointSize,
                            yAxis->labelFont().weight(),
                            yAxis->labelFont().italic()));
    yAxis->setTickLabelFont(QFont(
                                yAxis->labelFont().family(),
                                newPointSize,
                                yAxis->labelFont().weight(),
                                yAxis->labelFont().italic()));
    getCP()->replot();
}

//=============================================================
void Plot::setLabelSignal(QCPAxis *axis)
{
    // changes the label of the axis selected in the context menu
    bool ok = false;
    QString newName = QInputDialog::getText(nullptr, Q_FUNC_INFO, "Change label:",
                                            QLineEdit::Normal, axis->label(), &ok);
    if (ok) {
        axis->setLabel(newName);
        getCP()->replot();
    }
}

//=============================================================
void Plot::titleDoubleClick(QMouseEvent *event)
{
    // change the title of the plot
    Q_UNUSED(event)
    if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender())) {
        // Set the plot title by double clicking on it
        bool ok;
        QString newTitle = QInputDialog::getText(nullptr, Q_FUNC_INFO, "New plot title:", QLineEdit::Normal, title->text(), &ok);
        if (ok) {
            title->setText(newTitle);
            getCP()->replot();
        }
    }
}

//=============================================================
void Plot::toggleLinLog(QCPAxis *axis, QCPAxis *axis2)
{
    // toggle between linear and logarithm scale of axis selected in context menu
    if(axis->scaleType() == QCPAxis::stLinear)
        setLog(axis, axis2);
    else
        setLin(axis, axis2);
    getCP()->replot();
}

//=============================================================
void Plot::setAxisTitle(const QChar &type, const QString &title)
{
    // sets a title to axis
    type.toUpper();
    if (type == 'X')
        mXTitle = title;
    else if (type == 'Y')
        mYTitle = title;
    else if (type == 'Z')
        mZTitle = title;
}

//=============================================================
void Plot::setAxisTitle(const QString &titleX, const QString &titleY, const QString &titleZ)
{
    //carefull with axes (y <-> z)
    mXTitle = titleX;
    mYTitle = titleY;
    mZTitle = titleZ;
    if (objectName() == "Graph")
        mZTitle = mYTitle;
}

//=============================================================
void Plot::setEntryName(int index, const QString &name)
{
    // set the name of graphe index 1, 2, ...
    if (getCP()->graphCount() == 0) {
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, "no Graph");
        msg.setInformativeText("No graph defined: add at least one entry (Plot::addEntry");
        msg.exec();
        return;
    }
    getCP()->graph(index - 1)->setName(name);
}

//=============================================================
void Plot::draw2D()
{
    // draw a 2D plot
    QVector<double> xx(mDimX), yy(mDimX);
    double ymax = -9999999;
    double ymin =  9999999;
    for (int ix = 0; ix < mDimX; ix++) {
        double y = mData2.at(ix)->y();
        if (y > ymax)
            ymax = y;
        if (y < ymin)
            ymin = y;
        xx.replace(ix, mData2.at(ix)->x());
        yy.replace(ix, y);
    }
    mZTitle = mYTitle;
    projection(' ', xx, yy, ymin, ymax);
}

//=============================================================
void Plot::draw3D()
{
    // make a 3D plot (x, z, y !! carefull with axis order)

    Q3DSurface *graph     = new Q3DSurface;
    graph->setTitle(mTitle);
    graph->axisX()->setTitle(mXTitle);
    graph->axisY()->setTitle(mZTitle);
    graph->axisZ()->setTitle(mYTitle);

    QWidget *container = QWidget::createWindowContainer(graph);

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2.0 , screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy((Qt::StrongFocus));

    mCanvas->setWindowTitle(mTitle);
    QHBoxLayout *mCanvasLayout = new QHBoxLayout();

    // setup the tool box
    QGroupBox *proTool = new QGroupBox("Projections", mCanvas);
    QRadioButton *rpax = new QRadioButton("all X");
    QRadioButton *rpay = new QRadioButton("all Y");
    QRadioButton *rss  = new QRadioButton("select slice");
    rss->setChecked(true);
    QRadioButton *rpsx = new QRadioButton("slice X");
    QRadioButton *rpsy = new QRadioButton("slice Y");
    QRadioButton *rcon = new QRadioButton("contours");
    QVBoxLayout *proToolLayout = new QVBoxLayout;
    proToolLayout->addWidget(rpax);
    proToolLayout->addWidget(rpay);
    proToolLayout->addWidget(rss);
    proToolLayout->addWidget(rpsx);
    proToolLayout->addWidget(rpsy);
    proToolLayout->addWidget(rcon);
    proTool->setLayout(proToolLayout);

    QVBoxLayout *toolBoxLayout   = new QVBoxLayout();
    toolBoxLayout->setAlignment(Qt::AlignTop);
    toolBoxLayout->addWidget(proTool);

    mCanvasLayout->addWidget(container, 1);
    mCanvasLayout->addLayout(toolBoxLayout);
    mCanvas->setLayout(mCanvasLayout);
    Graph3D *graph3D = new Graph3D(graph, mData3, mBegX, mEndX, mDimX, mBegY, mEndY, mDimY, mMin, mMax);
    connect(rpax, &QRadioButton::clicked, this, &Plot::proX);
    connect(rpay, &QRadioButton::clicked, this, &Plot::proY);
    connect(rcon, &QRadioButton::clicked, this, &Plot::contour);
    connect(rss,  &QRadioButton::clicked, graph3D, &Graph3D::select);
    connect(rpsx, &QRadioButton::clicked, graph3D, &Graph3D::sliceOnX);
    connect(rpsy, &QRadioButton::clicked, graph3D, &Graph3D::sliceOnY);

    mCanUsed = true;
    mCanvas->show();
}

//=============================================================
void Plot::drawChart()
{
    // draw a chart with custom labels and error bars

    if (getCP()->graphCount() == 0) {
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, "no Graph");
        msg.setInformativeText("No graph defined: add at least one entry (Plot::addEntry");
        msg.exec();
        return;
    }
    getCP()->setParent(getCanvas());

//    QCPStatisticalBox *statistical = new QCPStatisticalBox(getCP()->xAxis, getCP()->yAxis);
//    statistical->setWidth(0.05);
//    statistical->setWhiskerWidth(0.1);
    getCP()->graph(0)->setLineStyle(QCPGraph::lsNone);
    getCP()->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));
    QCPErrorBars *errors = new QCPErrorBars(getCP()->xAxis, getCP()->yAxis);
    errors->removeFromLegend();
    errors->setDataPlottable(getCP()->graph(0));

    getCP()->graph(1)->setPen(QPen(Qt::red));
    getCP()->graph(1)->setLineStyle(QCPGraph::lsNone);
    getCP()->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, Qt::red, Qt::white, 10));

    QVector<QString> labels;
    QVector<double> ythe, yexp, yerr, ycomp1, yerrcomp1, ycomp2, yerrcomp2;
    QMapIterator<QString, double> it(mDataC);
    QString previous("");
    while (it.hasNext()) {
        it.next();
        if (it.key() != previous) {
            previous = it.key();
            QString keyLabel = it.key().section('.', 1);
            labels.append(keyLabel);
            QList<double> values = mDataC.values(it.key());
            int index = values.size() - 1;
            ythe.append(values.at(index--));
            yexp.append(values.at(index--));
            yerr.append(values.at(index--));
            if (index > 0) {
                ycomp1.append(values.at(index--));
                yerrcomp1.append(values.at(index--));
                if (index > 0 ) {
                    ycomp2.append(values.at(index--));
                    yerrcomp2.append(values.at(index));
                }
            }
       }
    }
    QVector<double> ticks;
    for (int i = 1; i <= labels.size(); i++ )
        ticks << i;
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    getCP()->xAxis2->setTicker(textTicker);
    getCP()->graph(0)->setData(ticks, yexp);
    errors->setData(yerr);
    getCP()->graph(1)->setData(ticks, ythe);
    mBegX = ticks.at(0) - 1;
    mEndX = ticks.last() + 1;
    getCP()->xAxis->setTickLabels(false);

    // make axis rects' left side line up:
    QCPMarginGroup *group = new QCPMarginGroup(getCP());
    getCP()->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, group);
    QFont font("Helvetica [Cronyx]", 10, 10, true);

    // create the bottom axis 1
    if (ycomp1.size() > 0) {
        QCPAxisRect *axisbox1 = new QCPAxisRect(getCP());
        axisbox1->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));
        getCP()->plotLayout()->addElement(2, 0, axisbox1);
        axisbox1->axis(QCPAxis::atBottom)->setLayer("axes");
        axisbox1->axis(QCPAxis::atBottom)->setTicker(textTicker);
        axisbox1->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
        axisbox1->setMargins(QMargins(0, 0, 0, 0));
        axisbox1->axis(QCPAxis::atBottom)->setTickLabels(false);
        QCPGraph *gr1 = getCP()->addGraph(axisbox1->axis(QCPAxis::atBottom), axisbox1->axis(QCPAxis::atRight));
        gr1->setData(ticks, ycomp1);
        gr1->removeFromLegend(mCP->legend);
        gr1->setLineStyle(QCPGraph::lsNone);
        gr1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));
        QCPErrorBars *errors1 = new QCPErrorBars(axisbox1->axis(QCPAxis::atBottom), axisbox1->axis(QCPAxis::atRight));
        errors1->removeFromLegend();
        errors1->setDataPlottable(gr1);
        errors1->setData(yerrcomp1);
        // interconnect x axis ranges of main and bottom axis rects:
        connect(getCP()->xAxis, SIGNAL(rangeChanged(QCPRange)), axisbox1->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
        connect(axisbox1->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), getCP()->xAxis, SLOT(setRange(QCPRange)));
        axisbox1->setMarginGroup(QCP::msLeft|QCP::msRight, group);
        double min = (*std::min_element(ycomp1.begin(), ycomp1.end()) -
                      *std::max_element(yerrcomp1.begin(), yerrcomp1.end()));
        double max = (*std::max_element(ycomp1.begin(), ycomp1.end()) +
                      *std::max_element(yerrcomp1.begin(), yerrcomp1.end())) * 1.2;
        if (min < 0)
            min *= 1.2;
        else
            min /= 1.2;
        setAxisCosmetics(axisbox1,min, max, 2);
        axisbox1->axis(QCPAxis::atLeft)->setLabel("(mod.-data)/mod.");
        axisbox1->axis(QCPAxis::atLeft)->setLabelFont(font);
        // create the bottom axis 2
        if (ycomp2.size() > 0) {
            QCPAxisRect *axisbox2 = new QCPAxisRect(getCP());
            axisbox2->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));
            getCP()->plotLayout()->addElement(3, 0, axisbox2);
            axisbox2->axis(QCPAxis::atBottom)->setLayer("axes");
            axisbox2->axis(QCPAxis::atBottom)->setTicker(textTicker);
            axisbox2->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
            axisbox2->setMargins(QMargins(0, 0, 0, 0));
            axisbox2->axis(QCPAxis::atBottom)->setTickLabels(false);
            QCPGraph *gr2 = getCP()->addGraph(axisbox2->axis(QCPAxis::atBottom), axisbox2->axis(QCPAxis::atRight));
            gr2->setData(ticks, ycomp2);
            gr2->removeFromLegend(mCP->legend);
            gr2->setLineStyle(QCPGraph::lsNone);
            gr2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));
            QCPErrorBars *errors2 = new QCPErrorBars(axisbox2->axis(QCPAxis::atBottom), axisbox2->axis(QCPAxis::atRight));
            errors2->removeFromLegend();
            errors2->setDataPlottable(gr2);
            errors2->setData(yerrcomp2);
            // interconnect x axis ranges of main and bottom axis rects:
            connect(getCP()->xAxis, SIGNAL(rangeChanged(QCPRange)), axisbox2->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
            connect(axisbox2->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), getCP()->xAxis, SLOT(setRange(QCPRange)));
            axisbox2->setMarginGroup(QCP::msLeft|QCP::msRight, group);
            double min = (*std::min_element(ycomp2.begin(), ycomp2.end()) -
                          *std::max_element(yerrcomp1.begin(), yerrcomp1.end()));
            double max = (*std::max_element(ycomp2.begin(), ycomp2.end()) +
                          *std::max_element(yerrcomp1.begin(), yerrcomp1.end())) * 1.2;
            if (min < 0)
                min *= 1.2;
            else
                min /= 1.2;
            setAxisCosmetics(axisbox2, min, max, 2);
            axisbox2->axis(QCPAxis::atLeft)->setLabel("(mod.-data)/σ_data");
            axisbox2->axis(QCPAxis::atLeft)->setLabelFont(font);
        }
    }
    double minexp = (*std::min_element(yexp.begin(), yexp.end()) -
                  *std::max_element(yerr.begin(), yerr.end()));
    double maxexp = (*std::max_element(yexp.begin(), yexp.end()) +
                  *std::max_element(yerr.begin(), yerr.end()));
    double minthe = *std::min_element(ythe.begin(), ythe.end());
    double maxthe = *std::max_element(ythe.begin(), ythe.end());
    double min = qMin(minexp, minthe);
    if (min < 0)
        min *= 1.2;
    else
        min /= 1.2;
    double max = qMax(maxexp, maxthe) * 1.2;

    setCosmetics(min, max);
    getCP()->xAxis2->setTickLabels(true);

    getCanvas()->setWindowTitle(mTitle);
    getCP()->replot();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(getCP());
    getCanvas()->setLayout(layout);
    getCanvas()->show();
}

//=============================================================
void Plot::drawGraph()
{
    // draw a X, Y graph

    static int koffset = 0;
    QVector<QColor> pcol( {Qt::darkBlue, Qt::blue, Qt::darkCyan, Qt::cyan});
    QVector<QColor> bcol( {QColor("#000080"), QColor("#0000ff"),
                           QColor("#008080"), QColor("#00ffff")});
    QWidget *localCanvas = getCanvas();
    getCP()->setParent(localCanvas);

    setCosmetics(mBegY, mEndY);

    localCanvas->setWindowTitle(mTitle);

    for (int j = 0; j < mCurves.size(); j++) {
        bcol[j].setAlpha(20);
        int dim = mCurves.at(j);
        QCPCurve *curv = new QCPCurve(getCP()->xAxis, getCP()->yAxis);
        curv->setName(mNames.at(j));
        curv->setPen(QPen(pcol[j]));
        curv->setBrush(QBrush(bcol[j]));
        QVector<QCPCurveData> *data = new QVector<QCPCurveData>;
        for (int i = 0; i < dim; i++) {
            data->append(QCPCurveData(i, mData2.at(koffset + i)->x(), mData2.at(koffset + i)->y()));
        }
        curv->data()->set(*data, true);
        koffset += dim;
    }
    getCP()->replot();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(getCP());
    localCanvas->setLayout(layout);
    localCanvas->show();
}

//=============================================================
QWidget *Plot::getCanvas() const
{
    QWidget * rv;
    if (mCanUsed)
        rv = new QWidget;
    else
        rv = mCanvas;
    return rv;
}

//=============================================================
QCustomPlot *Plot::getCP() const
{
    QCustomPlot * rv;
    if (mCanUsed)
        rv = new QCustomPlot(mCanvas);
    else
        rv = mCP;
    return rv;
}

//=============================================================
void Plot::init()
{
   mCanvas = new QWidget;
    mCanvas->setAttribute(Qt::WA_DeleteOnClose);
    mCP = new QCustomPlot(mCanvas);

    mText = new QCPItemText(mCP);
    mText->setFont(QFont("Helvetica [Cronyx]", 10, 10, true));

    mCP->legend->setSelectableParts(QCPLegend::spLegendBox | QCPLegend::spItems);
    connect(mCP, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this,
                 SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

    mCP->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mCP, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    mCP->plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(mCP, mTitle, QFont("sans", 17, QFont::Bold));
    mCP->plotLayout()->addElement(0, 0, title);
    connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));

    connect(mCP, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveSignal(QMouseEvent*)));
    connect(mCP, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressSignal(QMouseEvent*)));
}

//=============================================================
void Plot::projection(const QChar &XorYorC, const QVector<double> &xx, const QVector<double> &zz, double zmin, double zmax)
{
    // display a 1D plot inside a mCanvas

    // define the mCanvas
    QWidget *localCanvas = getCanvas();
    getCP()->setParent(localCanvas);

    setCosmetics(zmin, zmax);

    if (XorYorC == ' ')
        localCanvas->setWindowTitle(mTitle);
    else if (XorYorC == 'X') {
        localCanvas->setWindowTitle(mTitle + ": Pro" + XorYorC.toUpper());
        getCP()->xAxis->setLabel(mXTitle);
    }
    else if (XorYorC == 'Y') {
        localCanvas->setWindowTitle(mTitle + ": Pro" + XorYorC.toUpper());
        getCP()->xAxis->setLabel(mYTitle);
    }
    // add a graph and data
    getCP()->addGraph();
    getCP()->graph(0)->addData(xx, zz);

    // defines drawing option: data
    QPen peny(Qt::blue);
    peny.setWidth(3);
    getCP()->graph(0)->setPen(peny);

    // layout the graph in the canevay
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(getCP());
    localCanvas->setLayout(layout);

    getCP()->replot();
    localCanvas->show();
}

//=============================================================
void Plot::setAxisCosmetics(QCPAxisRect *axis, double zmin, double zmax, int scale)
{
    QCPAxis *xAxis  = axis->axis(QCPAxis::atBottom);
    QCPAxis *xAxis2 = axis->axis(QCPAxis::atTop);
    QCPAxis *yAxis  = axis->axis(QCPAxis::atLeft);
    QCPAxis *yAxis2 = axis->axis(QCPAxis::atRight);

    // X Axis
    xAxis->setLabel(mXTitle);
    xAxis->setRange(mBegX, mEndX);
    xAxis->setScaleType(QCPAxis::stLinear); //stLogarithmic

    // Y Axis
    yAxis->setLabel(mZTitle);
    yAxis->setRange(zmin, zmax);
    yAxis->setScaleType(QCPAxis::stLinear); //stLogarithmic

    // defines drawing option: axis

    axis->setupFullAxesBox(true);
    QColor color = QColor(Qt::black);
    QBrush brush(color, Qt::NoBrush);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    qreal width = 2.0;
    QPen pen(brush, width);
    xAxis->setBasePen(pen);
    yAxis->setBasePen(pen);
    xAxis->setTickPen(pen);
    yAxis->setTickPen(pen);
    xAxis2->setBasePen(pen);
    yAxis2->setBasePen(pen);

    int pointSize = 20 / scale;
    QFont font("Helvetica [Cronyx]", pointSize, 10, true);
    xAxis->setLabelFont(font);
    yAxis->setLabelFont(font);
    xAxis->setLabelColor(color);
    yAxis->setLabelColor(color);
    xAxis->setTickLabelFont(font);
    yAxis->setTickLabelFont(font);
    xAxis->setTickLabelColor(color);
    yAxis->setTickLabelColor(color);

    int ticklength = 10 / scale;
    xAxis->setTickLength(ticklength);
    yAxis->setTickLength(ticklength);
    xAxis->setSubTickLength(ticklength / 2.);
    yAxis->setSubTickLength(ticklength / 2.);
    xAxis2->setTickLength(ticklength);
    yAxis2->setTickLength(ticklength);
    xAxis2->setSubTickLength(ticklength / 2.);
    yAxis2->setSubTickLength(ticklength / 2.);

    xAxis->grid()->setVisible(true);
    yAxis->grid()->setVisible(true);
    xAxis->grid()->setSubGridVisible(false);
    yAxis->grid()->setSubGridVisible(false);}

//=============================================================
void Plot::setCosmetics(double zmin, double zmax)
{
    // do the cosmetics setting
    QWidget * localCanvas = qobject_cast<QWidget*>(getCP()->parent());

    //canvas size and properties
    QRect screenSize = QApplication::desktop()->screenGeometry();
    localCanvas->setMinimumSize(QSize(screenSize.width() / 2.0, screenSize.height() / 1.6));
    localCanvas->setMaximumSize(QSize(screenSize.width(), screenSize.height()));
    localCanvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    localCanvas->setFocusPolicy(Qt::StrongFocus);

    getCP()->setOpenGl(false);

    getCP()->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                         QCP::iSelectLegend | QCP::iSelectPlottables);

    QColor color(Qt::white);
    QBrush brush(color, Qt::NoBrush);
//    getCP()->setBackground(brush);

    setAxisCosmetics(getCP()->axisRect(), zmin, zmax);
    getCP()->legend->setVisible(true);
    getCP()->legend->setBrush(QBrush(QColor(255,255,255,150)));
    getCP()->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // make legend align in top left corner or axis rect
}

//=============================================================
void Plot::setLin(QCPAxis *axis, QCPAxis *axis2)
{
    axis->setScaleType(QCPAxis::stLinear);
    axis2->setScaleType(QCPAxis::stLinear);
    QSharedPointer<QCPAxisTicker> ticker(new QCPAxisTicker);
    axis->setTicker(ticker);
    axis2->setTicker(ticker);
    axis->setNumberFormat("f"); // e = exponential, b = beautiful decimal powers
    axis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"

}

//=============================================================
void Plot::setLog(QCPAxis *axis, QCPAxis *axis2)
{
    // set log scale
    axis->setScaleType(QCPAxis::stLogarithmic);
    axis2->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    axis->setTicker(logTicker);
    axis2->setTicker(logTicker);
    axis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
    axis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
}
