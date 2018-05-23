
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
    mMax(max), mMin(min),
    mTitle(title), mXTitle("X"), mYTitle("Y"), mZTitle("Z")
{
    setObjectName("3D");
    mCanvas = new QWidget;
    mCanvas->setAttribute(Qt::WA_DeleteOnClose);
    mCP = new QCustomPlot();
}

//=============================================================
Plot::Plot(const QString &title, int dim, double sX, double eX, double sY, double eY) : QObject(),
    mBegX(sX), mBegY(sY), mDimX(dim), mDimY(dim), mEndX(eX), mEndY(eY),
    mMax(0.), mMin(0.),
    mTitle(title), mXTitle("X"), mYTitle(""), mZTitle("Y")
{
    setObjectName("Graph");
    mCanvas = new QWidget;
    mCanvas->setAttribute(Qt::WA_DeleteOnClose);
    mCP = new QCustomPlot(mCanvas);
}

//=============================================================
Plot::Plot(const QString &title, double sX, double eX, int dX) : QObject(),
    mBegX(sX), mBegY(0.), mDimX(dX), mDimY(0.), mEndX(eX), mEndY(0.),
    mMax(0), mMin(0),
    mTitle(title), mXTitle("X"), mYTitle("Y"), mZTitle("")
{
    // ctor

    setObjectName("2D");
    mCanvas = new QWidget;
    mCanvas->setAttribute(Qt::WA_DeleteOnClose);
    mCP = new QCustomPlot(mCanvas);
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
void Plot::contour()
{
    // make a contour plot

    QWidget *localCanvas = getCanvas();
    QCustomPlot * localCP = getCP();
    localCP->setParent(localCanvas);

    setCosmetics(localCP);

    localCanvas->setWindowTitle(mTitle + ": Contour X, Y");

    // set up the QCPColorMap:
    QCPColorMap *colorMap = new QCPColorMap(localCP->xAxis, localCP->yAxis);
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
    QCPColorScale *colorScale = new QCPColorScale(localCP);
    localCP->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel(mZTitle);

    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpJet);

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();
    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(localCP);
    localCP->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    localCP->rescaleAxes();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(localCP);
    localCanvas->setLayout(layout);

    localCanvas->show();
}

//=============================================================
void Plot::draw()
{
    if (objectName() == "3D")
        draw3D();
    else if (objectName() == "2D")
        draw2D();
    else if (objectName() == "Graph")
        drawGraph();
}

//=============================================================
void Plot::fill(double x, double y, double z)
{
    // populate the data array

    QVector3D *vec = new QVector3D(x, z, y);
    mData3.append(vec);
}

//=============================================================
void Plot::fill(double x, double y)
{
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
    // draw projection on X axis
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
void Plot::drawGraph()
{
    // draw a X, Y graph

    static int koffset = 0;
    QVector<QColor> pcol( {Qt::darkBlue, Qt::blue, Qt::darkCyan, Qt::cyan});
    QVector<QColor> bcol( {QColor("#000080"), QColor("#0000ff"),
                           QColor("#008080"), QColor("#00ffff")});
    QWidget *localCanvas = getCanvas();
    QCustomPlot * localCP = getCP();
    localCP->setParent(localCanvas);

    setCosmetics(localCP, mBegY, mEndY);


    localCanvas->setWindowTitle(mTitle);

    for (int j = 0; j < mCurves.size(); j++) {
        bcol[j].setAlpha(20);
        int dim = mCurves.at(j);
        QCPCurve *curv = new QCPCurve(localCP->xAxis, localCP->yAxis);
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

//    localCP->rescaleAxes();
    localCP->replot();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(localCP);
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
        rv = new QCustomPlot;
    else
        rv = mCP;
    return rv;
}

//=============================================================
void Plot::projection(const QChar &XorYorC, const QVector<double> &xx, const QVector<double> &zz, double zmin, double zmax)
{
    // display a 1D plot inside a mCanvas

    // define the mCanvas
    QWidget *localCanvas = getCanvas();
    QCustomPlot * localCP = getCP();
    localCP->setParent(localCanvas);

    setCosmetics(localCP, zmin, zmax);

    if (XorYorC == ' ')
        localCanvas->setWindowTitle(mTitle);
    else if (XorYorC == 'X') {
        localCanvas->setWindowTitle(mTitle + ": Pro" + XorYorC.toUpper());
        localCP->xAxis->setLabel(mXTitle);
    }
    else if (XorYorC == 'Y') {
        localCanvas->setWindowTitle(mTitle + ": Pro" + XorYorC.toUpper());
        localCP->xAxis->setLabel(mYTitle);
    }
    // add a graph and data
    localCP->addGraph();
    localCP->graph(0)->addData(xx, zz);

    // defines drawing option: data
    QPen peny(Qt::blue);
    peny.setWidth(3);
    localCP->graph(0)->setPen(peny);

    // layout the graph in the canevay
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(localCP);
    localCanvas->setLayout(layout);

    localCP->replot();
    localCanvas->show();
}

//=============================================================
void Plot::setCosmetics(QCustomPlot *localCP, double zmin, double zmax)
{
    // do the cosmetics setting
    QWidget * localCanvas = qobject_cast<QWidget*>(localCP->parent());

    //canvas size and properties
    QRect screenSize = QGuiApplication::screens().at(0)->geometry();
//            QApplication::desktop()->screenGeometry();
    localCanvas->setMinimumSize(QSize(screenSize.width() / 2.0, screenSize.height() / 1.6));
    localCanvas->setMaximumSize(QSize(screenSize.width(), screenSize.height()));
    localCanvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    localCanvas->setFocusPolicy(Qt::StrongFocus);

    localCP->setOpenGl(false);
    localCP->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    QColor color(Qt::white);
    QBrush brush(color, Qt::NoBrush);
//    localCP->setBackground(brush);

    // X Axis
    localCP->xAxis->setLabel(mXTitle);
    localCP->xAxis->setRange(mBegX, mEndX);
    localCP->xAxis->setScaleType(QCPAxis::stLinear); //stLogarithmic

    // Y Axis
    localCP->yAxis->setLabel(mZTitle);
    localCP->yAxis->setRange(zmin, zmax + 0.5);
    localCP->xAxis->setScaleType(QCPAxis::stLinear); //stLogarithmic

    localCP->rescaleAxes();

    // defines drawing option: axis

    localCP->axisRect()->setupFullAxesBox(true);
    color = QColor(Qt::black);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    qreal width = 2.0;
    QPen pen(brush, width);
    localCP->xAxis->setBasePen(pen);
    localCP->yAxis->setBasePen(pen);
    localCP->xAxis->setTickPen(pen);
    localCP->yAxis->setTickPen(pen);
    localCP->axisRect()->axis(QCPAxis::atTop)->setBasePen(pen);
    localCP->axisRect()->axis(QCPAxis::atRight)->setBasePen(pen);

    QFont font("Helvetica [Cronyx]", 20, 10, true);
    localCP->xAxis->setLabelFont(font);
    localCP->yAxis->setLabelFont(font);
    localCP->xAxis->setLabelColor(color);
    localCP->yAxis->setLabelColor(color);
    localCP->xAxis->setTickLabelFont(font);
    localCP->yAxis->setTickLabelFont(font);
    localCP->xAxis->setTickLabelColor(color);
    localCP->yAxis->setTickLabelColor(color);

    int ticklength = 10.;
    localCP->xAxis->setTickLength(ticklength);
    localCP->yAxis->setTickLength(ticklength);
    localCP->xAxis->setSubTickLength(ticklength / 2.);
    localCP->yAxis->setSubTickLength(ticklength / 2.);
    localCP->axisRect()->axis(QCPAxis::atTop)->setTickLength(ticklength);
    localCP->axisRect()->axis(QCPAxis::atRight)->setTickLength(ticklength);
    localCP->axisRect()->axis(QCPAxis::atTop)->setSubTickLength(ticklength / 2.);
    localCP->axisRect()->axis(QCPAxis::atRight)->setSubTickLength(ticklength / 2.);

    localCP->xAxis->grid()->setVisible(true);
    localCP->yAxis->grid()->setVisible(true);
    localCP->xAxis->grid()->setSubGridVisible(false);
    localCP->yAxis->grid()->setSubGridVisible(false);

    localCP->legend->setVisible(true);
    localCP->legend->setBrush(QBrush(QColor(255,255,255,150)));
    localCP->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // make legend align in top left corner or axis rect
}
