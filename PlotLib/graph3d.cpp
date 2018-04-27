#include <QDebug>

#include "graph3d.h"

//=============================================================
Graph3D::Graph3D(Q3DSurface *graph, const QVector<QVector3D *> &data, int sX, int eX, int dX, int sY, int eY, int dY, float min, float max) :
    mGraph(graph), mMax(max), mMin(min),
    mSx(sX), mEx(eX), mDx(dX), mSy(sY), mEy(eY), mDy(dY),
    mFindMax(false), mFindMin(false)
{
    // create the data to plot and display options

    if (mMax == -1 ) {
        mFindMax = true;
        mMax = -99999;
    }
    if (mMin == -1 ) {
        mFindMin = true;
        mMin = 99999;
    }
    mGraphDataProxy = new QSurfaceDataProxy;
    mGraphData      = new QSurface3DSeries(mGraphDataProxy);
    fillData(data);
    draw();

}

//=============================================================
Graph3D::~Graph3D()
{
    delete mGraph;
}

//=============================================================
void Graph3D::draw()
{
    mGraphData->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    mGraphData->setFlatShadingEnabled(true);

    // axis
    mGraph->axisX()->setLabelFormat("%.2f");
    mGraph->axisZ()->setLabelFormat("%.2f");
    mGraph->axisX()->setRange(mSx, mEx);
    mGraph->axisY()->setRange(mMin, mMax);
    mGraph->axisZ()->setRange(mSy, mEy);
    mGraph->axisX()->setLabelAutoRotation(30);
    mGraph->axisY()->setLabelAutoRotation(90);
    mGraph->axisZ()->setLabelAutoRotation(30);
    mGraph->axisX()->setTitleVisible(true);
    mGraph->axisY()->setTitleVisible(true);
    mGraph->axisZ()->setTitleVisible(true);

    // orientation
     mGraph->scene()->activeCamera()->setCameraPosition(0.0f, 0.0f, 100.0f);
     mGraph->scene()->activeCamera()->setXRotation(0.);
     mGraph->scene()->activeCamera()->setYRotation(0.);
    // theme
    mGraph->activeTheme()->setType(Q3DTheme::ThemePrimaryColors);
    mGraph->addSeries(mGraphData);
}

//=============================================================
void Graph3D::fillData(const QVector<QVector3D *> &data)
{
    // fill the data
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(data.size());
    int vindex = 0;
    for (int iy = 0; iy < mDy; iy++) {
        int index = 0;
        QSurfaceDataRow *newRow = new QSurfaceDataRow(mDx);
        for (int ix = 0; ix < mDx; ix++) {
            QVector3D * vec = data.at(vindex++);
            if (mFindMax)
                if (vec->y() > mMax)
                    mMax = vec->y() * 1.2;
            if (mFindMin)
                if (vec->y() < mMin)
                    mMin = vec->y() * 0.8;
            (*newRow)[index++].setPosition(*(vec));
        }
        *dataArray << newRow;
    }
    mGraphDataProxy->resetArray(dataArray);
}
