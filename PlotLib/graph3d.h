#ifndef GRAPH3D_H
#define GRAPH3D_H

#include <QObject>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

class Graph3D : public QObject
{
    Q_OBJECT
public:
    explicit Graph3D(Q3DSurface *graph, const QVector<QVector3D *> &data,
                     int sX, int eX, int dX,
                     int sY, int eY, int dY, float min, float max);
    ~Graph3D();

public slots:

    void sliceOnX() { mGraph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                 | QAbstract3DGraph::SelectionSlice); }
    void sliceOnY() { mGraph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                 | QAbstract3DGraph::SelectionSlice); }
    void select()   { mGraph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
private:
    void draw();
    void fillData(const QVector<QVector3D *> &data);

    Q3DSurface        *mGraph;          // the graph to plot
    QSurfaceDataProxy *mGraphDataProxy; // data proxy
    QSurface3DSeries  *mGraphData;      // data
    double            mMax;             // range in z [mMax, mMin]
    double            mMin;             // range in z [mMax, mMin]
    int               mSx;              // range in x [mSx, mEx]
    int               mEx;              // range in x [mSx, mEx]
    int               mDx;              // number of bins in X
    int               mSy;              // range in x [mSy, mEy]
    int               mEy;              // range in x [mSy, mEy]
    int               mDy;              // number of bins in Y
    bool              mFindMax;         // find z max yes or no
    bool              mFindMin;         // find z min yes or no
};

#endif // GRAPH3D_H
