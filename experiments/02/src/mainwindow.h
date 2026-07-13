#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QTimer>
#include <QList>
#include <QCloseEvent>

#include "components.hpp"
#include "indexGraph.hpp"
#include "./ui_mainwindow.h"

#include <functional>

struct TabCanvasState { double zoomLevel; int scrollX; int scrollY; TabCanvasState():zoomLevel(1.0),scrollX(0),scrollY(0){} };

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent* event) override;
private slots:
    void onNewFile();
    void onOpenFile();
    void onSave();
    void onSaveAs();
    void onCloseTab();
    void onRename();
    void onClearGraph();
    void onClearEdges();
    void onResort();
    void onCompleteGraph();
    void onSelectConnectedComponent();
    void onSelectAll();
    void onCheckValidity();
    void onCheckSelfLoop();
    void onCheckCycle();
    void onCheckIsolated();
    void onCheckReachability();
    void onRunDFS();
    void onRunBFS();
    void onRunDijkstra();
    void onRunBellmanFord();
    void onAlgorithmStep(int step,int total,const QList<_usdint_>& h,const QList<_usdint_>& f,const QList<EdgeKey>& eh,const QList<EdgeKey>& ef);
    void onAlgorithmFinished(const vector<_usdint_>& p);
    void onAlgorithmError(const QString& msg);
    void onAbout();
    void onTutorial();
    void onToggleDebug();
    void onSetPhysicsFps(int fps);
    void onSetRenderFps(int fps);
    void onPhysicsConfig();
    void updateRenderFpsMenu();
    void onTrackCenter();
    void onCanvasNodeClicked(_usdint_ id,Qt::KeyboardModifiers mods);
    void onCanvasEdgeClicked(EdgeKey ek,Qt::KeyboardModifiers mods);
    void onCanvasRightClicked(QPointF sp);
    void onCanvasDoubleClicked();
    void onCanvasKeyPressed(int key, Qt::KeyboardModifiers mods = Qt::NoModifier);
    void onEdgeCreatedByDrag(_usdint_ f,_usdint_ t);
    void onDeleteNode(_usdint_ id);
    void onDeleteEdge(EdgeKey ek);
    void onDeleteSelected();
    void onEditEdgeWeight(EdgeKey ek);
    void onSetStartNode(_usdint_ id);
    void onSetEndNode(_usdint_ id);
    void onCancelStartNode();
    void onCancelEndNode();
    void onMoveModeStart();
    void onNodeMoveFinished();
    void onAddNode(QPointF sp);
    void onTabClicked(int index);
    void onTabDoubleClicked(int index);
    void onTabContextMenu(int index, QPoint globalPos);
    void onTabsReordered(int f,int t);
    void onErrorFlashTick();
private:
    void createMenus();
    void connectSignals();
    void addTab(const QString& name,const QString& path="");
    void removeTab(int index);
    void switchTab(int index);
    GraphDocument* currentDocument() const;
    void showError(const QString& msg);
    void showCheckResult(const QString& msg, bool ok);
    void showStatusProgress(int pct, const QString& label);
    void hideStatusProgress();

    Ui::MainWindow* ui;
    TabBarWidget* m_tabBar;
    GraphCanvas* m_canvas;
    AlgorithmRunner* m_algoRunner;
    ContextMenuManager* m_menuManager;
    SettingsService* m_settings;
    GraphFileService* m_fileService;
    QList<GraphDocument*> m_documents;
    QMap<int,TabCanvasState> m_canvasStates;
    int m_currentDocIndex;
    QString m_appDir;
    QTimer* m_errorFlashTimer;
    int m_errorFlashCount;
    QString m_flashColor;  /* "red" / "green" */
    bool m_algoFinished;
    QActionGroup* m_renderFpsGroup;
    QList<QAction*> m_renderFpsActions;
};

#endif
