#ifndef PLOT_H
#define PLOT_H

#include <QMap>
#include <QObject>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class QMouseEvent;
class QPoint;
class QCustomPlot;
class QCPAbstractLegendItem;
class QCPAxis;
class QCPAxisRect;
class QCPLegend;
class QCPItemText;

class Plot : public QObject
{
    Q_OBJECT
public:
    //3D
    explicit Plot(const QString & title, double sX, double eX, int dX, double sY, double eY, int dY, double min, double max = -99999);
    //Graph
    explicit Plot(const QString & title, int dim, double sX, double eX, double sY, double eY);
    //2D
    explicit Plot(const QString & title, double sX, double eX, int dX);
    //Chart
    explicit Plot(const QString & title); // a chart
    // a test
    explicit Plot();
    ~Plot();

    void addEntry(const QString & key, double the, double exp, double err, double comp1 = 999., double errcomp1 = 0., double comp2 = 999., double errcomp2 = 0.);
    void addGraph(const QString &name, const QVector<double> &x, const QVector<double> &y);
    void clear();
    void draw();
    void fill(double x, double y, double z);
    void fill(double x, double y);
    void setAxisTitle(const QChar &type, const QString &title);
    void setAxisTitle(const QString &titleX, const QString &titleY, const QString &titleZ = "");
    void setEntryName(int index, const QString& name);
    void setTitle(const QString &title) { mTitle = title; }

signals:

public slots:
    void addGraph() {;}
    void contour();
    void contextMenuRequest(QPoint pos);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void mouseMoveSignal(QMouseEvent *event);
    void mousePressSignal(QMouseEvent *event);
    void moveLegend();
    void proX();
    void proY();
    void removeSelectedGraph();
    void resizeGraph(QCPAxisRect *ar, bool plus);
    void setLabelSignal(QCPAxis *axis);
    void titleDoubleClick(QMouseEvent *event);
    void toggleLinLog(QCPAxis *axis, QCPAxis *axis2);

private:
    void           draw2D();
    void           draw3D();
    void           drawChart();
    void           drawGraph();
    QWidget *      getCanvas() const;
    QCustomPlot *  getCP() const;
    void           init();
    void           projection(const QChar &XorYorC, const QVector<double> &xx, const QVector<double> &zz, double zmin, double zmax);
    void           setAxisCosmetics(QCPAxisRect *axis, double zmin, double zmax, int scale = 1);
    void           setCosmetics(double zmin= 0.0, double zmax = 10.0);
    void           setLin(QCPAxis *axis, QCPAxis *axis2);
    void           setLog(QCPAxis *axis, QCPAxis *axis2);

    double                 mBegX;   // x value where plot starts
    double                 mBegY;   // y value where plot starts
    QWidget               *mCanvas; // the canvas that contains the customplot object
    bool                   mCanUsed;// tells if mCanvas is used or not
    QCustomPlot           *mCP;     // the customplot object
    QList<int>             mCurves; // stores the nbr of points for each curve
    QVector<QVector2D *>   mData2;  // the 2D data to plot or a graph
    QVector<QVector3D *>   mData3;  // the 3D data to plot
    QMap<QString, double>  mDataC;  // the data for a graph: X = QString, Y = double
    int                    mDimX;   // number of bins in x
    int                    mDimY;   // number of bins in y
    double                 mEndX;   // x value where plot ends
    double                 mEndY;   // y value where plot ends
    double                 mMax;    // max z value t plot
    int                    mMaxArg; // argument counts for addEntry
    double                 mMin;    // min z value t plot
    QList<QString>         mNames;  // name of each graph/curve
    QCPItemText           *mText;   // text to display coords on graph
    QString                mTitle;  // title of the graph
    QString                mXTitle; // x Axis title
    QString                mYTitle; // x Axis title
    QString                mZTitle; // x Axis title
};

#endif // PLOT_H
