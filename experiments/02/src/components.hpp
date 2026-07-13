#pragma once
#include <QObject>
#include <QWidget>
#include <QInputDialog>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

/* RangeSlider — 双滑块区间拖动条 */
class RangeSlider : public QWidget {
    Q_OBJECT
public:
    RangeSlider(double lo,double hi,double rmin,double rmax,QWidget* p=nullptr)
        :QWidget(p),m_lo(lo),m_hi(hi),m_min(rmin),m_max(rmax),m_drag(0){setFixedHeight(30);setMinimumWidth(200);}
    double low() const {return m_lo;} double high() const {return m_hi;}
    void setLow(double v){m_lo=qBound(m_min,v,m_hi);update();}
    void setHigh(double v){m_hi=qBound(m_lo,v,m_max);update();}
    void setRange(double lo,double hi){m_lo=qBound(m_min,lo,m_max);m_hi=qBound(m_min,hi,m_max);if(m_lo>m_hi)m_lo=m_hi;update();}
signals:
    void rangeChanged(double lo,double hi);
protected:
    void paintEvent(QPaintEvent*)override{
        QPainter p(this);p.setRenderHint(QPainter::Antialiasing);
        int w=width(),h=height(),th=6,ty=(h-th)/2;
        p.setBrush(QColor("#444"));p.setPen(Qt::NoPen);
        p.drawRoundedRect(0,ty,w-1,th,3,3);
        double rng=m_max-m_min;int lx=(int)((m_lo-m_min)/rng*(w-20)+10),hx=(int)((m_hi-m_min)/rng*(w-20)+10);
        p.setBrush(QColor("#5294e2"));
        p.drawRoundedRect(lx,ty,hx-lx,th,3,3);
        p.setBrush(QColor("#ddd"));p.setPen(QPen(QColor("#888"),1));
        p.drawRoundedRect(lx-5,0,11,h-1,4,4);
        p.drawRoundedRect(hx-5,0,11,h-1,4,4);
    }
    void mousePressEvent(QMouseEvent* e)override{
        int x=e->pos().x(),w=width()-20;double rng=m_max-m_min;
        int lx=(int)((m_lo-m_min)/rng*w)+10,hx=(int)((m_hi-m_min)/rng*w)+10;
        int dl=abs(x-lx),dh=abs(x-hx);
        m_drag=(dl<dh)?1:2;if(m_drag)m_mouseMove(e);
    }
    void mouseMoveEvent(QMouseEvent* e)override{m_mouseMove(e);}
    void mouseReleaseEvent(QMouseEvent*)override{m_drag=0;}
private:
    void m_mouseMove(QMouseEvent* e){
        int x=qBound(10,e->pos().x(),width()-10);double v=m_min+(x-10.0)/(width()-20)*(m_max-m_min);
        if(m_drag==1){m_lo=qBound(m_min,v,m_hi);update();emit rangeChanged(m_lo,m_hi);}
        else if(m_drag==2){m_hi=qBound(m_lo,v,m_max);update();emit rangeChanged(m_lo,m_hi);}
    }
    double m_lo,m_hi,m_min,m_max;int m_drag;
};
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsPolygonItem>
#include <QMap>
#include <QSet>
#include <QList>
#include <QPair>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QTimer>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QScrollBar>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QInputDialog>
#include <QMessageBox>
#include <QScreen>
#include <QDesktopServices>
#include <QShortcut>
#include <QUrl>
#include <QMutex>
#include <QStringList>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QQueue>
#include <QStack>
#include <QSet>
#include <functional>
#include <atomic>
#include <cmath>

#include "indexGraph.hpp"
#include "defines.hpp"

typedef QPair<unsigned int, unsigned int> EdgeKey;

/* GraphDocument — 图文档模型（唯一拥有 indexGraph 实例） */

class GraphDocument : public QObject {
    Q_OBJECT
public:
    explicit GraphDocument(QObject* parent = nullptr);
    ~GraphDocument();

    /* indexGraph 原始接口委托 */
    indexGraph* graph() const { return m_graph; }
    _usdint_ nodeCount() const { return m_graph->node_count(); }
    _usdint_ edgeCount() const { return m_graph->edge_count(); }
    _usdint_ maxId() const { return m_graph->get_maxid(); }
    bool empty() const { return m_graph->empty(); }
    bool nodeValid(_usdint_ id) const { return m_graph->node_valid(id); }
    bool edgeValid(_usdint_ f,_usdint_ t) const { return m_graph->edge_valid(f,t); }
    bool adjoin(_usdint_ f,_usdint_ t) const { return m_graph->adjoin(f,t); }
    bool hasNegativeEdge() const { return m_graph->has_negative_edge(); }
    _usdint_ areaCount() const { return m_graph->area_count(); }
    _usdint_ outDeg(_usdint_ id) const { return m_graph->out_deg(id); }
    _usdint_ inDeg(_usdint_ id) const { return m_graph->in_deg(id); }
    int edgeWeight(_usdint_ f,_usdint_ t) const { return m_graph->get_edge_weight(f,t); }
    vector<_usdint_> adjacency(_usdint_ id) const { return m_graph->get_adjoin(id); }
    vector<_usdint_> adjacencyFrom(_usdint_ id) const { return m_graph->get_adjoin_asf(id); }
    vector<_usdint_> adjacencyTo(_usdint_ id) const { return m_graph->get_adjoin_ast(id); }
    vector<_usdint_> BFS(_usdint_ o) const { return m_graph->BFS(o); }
    vector<_usdint_> DFS(_usdint_ o) const { return m_graph->DFS(o); }
    vector<_usdint_> dijkstra(_usdint_ s,_usdint_ t) const { return m_graph->dijkstra(s,t); }

    /* 图结构修改 */
    _usdint_ addNode(QPointF pos = QPointF(0,0));
    void deleteNode(_usdint_ id, bool resortNodes = true);
    void addEdge(_usdint_ f, _usdint_ t, int w = 1);
    void deleteEdge(_usdint_ f, _usdint_ t);
    void modifyEdge(_usdint_ f, _usdint_ t, int w);
    void resort();
    void clear();
    void clearAllEdges();
    void completeToCompleteGraph();
    void build(_usdint_ n, _usdint_ m, vector<indexGraph::ewl>& el);

    /* 结点布局 */
    QPointF nodePosition(_usdint_ id) const { return m_nodePositions.value(id,QPointF(0,0)); }
    void setNodePosition(_usdint_ id,QPointF p) { m_nodePositions[id]=p; }
    const QMap<_usdint_,QPointF>& allNodePositions() const { return m_nodePositions; }
    bool hasPosition(_usdint_ id) const { return m_nodePositions.contains(id); }
    void moveNodes(const QList<_usdint_>& ids, QPointF d);
    bool isLayoutLocked() const { return m_layoutLocked; }
    void lockLayout() { m_layoutLocked=true; }
    void unlockLayout() { m_layoutLocked=false; }
    void forceLayout();
    void enforceMinSpacing(double minDist=DEF_LAYOUT_SPACING_MIN);
    /* 布局参数 — 由 GraphCanvas 在 forceLayout 前推送运行时物理参数 */
    double m_loIDEAL, m_loREPULSE, m_loATTRACT, m_loCENTER, m_loMIN_DIST, m_loOVERLAP;

    /* 选择 */
    void selectNode(_usdint_ id, bool multi = false);
    void deselectNode(_usdint_ id) { m_selectedNodes.remove(id); emit selectionChanged(); }
    void selectEdge(const EdgeKey& ek, bool multi = false);
    void deselectEdge(const EdgeKey& ek) { m_selectedEdges.remove(ek); emit selectionChanged(); }
    void clearSelection();
    bool isNodeSelected(_usdint_ id) const { return m_selectedNodes.contains(id); }
    bool isEdgeSelected(const EdgeKey& ek) const { return m_selectedEdges.contains(ek); }
    QList<_usdint_> selectedNodeIds() const { return QList<_usdint_>(m_selectedNodes.begin(),m_selectedNodes.end()); }
    QList<EdgeKey> selectedEdgeKeys() const { return QList<EdgeKey>(m_selectedEdges.begin(),m_selectedEdges.end()); }
    bool hasSelection() const { return !m_selectedNodes.isEmpty()||!m_selectedEdges.isEmpty(); }

    /* 起终点 */
    void setStartNode(_usdint_ id) { m_startNode=id; emit startEndChanged(); }
    void setEndNode(_usdint_ id) { m_endNode=id; emit startEndChanged(); }
    _usdint_ startNode() const { return m_startNode; }
    _usdint_ endNode() const { return m_endNode; }
    void clearStartEnd() { m_startNode=m_endNode=0; emit startEndChanged(); }

    /* 文档元数据 */
    QString filePath() const { return m_filePath; }
    void setFilePath(const QString& p) { m_filePath=p; }
    QString fileName() const { return m_fileName; }
    void setFileName(const QString& n) { m_fileName=n; }
    bool isModified() const { return m_modified; }
    void setModified(bool m);
signals:
    void graphChanged();
    void layoutChanged();
    void selectionChanged();
    void startEndChanged();
    void documentModified(bool modified);
    void layoutProgress(int pct);
private:
    friend class GraphCanvas;
    indexGraph* m_graph;
    QMap<_usdint_,QPointF> m_nodePositions;
    /* 由 GraphCanvas 直接访问 */
    bool m_layoutLocked;
    bool m_needsBurstLayout;  /* 加载/修复时批量预运行，用户编辑时不触发 */
    QSet<_usdint_> m_selectedNodes;
    QSet<EdgeKey> m_selectedEdges;
    _usdint_ m_startNode, m_endNode;
    QString m_filePath, m_fileName;
    bool m_modified;
};

/* InfoOverlay — 图信息浮层（右上角）*/

class InfoOverlay : public QWidget {
    Q_OBJECT
public:
    explicit InfoOverlay(QWidget* parent);
    void refresh(const GraphDocument* doc);
    void resetPosition();
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
private:
    void updateSize();
    QWidget* m_titleBar; QLabel* m_title; QLabel* m_content;
    bool m_collapsed, m_dragging; QPoint m_dragStart, m_panelStart;
    bool m_resizing; int m_resizeStartX, m_resizeStartW;
};

/* MinimapOverlay — 缩略图导航（左下角） */

class MinimapOverlay : public QGraphicsView {
    Q_OBJECT
public:
    explicit MinimapOverlay(QWidget* parent);
    void syncWithCanvas(class GraphCanvas* canvas);
    void resetPosition();
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
private:
    void renderMinimap(class GraphCanvas* canvas);
    void updateViewportIndicator(class GraphCanvas* canvas);
    bool isInHandle(const QPoint& pos) const;
    QGraphicsScene* m_miniScene;
    QGraphicsRectItem* m_viewportIndicator;
    bool m_dragging;
    bool m_widgetDragging; QPoint m_widgetDragStart;
};

class SettingsService; /* 前向声明, 定义位于下方 */

class PhysicsDialog : public QDialog {
    Q_OBJECT
public:
    explicit PhysicsDialog(const QJsonObject& currentParams, QWidget* parent = nullptr);
    QJsonObject getParams() const;
    static QJsonObject defaultParams();
private:
    struct Field { QSlider* slider; QLabel* valLabel; double min,max,step,defVal; QString key; };
    struct RangeField { RangeSlider* range; QLabel* valLabel; double rmin,rmax,defLo,defHi; QString keyLo,keyHi; };
    QList<Field> m_fields;
    QList<RangeField> m_rfields;
    bool eventFilter(QObject* obj, QEvent* event) override;
    QHBoxLayout* makeSliderRow(const QString& label, const QString& key,
                double defVal, double min, double max, double step);
    QHBoxLayout* makeRangeRow(const QString& label, const QString& keyLo, const QString& keyHi,
                double defLo, double defHi, double rmin, double rmax);
};

/* GraphCanvas — 可交互画布 */

class GraphCanvas : public QGraphicsView {
    Q_OBJECT
public:
    explicit GraphCanvas(QWidget* parent);
    ~GraphCanvas() {}

    void setDocument(GraphDocument* doc);
    GraphDocument* document() const { return m_document; }

    void setZoomLevel(double level);
    double zoomLevel() const { return m_zoomLevel; }
    void fitToContent();
    void centerOnNode(_usdint_ id);

    InfoOverlay* infoOverlay() const { return m_infoOverlay; }
    MinimapOverlay* minimapOverlay() const { return m_minimapOverlay; }

    enum class InteractMode { Normal, NodeMoving };
    void setInteractMode(InteractMode mode);
    InteractMode interactMode() const { return m_interactMode; }
    _usdint_ movingNodeId() const { return m_movingNodeId; }

    void rebuildScene();
    void updateSelectionHighlight();
    void showSelectionInfo();
    void updateStartEndLabels();
    void clearAlgorithmHighlights();
    void highlightNode(_usdint_ id, const QColor& c);
    void fadeNode(_usdint_ id);
    void highlightEdge(_usdint_ from, _usdint_ to, const QColor& c);
    void fadeEdge(_usdint_ from, _usdint_ to);
    void flashItems(const QList<_usdint_>& nodeIds, const QList<EdgeKey>& edgeKeys, int durationMs=3000, const QColor& color=QColor(DEF_ALGO_STEP));
    void updateNodePositions(const QMap<_usdint_,QPointF>& ps);
    void loadPhysicsParams(const SettingsService* settings);
    QGraphicsItem* findItemAt(QPointF sp);
    void setPhysicsFps(int fps);
    void setRenderFps(int fps);
    void centerOnGraphCenter();

signals:
    void zoomChanged(double level);
    void nodeClicked(_usdint_ id, Qt::KeyboardModifiers mods);
    void edgeClicked(EdgeKey ek, Qt::KeyboardModifiers mods);
    void canvasClicked(QPointF scenePos, Qt::KeyboardModifiers mods);
    void canvasDoubleClicked();
    void canvasRightClicked(QPointF scenePos);
    void edgeCreatedByDrag(_usdint_ from, _usdint_ to);
    void nodeMoveDelta(QPointF delta);
    void nodeMoveFinished();
    void physicsToggled(bool paused);
    void keyPressed(int key, Qt::KeyboardModifiers mods = Qt::NoModifier);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void showHoverText(QGraphicsItem* item);
    void hideHoverText();
    void updateEdgeLine(_usdint_ from, _usdint_ to);
    void physicsTick();
    void burstPhysics(int maxMs);
    void startPhysics(bool layoutPhase=false);
    void stopPhysics();

    GraphDocument* m_document;
    QGraphicsScene* m_scene;
    InfoOverlay* m_infoOverlay;
    MinimapOverlay* m_minimapOverlay;
    double m_zoomLevel;
    InteractMode m_interactMode;

    QMap<_usdint_,QGraphicsEllipseItem*> m_nodeItems;
    QMap<_usdint_,QGraphicsTextItem*> m_nodeTextItems;
    QMap<EdgeKey,QGraphicsLineItem*> m_edgeItems;
    QMap<EdgeKey,QGraphicsPolygonItem*> m_arrowItems;
    QGraphicsTextItem* m_startLabel, * m_endLabel;
    QLabel* m_zoomLabel; QLabel* m_selectInfoLabel; QTimer* m_zoomFadeTimer; QTimer* m_panTimer; QTimer* m_moveFlashTimer;

    bool m_panning; QPoint m_lastPanPos; bool m_wasdDown[4]; bool m_wasdActive; double m_wasdVx, m_wasdVy;
    bool m_edgeDragging; _usdint_ m_edgeDragSource; bool m_edgeBidirectional; QGraphicsLineItem* m_tempEdgeLine;
    bool m_moveAnchorSet; QPointF m_moveAnchor; _usdint_ m_movingNodeId;
    bool m_leftDragPan; QPoint m_leftDragStart;
    bool m_zHeld;
    /* 物理模拟状态 */
    QTimer* m_physicsTimer; QMap<_usdint_,QPointF> m_nodeVelocities; bool m_physicsActive; bool m_physicsPaused; bool m_physicsLayoutPhase;
    /* 运行时物理参数（可被 settings.json 覆盖） */
    double m_pK_REP, m_pK_UNI, m_pK_BI, m_pG_UNIV, m_pGAMMA_L, m_pGAMMA_D, m_pV_MAX, m_pSTOP_VEL, m_pIDLE_VEL, m_pIDEAL, m_pIDEAL_MAX, m_pMIN_DIST, m_pMAX_DIST, m_pCUTOFF;

    static constexpr double NR = DEF_NODE_RADIUS, AL = DEF_ARROW_SIZE, AW = DEF_ARROW_WIDTH, BO = DEF_BIDIR_OFFSET;
};

/* AlgorithmRunner — 算法步进执行器 */

struct AlgorithmStep {
    QList<_usdint_> newHighlights, previousFades;
    QList<EdgeKey> newEdgeHighlights, previousEdgeFades;
};

class AlgorithmRunner : public QObject {
    Q_OBJECT
public:
    enum class Algorithm { DFS, BFS, Dijkstra, BellmanFord };

    explicit AlgorithmRunner(QObject* parent = nullptr);
    bool start(Algorithm algo, const GraphDocument* doc, _usdint_ source, _usdint_ target = 0);
    void pause() { if(m_running) m_paused=true; }
    void resume() { if(m_running) m_paused=false; }
    void stop();
    bool isRunning() const { return m_running; }
    bool isPaused() const { return m_paused; }

signals:
    void stepExecuted(int step,int total,const QList<_usdint_>& highlights,const QList<_usdint_>& fades,const QList<EdgeKey>& edgeHighlights,const QList<EdgeKey>& edgeFades);
    void finished(const std::vector<_usdint_>& finalPath);
    void error(const QString& message);

private slots:
    void onTimerTick();

private:
    void buildDFSSteps(_usdint_ source);
    void buildBFSSteps(_usdint_ source);
    void buildDijkstraSteps(_usdint_ source, _usdint_ target);
    void buildBellmanFordSteps(_usdint_ source, _usdint_ target);
    vector<_usdint_> externalBellmanFord(_usdint_ source, _usdint_ target) const;

    Algorithm m_algo; const GraphDocument* m_doc; QTimer* m_timer;
    int m_currentStep, m_totalSteps; bool m_running, m_paused;
    QList<AlgorithmStep> m_steps; std::vector<_usdint_> m_finalPath;
};

/* TabBarWidget — 标签栏控件 */

class TabBarWidget : public QWidget {
    Q_OBJECT
public:
    explicit TabBarWidget(QWidget* parent);
    void addTab(const QString& name);
    void removeTab(int index);
    void renameTab(int index, const QString& name);
    void selectTab(int index);
    int currentIndex() const { return m_currentIndex; }
    int tabCount() const { return m_buttons.size(); }
signals:
    void tabClicked(int index);
    void tabDoubleClicked(int index);
    void tabContextMenu(int index, QPoint globalPos);
    void tabsReordered(int from, int to);
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
private:
    QHBoxLayout* m_layout; QList<QPushButton*> m_buttons;
    int m_currentIndex, m_dragIndex; bool m_dragging; QPoint m_dragStartPos;
};

/* ContextMenuManager — 右键菜单管理器 */

class ContextMenuManager : public QObject {
    Q_OBJECT
public:
    explicit ContextMenuManager(QWidget* parent) : QObject(parent), m_parent(parent) {}
    void showNodeMenu(_usdint_ nodeId, QPoint sp, GraphDocument* doc);
    void showEdgeMenu(EdgeKey ek, QPoint sp);
    void showCanvasMenu(QPointF scenePos, QPoint sp);
signals:
    void deleteNodeRequested(_usdint_ id);
    void deleteEdgeRequested(EdgeKey ek);
    void edgeWeightEditRequested(EdgeKey ek);
    void setStartNodeRequested(_usdint_ id);
    void setEndNodeRequested(_usdint_ id);
    void cancelStartNodeRequested();
    void cancelEndNodeRequested();
    void deleteSelectedRequested();
    void moveModeRequested();
    void addNodeRequested(QPointF scenePos);
    void trackCenterRequested();
private:
    QWidget* m_parent;
};

/* DebugLogger — 异步调试日志 + 全局调试标记 */

extern std::atomic<bool> DEBUG_MODE;
void COUT_DEBUG(const QString& msg);

class DebugLogger : public QObject {
    Q_OBJECT
public:
    static DebugLogger& instance();
    void log(const QString& msg);
    void flush();
    static void writeCrashLog(const QString& type, const QString& msg);
private:
    DebugLogger();
    QMutex m_mutex; QStringList m_queue; QTimer* m_flushTimer;
};

/* SettingsService — 配置持久化 */

class SettingsService : public QObject {
    Q_OBJECT
public:
    explicit SettingsService(const QString& appDir, QObject* parent = nullptr);
    bool debugMode() const;
    void setDebugMode(bool on);
    QRect windowGeometry() const;
    void setWindowGeometry(const QRect& rect);
    int physicsFps() const;
    void setPhysicsFps(int fps);
    int renderFps() const;
    void setRenderFps(int fps);
    /* 物理参数存取 */
    double physicsParam(const QString& key, double def) const;
    void setPhysicsParam(const QString& key, double val);
    QJsonObject allPhysicsParams() const;
    void setAllPhysicsParams(const QJsonObject& obj);
    void ensurePhysicsDefaults();
    void load();
    void save();
private:
    QString m_filePath; QJsonObject m_settings;
};

/* GraphFileService — 图文件读写 */

class GraphFileService : public QObject {
    Q_OBJECT
public:
    explicit GraphFileService(QObject* parent = nullptr) : QObject(parent) {}
    GraphDocument* loadFromFile(const QString& path, QObject* docParent = nullptr);
    bool saveToFile(GraphDocument* doc, const QString& path);
    QString getOpenFileName(QWidget* p, const QString& appDir);
    QString getSaveFileName(QWidget* p, const QString& appDir, const QString& defaultName);
};

/* 所有内联实现 */

/* ─── GraphDocument ─── */

inline GraphDocument::GraphDocument(QObject* parent) : QObject(parent), m_graph(new indexGraph()), m_layoutLocked(false), m_needsBurstLayout(false), m_startNode(0), m_endNode(0), m_modified(false), m_loIDEAL(0), m_loREPULSE(0), m_loATTRACT(0), m_loCENTER(0), m_loMIN_DIST(0), m_loOVERLAP(0) {}
inline GraphDocument::~GraphDocument() { delete m_graph; }

inline _usdint_ GraphDocument::addNode(QPointF pos) {
    m_graph->add_node(); _usdint_ id=m_graph->get_maxid();
    if(pos==QPointF(0,0)){
        /* 默认位置: 围绕现有结点簇按最小间距摆放 */
        _usdint_ mid=m_graph->get_maxid();double sx=0,sy=0;int cnt=0;
        for(_usdint_ i=1;i<=mid;i++){if(m_graph->node_valid(i)&&m_nodePositions.contains(i)&&i!=id){QPointF p=m_nodePositions[i];sx+=p.x();sy+=p.y();cnt++;}}
        if(cnt>0){QPointF center(sx/cnt,sy/cnt);double r=sqrt((double)cnt)*DEF_LAYOUT_SPACING_MIN;double a=cnt*2.3999;pos=center+QPointF(cos(a)*r,sin(a)*r);}
    }
    m_nodePositions[id]=pos;
    m_modified=true; m_layoutLocked=true; emit graphChanged(); emit documentModified(true); return id;
}
inline void GraphDocument::deleteNode(_usdint_ id,bool resortNodes) {
    if(!m_graph->node_valid(id))return; bool n1=(id==1);
    m_graph->del_node(id,resortNodes); m_nodePositions.remove(id); m_selectedNodes.remove(id);
    if(n1)m_layoutLocked=false; m_modified=true; emit graphChanged(); emit documentModified(true);
}
inline void GraphDocument::addEdge(_usdint_ f,_usdint_ t,int w) { m_graph->add_edge(f,t,w); m_modified=true; emit graphChanged(); emit documentModified(true); }
inline void GraphDocument::deleteEdge(_usdint_ f,_usdint_ t) { m_graph->del_edge(f,t); m_modified=true; emit graphChanged(); emit documentModified(true); }
inline void GraphDocument::modifyEdge(_usdint_ f,_usdint_ t,int w) { m_graph->mod_edge(f,t,w); m_modified=true; emit graphChanged(); emit documentModified(true); }
inline void GraphDocument::setModified(bool m) { if(m_modified!=m){m_modified=m;emit documentModified(m);} }

inline void GraphDocument::resort() {
    /* 保留旧布局：先记录 oldID→位置 映射 */
    QMap<_usdint_,QPointF> oldPos=m_nodePositions;
    _usdint_ mid=m_graph->get_maxid();
    /* 收集有效旧ID并排序（与 indexGraph::resort 的排序逻辑一致） */
    QList<_usdint_> oldIds;
    for(_usdint_ i=1;i<=mid;i++){if(m_graph->node_valid(i))oldIds.append(i);}
    sort(oldIds.begin(),oldIds.end());
    /* old→new ID 映射 */
    QMap<_usdint_,_usdint_> idMap;
    for(int i=0;i<oldIds.size();i++)idMap[oldIds[i]]=(_usdint_)(i+1);
    m_graph->resort();
    /* 恢复位置，使用新ID */
    m_nodePositions.clear();
    for(auto it=oldPos.begin();it!=oldPos.end();++it){
        _usdint_ oldId=it.key();
        if(idMap.contains(oldId))m_nodePositions[idMap[oldId]]=it.value();}
    m_selectedNodes.clear(); m_selectedEdges.clear();
    m_layoutLocked=false; m_needsBurstLayout=true; m_startNode=m_endNode=0;
    m_modified=true; emit graphChanged(); emit documentModified(true);
    emit startEndChanged(); emit selectionChanged();
}
inline void GraphDocument::clear() {
    m_graph->clear(); m_nodePositions.clear(); m_selectedNodes.clear(); m_selectedEdges.clear();
    m_startNode=m_endNode=0; m_layoutLocked=false; m_modified=true;
    emit graphChanged(); emit documentModified(true); emit startEndChanged(); emit selectionChanged();
}
inline void GraphDocument::clearAllEdges() {
    _usdint_ mid=m_graph->get_maxid();
    for(_usdint_ nid=1;nid<=mid;nid++){if(!m_graph->node_valid(nid))continue;for(_usdint_ tid:m_graph->get_adjoin_asf(nid))m_graph->del_edge(nid,tid);}
    m_modified=true; emit graphChanged(); emit documentModified(true);
}
inline void GraphDocument::completeToCompleteGraph() {
    _usdint_ mid=m_graph->get_maxid(); int added=0;
    for(_usdint_ i=1;i<=mid;i++){if(!m_graph->node_valid(i))continue;
        for(_usdint_ j=i+1;j<=mid;j++){if(!m_graph->node_valid(j))continue;
            bool has=m_graph->edge_valid(i,j)||m_graph->edge_valid(j,i);
            if(!has){m_graph->add_edge(i,j,1);m_graph->add_edge(j,i,1);added+=2;}}}
    if(added>0){m_needsBurstLayout=true;m_modified=true;emit graphChanged();emit documentModified(true);}
}
inline void GraphDocument::build(_usdint_ n,_usdint_ m,vector<indexGraph::ewl>& el) {
    m_graph->build(n,m,el); m_nodePositions.clear(); m_selectedNodes.clear(); m_selectedEdges.clear();
    m_startNode=m_endNode=0; m_layoutLocked=false; m_needsBurstLayout=true; m_modified=false; emit graphChanged();
}
inline void GraphDocument::moveNodes(const QList<_usdint_>& ids,QPointF d) {
    for(_usdint_ id:ids)if(m_nodePositions.contains(id))m_nodePositions[id]+=d; m_modified=true; emit layoutChanged();
}
inline void GraphDocument::selectNode(_usdint_ id,bool multi) {
    if(!multi){m_selectedNodes.clear();m_selectedEdges.clear();}
    if(m_selectedNodes.contains(id))m_selectedNodes.remove(id);else m_selectedNodes.insert(id);
    emit selectionChanged();
}
inline void GraphDocument::selectEdge(const EdgeKey& ek,bool multi) {
    if(!multi){m_selectedEdges.clear();m_selectedNodes.clear();}
    if(m_selectedEdges.contains(ek))m_selectedEdges.remove(ek);else m_selectedEdges.insert(ek);
    emit selectionChanged();
}
inline void GraphDocument::clearSelection() { m_selectedNodes.clear();m_selectedEdges.clear();emit selectionChanged(); }

inline void GraphDocument::forceLayout() {
    _usdint_ nc=m_graph->node_count(),mid=m_graph->get_maxid(); if(nc==0)return; m_layoutLocked=false;
    if(!m_nodePositions.contains(1))m_nodePositions[1]=QPointF(0,0);
    if(!m_graph->node_valid(1)){/* node 1 missing, pick first valid */for(_usdint_ i=2;i<=mid;i++){if(m_graph->node_valid(i)){m_nodePositions[i]=QPointF(0,0);m_layoutLocked=true;return;}}}
    if(nc==1){m_layoutLocked=true;return;}
    _usdint_ vc=0; for(_usdint_ i=2;i<=mid;i++){if(m_graph->node_valid(i))vc++;}
    if(vc==0){m_layoutLocked=true;return;}
    /* 使用运行时布局参数（可由物理对话框调整），未设置则回退 define */
    double _lSpacing=m_loIDEAL>0?m_loIDEAL:DEF_LAYOUT_SPACING;
    double _lRepulse=m_loREPULSE>0?m_loREPULSE:DEF_LAYOUT_REPULSE;
    double _lAttract=m_loATTRACT>0?m_loATTRACT:DEF_LAYOUT_ATTRACT;
    double _lCenter=m_loCENTER>0?m_loCENTER:DEF_LAYOUT_CENTER;
    double _lMinDist=m_loMIN_DIST>0?m_loMIN_DIST:DEF_LAYOUT_MIN_DIST;
    double _lOverlap=m_loOVERLAP>0?m_loOVERLAP:DEF_LAYOUT_OVERLAP;
    double sp=_lSpacing;
    {
        _usdint_ vc=0;for(_usdint_ i=2;i<=mid;i++){if(m_graph->node_valid(i))vc++;}
        double ir=qMax(100.0,sp*(double)vc/(2.0*3.14159265));
        QPointF c=m_nodePositions[1];double as=vc>0?(2.0*3.14159265)/(double)vc:0;_usdint_ idx2=0;
        for(_usdint_ i=2;i<=mid;i++){if(!m_graph->node_valid(i))continue;m_nodePositions[i]=QPointF(c.x()+ir*cos(as*idx2),c.y()+ir*sin(as*idx2));idx2++;}
        int mi=nc>50?80:150;double rc=nc>50?400.0:1e9;
        const double rk=_lRepulse,ak=_lAttract,ck=_lCenter,dm=DEF_LAYOUT_DAMPING,md=_lMinDist,od=_lOverlap;
        QMap<_usdint_,QPointF> vel;for(_usdint_ i=1;i<=mid;i++){if(m_graph->node_valid(i))vel[i]=QPointF(0,0);}
        for(int it=0;it<mi;it++){
            QMap<_usdint_,QPointF> fc;for(_usdint_ i=1;i<=mid;i++){if(m_graph->node_valid(i))fc[i]=QPointF(0,0);}
            for(_usdint_ i=2;i<=mid;i++){if(!m_graph->node_valid(i))continue;QPointF pi=m_nodePositions[i];
                for(_usdint_ j=i+1;j<=mid;j++){if(!m_graph->node_valid(j))continue;QPointF pj=m_nodePositions[j];
                    QPointF d=pi-pj;double ds=d.x()*d.x()+d.y()*d.y();if(ds>rc)continue;ds=sqrt(ds);if(ds<md)ds=md;
                    double f=rk/(ds*ds);if(ds<od)f*=2.0;QPointF u=d/ds;fc[i]+=u*f;fc[j]-=u*f;}}
            for(_usdint_ i=1;i<=mid;i++){if(!m_graph->node_valid(i))continue;
                for(_usdint_ j:m_graph->get_adjoin(i)){if(!m_graph->node_valid(j)||i>=j)continue;
                    QPointF d=m_nodePositions[j]-m_nodePositions[i];double ds=sqrt(d.x()*d.x()+d.y()*d.y());if(ds<md)ds=md;
                    double f=ak*(ds-sp);QPointF u=d/ds;fc[i]+=u*f;fc[j]-=u*f;}}
            for(_usdint_ i=2;i<=mid;i++){if(!m_graph->node_valid(i))continue;
                double ds=sqrt((m_nodePositions[i].x()-c.x())*(m_nodePositions[i].x()-c.x())+(m_nodePositions[i].y()-c.y())*(m_nodePositions[i].y()-c.y()));if(ds<md)ds=md;fc[i]+=(c-m_nodePositions[i])*ck;}
            bool conv=true;
            for(_usdint_ i=2;i<=mid;i++){if(!m_graph->node_valid(i))continue;vel[i]=vel[i]*dm+fc[i];
                double v=sqrt(vel[i].x()*vel[i].x()+vel[i].y()*vel[i].y());if(v>10)vel[i]=vel[i]*10/v;m_nodePositions[i]+=vel[i];if(v>0.5)conv=false;}
            if(conv)break;
        }
    }
    /* 连通分量各自独立力导向 */
    {
        QMap<_usdint_,bool> vis;for(_usdint_ i=1;i<=mid;i++)vis[i]=false;
        QList<QList<_usdint_>> comps;
        for(_usdint_ i=1;i<=mid;i++){if(!m_graph->node_valid(i)||vis[i])continue;
            QList<_usdint_> comp;QQueue<_usdint_> q;q.enqueue(i);vis[i]=true;
            while(!q.isEmpty()){_usdint_ u=q.dequeue();comp.append(u);
                for(_usdint_ v:m_graph->get_adjoin(u)){if(m_graph->node_valid(v)&&!vis[v]){vis[v]=true;q.enqueue(v);}}}
            comps.append(comp);}
        /* 每个分量内部力导向 */
        for(auto& comp:comps){
            if(comp.size()<=1)continue;
            _usdint_ anchor=comp[0];
            QPointF ca=m_nodePositions[anchor];
            int mi2=comp.size()>50?100:200;
            const double rk2=_lRepulse,ak2=_lAttract,ck2=0.02,dm2=DEF_LAYOUT_DAMPING,md2=_lMinDist,od2=_lOverlap;
            QMap<_usdint_,QPointF> vel2;
            for(_usdint_ id:comp)vel2[id]=QPointF(0,0);
            for(int it=0;it<mi2;it++){
                QMap<_usdint_,QPointF> fc2;for(_usdint_ id:comp)fc2[id]=QPointF(0,0);
                for(size_t ii=0;ii<comp.size();ii++){_usdint_ u=comp[ii];if(u==anchor)continue;QPointF pu=m_nodePositions[u];
                    for(size_t jj=ii+1;jj<comp.size();jj++){_usdint_ v=comp[jj];if(v==anchor)continue;QPointF pv=m_nodePositions[v];
                        QPointF dd=pu-pv;double ds=dd.x()*dd.x()+dd.y()*dd.y();ds=sqrt(ds);if(ds<md2)ds=md2;
                        double f=rk2/(ds*ds);if(ds<od2)f*=2.0;QPointF uu=dd/ds;fc2[u]+=uu*f;fc2[v]-=uu*f;}}
                for(_usdint_ u:comp){if(u==anchor)continue;
                    for(_usdint_ v:m_graph->get_adjoin(u)){if(!m_graph->node_valid(v)||!comp.contains(v)||u>=v)continue;
                        QPointF dd=m_nodePositions[v]-m_nodePositions[u];double ds=sqrt(dd.x()*dd.x()+dd.y()*dd.y());if(ds<md2)ds=md2;
                        double f=ak2*(ds-_lSpacing);QPointF uu=dd/ds;fc2[u]+=uu*f;fc2[v]-=uu*f;}}
                for(_usdint_ u:comp){if(u==anchor)continue;
                    double ds=sqrt((m_nodePositions[u].x()-ca.x())*(m_nodePositions[u].x()-ca.x())+(m_nodePositions[u].y()-ca.y())*(m_nodePositions[u].y()-ca.y()));if(ds<md2)ds=md2;fc2[u]+=(ca-m_nodePositions[u])*ck2;}
                bool conv2=true;
                for(_usdint_ u:comp){if(u==anchor)continue;vel2[u]=vel2[u]*dm2+fc2[u];
                    double v=sqrt(vel2[u].x()*vel2[u].x()+vel2[u].y()*vel2[u].y());if(v>10)vel2[u]=vel2[u]*10/v;m_nodePositions[u]+=vel2[u];if(v>0.5)conv2=false;}
                if(conv2)break;
            }
        }
    }
    m_layoutLocked=true; emit layoutProgress(100); emit layoutChanged();
    /* 优先级1: 结点间距约束 */
    emit layoutProgress(5);
    enforceMinSpacing();
    /* 优先级2: 边不穿过结点 */
    if(nc>2&&nc<=200){
        for(int pass=0;pass<50;pass++){
            bool anyHit=false;
            for(_usdint_ nid=1;nid<=mid;nid++){if(!m_graph->node_valid(nid)||!m_nodePositions.contains(nid)||nid==1)continue;
                QPointF nc2=m_nodePositions[nid];
                for(_usdint_ i=1;i<=mid;i++){if(!m_graph->node_valid(i)||!m_nodePositions.contains(i))continue;
                    for(_usdint_ j:m_graph->get_adjoin_asf(i)){if(!m_graph->node_valid(j)||!m_nodePositions.contains(j))continue;
                        if(nid==i||nid==j)continue;
                        QPointF a=m_nodePositions[i],b=m_nodePositions[j],ab=b-a;
                        double ls=ab.x()*ab.x()+ab.y()*ab.y();if(ls<0.01)continue;
                        double t=qBound(0.0,QPointF::dotProduct(nc2-a,ab)/ls,1.0);
                        QPointF prj=a+ab*t;QPointF d=nc2-prj;
                        double dist=sqrt(d.x()*d.x()+d.y()*d.y());
                        if(dist<_lOverlap&&dist>0.1){anyHit=true;QPointF u=d/dist;m_nodePositions[nid]+=u*(_lOverlap-dist)*1.0;}
                    }
                }
            }
            if(!anyHit)break;
        }
    }
    /* 优先级1复查 */
    emit layoutProgress(40);
    enforceMinSpacing();
    /* 连通分量分层排列: 以1号分量为中心环绕 */
    if(nc>2){
        QMap<_usdint_,bool> vis;for(_usdint_ i=1;i<=mid;i++)vis[i]=false;
        QList<QList<_usdint_>> comps;
        for(_usdint_ i=1;i<=mid;i++){if(!m_graph->node_valid(i)||vis[i])continue;
            QList<_usdint_> comp;QQueue<_usdint_> q;q.enqueue(i);vis[i]=true;
            while(!q.isEmpty()){_usdint_ u=q.dequeue();comp.append(u);
                for(_usdint_ v:m_graph->get_adjoin(u)){if(m_graph->node_valid(v)&&!vis[v]){vis[v]=true;q.enqueue(v);}}}
            comps.append(comp);}
        if(comps.size()>1){
            struct CB{double x1,y1,x2,y2,cx,cy,w,h;};
            QList<CB> boxes;int comp1Idx=-1;
            for(int ci=0;ci<comps.size();ci++){CB b;b.x1=1e9;b.y1=1e9;b.x2=-1e9;b.y2=-1e9;
                for(_usdint_ id:comps[ci]){QPointF p=m_nodePositions[id];b.x1=qMin(b.x1,p.x());b.y1=qMin(b.y1,p.y());b.x2=qMax(b.x2,p.x());b.y2=qMax(b.y2,p.y());}
                b.cx=(b.x1+b.x2)/2;b.cy=(b.y1+b.y2)/2;b.w=b.x2-b.x1;b.h=b.y2-b.y1;
                boxes.append(b);if(comps[ci].contains(1))comp1Idx=ci;}
            /* 以1号分量为圆心，其余分量环绕 */
            if(comp1Idx>=0&&comps[0].contains(1)){}else if(comp1Idx>=0){std::swap(comps[0],comps[comp1Idx]);std::swap(boxes[0],boxes[comp1Idx]);}
            int others=comps.size()-1;if(others>0){
                /* 紧密装箱：按面积降序, 螺旋搜索非重叠位置 */
                double pad=_lMinDist+DEF_NODE_RADIUS;
                /* 按包围盒面积降序排列(Component1保持第0位) */
                QList<int> ciOrder;for(int ci=1;ci<comps.size();ci++)ciOrder.append(ci);
                sort(ciOrder.begin(),ciOrder.end(),[&](int a,int b){return (boxes[a].w+pad)*(boxes[a].h+pad)>(boxes[b].w+pad)*(boxes[b].h+pad);});
                /* 已放置包围盒列表(用于重叠检测) */
                struct Placed{double l,t,r,b;};
                QList<Placed> placed;placed.append({boxes[0].x1-pad,boxes[0].y1-pad,boxes[0].x2+pad,boxes[0].y2+pad});
                auto overlap=[&](double l,double t,double r,double b)->bool{
                    for(auto& p:placed){if(!(r<=p.l||l>=p.r||b<=p.t||t>=p.b))return true;}return false;};
                for(int ci:ciOrder){
                    double bw=boxes[ci].w+pad*2,bh=boxes[ci].h+pad*2;
                    /* 从中心螺旋向外搜索非重叠位置 */
                    bool found=false;double bestX=0,bestY=0;
                    for(double rad=20;rad<8000&&!found;rad+=25){
                        int steps=(int)(2*3.14159265*rad/25);if(steps<8)steps=8;
                        for(int s=0;s<steps&&!found;s++){
                            double ang=2*3.14159265*s/steps;
                            double cx=boxes[0].cx+rad*cos(ang),cy=boxes[0].cy+rad*sin(ang);
                            double l=cx-bw/2,t=cy-bh/2,r=cx+bw/2,b=cy+bh/2;
                            if(!overlap(l,t,r,b)){bestX=cx-boxes[ci].cx;bestY=cy-boxes[ci].cy;found=true;}
                        }
                    }
                    if(found){for(_usdint_ id:comps[ci])m_nodePositions[id]+=QPointF(bestX,bestY);
                        placed.append({boxes[ci].x1+bestX-pad,boxes[ci].y1+bestY-pad,boxes[ci].x2+bestX+pad,boxes[ci].y2+bestY+pad});}
                }
                /* 分量间力导向：强引力拉向1号+分量互斥+微形变 */
QMap<int,QPointF> cen;for(int ci=0;ci<comps.size();ci++){double sx=0,sy=0;for(_usdint_ id:comps[ci]){sx+=m_nodePositions[id].x();sy+=m_nodePositions[id].y();}cen[ci]=QPointF(sx/comps[ci].size(),sy/comps[ci].size());}
                QPointF c0=cen[0];QMap<int,QPointF> cv;for(int ci=0;ci<comps.size();ci++)cv[ci]=QPointF(0,0);
                for(int it=0;it<200;it++){
                    QMap<int,QPointF> fc;for(int ci=0;ci<comps.size();ci++)fc[ci]=QPointF(0,0);
                    double r=12.0*(1+comps.size()/3.0);
                    for(int ci=1;ci<comps.size();ci++){QPointF pi=cen[ci];
                        for(int cj=ci+1;cj<comps.size();cj++){QPointF pj=cen[cj];
                            QPointF d=pi-pj;double ds=sqrt(d.x()*d.x()+d.y()*d.y());if(ds>r)continue;if(ds<5)ds=5;
                            double f=4000.0/(ds*ds);QPointF u=d/ds;fc[ci]+=u*f;fc[cj]-=u*f;}
                        /* 强引力拉向1号分量（比内部力弱） */
                        QPointF d2=c0-pi;double ds2=sqrt(d2.x()*d2.x()+d2.y()*d2.y());if(ds2<5)ds2=5;
                        fc[ci]+=d2/ds2*ds2*0.01;
                        /* 与1号分量保持距离 */
                        double ds3=ds2;if(ds3<5)ds3=5;double f3=2500.0/(ds3*ds3);fc[ci]-=d2/ds3*f3;
                    }
                    bool conv=true;
                    for(int ci=1;ci<comps.size();ci++){cv[ci]=cv[ci]*0.82+fc[ci];
                        double v=sqrt(cv[ci].x()*cv[ci].x()+cv[ci].y()*cv[ci].y());if(v>6)cv[ci]=cv[ci]*6/v;
                        QPointF mv=cv[ci];cen[ci]+=mv;
                        for(_usdint_ id:comps[ci])m_nodePositions[id]+=mv;
                        if(sqrt(mv.x()*mv.x()+mv.y()*mv.y())>0.3)conv=false;}
                    if(conv)break;
                }
                /* 微引力作用在各分量结点上(比分量内部引力小) */
                for(int ci=1;ci<comps.size();ci++){for(_usdint_ id:comps[ci]){if(id==1)continue;
                    QPointF d=c0-m_nodePositions[id];double ds=sqrt(d.x()*d.x()+d.y()*d.y());if(ds<5)ds=5;
                    m_nodePositions[id]+=d/ds*ds*0.001;}}
                

            }
            /* 分量边界结点斥力：防止过度紧凑 */
            for(int pass=0;pass<15;pass++){bool anyPush=false;
                for(int ci=0;ci<comps.size();ci++){for(_usdint_ nid:comps[ci]){if(nid==1)continue;QPointF np=m_nodePositions[nid];
                    for(int cj=ci+1;cj<comps.size();cj++){for(_usdint_ nid2:comps[cj]){if(nid2==1)continue;
                        QPointF d=np-m_nodePositions[nid2];double ds=sqrt(d.x()*d.x()+d.y()*d.y());
                        if(ds<_lMinDist+DEF_NODE_RADIUS&&ds>0.1){anyPush=true;QPointF u=d/ds;
                            double push=(_lMinDist+DEF_NODE_RADIUS-ds)/2.0+1.0;
                            m_nodePositions[nid]+=u*push;m_nodePositions[nid2]-=u*push;}}}}
                if(!anyPush)break;}
            /* 重建包围盒 */
            for(int ci=0;ci<comps.size();ci++){CB& b=boxes[ci];b.x1=1e9;b.y1=1e9;b.x2=-1e9;b.y2=-1e9;
                for(_usdint_ id:comps[ci]){QPointF p=m_nodePositions[id];b.x1=qMin(b.x1,p.x());b.y1=qMin(b.y1,p.y());b.x2=qMax(b.x2,p.x());b.y2=qMax(b.y2,p.y());}
                b.cx=(b.x1+b.x2)/2;b.cy=(b.y1+b.y2)/2;b.w=b.x2-b.x1;b.h=b.y2-b.y1;}
            /* 分量间去重叠 */
            for(int pass=0;pass<20;pass++){bool anyOver=false;
                for(int ci=0;ci<comps.size();ci++){CB& bi=boxes[ci];
                    for(int cj=ci+1;cj<comps.size();cj++){CB& bj=boxes[cj];
                        double ox=qMax(0.0,qMin(bi.x2,bj.x2)-qMax(bi.x1,bj.x1));
                        double oy=qMax(0.0,qMin(bi.y2,bj.y2)-qMax(bi.y1,bj.y1));
                        if(ox>0&&oy>0){anyOver=true;double pad=30;
                            if(ox<oy){double push=(ox+pad)/2;if(bi.cx<bj.cx){for(_usdint_ id:comps[ci])m_nodePositions[id].rx()-=push;for(_usdint_ id:comps[cj])m_nodePositions[id].rx()+=push;bi.cx-=push;bi.x1-=push;bi.x2-=push;bj.cx+=push;bj.x1+=push;bj.x2+=push;}else{for(_usdint_ id:comps[ci])m_nodePositions[id].rx()+=push;for(_usdint_ id:comps[cj])m_nodePositions[id].rx()-=push;bi.cx+=push;bi.x1+=push;bi.x2+=push;bj.cx-=push;bj.x1-=push;bj.x2-=push;}}
                            else{double push=(oy+pad)/2;if(bi.cy<bj.cy){for(_usdint_ id:comps[ci])m_nodePositions[id].ry()-=push;for(_usdint_ id:comps[cj])m_nodePositions[id].ry()+=push;bi.cy-=push;bi.y1-=push;bi.y2-=push;bj.cy+=push;bj.y1+=push;bj.y2+=push;}else{for(_usdint_ id:comps[ci])m_nodePositions[id].ry()+=push;for(_usdint_ id:comps[cj])m_nodePositions[id].ry()-=push;bi.cy+=push;bi.y1+=push;bi.y2+=push;bj.cy-=push;bj.y1-=push;bj.y2-=push;}}}}}
                if(!anyOver)break;}
        }
    }
    /* 优先级3: 边交叉消减 */
    if(nc>2&&nc<=200){
        _usdint_ ec=m_graph->edge_count();int xpasses=ec>100?10:(ec>50?20:30);
        for(int pass=0;pass<xpasses;pass++){
            bool anyCross=false;
            for(_usdint_ i=1;i<=mid;i++){if(!m_graph->node_valid(i)||!m_nodePositions.contains(i))continue;
                for(_usdint_ j:m_graph->get_adjoin_asf(i)){if(!m_graph->node_valid(j)||!m_nodePositions.contains(j)||i>=j)continue;
                    QPointF a=m_nodePositions[i],b=m_nodePositions[j];
                    for(_usdint_ k=1;k<=mid;k++){if(!m_graph->node_valid(k)||!m_nodePositions.contains(k))continue;
                        for(_usdint_ l:m_graph->get_adjoin_asf(k)){if(!m_graph->node_valid(l)||!m_nodePositions.contains(l))continue;
                            if((i==k&&j==l)||(i==l&&j==k))continue;
                            QPointF c=m_nodePositions[k],d=m_nodePositions[l];
                            auto cross2d=[](const QPointF& p,const QPointF& q){return p.x()*q.y()-p.y()*q.x();};
                            QPointF ab=b-a,cd=d-c,ac=c-a;
                            double d1=cross2d(ab,cd);if(fabs(d1)<1e-9)continue;
                            double t=cross2d(ac,cd)/d1,u=cross2d(ac,ab)/d1;
                            if(t>0.01&&t<0.99&&u>0.01&&u<0.99){
                                anyCross=true;
                                QPointF midP=a+ab*t;QPointF pushDir(-ab.y(),ab.x());
                                double pl=sqrt(pushDir.x()*pushDir.x()+pushDir.y()*pushDir.y());
                                if(pl>0){pushDir/=pl;double f=3.0;
                                    if(i!=1)m_nodePositions[i]-=pushDir*f;
                                    if(j!=1)m_nodePositions[j]-=pushDir*f;
                                    if(k!=1)m_nodePositions[k]+=pushDir*f;
                                    if(l!=1)m_nodePositions[l]+=pushDir*f;
                                }
                            }
                        }
                    }
                }
            }
            if(!anyCross)break;
        }
    }
    /* 最终间距+边-结点保证 */
    emit layoutProgress(85);
    enforceMinSpacing();
    for(int pass=0;pass<30;pass++){bool anyHit=false;
        for(_usdint_ nid=1;nid<=mid;nid++){if(!m_graph->node_valid(nid)||!m_nodePositions.contains(nid)||nid==1)continue;
            QPointF nc2=m_nodePositions[nid];
            for(_usdint_ i=1;i<=mid;i++){if(!m_graph->node_valid(i)||!m_nodePositions.contains(i))continue;
                for(_usdint_ j:m_graph->get_adjoin_asf(i)){if(!m_graph->node_valid(j)||!m_nodePositions.contains(j))continue;
                    if(nid==i||nid==j)continue;QPointF a=m_nodePositions[i],b=m_nodePositions[j],ab=b-a;
                    double ls=ab.x()*ab.x()+ab.y()*ab.y();if(ls<0.01)continue;
                    double t=qBound(0.0,QPointF::dotProduct(nc2-a,ab)/ls,1.0);
                    QPointF prj=a+ab*t;QPointF d=nc2-prj;double dist=sqrt(d.x()*d.x()+d.y()*d.y());
                    if(dist<_lOverlap&&dist>0.1){anyHit=true;QPointF u=d/dist;m_nodePositions[nid]+=u*(_lOverlap-dist)*1.5;}}}}
        if(!anyHit)break;}
    }
    enforceMinSpacing();
}
inline void GraphDocument::enforceMinSpacing(double minDist){
    _usdint_ mid=m_graph->get_maxid(); bool anyOverlap=true;
    for(int pass=0;pass<20&&anyOverlap;pass++){
        anyOverlap=false;
        for(_usdint_ i=1;i<=mid;i++){if(!m_graph->node_valid(i)||!m_nodePositions.contains(i))continue;
            for(_usdint_ j=i+1;j<=mid;j++){if(!m_graph->node_valid(j)||!m_nodePositions.contains(j))continue;
                QPointF d=m_nodePositions[j]-m_nodePositions[i];
                double dist=sqrt(d.x()*d.x()+d.y()*d.y());
                if(dist<minDist&&dist>0.01){
                    anyOverlap=true;
                    QPointF u=d/dist; double push=(minDist-dist)/2.0+1.0;
                    if(i!=1)m_nodePositions[i]-=u*push;
                    if(j!=1)m_nodePositions[j]+=u*push;
                }
            }
        }
    }
    if(anyOverlap)emit layoutChanged();
}

inline InfoOverlay::InfoOverlay(QWidget* parent) : QWidget(parent), m_collapsed(false), m_dragging(false), m_resizing(false) {
    setObjectName("infoOverlay"); setMinimumWidth(140);
    setStyleSheet("background-color:rgba(40,40,40,220);border:1px solid #555;border-radius:4px;");
    QVBoxLayout* lay=new QVBoxLayout(this); lay->setContentsMargins(4,2,4,2); lay->setSpacing(0);
    m_titleBar=new QWidget(this); m_titleBar->setObjectName("infoTitleBar");
    m_titleBar->setFixedHeight(24); m_titleBar->setCursor(Qt::PointingHandCursor);
    m_titleBar->setStyleSheet("background-color:rgba(60,60,60,200);border-radius:3px;"); m_titleBar->installEventFilter(this);
    QHBoxLayout* tl=new QHBoxLayout(m_titleBar); tl->setContentsMargins(4,0,4,0);
    m_title=new QLabel("▼ 图信息",m_titleBar); m_title->setStyleSheet("color:#fff;font-weight:bold;font-size:11px;");
    tl->addWidget(m_title); tl->addStretch(); lay->addWidget(m_titleBar);
    m_content=new QLabel("",this); m_content->setStyleSheet("color:#ccc;font-size:11px;"); m_content->setWordWrap(true); lay->addWidget(m_content);
    updateSize(); resetPosition(); show(); raise();
    installEventFilter(this);
}
inline void InfoOverlay::refresh(const GraphDocument* doc) {
    if(!doc||doc->empty()){m_content->setText("空图");return;}
    _usdint_ nc=doc->nodeCount(),ec=doc->edgeCount(),to=0,ti=0,mid=doc->maxId();
    for(_usdint_ i=1;i<=mid;i++){if(!doc->nodeValid(i))continue;to+=doc->outDeg(i);ti+=doc->inDeg(i);}
    m_content->setText(QString("结点数:%1\n边数:%2\n总出度:%3\n总入度:%4\n连通分量:%5").arg(nc).arg(ec).arg(to).arg(ti).arg(doc->areaCount()));
}
inline void InfoOverlay::resetPosition(){QWidget* pw=parentWidget();if(pw){QGraphicsView* gv=qobject_cast<QGraphicsView*>(pw);QPoint tr=gv?gv->viewport()->rect().topRight():pw->rect().topRight();move(tr.x()-width()-10,tr.y()+10);}}
inline void InfoOverlay::updateSize(){m_content->setVisible(!m_collapsed);setFixedHeight(m_collapsed?32:160);}
inline bool InfoOverlay::eventFilter(QObject* obj,QEvent* event) {
    if(obj==m_titleBar){
        if(event->type()==QEvent::MouseButtonPress){QMouseEvent* me=static_cast<QMouseEvent*>(event);if(me->button()==Qt::LeftButton){m_dragging=true;m_dragStart=me->globalPosition().toPoint();m_panelStart=pos();return true;}}
        if(event->type()==QEvent::MouseMove&&m_dragging){QMouseEvent* me=static_cast<QMouseEvent*>(event);move(m_panelStart+me->globalPosition().toPoint()-m_dragStart);return true;}
        if(event->type()==QEvent::MouseButtonRelease&&m_dragging){m_dragging=false;return true;}
        if(event->type()==QEvent::MouseButtonDblClick){m_collapsed=!m_collapsed;m_title->setText(m_collapsed?"▶ 图信息":"▼ 图信息");updateSize();return true;}
        return false;
    }
    if(obj==this){
        if(event->type()==QEvent::MouseButtonPress){QMouseEvent* me=static_cast<QMouseEvent*>(event);if(me->button()==Qt::LeftButton&&me->pos().x()>=width()-8){m_resizing=true;m_resizeStartX=me->globalPosition().toPoint().x();m_resizeStartW=width();setCursor(Qt::SizeHorCursor);return true;}}
        if(event->type()==QEvent::MouseMove){QMouseEvent* me=static_cast<QMouseEvent*>(event);if(m_resizing){int nw=m_resizeStartW+me->globalPosition().toPoint().x()-m_resizeStartX;if(nw<80)nw=80;if(nw>400)nw=400;setFixedWidth(nw);return true;}if(me->pos().x()>=width()-8)setCursor(Qt::SizeHorCursor);else if(!m_resizing)setCursor(Qt::ArrowCursor);}
        if(event->type()==QEvent::MouseButtonRelease&&m_resizing){m_resizing=false;setCursor(Qt::ArrowCursor);return true;}
    }
    return QWidget::eventFilter(obj,event);
}

/* ─── MinimapOverlay ─── */

inline MinimapOverlay::MinimapOverlay(QWidget* parent) : QGraphicsView(parent), m_dragging(false), m_widgetDragging(false) {
    setObjectName("minimapOverlay"); setFixedSize(150,100);
    setStyleSheet("border:1px solid #888;background-color:rgba(30,30,30,180);");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setInteractive(false); setRenderHint(QPainter::Antialiasing);
    m_miniScene=new QGraphicsScene(this); setScene(m_miniScene);
    m_viewportIndicator=m_miniScene->addRect(0,0,1,1,QPen(QColor(DEF_MINIMAP_VP,180),1),QBrush(QColor(DEF_MINIMAP_VP,30)));
    m_viewportIndicator->setZValue(10); show(); raise();
}
inline void MinimapOverlay::resetPosition(){if(parentWidget()){QPoint p=parentWidget()->rect().bottomLeft();move(p.x()+10,p.y()-height()-10);}}

inline void MinimapOverlay::renderMinimap(GraphCanvas* canvas) {
    m_miniScene->clear(); m_viewportIndicator=nullptr;
    const GraphDocument* doc=canvas->document(); if(!doc)return;
    _usdint_ nc=doc->nodeCount(),mid=doc->maxId(); double scale=DEF_MINIMAP_SCALE;
    QMap<_usdint_,QPointF> mp;
    for(auto it=doc->allNodePositions().begin();it!=doc->allNodePositions().end();++it) mp[it.key()]=it.value()*scale;
    for(_usdint_ nid=1;nid<=mid;nid++){if(!doc->nodeValid(nid))continue;
        for(_usdint_ tid:doc->adjacencyFrom(nid)){if(!mp.contains(nid)||!mp.contains(tid))continue;
            QPointF a=mp[nid],b=mp[tid],d=b-a;double l=sqrt(d.x()*d.x()+d.y()*d.y());if(l>0){d/=l;a+=d*3;b-=d*3;}
            m_miniScene->addLine(a.x(),a.y(),b.x(),b.y(),QPen(QColor(DEF_MINIMAP_EDGE),0.5));}}
    for(auto it=mp.begin();it!=mp.end();++it) m_miniScene->addEllipse(it.value().x()-2,it.value().y()-2,4,4,QPen(QColor(DEF_MINIMAP_NODE)),QBrush(QColor(DEF_MINIMAP_NODE)));
    m_viewportIndicator=m_miniScene->addRect(0,0,1,1,QPen(QColor(DEF_MINIMAP_VP,180),1),QBrush(QColor(DEF_MINIMAP_VP,30)));m_viewportIndicator->setZValue(10);
    QRectF b=m_miniScene->itemsBoundingRect().adjusted(-5,-5,5,5);if(!b.isNull())fitInView(b,Qt::KeepAspectRatio);
}
inline void MinimapOverlay::updateViewportIndicator(GraphCanvas* canvas){if(!m_viewportIndicator||!canvas)return;QRectF vr=canvas->mapToScene(canvas->viewport()->rect()).boundingRect();double s=0.15;m_viewportIndicator->setRect(vr.x()*s,vr.y()*s,vr.width()*s,vr.height()*s);}
inline void MinimapOverlay::syncWithCanvas(GraphCanvas* canvas){if(!canvas||!canvas->document()||canvas->document()->empty()){m_miniScene->clear();m_viewportIndicator=m_miniScene->addRect(0,0,1,1,QPen(QColor(DEF_MINIMAP_VP,180),1),QBrush(QColor(DEF_MINIMAP_VP,30)));m_viewportIndicator->setZValue(10);return;}renderMinimap(canvas);updateViewportIndicator(canvas);}
inline void MinimapOverlay::mousePressEvent(QMouseEvent* event){
    if(event->button()==Qt::LeftButton&&isInHandle(event->pos())){
        m_widgetDragging=true;m_widgetDragStart=event->globalPosition().toPoint()-pos();
        event->accept();return;
    }
    if(event->button()==Qt::LeftButton){
        m_dragging=true;
        GraphCanvas* gc=qobject_cast<GraphCanvas*>(parentWidget());
        if(gc){QPointF mp=mapToScene(event->pos());gc->centerOn(QPointF(mp.x()/0.15,mp.y()/0.15));}
        event->accept();return;
    }
    QGraphicsView::mousePressEvent(event);
}
inline void MinimapOverlay::mouseMoveEvent(QMouseEvent* event){
    if(m_widgetDragging){
        QPoint np=event->globalPosition().toPoint()-m_widgetDragStart;
        QWidget* pw=parentWidget();if(pw){np.setX(qBound(0,np.x(),pw->width()-width()));np.setY(qBound(0,np.y(),pw->height()-height()));}
        move(np);event->accept();return;
    }
    if(m_dragging){
        GraphCanvas* gc=qobject_cast<GraphCanvas*>(parentWidget());
        if(gc){QPointF mp=mapToScene(event->pos());gc->centerOn(QPointF(mp.x()/0.15,mp.y()/0.15));}
        event->accept();return;
    }
    QGraphicsView::mouseMoveEvent(event);
}
inline void MinimapOverlay::mouseReleaseEvent(QMouseEvent* event){
    if(event->button()==Qt::LeftButton&&m_widgetDragging){m_widgetDragging=false;event->accept();return;}
    if(event->button()==Qt::LeftButton&&m_dragging){m_dragging=false;event->accept();return;}
    QGraphicsView::mouseReleaseEvent(event);
}
inline void MinimapOverlay::mouseDoubleClickEvent(QMouseEvent* event){
    if(event->button()==Qt::LeftButton){
        GraphCanvas* gc=qobject_cast<GraphCanvas*>(parentWidget());
        if(gc)gc->centerOnGraphCenter();
        event->accept();return;
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}
inline bool MinimapOverlay::isInHandle(const QPoint& pos) const {return pos.y()<=14;}
inline void MinimapOverlay::paintEvent(QPaintEvent* event){
    QGraphicsView::paintEvent(event);
    QPainter p(viewport());p.fillRect(0,0,width(),15,QColor(60,60,60,180));
    p.setPen(QColor(180,180,180));p.drawText(QRect(0,0,width(),15),Qt::AlignCenter,"⋮⋮⋮");
}

/* ─── GraphCanvas ─── */

inline GraphCanvas::GraphCanvas(QWidget* parent) : QGraphicsView(parent), m_document(nullptr), m_zoomLevel(1.0), m_interactMode(InteractMode::Normal), m_panning(false), m_edgeDragging(false), m_edgeBidirectional(false), m_edgeDragSource(0), m_tempEdgeLine(nullptr), m_moveAnchorSet(false), m_movingNodeId(0), m_wasdActive(false), m_leftDragPan(false), m_zHeld(false), m_startLabel(nullptr), m_endLabel(nullptr) {
    m_wasdDown[0]=m_wasdDown[1]=m_wasdDown[2]=m_wasdDown[3]=false; m_wasdVx=m_wasdVy=0;
    m_scene=new QGraphicsScene(this); setScene(m_scene);
    m_scene->setBackgroundBrush(QBrush(QColor(DEF_CANVAS_BG)));
    setStyleSheet("QGraphicsView{border:none;background:#2d2d2d;}");
    setRenderHint(QPainter::Antialiasing); setDragMode(QGraphicsView::NoDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate); setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setFocusPolicy(Qt::StrongFocus); m_infoOverlay=new InfoOverlay(this); m_minimapOverlay=new MinimapOverlay(this);
    /* 缩放指示器 */
    m_zoomLabel=new QLabel("100%",this); m_zoomLabel->setStyleSheet("background-color:rgba(40,40,40,200);color:#fff;font-size:12px;padding:2px 8px;border-radius:3px;");m_zoomLabel->hide();
    m_selectInfoLabel=new QLabel(this); m_selectInfoLabel->setStyleSheet("background-color:rgba(40,40,40,220);color:#eee;font-size:11px;padding:2px 10px;border-radius:3px;");m_selectInfoLabel->hide();
    m_zoomFadeTimer=new QTimer(this);m_zoomFadeTimer->setSingleShot(true);m_zoomFadeTimer->setInterval(1000);connect(m_zoomFadeTimer,&QTimer::timeout,m_zoomLabel,&QLabel::hide);
    m_panTimer=new QTimer(this);m_panTimer->setInterval(16);connect(m_panTimer,&QTimer::timeout,this,[this](){const double accel=1.2,drag=0.82,maxV=16.0;double ax=0,ay=0;if(m_wasdDown[0])ay+=1;if(m_wasdDown[2])ay-=1;if(m_wasdDown[1])ax+=1;if(m_wasdDown[3])ax-=1;double al=sqrt(ax*ax+ay*ay);if(al>0){ax=ax/al*accel;ay=ay/al*accel;}m_wasdVx=(m_wasdVx+ax)*drag;m_wasdVy=(m_wasdVy+ay)*drag;double sv=sqrt(m_wasdVx*m_wasdVx+m_wasdVy*m_wasdVy);if(sv>maxV){m_wasdVx=m_wasdVx*maxV/sv;m_wasdVy=m_wasdVy*maxV/sv;}double step=1.0/qMax(0.15,m_zoomLevel);int dx=(int)(m_wasdVx*step),dy=(int)(m_wasdVy*step);if(dx!=0||dy!=0){horizontalScrollBar()->setValue(horizontalScrollBar()->value()-dx);verticalScrollBar()->setValue(verticalScrollBar()->value()-dy);if(m_minimapOverlay)m_minimapOverlay->syncWithCanvas(this);}if(fabs(m_wasdVx)<0.01&&fabs(m_wasdVy)<0.01&&!m_wasdDown[0]&&!m_wasdDown[1]&&!m_wasdDown[2]&&!m_wasdDown[3]){m_wasdActive=false;m_panTimer->stop();}});
    m_moveFlashTimer=new QTimer(this);m_moveFlashTimer->setInterval(400);connect(m_moveFlashTimer,&QTimer::timeout,this,[this](){static bool on=false;on=!on;if(m_document){for(_usdint_ id:m_document->selectedNodeIds()){if(m_nodeItems.contains(id))m_nodeItems[id]->setPen(QPen(on?QColor(DEF_ALGO_STEP):QColor(DEF_ALGO_SELECT),on?3:2));}}});
    m_physicsActive=false;m_physicsPaused=false;m_physicsLayoutPhase=false;m_physicsTimer=new QTimer(this);m_physicsTimer->setInterval(1000/DEF_PHYSICS_FPS);connect(m_physicsTimer,&QTimer::timeout,this,&GraphCanvas::physicsTick);
    /* 初始化运行时物理参数（默认等于#define） */
    m_pK_REP=DEF_PHYSICS_K_REP;m_pK_UNI=DEF_PHYSICS_K_UNI;m_pK_BI=DEF_PHYSICS_K_BI;m_pG_UNIV=DEF_PHYSICS_G_UNIV;
    m_pGAMMA_L=DEF_PHYSICS_GAMMA_L;m_pGAMMA_D=DEF_PHYSICS_GAMMA_D;
    m_pV_MAX=DEF_PHYSICS_V_MAX;m_pSTOP_VEL=DEF_PHYSICS_STOP_VEL;m_pIDLE_VEL=DEF_PHYSICS_IDLE_VEL;
    m_pIDEAL=DEF_PHYSICS_IDEAL;m_pIDEAL_MAX=DEF_PHYSICS_IDEAL+30;m_pMIN_DIST=DEF_PHYSICS_MIN_DIST;m_pMAX_DIST=DEF_PHYSICS_MAX_DIST;m_pCUTOFF=DEF_PHYSICS_CUTOFF;
    QShortcut* tabShort=new QShortcut(QKeySequence("Tab"),this);connect(tabShort,&QShortcut::activated,this,[this](){m_physicsPaused=!m_physicsPaused;emit physicsToggled(m_physicsPaused);if(!m_physicsPaused){m_nodeVelocities.clear();startPhysics(m_physicsLayoutPhase);}});
}
inline void GraphCanvas::loadPhysicsParams(const SettingsService* settings){if(!settings)return;m_pK_REP=settings->physicsParam("K_REP",DEF_PHYSICS_K_REP);m_pK_UNI=settings->physicsParam("K_UNI",DEF_PHYSICS_K_UNI);m_pK_BI=settings->physicsParam("K_BI",DEF_PHYSICS_K_BI);m_pG_UNIV=settings->physicsParam("G_UNIV",DEF_PHYSICS_G_UNIV);m_pGAMMA_L=settings->physicsParam("GAMMA_L",DEF_PHYSICS_GAMMA_L);m_pGAMMA_D=settings->physicsParam("GAMMA_D",DEF_PHYSICS_GAMMA_D);m_pV_MAX=settings->physicsParam("V_MAX",DEF_PHYSICS_V_MAX);m_pSTOP_VEL=settings->physicsParam("STOP_VEL",DEF_PHYSICS_STOP_VEL);m_pIDLE_VEL=settings->physicsParam("IDLE_VEL",DEF_PHYSICS_IDLE_VEL);m_pIDEAL=settings->physicsParam("IDEAL",DEF_PHYSICS_IDEAL);m_pIDEAL_MAX=settings->physicsParam("IDEAL_MAX",DEF_PHYSICS_IDEAL+30);m_pMIN_DIST=settings->physicsParam("MIN_DIST",DEF_PHYSICS_MIN_DIST);m_pMAX_DIST=settings->physicsParam("MAX_DIST",DEF_PHYSICS_MAX_DIST);m_pCUTOFF=settings->physicsParam("CUTOFF",DEF_PHYSICS_CUTOFF);if(m_physicsActive&&!m_physicsPaused){m_nodeVelocities.clear();startPhysics(m_physicsLayoutPhase);}}
inline void GraphCanvas::setDocument(GraphDocument* doc){
    if(m_document)disconnect(m_document,nullptr,this,nullptr); m_document=doc;
    if(!m_document){m_scene->clear();m_nodeItems.clear();m_nodeTextItems.clear();m_edgeItems.clear();m_arrowItems.clear();m_startLabel=m_endLabel=nullptr;m_infoOverlay->hide();m_minimapOverlay->hide();return;}
    m_infoOverlay->show();m_minimapOverlay->show();connect(m_document,&GraphDocument::graphChanged,this,&GraphCanvas::rebuildScene);connect(m_document,&GraphDocument::graphChanged,this,[this](){if(m_minimapOverlay)m_minimapOverlay->syncWithCanvas(this);});connect(m_document,&GraphDocument::layoutChanged,this,[this](){if(!m_document)return;updateNodePositions(m_document->allNodePositions());startPhysics();if(m_minimapOverlay)m_minimapOverlay->syncWithCanvas(this);});connect(m_document,&GraphDocument::selectionChanged,this,&GraphCanvas::updateSelectionHighlight);connect(m_document,&GraphDocument::selectionChanged,this,&GraphCanvas::showSelectionInfo);connect(m_document,&GraphDocument::startEndChanged,this,&GraphCanvas::updateStartEndLabels);}
inline void GraphCanvas::setZoomLevel(double l){QPointF vc=viewport()?mapToScene(viewport()->rect().center()):QPointF();m_zoomLevel=qBound(0.15,l,4.0);resetTransform();scale(m_zoomLevel,m_zoomLevel);if(!vc.isNull())centerOn(vc);if(m_minimapOverlay)m_minimapOverlay->syncWithCanvas(this);m_zoomLabel->setText(QString("%1%").arg((int)(m_zoomLevel*100)));m_zoomLabel->adjustSize();QRect vr=viewport()->rect();m_zoomLabel->move(vr.right()-m_zoomLabel->width()-10,vr.bottom()-m_zoomLabel->height()-10);m_zoomLabel->show();m_zoomLabel->raise();m_zoomFadeTimer->start();}
inline void GraphCanvas::fitToContent(){QRectF b=m_scene->itemsBoundingRect();if(!b.isNull()){b.adjust(-30,-30,30,30);fitInView(b,Qt::KeepAspectRatio);m_zoomLevel=transform().m11();emit zoomChanged(m_zoomLevel);}}
inline void GraphCanvas::centerOnNode(_usdint_ id){if(m_document&&m_document->hasPosition(id))centerOn(m_document->nodePosition(id));}
inline void GraphCanvas::setInteractMode(InteractMode m){
    if(m==InteractMode::NodeMoving&&m_interactMode!=InteractMode::NodeMoving){m_moveAnchorSet=false;m_moveFlashTimer->start();}
    if(m!=InteractMode::NodeMoving&&m_interactMode==InteractMode::NodeMoving){m_moveFlashTimer->stop();updateSelectionHighlight();}
    m_interactMode=m;setCursor(m==InteractMode::NodeMoving?Qt::OpenHandCursor:Qt::ArrowCursor);
}
inline void GraphCanvas::clearAlgorithmHighlights(){for(auto it=m_nodeItems.begin();it!=m_nodeItems.end();++it)it.value()->setBrush(QBrush(QColor(DEF_NODE_FILL)));updateSelectionHighlight();}
inline void GraphCanvas::highlightNode(_usdint_ id,const QColor& c){if(m_nodeItems.contains(id))m_nodeItems[id]->setBrush(QBrush(c));}
inline void GraphCanvas::fadeNode(_usdint_ id){if(m_nodeItems.contains(id))m_nodeItems[id]->setBrush(QBrush(QColor(DEF_ALGO_FADE)));}
inline void GraphCanvas::highlightEdge(_usdint_ from,_usdint_ to,const QColor& c){EdgeKey ek(from,to);if(m_edgeItems.contains(ek))m_edgeItems[ek]->setPen(QPen(c,3));if(m_arrowItems.contains(ek)){m_arrowItems[ek]->setPen(QPen(c));m_arrowItems[ek]->setBrush(QBrush(c));}}
inline void GraphCanvas::fadeEdge(_usdint_ from,_usdint_ to){EdgeKey ek(from,to);QColor fc(DEF_ALGO_FADE);if(m_edgeItems.contains(ek))m_edgeItems[ek]->setPen(QPen(fc,2));if(m_arrowItems.contains(ek)){m_arrowItems[ek]->setPen(QPen(fc));m_arrowItems[ek]->setBrush(QBrush(fc));}}
inline void GraphCanvas::flashItems(const QList<_usdint_>& nodeIds,const QList<EdgeKey>& edgeKeys,int durationMs,const QColor& color){
    for(_usdint_ id:nodeIds){if(m_nodeItems.contains(id)){m_nodeItems[id]->setPen(QPen(color,4));m_nodeItems[id]->setBrush(QBrush(color));}}
    for(const EdgeKey& ek:edgeKeys){if(m_edgeItems.contains(ek))m_edgeItems[ek]->setPen(QPen(color,3));if(m_arrowItems.contains(ek)){m_arrowItems[ek]->setPen(QPen(color));m_arrowItems[ek]->setBrush(QBrush(color));}}
    QTimer::singleShot(durationMs,this,[this](){updateSelectionHighlight();clearAlgorithmHighlights();});
}

inline void GraphCanvas::rebuildScene() {
    m_scene->clear(); m_nodeItems.clear(); m_nodeTextItems.clear(); m_edgeItems.clear(); m_arrowItems.clear();
    m_startLabel=m_endLabel=nullptr; hideHoverText();
    if(!m_document||m_document->empty()){if(m_document)m_infoOverlay->refresh(m_document);else{m_infoOverlay->hide();m_minimapOverlay->hide();}if(m_document)m_minimapOverlay->syncWithCanvas(this);return;}
    m_infoOverlay->show();m_minimapOverlay->show();
    _usdint_ nc=m_document->nodeCount(),mid=m_document->maxId(); if(nc==0)return;
    if(!m_document->isLayoutLocked()){
        QPointF vc=mapToScene(viewport()->rect().center());
        if(m_document->hasPosition(1)){
            QPointF d=vc-m_document->nodePosition(1);
            QMap<_usdint_,QPointF> ps=m_document->allNodePositions();
            for(auto it=ps.begin();it!=ps.end();++it){it.value()+=d;m_document->setNodePosition(it.key(),it.value());}
            m_document->setNodePosition(1,vc);
        }
        m_document->m_loIDEAL=m_pIDEAL;m_document->m_loREPULSE=m_pK_REP;m_document->m_loATTRACT=m_pK_UNI*0.5;
        m_document->m_loCENTER=m_pG_UNIV*0.01;m_document->m_loMIN_DIST=m_pMIN_DIST;m_document->m_loOVERLAP=m_pMIN_DIST-10;
        m_document->forceLayout();
    }
    QSet<EdgeKey> drawn;
    for(_usdint_ nid=1;nid<=mid;nid++){if(!m_document->nodeValid(nid))continue;for(_usdint_ tid:m_document->adjacencyFrom(nid)){EdgeKey ek(nid,tid);if(drawn.contains(ek))continue;drawn.insert(ek);if(!m_document->hasPosition(nid)||!m_document->hasPosition(tid))continue;
        QPointF p1=m_document->nodePosition(nid),p2=m_document->nodePosition(tid);bool hr=m_document->edgeValid(tid,nid);double ox=0,oy=0;if(hr){QPointF d=p2-p1;double l=sqrt(d.x()*d.x()+d.y()*d.y());if(l>0){ox=d.y()*BO/l;oy=-d.x()*BO/l;}}
        QPointF s1(p1.x()+ox,p1.y()+oy),s2(p2.x()+ox,p2.y()+oy),dv=s2-s1;double dl=sqrt(dv.x()*dv.x()+dv.y()*dv.y());if(dl>0){QPointF u=dv/dl;s1+=u*NR;s2-=u*NR;}
        QGraphicsLineItem* li=m_scene->addLine(s1.x(),s1.y(),s2.x(),s2.y(),QPen(QColor(DEF_EDGE_COLOR),2));li->setZValue(0);QVariant ekv=QVariant::fromValue(ek);li->setData(0,ekv);m_edgeItems[ek]=li;
        if(dl>0){QPointF u=dv/dl;QPointF lp=s2-u*AL+QPointF(-u.y()*AW,u.x()*AW),rp=s2-u*AL+QPointF(u.y()*AW,-u.x()*AW);QPolygonF arrow;arrow<<s2<<lp<<rp;m_arrowItems[ek]=m_scene->addPolygon(arrow,QPen(QColor(DEF_EDGE_COLOR)),QBrush(QColor(DEF_EDGE_COLOR)));m_arrowItems[ek]->setZValue(0);}}}
    for(_usdint_ i=1;i<=mid;i++){if(!m_document->hasPosition(i))continue;QPointF pos=m_document->nodePosition(i);
        QGraphicsEllipseItem* ell=m_scene->addEllipse(pos.x()-NR,pos.y()-NR,NR*2,NR*2,QPen(QColor(DEF_NODE_BORDER),2),QBrush(QColor(DEF_NODE_FILL)));ell->setZValue(1);ell->setData(0,i);m_nodeItems[i]=ell;
        QGraphicsTextItem* txt=m_scene->addText(QString::number(i),QFont("Arial",10,QFont::Bold));txt->setDefaultTextColor(Qt::white);txt->setPos(pos.x()-txt->boundingRect().width()/2,pos.y()-txt->boundingRect().height()/2);txt->setZValue(2);txt->setParentItem(ell);txt->setData(0,i);m_nodeTextItems[i]=txt;}
    m_scene->setSceneRect(m_scene->itemsBoundingRect().adjusted(-5000,-5000,5000,5000));
    if(!m_document->isLayoutLocked()){fitToContent();if(m_zoomLevel>2.5)setZoomLevel(2.5);if(m_zoomLevel<0.3)setZoomLevel(0.3);}else setZoomLevel(m_zoomLevel);
    updateStartEndLabels();updateSelectionHighlight();m_infoOverlay->refresh(m_document);m_minimapOverlay->syncWithCanvas(this);
    /* 仅加载/修复时预运行物理到稳态，用户增删结点/边时不预运行 */
    if(m_document->m_needsBurstLayout){m_document->m_needsBurstLayout=false;if(!m_document->empty())burstPhysics(1000);}
    startPhysics(true);
}

inline void GraphCanvas::updateSelectionHighlight(){
    for(auto it=m_nodeItems.begin();it!=m_nodeItems.end();++it)it.value()->setPen(QPen(QColor(DEF_EDGE_COLOR),2));
    for(auto it=m_edgeItems.begin();it!=m_edgeItems.end();++it)it.value()->setPen(QPen(QColor(DEF_EDGE_COLOR),2));
    for(auto it=m_arrowItems.begin();it!=m_arrowItems.end();++it){it.value()->setPen(QPen(QColor(0x88,0x88,0x88)));it.value()->setBrush(QBrush(QColor(0x88,0x88,0x88)));}
    if(!m_document)return;
    for(_usdint_ id:m_document->selectedNodeIds()){if(m_nodeItems.contains(id))m_nodeItems[id]->setPen(QPen(QColor(DEF_SELECT_COLOR),DEF_SELECT_WIDTH));}
    for(const EdgeKey& ek:m_document->selectedEdgeKeys()){
        if(m_edgeItems.contains(ek))m_edgeItems[ek]->setPen(QPen(QColor(DEF_SELECT_COLOR),DEF_SELECT_WIDTH));
        if(m_arrowItems.contains(ek)){m_arrowItems[ek]->setPen(QPen(QColor(0x33,0xAA,0xFF)));m_arrowItems[ek]->setBrush(QBrush(QColor(0x33,0xAA,0xFF)));}
    }
}
inline void GraphCanvas::showSelectionInfo(){
    hideHoverText(); m_selectInfoLabel->hide();
    if(!m_document)return;
    auto nids=m_document->selectedNodeIds();
    auto eids=m_document->selectedEdgeKeys();
    QString text;
    if(nids.size()==1){_usdint_ nid=nids[0];if(m_document->nodeValid(nid))text=QString("结点 %1 | 出度:%2 入度:%3").arg(nid).arg(m_document->outDeg(nid)).arg(m_document->inDeg(nid));}
    else if(eids.size()==1){EdgeKey ek=eids[0];if(m_document->edgeValid(ek.first,ek.second))text=QString("%1 → %2 | 权重:%3").arg(ek.first).arg(ek.second).arg(m_document->edgeWeight(ek.first,ek.second));}
    else if(eids.size()>1){text=QString("已选中 %1 条边").arg(eids.size());}
    else if(nids.size()>1){text=QString("已选中 %1 个结点").arg(nids.size());}
    if(text.isEmpty())return;
    m_selectInfoLabel->setText(text); m_selectInfoLabel->adjustSize();
    QRect vr=viewport()->rect();m_selectInfoLabel->move((vr.width()-m_selectInfoLabel->width())/2,vr.bottom()-m_selectInfoLabel->height()-30);
    m_selectInfoLabel->show(); m_selectInfoLabel->raise();
}
inline void GraphCanvas::updateStartEndLabels(){
    if(m_startLabel){m_scene->removeItem(m_startLabel);delete m_startLabel;m_startLabel=nullptr;}
    if(m_endLabel){m_scene->removeItem(m_endLabel);delete m_endLabel;m_endLabel=nullptr;}
    if(!m_document)return;
    _usdint_ sid=m_document->startNode();
    if(sid>0&&m_document->nodeValid(sid)&&m_nodeItems.contains(sid)){
        m_startLabel=new QGraphicsTextItem();
        m_startLabel->setHtml("<div style='background:#1a5c1a;color:#0f0;padding:1px 5px;border-radius:3px;font-size:10px;'>起点</div>");
        m_startLabel->setZValue(5);
        m_scene->addItem(m_startLabel);
        QPointF p=m_document->nodePosition(sid);
        m_startLabel->setPos(p.x()-m_startLabel->boundingRect().width()/2,p.y()-35);
    }
    _usdint_ eid=m_document->endNode();
    if(eid>0&&m_document->nodeValid(eid)&&m_nodeItems.contains(eid)){
        m_endLabel=new QGraphicsTextItem();
        m_endLabel->setHtml("<div style='background:#5c1a1a;color:#f44;padding:1px 5px;border-radius:3px;font-size:10px;'>终点</div>");
        m_endLabel->setZValue(5);
        m_scene->addItem(m_endLabel);
        QPointF p=m_document->nodePosition(eid);
        m_endLabel->setPos(p.x()-m_endLabel->boundingRect().width()/2,p.y()-35);
    }
}
inline void GraphCanvas::updateNodePositions(const QMap<_usdint_,QPointF>& ps){for(auto it=ps.begin();it!=ps.end();++it){if(!m_nodeItems.contains(it.key()))continue;m_nodeItems[it.key()]->setRect(it.value().x()-NR,it.value().y()-NR,NR*2,NR*2);if(m_nodeTextItems.contains(it.key())){QGraphicsTextItem* txt=m_nodeTextItems[it.key()];txt->setPos(it.value().x()-txt->boundingRect().width()/2,it.value().y()-txt->boundingRect().height()/2);}}if(m_startLabel&&m_document){_usdint_ sid=m_document->startNode();if(sid>0&&ps.contains(sid)){QPointF p=ps[sid];m_startLabel->setPos(p.x()-m_startLabel->boundingRect().width()/2,p.y()-35);}}if(m_endLabel&&m_document){_usdint_ eid=m_document->endNode();if(eid>0&&ps.contains(eid)){QPointF p=ps[eid];m_endLabel->setPos(p.x()-m_endLabel->boundingRect().width()/2,p.y()-35);}}for(auto it=m_edgeItems.begin();it!=m_edgeItems.end();++it)updateEdgeLine(it.key().first,it.key().second);if(m_minimapOverlay)m_minimapOverlay->syncWithCanvas(this);}
inline void GraphCanvas::updateEdgeLine(_usdint_ f,_usdint_ t){EdgeKey ek(f,t);if(!m_edgeItems.contains(ek)||!m_document||!m_document->hasPosition(f)||!m_document->hasPosition(t))return;QPointF p1=m_document->nodePosition(f),p2=m_document->nodePosition(t);if(!std::isfinite(p1.x())||!std::isfinite(p1.y())||!std::isfinite(p2.x())||!std::isfinite(p2.y()))return;bool hr=m_document->edgeValid(t,f);double ox=0,oy=0;if(hr){QPointF d=p2-p1;double l=sqrt(d.x()*d.x()+d.y()*d.y());if(l>0){ox=d.y()*BO/l;oy=-d.x()*BO/l;}}QPointF s1(p1.x()+ox,p1.y()+oy),s2(p2.x()+ox,p2.y()+oy),dv=s2-s1;double dl=sqrt(dv.x()*dv.x()+dv.y()*dv.y());if(dl>0){QPointF u=dv/dl;s1+=u*NR;s2-=u*NR;}m_edgeItems[ek]->setLine(s1.x(),s1.y(),s2.x(),s2.y());if(m_arrowItems.contains(ek)&&dl>0){QPointF u=dv/dl;QPointF lp=s2-u*AL+QPointF(-u.y()*AW,u.x()*AW),rp=s2-u*AL+QPointF(u.y()*AW,-u.x()*AW);QPolygonF arrow;arrow<<s2<<lp<<rp;m_arrowItems[ek]->setPolygon(arrow);}}
inline void GraphCanvas::startPhysics(bool layoutPhase){if(!m_physicsActive&&m_document&&!m_document->empty()){m_physicsActive=true;m_physicsLayoutPhase=layoutPhase;m_physicsTimer->start();}}
inline void GraphCanvas::burstPhysics(int maxMs){if(!m_document||m_document->empty())return;
    double gamma=m_physicsLayoutPhase?m_pGAMMA_L:m_pGAMMA_D;
    double stopV=m_physicsLayoutPhase?m_pSTOP_VEL:m_pIDLE_VEL;
    auto startT=std::chrono::steady_clock::now();
    int ticks=0;
    while(ticks<5000){
        auto now=std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(now-startT).count()>=maxMs)break;
        ticks++;
        _usdint_ mid=m_document->maxId();
        QMap<_usdint_,QPointF> acc;for(_usdint_ i=1;i<=mid;i++){if(m_document->nodeValid(i))acc[i]=QPointF(0,0);}
        QMap<_usdint_,int> comp;int compCnt=0;
        for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i)||comp.contains(i))continue;
            QQueue<_usdint_> q;q.enqueue(i);comp[i]=compCnt;
            while(!q.isEmpty()){_usdint_ u=q.dequeue();for(_usdint_ v:m_document->adjacency(u)){if(m_document->nodeValid(v)&&!comp.contains(v)){comp[v]=compCnt;q.enqueue(v);}}}
            compCnt++;}
        for(int c=0;c<compCnt;c++){
            for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i)||comp.value(i)!=c)continue;
                for(_usdint_ j:m_document->adjacency(i)){if(!m_document->nodeValid(j)||comp.value(j)!=c||i>=j)continue;
                    if(i==m_movingNodeId||j==m_movingNodeId)continue;
                    QPointF d=m_document->nodePosition(j)-m_document->nodePosition(i);
                    double ds=sqrt(d.x()*d.x()+d.y()*d.y());if(ds<1.0)ds=1.0;
                    double f=m_document->edgeValid(i,j)&&m_document->edgeValid(j,i)
                        ? m_pK_BI*(ds-m_pIDEAL) : m_pK_UNI*(ds-m_pIDEAL);
                    QPointF u=d/ds;acc[i]+=u*f;acc[j]-=u*f;}
            }}
        for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;QPointF pi=m_document->nodePosition(i);
            for(_usdint_ j=i+1;j<=mid;j++){if(!m_document->nodeValid(j))continue;
                if(m_document->edgeValid(i,j)||m_document->edgeValid(j,i))continue;
                QPointF d=pi-m_document->nodePosition(j);double ds2=d.x()*d.x()+d.y()*d.y();
                if(ds2>m_pCUTOFF*m_pCUTOFF)continue;
                double ds=sqrt(ds2);if(ds<1.0)ds=1.0;
                double f=m_pK_REP/(ds*ds+1.0);QPointF u=d/ds;acc[i]+=u*f;acc[j]-=u*f;}}
        QMap<int,QPointF> compCenter;QMap<int,int> compSize2;
        for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;
            int c=comp.value(i,-1);if(c<0)continue;
            if(!compCenter.contains(c)){compCenter[c]=QPointF(0,0);compSize2[c]=0;}
            compCenter[c]+=m_document->nodePosition(i);compSize2[c]++;}
        for(auto it=compCenter.begin();it!=compCenter.end();++it)it.value()/=compSize2[it.key()];
        QPointF globalCenter2(0,0);int gcnt=0;
        for(_usdint_ i=1;i<=mid;i++){if(m_document->nodeValid(i)){globalCenter2+=m_document->nodePosition(i);gcnt++;}}
        if(gcnt>0)globalCenter2/=gcnt;
        for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;
            int c=comp.value(i,-1);
            QPointF target=(c>=0&&compSize2.value(c,0)>1)?compCenter[c]:globalCenter2;
            QPointF d=target-m_document->nodePosition(i);
            double ds=sqrt(d.x()*d.x()+d.y()*d.y());if(ds<1.0)ds=1.0;
            double kComp=(compSize2.value(c,0)>1)?m_pG_UNIV*0.00004:m_pG_UNIV*0.000002;
            acc[i]+=d/ds*ds*kComp;}
        for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i)||!m_nodeVelocities.contains(i))continue;
            acc[i]-=m_nodeVelocities[i]*gamma;}
        double maxV=0;
        for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i)||i==m_movingNodeId)continue;
            if(!m_nodeVelocities.contains(i))m_nodeVelocities[i]=QPointF(0,0);
            m_nodeVelocities[i]=m_nodeVelocities[i]+acc[i];
            double v=sqrt(m_nodeVelocities[i].x()*m_nodeVelocities[i].x()+m_nodeVelocities[i].y()*m_nodeVelocities[i].y());
            if(v>m_pV_MAX)m_nodeVelocities[i]=m_nodeVelocities[i]*m_pV_MAX/v;
            v=sqrt(m_nodeVelocities[i].x()*m_nodeVelocities[i].x()+m_nodeVelocities[i].y()*m_nodeVelocities[i].y());
            if(v>maxV)maxV=v;
            QPointF np=m_document->nodePosition(i)+m_nodeVelocities[i];
            np.setX(qBound(DEF_PHYSICS_POS_MIN,np.x(),DEF_PHYSICS_POS_MAX));
            np.setY(qBound(DEF_PHYSICS_POS_MIN,np.y(),DEF_PHYSICS_POS_MAX));
            m_document->setNodePosition(i,np);}
        for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;
            QPointF p=m_document->nodePosition(i);
            if(!std::isfinite(p.x())||!std::isfinite(p.y()))m_document->setNodePosition(i,QPointF(0,0));}
        for(int _pc=0;_pc<2;_pc++){for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;QPointF pi=m_document->nodePosition(i);
            for(_usdint_ j=i+1;j<=mid;j++){if(!m_document->nodeValid(j))continue;QPointF pj=m_document->nodePosition(j);
                QPointF d=pi-pj;double ds2=d.x()*d.x()+d.y()*d.y();
                if(ds2<m_pMIN_DIST*m_pMIN_DIST&&ds2>0.01){
                    double ds=sqrt(ds2);QPointF unit=d/ds;double push=(m_pMIN_DIST-ds)/2.0;
                    m_document->setNodePosition(j,pj-unit*push);
                    m_document->setNodePosition(i,pi+unit*push);
                    pi=m_document->nodePosition(i);
                } else if(ds2>m_pMAX_DIST*m_pMAX_DIST&&(m_document->edgeValid(i,j)||m_document->edgeValid(j,i))){
                    double ds=sqrt(ds2);QPointF unit=d/ds;double pull=(ds-m_pMAX_DIST)/2.0;
                    m_document->setNodePosition(j,pj+unit*pull);
                    m_document->setNodePosition(i,pi-unit*pull);
                    pi=m_document->nodePosition(i);}}}}
        if(maxV<stopV)break;}
    if(ticks>0)updateNodePositions(m_document->allNodePositions());
}
inline void GraphCanvas::stopPhysics(){m_physicsActive=false;m_physicsTimer->stop();m_nodeVelocities.clear();}
inline void GraphCanvas::setPhysicsFps(int fps){if(fps<1)fps=1;m_physicsTimer->setInterval(1000/fps);}
inline void GraphCanvas::setRenderFps(int fps){Q_UNUSED(fps);}
inline void GraphCanvas::centerOnGraphCenter(){
    if(!m_document||m_document->empty())return;
    _usdint_ mid=m_document->maxId();double sx=0,sy=0;int cnt=0;
    for(_usdint_ i=1;i<=mid;i++){if(m_document->nodeValid(i)){QPointF p=m_document->nodePosition(i);sx+=p.x();sy+=p.y();cnt++;}}
    if(cnt>0)centerOn(QPointF(sx/cnt,sy/cnt));
}
inline void GraphCanvas::physicsTick(){
    if(m_physicsPaused)return;
    if(!m_document||m_document->empty()){stopPhysics();return;}
    _usdint_ mid=m_document->maxId();
    double gamma=m_physicsLayoutPhase?m_pGAMMA_L:m_pGAMMA_D;
    double stopV=m_physicsLayoutPhase?m_pSTOP_VEL:m_pIDLE_VEL;
    QMap<_usdint_,QPointF> acc;for(_usdint_ i=1;i<=mid;i++){if(m_document->nodeValid(i))acc[i]=QPointF(0,0);}
    /* 连通块内部边弹簧 */
    QMap<_usdint_,int> comp;int compCnt=0;
    for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i)||comp.contains(i))continue;
        QQueue<_usdint_> q;q.enqueue(i);comp[i]=compCnt;
        while(!q.isEmpty()){_usdint_ u=q.dequeue();for(_usdint_ v:m_document->adjacency(u)){if(m_document->nodeValid(v)&&!comp.contains(v)){comp[v]=compCnt;q.enqueue(v);}}}
        compCnt++;}
    for(int c=0;c<compCnt;c++){
        for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i)||comp.value(i)!=c)continue;
            for(_usdint_ j:m_document->adjacency(i)){if(!m_document->nodeValid(j)||comp.value(j)!=c||i>=j)continue;
                if(i==m_movingNodeId||j==m_movingNodeId)continue;
                QPointF d=m_document->nodePosition(j)-m_document->nodePosition(i);
                double ds=sqrt(d.x()*d.x()+d.y()*d.y());if(ds<1.0)ds=1.0;
                double f=m_document->edgeValid(i,j)&&m_document->edgeValid(j,i)
                    ? m_pK_BI*(ds-m_pIDEAL) : m_pK_UNI*(ds-m_pIDEAL);
                QPointF u=d/ds;acc[i]+=u*f;acc[j]-=u*f;}
        }}
    /* 全局排斥（所有非相邻结点对，含跨分量） */
    for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;QPointF pi=m_document->nodePosition(i);
        for(_usdint_ j=i+1;j<=mid;j++){if(!m_document->nodeValid(j))continue;
            if(m_document->edgeValid(i,j)||m_document->edgeValid(j,i))continue;
            QPointF d=pi-m_document->nodePosition(j);double ds2=d.x()*d.x()+d.y()*d.y();
            if(ds2>m_pCUTOFF*m_pCUTOFF)continue;
            double ds=sqrt(ds2);if(ds<1.0)ds=1.0;
            double f=m_pK_REP/(ds*ds+1.0);QPointF u=d/ds;acc[i]+=u*f;acc[j]-=u*f;}}
    /* 分量向心力（替代全局万有引力，避免跨分量结点被拉拢堆叠） */
    /* 计算各分量质心与大小 */
    QMap<int,QPointF> compCenter;QMap<int,int> compSize2;
    for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;
        int c=comp.value(i,-1);if(c<0)continue;
        if(!compCenter.contains(c)){compCenter[c]=QPointF(0,0);compSize2[c]=0;}
        compCenter[c]+=m_document->nodePosition(i);compSize2[c]++;}
    for(auto it=compCenter.begin();it!=compCenter.end();++it)it.value()/=compSize2[it.key()];
    /* 全局质心（孤立结点的弱锚点） */
    QPointF globalCenter2(0,0);int gcnt=0;
    for(_usdint_ i=1;i<=mid;i++){if(m_document->nodeValid(i)){globalCenter2+=m_document->nodePosition(i);gcnt++;}}
    if(gcnt>0)globalCenter2/=gcnt;
    for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;
        int c=comp.value(i,-1);
        /* 多节点分量：用分量质心；孤立结点：用全局质心但系数极弱 */
        QPointF target=(c>=0&&compSize2.value(c,0)>1)?compCenter[c]:globalCenter2;
        QPointF d=target-m_document->nodePosition(i);
        double ds=sqrt(d.x()*d.x()+d.y()*d.y());if(ds<1.0)ds=1.0;
        /* 多节点分量弱向心 (≈0.001)，仅防无限漂移；孤立结点极弱 (≈0.00005)，避免堆叠 */
        double kComp=(compSize2.value(c,0)>1)?m_pG_UNIV*0.00004:m_pG_UNIV*0.000002;
        acc[i]+=d/ds*ds*kComp;}
    /* 全局阻尼 */
    for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i)||!m_nodeVelocities.contains(i))continue;
        acc[i]-=m_nodeVelocities[i]*gamma;}
    /* 速度积分 + 位更新 */
    double maxV=0;
    for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i)||i==m_movingNodeId)continue;
        if(!m_nodeVelocities.contains(i))m_nodeVelocities[i]=QPointF(0,0);
        m_nodeVelocities[i]=m_nodeVelocities[i]+acc[i];
        double v=sqrt(m_nodeVelocities[i].x()*m_nodeVelocities[i].x()+m_nodeVelocities[i].y()*m_nodeVelocities[i].y());
        if(v>m_pV_MAX)m_nodeVelocities[i]=m_nodeVelocities[i]*m_pV_MAX/v;
        v=sqrt(m_nodeVelocities[i].x()*m_nodeVelocities[i].x()+m_nodeVelocities[i].y()*m_nodeVelocities[i].y());
        if(v>maxV)maxV=v;
        QPointF np=m_document->nodePosition(i)+m_nodeVelocities[i];
        np.setX(qBound(DEF_PHYSICS_POS_MIN,np.x(),DEF_PHYSICS_POS_MAX));
        np.setY(qBound(DEF_PHYSICS_POS_MIN,np.y(),DEF_PHYSICS_POS_MAX));
        m_document->setNodePosition(i,np);}
    for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;
        QPointF p=m_document->nodePosition(i);
        if(!std::isfinite(p.x())||!std::isfinite(p.y()))m_document->setNodePosition(i,QPointF(0,0));}
    /* 间距约束 */
    for(int _pc=0;_pc<2;_pc++){for(_usdint_ i=1;i<=mid;i++){if(!m_document->nodeValid(i))continue;QPointF pi=m_document->nodePosition(i);
        for(_usdint_ j=i+1;j<=mid;j++){if(!m_document->nodeValid(j))continue;QPointF pj=m_document->nodePosition(j);
            QPointF d=pi-pj;double ds2=d.x()*d.x()+d.y()*d.y();
            if(ds2<m_pMIN_DIST*m_pMIN_DIST&&ds2>0.01){
                double ds=sqrt(ds2);QPointF unit=d/ds;double push=(m_pMIN_DIST-ds)/2.0;
                m_document->setNodePosition(j,pj-unit*push);
                m_document->setNodePosition(i,pi+unit*push);
                pi=m_document->nodePosition(i);
            } else if(ds2>m_pMAX_DIST*m_pMAX_DIST&&(m_document->edgeValid(i,j)||m_document->edgeValid(j,i))){
                double ds=sqrt(ds2);QPointF unit=d/ds;double pull=(ds-m_pMAX_DIST)/2.0;
                m_document->setNodePosition(j,pj+unit*pull);
                m_document->setNodePosition(i,pi-unit*pull);
                pi=m_document->nodePosition(i);
            }
            }}}
    if(maxV<stopV){stopPhysics();return;}
    updateNodePositions(m_document->allNodePositions());
}
inline QGraphicsItem* GraphCanvas::findItemAt(QPointF sp){if(!m_document)return nullptr;double md=25;_usdint_ cl=0;for(auto it=m_document->allNodePositions().begin();it!=m_document->allNodePositions().end();++it){double d=sqrt((it.value().x()-sp.x())*(it.value().x()-sp.x())+(it.value().y()-sp.y())*(it.value().y()-sp.y()));if(d<md){md=d;cl=it.key();}}if(cl!=0&&m_document->nodeValid(cl)&&m_nodeItems.contains(cl))return m_nodeItems[cl];md=10;for(auto it=m_edgeItems.begin();it!=m_edgeItems.end();++it){QLineF lf(it.value()->line());QPointF d=lf.p2()-lf.p1();double ls=d.x()*d.x()+d.y()*d.y();if(ls<0.001)continue;double t=qBound(0.0,QPointF::dotProduct(sp-lf.p1(),d)/ls,1.0);QPointF pr=lf.p1()+d*t;double d2=sqrt((pr.x()-sp.x())*(pr.x()-sp.x())+(pr.y()-sp.y())*(pr.y()-sp.y()));if(d2<md)return it.value();}return nullptr;}
inline void GraphCanvas::showHoverText(QGraphicsItem* item){if(!item||!m_document)return;hideHoverText();QString text;if(item->type()==QGraphicsEllipseItem::Type){_usdint_ nid=item->data(0).toUInt();if(m_document->nodeValid(nid))text=QString("编号:%1 出度:%2 入度:%3").arg(nid).arg(m_document->outDeg(nid)).arg(m_document->inDeg(nid));}else if(item->type()==QGraphicsLineItem::Type){EdgeKey ek=item->data(0).value<EdgeKey>();if(m_document->edgeValid(ek.first,ek.second))text=QString("%1→%2 权重:%3").arg(ek.first).arg(ek.second).arg(m_document->edgeWeight(ek.first,ek.second));}if(text.isEmpty())return;QGraphicsTextItem* ht=new QGraphicsTextItem();ht->setHtml(QString("<div style='background:#222;color:#eee;padding:2px 6px;border:1px solid #555;border-radius:3px;font-size:11px;white-space:nowrap;'>%1</div>").arg(text));ht->setData(1,"hover");ht->setParentItem(item);ht->setPos(-ht->boundingRect().width()/2,-30);ht->setZValue(5);}
inline void GraphCanvas::hideHoverText(){QList<QGraphicsItem*> del;for(QGraphicsItem* it:m_scene->items()){if(it->data(1).toString()=="hover")del.append(it);}for(QGraphicsItem* it:del)delete it;m_selectInfoLabel->hide();}
inline void GraphCanvas::wheelEvent(QWheelEvent* e){if(e->modifiers()&Qt::ControlModifier){double f=1.15;m_zoomLevel*=(e->angleDelta().y()>0?f:1.0/f);m_zoomLevel=qBound(0.15,m_zoomLevel,4.0);setZoomLevel(m_zoomLevel);emit zoomChanged(m_zoomLevel);}e->ignore();}
inline void GraphCanvas::mousePressEvent(QMouseEvent* e){if(e->button()==Qt::MiddleButton){m_panning=true;m_lastPanPos=e->pos();setCursor(Qt::ClosedHandCursor);e->accept();return;}if(m_interactMode==InteractMode::NodeMoving){if(e->button()==Qt::LeftButton){QPointF sp=mapToScene(e->pos());QGraphicsItem* it=findItemAt(sp);if(it&&it->type()==QGraphicsEllipseItem::Type&&m_document){_usdint_ nid=it->data(0).toUInt();if(m_document->isNodeSelected(nid)){m_movingNodeId=nid;setCursor(Qt::ClosedHandCursor);e->accept();return;}}m_leftDragPan=true;m_leftDragStart=e->pos();e->accept();return;}e->accept();return;}if(e->button()==Qt::LeftButton&&(QApplication::keyboardModifiers()&Qt::ShiftModifier)){QGraphicsItem* it=findItemAt(mapToScene(e->pos()));if(it&&it->type()==QGraphicsEllipseItem::Type&&m_document){_usdint_ src=it->data(0).toUInt();if(m_document->nodeValid(src)){m_edgeDragging=true;m_edgeBidirectional=m_zHeld;m_edgeDragSource=src;QPointF c=m_document->nodePosition(src);m_tempEdgeLine=new QGraphicsLineItem();m_tempEdgeLine->setPen(QPen(m_zHeld?QColor(0x00,0xAA,0xFF):QColor(0x33,0xAA,0xFF),m_zHeld?3:2,Qt::DashLine));m_tempEdgeLine->setLine(c.x(),c.y(),c.x(),c.y());m_tempEdgeLine->setZValue(0);m_scene->addItem(m_tempEdgeLine);e->accept();return;}}}if(e->button()==Qt::LeftButton){QPointF sp=mapToScene(e->pos());QGraphicsItem* it=findItemAt(sp);if(it){if(it->type()==QGraphicsEllipseItem::Type)emit nodeClicked(it->data(0).toUInt(),e->modifiers());else if(it->type()==QGraphicsLineItem::Type)emit edgeClicked(it->data(0).value<EdgeKey>(),e->modifiers());}else{m_leftDragPan=true;m_leftDragStart=e->pos();emit canvasClicked(sp,e->modifiers());}e->accept();return;}QGraphicsView::mousePressEvent(e);}
inline void GraphCanvas::mouseMoveEvent(QMouseEvent* e){if(m_panning||m_leftDragPan){QPoint d=e->pos()-(m_panning?m_lastPanPos:m_leftDragStart);if(m_panning)m_lastPanPos=e->pos();else m_leftDragStart=e->pos();horizontalScrollBar()->setValue(horizontalScrollBar()->value()-d.x());verticalScrollBar()->setValue(verticalScrollBar()->value()-d.y());if(m_minimapOverlay)m_minimapOverlay->syncWithCanvas(this);e->accept();return;}if(m_interactMode==InteractMode::NodeMoving&&m_movingNodeId&&m_document){QPointF sp=mapToScene(e->pos());m_document->setNodePosition(m_movingNodeId,sp);updateNodePositions(m_document->allNodePositions());e->accept();return;}if(m_edgeDragging&&m_document&&m_tempEdgeLine){if(!(QApplication::keyboardModifiers()&Qt::ShiftModifier)){m_scene->removeItem(m_tempEdgeLine);delete m_tempEdgeLine;m_tempEdgeLine=nullptr;m_edgeDragging=false;m_edgeDragSource=0;e->accept();return;}bool biNow=m_zHeld;if(biNow!=m_edgeBidirectional){m_edgeBidirectional=biNow;m_tempEdgeLine->setPen(QPen(biNow?QColor(0x00,0xAA,0xFF):QColor(0x33,0xAA,0xFF),biNow?3:2,Qt::DashLine));}QPointF sp=mapToScene(e->pos());m_tempEdgeLine->setLine(m_document->nodePosition(m_edgeDragSource).x(),m_document->nodePosition(m_edgeDragSource).y(),sp.x(),sp.y());viewport()->update();e->accept();return;}QGraphicsView::mouseMoveEvent(e);}
inline void GraphCanvas::mouseReleaseEvent(QMouseEvent* e){if(m_panning&&e->button()==Qt::MiddleButton){m_panning=false;setCursor(Qt::ArrowCursor);e->accept();return;}if(m_leftDragPan&&e->button()==Qt::LeftButton){m_leftDragPan=false;e->accept();return;}if(m_interactMode==InteractMode::NodeMoving&&e->button()==Qt::LeftButton){m_movingNodeId=0;setCursor(Qt::OpenHandCursor);e->accept();return;}if(m_edgeDragging&&e->button()==Qt::LeftButton){QPointF sp=mapToScene(e->pos());QGraphicsItem* tg=findItemAt(sp);if(m_tempEdgeLine){m_scene->removeItem(m_tempEdgeLine);delete m_tempEdgeLine;m_tempEdgeLine=nullptr;}_usdint_ src=m_edgeDragSource;bool bi=m_edgeBidirectional;m_edgeDragging=false;m_edgeBidirectional=false;m_edgeDragSource=0;if(tg&&tg->type()==QGraphicsEllipseItem::Type&&m_document){_usdint_ tgt=tg->data(0).toUInt();if(m_document->nodeValid(tgt)&&tgt!=src&&!m_document->edgeValid(src,tgt)){emit edgeCreatedByDrag(src,tgt);if(bi&&!m_document->edgeValid(tgt,src))emit edgeCreatedByDrag(tgt,src);}}e->accept();return;}QGraphicsView::mouseReleaseEvent(e);}
inline void GraphCanvas::mouseDoubleClickEvent(QMouseEvent* e){QGraphicsItem* it=itemAt(e->pos());if(!it)emit canvasDoubleClicked();e->accept();}
inline void GraphCanvas::contextMenuEvent(QContextMenuEvent* e){if(m_interactMode==InteractMode::NodeMoving){e->accept();return;}emit canvasRightClicked(mapToScene(e->pos()));e->accept();}
inline void GraphCanvas::keyPressEvent(QKeyEvent* e){switch(e->key()){case DEF_KEY_ALGO_STEP:case DEF_KEY_ALGO_RESTORE:emit keyPressed(e->key());e->accept();return;case DEF_KEY_NEXT_NODE:case DEF_KEY_PREV_NODE:emit keyPressed(e->key());e->accept();return;case DEF_KEY_MOVE_MODE:if(m_interactMode==InteractMode::NodeMoving){m_movingNodeId=0;setInteractMode(InteractMode::Normal);}else{setInteractMode(InteractMode::NodeMoving);}e->accept();return;case DEF_KEY_WASD_A:if(e->modifiers()&Qt::ControlModifier){emit keyPressed(DEF_KEY_WASD_A,Qt::ControlModifier);e->accept();return;}m_wasdDown[1]=true;m_wasdActive=true;if(!m_panTimer->isActive())m_panTimer->start();e->accept();return;case DEF_KEY_WASD_D:if(e->modifiers()&Qt::ControlModifier){emit keyPressed(DEF_KEY_WASD_D,Qt::ControlModifier);e->accept();return;}m_wasdDown[3]=true;m_wasdActive=true;if(!m_panTimer->isActive())m_panTimer->start();e->accept();return;case DEF_KEY_BIDIR_DRAG:m_zHeld=true;e->accept();return;case DEF_KEY_WASD_Q:if(e->modifiers()&Qt::ControlModifier){emit keyPressed(DEF_KEY_WASD_Q,Qt::ControlModifier);e->accept();return;}e->accept();return;
        case DEF_KEY_WASD_W:m_wasdDown[0]=true;m_wasdActive=true;if(!m_panTimer->isActive())m_panTimer->start();e->accept();return;case DEF_KEY_WASD_S:m_wasdDown[2]=true;m_wasdActive=true;if(!m_panTimer->isActive())m_panTimer->start();e->accept();return;default:QGraphicsView::keyPressEvent(e);return;}}
inline void GraphCanvas::keyReleaseEvent(QKeyEvent* e){switch(e->key()){case DEF_KEY_BIDIR_DRAG:m_zHeld=false;break;
        case DEF_KEY_WASD_W:m_wasdDown[0]=false;break;case DEF_KEY_WASD_A:m_wasdDown[1]=false;break;case DEF_KEY_WASD_S:m_wasdDown[2]=false;break;case DEF_KEY_WASD_D:m_wasdDown[3]=false;break;default:QGraphicsView::keyReleaseEvent(e);return;}e->accept();}
inline void GraphCanvas::resizeEvent(QResizeEvent* e){QGraphicsView::resizeEvent(e);if(m_infoOverlay)m_infoOverlay->resetPosition();if(m_minimapOverlay){m_minimapOverlay->resetPosition();m_minimapOverlay->syncWithCanvas(this);}if(m_zoomLabel&&m_zoomLabel->isVisible()){QRect vr=viewport()->rect();m_zoomLabel->move(vr.right()-m_zoomLabel->width()-10,vr.bottom()-m_zoomLabel->height()-10);}if(m_selectInfoLabel&&m_selectInfoLabel->isVisible()){QRect vr=viewport()->rect();m_selectInfoLabel->move((vr.width()-m_selectInfoLabel->width())/2,vr.bottom()-m_selectInfoLabel->height()-30);}}

/* ─── AlgorithmRunner ─── */

inline AlgorithmRunner::AlgorithmRunner(QObject* parent) : QObject(parent), m_doc(nullptr), m_timer(new QTimer(this)), m_currentStep(0), m_totalSteps(0), m_running(false), m_paused(false) { connect(m_timer,&QTimer::timeout,this,&AlgorithmRunner::onTimerTick); }
inline void AlgorithmRunner::stop(){m_timer->stop();m_running=false;m_paused=false;m_currentStep=0;m_totalSteps=0;}
inline void AlgorithmRunner::onTimerTick(){if(m_paused)return;if(m_currentStep>=m_totalSteps){m_timer->stop();m_running=false;emit finished(m_finalPath);return;}const AlgorithmStep& s=m_steps[m_currentStep];emit stepExecuted(m_currentStep+1,m_totalSteps,s.newHighlights,s.previousFades,s.newEdgeHighlights,s.previousEdgeFades);m_currentStep++;}

inline bool AlgorithmRunner::start(Algorithm algo,const GraphDocument* doc,_usdint_ source,_usdint_ target){stop();m_doc=doc;m_algo=algo;m_currentStep=0;m_steps.clear();m_finalPath.clear();if(!m_doc){emit error("文档为空");return false;}switch(algo){case Algorithm::DFS:buildDFSSteps(source);break;case Algorithm::BFS:buildBFSSteps(source);break;case Algorithm::Dijkstra:buildDijkstraSteps(source,target);break;case Algorithm::BellmanFord:buildBellmanFordSteps(source,target);break;}if(m_steps.isEmpty()){emit error("无可执行步骤");return false;}m_totalSteps=m_steps.size();m_running=true;m_paused=false;m_timer->start(1000);return true;}
inline void AlgorithmRunner::buildDFSSteps(_usdint_ source){
    if(!m_doc)return;
    _usdint_ mid=m_doc->maxId();
    QMap<_usdint_,bool> vis;for(_usdint_ i=1;i<=mid;i++)if(m_doc->nodeValid(i))vis[i]=false;
    /* 首步：高亮起点 */
    AlgorithmStep s0;s0.newHighlights.append(source);m_steps.append(s0);vis[source]=true;
    /* 带回溯动画的迭代 DFS */
    struct Frame{ _usdint_ node; int ci; QList<_usdint_> ch; EdgeKey inEdge; };
    QStack<Frame> stk;
    Frame f0;f0.node=source;f0.ci=0;f0.inEdge=EdgeKey(0,0);
    for(_usdint_ v:m_doc->adjacencyFrom(source)){if(m_doc->nodeValid(v))f0.ch.append(v);}
    stk.push(f0);
    while(!stk.isEmpty()){
        Frame& top=stk.top();
        _usdint_ nextChild=0;
        while(top.ci<top.ch.size()&&!nextChild){_usdint_ c=top.ch[top.ci];top.ci++;if(!vis[c])nextChild=c;}
        if(nextChild){
            vis[nextChild]=true;
            /* 前进步 */
            AlgorithmStep fwd;
            fwd.newHighlights.append(nextChild);
            fwd.previousFades.append(top.node);
            EdgeKey ek=m_doc->edgeValid(top.node,nextChild)?EdgeKey(top.node,nextChild)
                     :(m_doc->edgeValid(nextChild,top.node)?EdgeKey(nextChild,top.node):EdgeKey(0,0));
            if(ek.first)fwd.newEdgeHighlights.append(ek);
            m_steps.append(fwd);
            Frame nf;nf.node=nextChild;nf.ci=0;nf.inEdge=ek;
            for(_usdint_ v:m_doc->adjacencyFrom(nextChild)){if(m_doc->nodeValid(v))nf.ch.append(v);}
            stk.push(nf);
        } else {
            /* 回溯步 */
            _usdint_ popped=top.node;EdgeKey poppedEdge=top.inEdge;
            stk.pop();
            if(!stk.isEmpty()){
                Frame& parent=stk.top();
                AlgorithmStep bt;
                bt.previousFades.append(popped);
                bt.newHighlights.append(parent.node);
                if(poppedEdge.first)bt.previousEdgeFades.append(poppedEdge);
                m_steps.append(bt);
            }
        }
    }
}
inline void AlgorithmRunner::buildBFSSteps(_usdint_ source){if(!m_doc)return;QMap<_usdint_,bool> vis;_usdint_ mid=m_doc->maxId();for(_usdint_ i=1;i<=mid;i++){if(m_doc->nodeValid(i))vis[i]=false;}QList<QList<_usdint_>> layers;QQueue<_usdint_> q;q.enqueue(source);vis[source]=true;while(!q.isEmpty()){int sz=q.size();QList<_usdint_> level;for(int i=0;i<sz;i++){_usdint_ u=q.dequeue();level.append(u);for(_usdint_ v:m_doc->adjacencyFrom(u)){if(!vis[v]){vis[v]=true;q.enqueue(v);}}}layers.append(level);}QList<_usdint_> prev;QList<EdgeKey> prevEdges;for(const auto& l:layers){AlgorithmStep s;s.newHighlights=l;s.previousFades=prev;for(_usdint_ u:prev){for(_usdint_ v:l){if(m_doc->edgeValid(u,v)){s.newEdgeHighlights.append(EdgeKey(u,v));if(prevEdges.contains(EdgeKey(u,v)))s.previousEdgeFades.append(EdgeKey(u,v));}}}prevEdges=s.newEdgeHighlights;m_steps.append(s);prev=l;}}
inline void AlgorithmRunner::buildDijkstraSteps(_usdint_ source,_usdint_ target){
    if(!m_doc)return;m_finalPath=m_doc->dijkstra(source,target);if(m_finalPath.empty())return;
    _usdint_ mid=m_doc->maxId();const int INF=1e9;
    QMap<_usdint_,int> dist;QMap<_usdint_,bool> vised;QMap<_usdint_,_usdint_> pre;
    for(_usdint_ i=1;i<=mid;i++){if(m_doc->nodeValid(i)){dist[i]=INF;vised[i]=false;pre[i]=0;}}
    dist[source]=0;QMap<_usdint_,int> pq;pq[source]=0;
    /* 首步：高亮起点 */
    AlgorithmStep s0;s0.newHighlights.append(source);m_steps.append(s0);
    _usdint_ prevNode=source;EdgeKey prevEdge(0,0);
    while(!pq.isEmpty()){
        _usdint_ u=0;int md=INF;
        for(auto it=pq.begin();it!=pq.end();++it){if(it.value()<md){md=it.value();u=it.key();}}
        if(u==0)break;pq.remove(u);if(vised[u])continue;vised[u]=true;
        if(u!=source){
            AlgorithmStep s;
            s.newHighlights.append(u);
            s.previousFades.append(prevNode);
            /* 用实际前驱边而非推测 */
            EdgeKey ek=m_doc->edgeValid(pre[u],u)?EdgeKey(pre[u],u)
                     :(m_doc->edgeValid(u,pre[u])?EdgeKey(u,pre[u]):EdgeKey(0,0));
            if(ek.first)s.newEdgeHighlights.append(ek);
            if(prevEdge.first)s.previousEdgeFades.append(prevEdge);
            prevEdge=ek;prevNode=u;
            m_steps.append(s);
        }
        for(_usdint_ v:m_doc->adjacencyFrom(u)){
            if(!m_doc->nodeValid(v)||vised[v])continue;
            int w=m_doc->edgeWeight(u,v);
            if(dist[u]+w<dist[v]){dist[v]=dist[u]+w;pre[v]=u;pq[v]=dist[v];}
        }
    }
}
inline vector<_usdint_> AlgorithmRunner::externalBellmanFord(_usdint_ source,_usdint_ target) const {if(!m_doc)return{};_usdint_ nc=m_doc->nodeCount(),mid=m_doc->maxId();const int INF=1e9;QMap<_usdint_,int> dist;QMap<_usdint_,_usdint_> pre;for(_usdint_ i=1;i<=mid;i++){if(m_doc->nodeValid(i)){dist[i]=INF;pre[i]=0;}}dist[source]=0;for(_usdint_ iter=0;iter<nc-1;iter++){bool ch=false;for(_usdint_ u=1;u<=mid;u++){if(!m_doc->nodeValid(u)||dist[u]==INF)continue;for(_usdint_ v:m_doc->adjacencyFrom(u)){if(!m_doc->nodeValid(v))continue;int w=m_doc->edgeWeight(u,v);if(dist[u]+w<dist[v]){dist[v]=dist[u]+w;pre[v]=u;ch=true;}}}if(!ch)break;}if(dist[target]==INF)return{};vector<_usdint_> path;_usdint_ cur=target;while(cur!=source){path.push_back(cur);cur=pre[cur];if(cur==0)return{};}path.push_back(source);reverse(path.begin(),path.end());return path;}
inline void AlgorithmRunner::buildBellmanFordSteps(_usdint_ source,_usdint_ target){
    if(!m_doc)return;
    _usdint_ nc=m_doc->nodeCount(),mid=m_doc->maxId();const int INF=1e9;
    QMap<_usdint_,int> dist;QMap<_usdint_,_usdint_> pre;
    for(_usdint_ i=1;i<=mid;i++){if(m_doc->nodeValid(i)){dist[i]=INF;pre[i]=0;}}
    dist[source]=0;
    /* 首步：高亮起点 */
    AlgorithmStep s0;s0.newHighlights.append(source);m_steps.append(s0);
    /* 逐轮松弛：每轮展示被改进距离的结点与边 */
    QList<_usdint_> prevImproved;QList<EdgeKey> prevEdges;
    for(_usdint_ iter=0;iter<nc-1;iter++){
        QList<_usdint_> improved;QList<EdgeKey> improvedEdges;
        for(_usdint_ u=1;u<=mid;u++){
            if(!m_doc->nodeValid(u)||dist[u]==INF)continue;
            for(_usdint_ v:m_doc->adjacencyFrom(u)){
                if(!m_doc->nodeValid(v))continue;
                int w=m_doc->edgeWeight(u,v);
                if(dist[u]+w<dist[v]){
                    dist[v]=dist[u]+w;pre[v]=u;
                    if(!improved.contains(v)){improved.append(v);improvedEdges.append(EdgeKey(u,v));}
                }
            }
        }
        if(improved.isEmpty())break;
        AlgorithmStep s;
        s.newHighlights=improved;
        s.previousFades=prevImproved;
        for(const EdgeKey& ek:improvedEdges)s.newEdgeHighlights.append(ek);
        s.previousEdgeFades=prevEdges;
        m_steps.append(s);
        prevImproved=improved;prevEdges=improvedEdges;
    }
    /* 提取最终路径（由 onAlgorithmFinished 用绿结点+蓝边展示） */
    if(dist[target]==INF)return;
    m_finalPath.clear();_usdint_ cur=target;
    while(cur!=source){m_finalPath.push_back(cur);cur=pre[cur];if(cur==0)return;}
    m_finalPath.push_back(source);
    reverse(m_finalPath.begin(),m_finalPath.end());
}

/* ─── TabBarWidget ─── */

inline TabBarWidget::TabBarWidget(QWidget* parent) : QWidget(parent), m_currentIndex(-1), m_dragIndex(-1), m_dragging(false) {
    setFixedHeight(32); setStyleSheet("background-color:#2b2b2b;");
    m_layout=new QHBoxLayout(this); m_layout->setContentsMargins(4,0,4,0); m_layout->setSpacing(2); m_layout->addStretch();
}inline void TabBarWidget::addTab(const QString& name){int idx=m_buttons.size();QPushButton* btn=new QPushButton(name,this);btn->setCheckable(true);btn->setFixedHeight(26);btn->setMinimumWidth(60);btn->setMaximumWidth(160);btn->setStyleSheet("QPushButton{background-color:#3c3f41;color:#bbb;border:none;padding:2px 8px;border-top-left-radius:4px;border-top-right-radius:4px;}QPushButton:hover{background-color:#4e5254;}QPushButton:checked{background-color:#5294e2;color:#fff;}");connect(btn,&QPushButton::clicked,this,[this,btn](){int i=m_buttons.indexOf(btn);if(i>=0)emit tabClicked(i);});btn->installEventFilter(this);m_buttons.append(btn);m_layout->insertWidget(m_layout->count()-1,btn);
    /* 标签过多时自动缩窄: 估算可用宽度,超过则让所有按钮等分 */
    int avail=width()-8-(m_buttons.size()-1)*2-20;int wPer=qBound(60,avail/m_buttons.size(),160);for(auto* b:m_buttons)b->setMaximumWidth(wPer);}
inline void TabBarWidget::removeTab(int index){if(index<0||index>=m_buttons.size())return;QPushButton* btn=m_buttons[index];m_layout->removeWidget(btn);m_buttons.removeAt(index);delete btn;if(m_buttons.isEmpty())m_currentIndex=-1;else if(index<=m_currentIndex)m_currentIndex=qMax(0,m_currentIndex-1);}
inline void TabBarWidget::renameTab(int index,const QString& name){if(index>=0&&index<m_buttons.size())m_buttons[index]->setText(name);}
inline void TabBarWidget::selectTab(int index){m_currentIndex=index;for(int i=0;i<m_buttons.size();i++)m_buttons[i]->setChecked(i==index);}
inline bool TabBarWidget::eventFilter(QObject* obj,QEvent* event){for(int i=0;i<m_buttons.size();i++){if(obj!=m_buttons[i])continue;if(event->type()==QEvent::MouseButtonPress){QMouseEvent* me=static_cast<QMouseEvent*>(event);if(me->button()==Qt::RightButton){int idx=m_buttons.indexOf(qobject_cast<QPushButton*>(obj));if(idx>=0)emit tabContextMenu(idx,me->globalPosition().toPoint());return true;}if(me->button()==Qt::LeftButton){m_dragIndex=i;m_dragStartPos=me->pos();m_dragging=false;}return false;}if(event->type()==QEvent::MouseButtonDblClick){int idx=m_buttons.indexOf(qobject_cast<QPushButton*>(obj));if(idx>=0)emit tabDoubleClicked(idx);return true;}if(event->type()==QEvent::MouseMove&&m_dragIndex==i){QMouseEvent* me=static_cast<QMouseEvent*>(event);if(!m_dragging&&(me->pos()-m_dragStartPos).manhattanLength()>10)m_dragging=true;if(m_dragging){QPoint mp=m_buttons[i]->mapToParent(me->pos());for(int j=0;j<m_buttons.size();j++){if(j==i)continue;if((mp.x()<m_buttons[j]->geometry().center().x()&&j<i)||(mp.x()>m_buttons[j]->geometry().center().x()&&j>i)){for(int k=0;k<m_buttons.size();k++)m_layout->removeWidget(m_buttons[k]);m_buttons.swapItemsAt(i,j);emit tabsReordered(i,j);m_dragIndex=j;i=j;for(int k=0;k<m_buttons.size();k++)m_layout->insertWidget(k,m_buttons[k]);break;}}}return true;}if(event->type()==QEvent::MouseButtonRelease&&m_dragIndex==i){m_dragIndex=-1;m_dragging=false;return false;}return false;}return QWidget::eventFilter(obj,event);}

/* ─── ContextMenuManager ─── */

inline void ContextMenuManager::showNodeMenu(_usdint_ nodeId,QPoint sp,GraphDocument* doc){QMenu menu(m_parent);QAction* del=menu.addAction("删除");QAction* ss=nullptr;QAction* se=nullptr;if(doc&&doc->startNode()==nodeId){ss=menu.addAction("取消起点");}else{ss=menu.addAction("设为起点");}if(doc&&doc->endNode()==nodeId){se=menu.addAction("取消终点");}else{se=menu.addAction("设为终点");}QAction* mv=menu.addAction("开始移动");QAction* trackAct=menu.addAction("追踪回图中心");QAction* act=menu.exec(sp);if(!act)return;if(act==del)emit deleteSelectedRequested();else if(act==ss){if(doc&&doc->startNode()==nodeId)emit cancelStartNodeRequested();else emit setStartNodeRequested(nodeId);}else if(act==se){if(doc&&doc->endNode()==nodeId)emit cancelEndNodeRequested();else emit setEndNodeRequested(nodeId);}else if(act==mv)emit moveModeRequested();else if(act==trackAct)emit trackCenterRequested();}
inline void ContextMenuManager::showEdgeMenu(EdgeKey ek,QPoint sp){QMenu menu(m_parent);QAction* del=menu.addAction("删除");QAction* ew=menu.addAction("编辑边权重");QAction* trackAct=menu.addAction("追踪回图中心");QAction* act=menu.exec(sp);if(!act)return;if(act==del)emit deleteSelectedRequested();else if(act==ew)emit edgeWeightEditRequested(ek);else if(act==trackAct)emit trackCenterRequested();}
inline void ContextMenuManager::showCanvasMenu(QPointF scenePos,QPoint sp){QMenu menu(m_parent);QAction* addAct=menu.addAction("新增结点");QAction* trackAct=menu.addAction("追踪回图中心");QAction* act=menu.exec(sp);if(!act)return;if(act==addAct)emit addNodeRequested(scenePos);else if(act==trackAct)emit trackCenterRequested();}

/* ─── DebugLogger ─── */

extern std::atomic<bool> DEBUG_MODE;
inline DebugLogger::DebugLogger() : QObject(nullptr), m_flushTimer(new QTimer(this)) { connect(m_flushTimer,&QTimer::timeout,this,&DebugLogger::flush); m_flushTimer->start(3000); }
inline DebugLogger& DebugLogger::instance(){static DebugLogger l;return l;}
inline void DebugLogger::log(const QString& msg){if(!DEBUG_MODE.load())return;QMutexLocker lock(&m_mutex);m_queue.append("["+QDateTime::currentDateTime().toString("hh:mm:ss")+"] "+msg);while(m_queue.size()>500)m_queue.removeFirst();}
inline void DebugLogger::flush(){QMutexLocker lock(&m_mutex);if(m_queue.isEmpty())return;QString date=QDateTime::currentDateTime().toString("yyyy-MM-dd");QDir().mkpath(DEF_PATH_LOGS);QFile f(DEF_PATH_LOGS "/"+date+".txt");bool isNew=!f.exists();if(f.open(QIODevice::Append|QIODevice::WriteOnly)){QTextStream ts(&f);if(isNew)ts<<DEF_LOG_HEADER<<" - "<<date<<"\n";while(!m_queue.isEmpty())ts<<m_queue.takeFirst()<<"\n";f.close();}}
inline void DebugLogger::writeCrashLog(const QString& type,const QString& msg){QString date=QDateTime::currentDateTime().toString("yyyy-MM-dd"),time=QDateTime::currentDateTime().toString("hh:mm:ss");QDir().mkpath(DEF_PATH_LOGS);QFile f(DEF_PATH_LOGS "/"+date+"-crash.txt");if(f.open(QIODevice::Append|QIODevice::WriteOnly)){QTextStream ts(&f);ts<<"=== Graph-Visual 崩溃日志 ===\n日期: "<<date<<" 时间: "<<time<<"\n类型: "<<type<<"\n信息: "<<msg<<"\n\n";f.close();}}
inline void COUT_DEBUG(const QString& msg){DebugLogger::instance().log(msg);}

/* ─── SettingsService ─── */

inline SettingsService::SettingsService(const QString& appDir,QObject* parent) : QObject(parent), m_filePath(appDir+DEF_PATH_SETTINGS) { load(); }
inline void SettingsService::load(){QFile f(m_filePath);if(f.exists()&&f.open(QIODevice::ReadOnly)){QByteArray d=f.readAll();f.close();QJsonDocument doc=QJsonDocument::fromJson(d);if(doc.isObject())m_settings=doc.object();}}
inline void SettingsService::save(){QFile f(m_filePath);if(f.open(QIODevice::WriteOnly)){f.write(QJsonDocument(m_settings).toJson());f.close();}}
inline bool SettingsService::debugMode() const { return m_settings.value("DEBUG_MODE").toBool(true); }
inline void SettingsService::setDebugMode(bool on){m_settings["DEBUG_MODE"]=on;}
inline QRect SettingsService::windowGeometry() const {if(!m_settings.contains("window_geometry")){QScreen* s=QApplication::primaryScreen();if(s){QRect ag=s->availableGeometry();return QRect(ag.left()+ag.width()/8,ag.top()+ag.height()/8,ag.width()*3/4,ag.height()*3/4);}return QRect(100,100,960,720);}QJsonObject geo=m_settings["window_geometry"].toObject();int x=geo["x"].toInt(100),y=geo["y"].toInt(100),w=geo.value("width").toInt(geo.value("w").toInt(640)),h=geo.value("height").toInt(geo.value("h").toInt(480));QScreen* s=QApplication::primaryScreen();if(s){QRect ag=s->availableGeometry();if(w>ag.width()*4/5||h>ag.height()*4/5){w=ag.width()/2;h=ag.height()/2;x=ag.left()+(ag.width()-w)/2;y=ag.top()+(ag.height()-h)/2;}else if(x<ag.left()||y<ag.top()||x+w>ag.right()||y+h>ag.bottom()){w=qMin(w,ag.width());h=qMin(h,ag.height());x=ag.left()+(ag.width()-w)/2;y=ag.top()+(ag.height()-h)/2;}}return QRect(x,y,w,h);}
inline void SettingsService::setWindowGeometry(const QRect& rect){QJsonObject g;g["x"]=rect.x();g["y"]=rect.y();g["width"]=rect.width();g["height"]=rect.height();m_settings["window_geometry"]=g;}
inline int SettingsService::physicsFps() const {return m_settings.value("physics_fps").toInt(DEF_PHYSICS_FPS);}
inline void SettingsService::setPhysicsFps(int fps){m_settings["physics_fps"]=fps;}
inline int SettingsService::renderFps() const {return m_settings.value("render_fps").toInt(DEF_PHYSICS_RENDER_FPS);}
inline void SettingsService::setRenderFps(int fps){m_settings["render_fps"]=fps;}
inline double SettingsService::physicsParam(const QString& key, double def) const {
    QJsonObject po=m_settings.value("physics_params").toObject();
    return po.value(key).toDouble(def);
}
inline void SettingsService::setPhysicsParam(const QString& key, double val){
    QJsonObject po=m_settings.value("physics_params").toObject();
    po[key]=val; m_settings["physics_params"]=po;
}
inline QJsonObject SettingsService::allPhysicsParams() const {
    return m_settings.value("physics_params").toObject();
}
inline void SettingsService::setAllPhysicsParams(const QJsonObject& obj){
    m_settings["physics_params"]=obj;
}inline void SettingsService::ensurePhysicsDefaults(){
    QJsonObject po=m_settings.value("physics_params").toObject();
    bool missing=false;
    auto addIfMissing=[&](const char* k,double d){if(!po.contains(k)){po[k]=d;missing=true;}};
    addIfMissing("K_REP",DEF_PHYSICS_K_REP);addIfMissing("K_UNI",DEF_PHYSICS_K_UNI);addIfMissing("K_BI",DEF_PHYSICS_K_BI);addIfMissing("G_UNIV",DEF_PHYSICS_G_UNIV);
    addIfMissing("GAMMA_L",DEF_PHYSICS_GAMMA_L);addIfMissing("GAMMA_D",DEF_PHYSICS_GAMMA_D);
    addIfMissing("V_MAX",DEF_PHYSICS_V_MAX);addIfMissing("STOP_VEL",DEF_PHYSICS_STOP_VEL);addIfMissing("IDLE_VEL",DEF_PHYSICS_IDLE_VEL);
    addIfMissing("IDEAL",DEF_PHYSICS_IDEAL);addIfMissing("MIN_DIST",DEF_PHYSICS_MIN_DIST);addIfMissing("CUTOFF",DEF_PHYSICS_CUTOFF);
    if(missing){m_settings["physics_params"]=po;save();}
}

/* ─── PhysicsDialog ─── */
inline PhysicsDialog::PhysicsDialog(const QJsonObject& current,QWidget* parent):QDialog(parent){
    setWindowTitle("物理参数设置");setMinimumWidth(550);
    QVBoxLayout* ml=new QVBoxLayout(this);
    QLabel* sec1=new QLabel("<b>间距约束</b>");sec1->setStyleSheet("color:#5294e2;");ml->addWidget(sec1);
    ml->addLayout(makeSliderRow("全局最小间距 MIN_DIST","MIN_DIST",DEF_PHYSICS_MIN_DIST,5,300,1));
    ml->addLayout(makeSliderRow("相邻结点理想间距 IDEAL","IDEAL",DEF_PHYSICS_IDEAL,10,500,1));
    QFrame* line=new QFrame;line->setFrameShape(QFrame::HLine);ml->addWidget(line);
    QLabel* sec2=new QLabel("<b>力学参数</b>");sec2->setStyleSheet("color:#5294e2;");ml->addWidget(sec2);
    ml->addLayout(makeSliderRow("斥力系数 K_REP","K_REP",DEF_PHYSICS_K_REP,100,50000,50));
    ml->addLayout(makeSliderRow("单向边刚度 K_UNI","K_UNI",DEF_PHYSICS_K_UNI,0.001,5.0,0.001));
    ml->addLayout(makeSliderRow("双向边刚度 K_BI","K_BI",DEF_PHYSICS_K_BI,0.001,0.5,0.001));
    ml->addLayout(makeSliderRow("万有引力 G_UNIV","G_UNIV",DEF_PHYSICS_G_UNIV,0.1,200,0.1));
    ml->addLayout(makeSliderRow("布局阻尼 GAMMA_L","GAMMA_L",DEF_PHYSICS_GAMMA_L,0.01,0.99,0.01));
    ml->addLayout(makeSliderRow("动态阻尼 GAMMA_D","GAMMA_D",DEF_PHYSICS_GAMMA_D,0.01,0.99,0.01));
    ml->addLayout(makeSliderRow("最大速度 V_MAX","V_MAX",DEF_PHYSICS_V_MAX,0.1,50.0,0.1));
    ml->addLayout(makeSliderRow("收敛阈值 STOP_VEL","STOP_VEL",DEF_PHYSICS_STOP_VEL,0.01,10.0,0.01));
    ml->addLayout(makeSliderRow("暂停阈值 IDLE_VEL","IDLE_VEL",DEF_PHYSICS_IDLE_VEL,0.001,5.0,0.001));
    ml->addLayout(makeSliderRow("斥力截止 CUTOFF","CUTOFF",DEF_PHYSICS_CUTOFF,50,1000,10));
    for(auto& f:m_fields){
        double v=f.defVal;
        if(current.contains(f.key))v=current[f.key].toDouble(f.defVal);
        int pos=qBound(0,(int)((v-f.min)/f.step+0.5),(int)((f.max-f.min)/f.step));
        f.slider->setValue(pos);f.valLabel->setText(QString::number(v,'f',f.step<1?4:f.step<0.1?2:0));
    }
    /* 按钮 */
    QHBoxLayout* bl=new QHBoxLayout;
    QPushButton* defBt=new QPushButton("恢复默认值");bl->addWidget(defBt);
    bl->addStretch();
    QPushButton* okBt=new QPushButton("应用");bl->addWidget(okBt);
    QPushButton* cancelBt=new QPushButton("取消");bl->addWidget(cancelBt);
    ml->addLayout(bl);
    connect(defBt,&QPushButton::clicked,this,[this](){
        for(auto& f:m_rfields)f.range->setRange(f.defLo,f.defHi);
        for(auto& f:m_fields)f.slider->setValue((int)((f.defVal-f.min)/f.step+0.5));
    });
    connect(okBt,&QPushButton::clicked,this,[this](){accept();});
    connect(cancelBt,&QPushButton::clicked,this,[this](){reject();});
}
inline QHBoxLayout* PhysicsDialog::makeSliderRow(const QString& label,const QString& key,double defVal,double min,double max,double step){
    QHBoxLayout* row=new QHBoxLayout;
    QLabel* lb=new QLabel(label);lb->setFixedWidth(200);row->addWidget(lb);
    int range=(int)((max-min)/step+0.5);
    QSlider* sl=new QSlider(Qt::Horizontal);sl->setRange(0,range);sl->setFixedWidth(200);row->addWidget(sl);
    QLabel* vl=new QLabel;vl->setFixedWidth(80);vl->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    vl->setStyleSheet("QLabel:hover{background:#3c3f41;border-radius:3px;}");
    row->addWidget(vl);
    m_fields.append({sl,vl,min,max,step,defVal,key});
    connect(sl,&QSlider::valueChanged,this,[this,vl,min,step](int pos){
        vl->setText(QString::number(min+pos*step,'f',step<1?4:step<0.1?2:0));
    });
    /* 双击数值标签弹出输入框 */
    vl->installEventFilter(this);
    vl->setProperty("key",key);vl->setProperty("min",min);vl->setProperty("max",max);vl->setProperty("step",step);
    return row;
}
inline QHBoxLayout* PhysicsDialog::makeRangeRow(const QString& label,const QString& keyLo,const QString& keyHi,double defLo,double defHi,double rmin,double rmax){
    QHBoxLayout* row=new QHBoxLayout;
    QLabel* lb=new QLabel(label);lb->setFixedWidth(200);row->addWidget(lb);
    RangeSlider* rs=new RangeSlider(defLo,defHi,rmin,rmax);rs->setFixedWidth(200);row->addWidget(rs);
    QLabel* vl=new QLabel;vl->setFixedWidth(80);vl->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    vl->setStyleSheet("QLabel:hover{background:#3c3f41;border-radius:3px;}");row->addWidget(vl);
    m_rfields.append({rs,vl,rmin,rmax,defLo,defHi,keyLo,keyHi});
    /* 双击数值标签弹出输入框 */
    vl->installEventFilter(this);
    vl->setProperty("rangeLo",rmin);vl->setProperty("rangeHi",rmax);vl->setProperty("keyLo",keyLo);vl->setProperty("keyHi",keyHi);
    return row;
}
inline QJsonObject PhysicsDialog::getParams() const {
    QJsonObject o;
    for(auto& f:m_rfields){o[f.keyLo]=f.range->low();o[f.keyHi]=f.range->high();}
    for(auto& f:m_fields){o[f.key]=f.min+f.slider->value()*f.step;}
    return o;
}
inline bool PhysicsDialog::eventFilter(QObject* obj,QEvent* event){
    if(event->type()==QEvent::MouseButtonDblClick&&obj->isWidgetType()){
        QWidget* w=static_cast<QWidget*>(obj);bool ok=false;
        QString cur=static_cast<QLabel*>(w)->text();
        if(w->property("key").isValid()){
            double min=w->property("min").toDouble(),max=w->property("max").toDouble();
            double v=QInputDialog::getDouble(w->parentWidget(),"输入数值","输入数值",cur.toDouble(),min,max,4,&ok);
            if(ok){v=qBound(min,v,max);for(auto& f:m_fields){if(f.valLabel==w){int pos=(int)((v-f.min)/f.step+0.5);f.slider->setValue(pos);break;}}}
        }else if(w->property("rangeLo").isValid()){
            QString s=QInputDialog::getText(w->parentWidget(),"输入范围","格式: 最小 最大",QLineEdit::Normal,cur,&ok);
            if(ok){QStringList parts=s.simplified().split(' ');if(parts.size()>=2){double lo=parts[0].toDouble(),hi=parts[1].toDouble(),rmin=w->property("rangeLo").toDouble(),rmax=w->property("rangeHi").toDouble();lo=qBound(rmin,lo,hi);hi=qBound(lo,hi,rmax);for(auto& f:m_rfields){if(f.valLabel==w){f.range->setRange(lo,hi);break;}}}}
        }
        return true;
    }
    return QDialog::eventFilter(obj,event);
}
inline QJsonObject PhysicsDialog::defaultParams(){
    QJsonObject o;
    o["K_REP"]=DEF_PHYSICS_K_REP;o["K_UNI"]=DEF_PHYSICS_K_UNI;o["K_BI"]=DEF_PHYSICS_K_BI;o["G_UNIV"]=DEF_PHYSICS_G_UNIV;
    o["GAMMA_L"]=DEF_PHYSICS_GAMMA_L;o["GAMMA_D"]=DEF_PHYSICS_GAMMA_D;
    o["V_MAX"]=DEF_PHYSICS_V_MAX;o["STOP_VEL"]=DEF_PHYSICS_STOP_VEL;o["IDLE_VEL"]=DEF_PHYSICS_IDLE_VEL;
    o["IDEAL"]=DEF_PHYSICS_IDEAL;o["MIN_DIST"]=DEF_PHYSICS_MIN_DIST;o["CUTOFF"]=DEF_PHYSICS_CUTOFF;
    return o;
}

/* ─── GraphFileService ─── */

inline GraphDocument* GraphFileService::loadFromFile(const QString& path,QObject* docParent){QFile f(path);if(!f.open(QIODevice::ReadOnly))return nullptr;QByteArray data=f.readAll();f.close();QJsonParseError pe;QJsonDocument doc=QJsonDocument::fromJson(data,&pe);if(pe.error!=QJsonParseError::NoError||!doc.isObject())return nullptr;QJsonObject root=doc.object();if(!root.contains("graph_name")||!root.contains("node_n")||!root.contains("edge_m")||!root.contains("edges"))return nullptr;int nodeN=root["node_n"].toInt(),edgeM=root["edge_m"].toInt();QJsonArray ea=root["edges"].toArray();vector<indexGraph::ewl> el;for(int i=0;i<ea.size();i++){QJsonObject eo=ea[i].toObject();if(!eo.contains("from")||!eo.contains("to"))return nullptr;_usdint_ f=eo["from"].toInt(),t=eo["to"].toInt();int w=eo["w"].toInt(1);if(f==0||t==0||f>(_usdint_)nodeN||t>(_usdint_)nodeN)return nullptr;el.push_back(indexGraph::ewl(f,t,w));}GraphDocument* gdoc=new GraphDocument(docParent);gdoc->build(nodeN,edgeM,el);QString gn=root["graph_name"].toString();if(gn.isEmpty())gn=QFileInfo(path).completeBaseName();gdoc->setFileName(gn);gdoc->setFilePath(path);return gdoc;}
inline bool GraphFileService::saveToFile(GraphDocument* doc,const QString& path){if(!doc)return false;doc->resort();/* 重排序确保ID连续，使加载时验证通过 */_usdint_ nc=doc->nodeCount(),ec=doc->edgeCount(),mid=doc->maxId(),to=0,ti=0;for(_usdint_ i=1;i<=mid;i++){if(!doc->nodeValid(i))continue;to+=doc->outDeg(i);ti+=doc->inDeg(i);}QJsonObject root;root["graph_name"]=doc->fileName();root["node_n"]=(int)nc;root["edge_m"]=(int)ec;QJsonArray ea;for(_usdint_ nid=1;nid<=mid;nid++){if(!doc->nodeValid(nid))continue;for(_usdint_ tid:doc->adjacencyFrom(nid)){QJsonObject eo;eo["from"]=(int)nid;eo["to"]=(int)tid;eo["w"]=doc->edgeWeight(nid,tid);ea.append(eo);}}root["edges"]=ea;QJsonObject d;d["total_out_degree"]=(int)to;d["total_in_degree"]=(int)ti;d["connected_components"]=(int)doc->areaCount();root["data"]=d;QFile f(path);if(!f.open(QIODevice::WriteOnly|QIODevice::Truncate))return false;f.write(QJsonDocument(root).toJson(QJsonDocument::Compact));f.close();doc->setFilePath(path);doc->setModified(false);return true;}
inline QString GraphFileService::getOpenFileName(QWidget* p,const QString& appDir){return QFileDialog::getOpenFileName(p,"打开图文件",appDir,DEF_FILE_FILTER_ALL);}
inline QString GraphFileService::getSaveFileName(QWidget* p,const QString& appDir,const QString& defaultName){return QFileDialog::getSaveFileName(p,"另存为",appDir+"/"+defaultName+DEF_FILE_EXT,DEF_FILE_FILTER);}
