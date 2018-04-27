#ifndef PLOT_H
#define PLOT_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class QCustomPlot;
class Plot : public QObject
{
    Q_OBJECT
public:
    explicit Plot(const QString & title, double sX, double eX, int dX, double sY, double eY, int dY, double min, double max = -99999);
    explicit Plot(const QString & title, int dim, double sX, double eX, double sY, double eY);
    explicit Plot(const QString & title, double sX, double eX, int dX);
    void addGraph(const QVector<double> &x, const QVector<double> &y);
    void draw();
    void fill(double x, double y, double z);
    void fill(double x, double y);
    void setAxisTitle(const QChar &type, const QString &title);
    void setAxisTitle(const QString &titleX, const QString &titleY, const QString &titleZ = "") {
        mXTitle = titleX; mYTitle = titleY; mZTitle = titleZ; }
    void setTitle(const QString &title) { mTitle = title; }

signals:

public slots:
    void contour();
    void proX();
    void proY();

private:
    void           draw2D();
    void           draw3D();
    void           drawGraph();
    QWidget *      getCanvas() const;
    QCustomPlot *  getCP() const;
    void           projection(const QChar &XorYorC, const QVector<double> &xx, const QVector<double> &zz, double zmin, double zmax);
    void           setCosmetics(QCustomPlot * cp, double zmin= 0.0, double zmax = 10.0);

    double               mBegX;   // x value where plot starts
    double               mBegY;   // y value where plot starts
    QWidget             *mCanvas; // the canvas that contains the customplot object
    QCustomPlot         *mCP;     // the customplot object
    QList<int>           mCurves; // stores the nbr of points for each curve
    QVector<QVector2D *> mData2;  // the 2D data to plot or a graph
    QVector<QVector3D *> mData3;  // the 3D data to plot
    int                  mDimX;   // number of bins in x
    int                  mDimY;   // number of bins in y
    double               mEndX;   // x value where plot ends
    double               mEndY;   // y value where plot ends
    bool                 mCanUsed;// tells if mCanvas is used or not
    double               mMax;    // max z value t plot
    double               mMin;    // min z value t plot
    QString              mTitle;  // title of the graph
    QString              mXTitle; // x Axis title
    QString              mYTitle; // x Axis title
    QString              mZTitle; // x Axis title
};

#endif // PLOT_H
