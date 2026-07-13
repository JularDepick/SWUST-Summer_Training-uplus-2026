#include "mainwindow.h"
#include <QShortcut>
#include <QActionGroup>
#include "defines.hpp"

std::atomic<bool> DEBUG_MODE(true);

static inline QString sbColor(int r,int g,int b){return QString("#%1%2%3").arg(r,2,16,QChar('0')).arg(g,2,16,QChar('0')).arg(b,2,16,QChar('0'));}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), m_currentDocIndex(-1), m_errorFlashCount(0), m_algoFinished(false), m_renderFpsGroup(nullptr) {
    ui->setupUi(this);
    m_appDir=QCoreApplication::applicationDirPath();
    m_settings=new SettingsService(m_appDir,this);
    m_fileService=new GraphFileService(this);
    m_menuManager=new ContextMenuManager(this);
    m_algoRunner=new AlgorithmRunner(this);
    DEBUG_MODE=m_settings->debugMode();
    QRect geo=m_settings->windowGeometry(); setGeometry(geo);
    setWindowTitle("图可视化 - Graph-Visual");
    m_tabBar=new TabBarWidget(ui->tabBarContainer);
    QHBoxLayout* tbl=new QHBoxLayout(ui->tabBarContainer); tbl->setContentsMargins(0,0,0,0); tbl->addWidget(m_tabBar);
    m_canvas=new GraphCanvas(ui->canvasContainer);
    QVBoxLayout* cl=new QVBoxLayout(ui->canvasContainer); cl->setContentsMargins(0,0,0,0); cl->addWidget(m_canvas);
    m_errorFlashTimer=new QTimer(this); connect(m_errorFlashTimer,&QTimer::timeout,this,&MainWindow::onErrorFlashTick);
    createMenus(); connectSignals();
    m_canvas->setPhysicsFps(m_settings->physicsFps());
    m_settings->ensurePhysicsDefaults();
    m_canvas->loadPhysicsParams(m_settings);
    QShortcut* fsShortcut=new QShortcut(QKeySequence(DEF_SHORTCUT_FULLSCREEN),this);
    connect(fsShortcut,&QShortcut::activated,this,[this](){if(isFullScreen()){showNormal();}else{showFullScreen();}});
    QShortcut* tcShortcut=new QShortcut(QKeySequence(DEF_SHORTCUT_TRACK_CENTER),this);
    connect(tcShortcut,&QShortcut::activated,this,&MainWindow::onTrackCenter);
    if(isFullScreen()||isMaximized())showNormal();
    addTab("未命名");
}

MainWindow::~MainWindow() { for(GraphDocument* d:m_documents)delete d; delete ui; }

/* ─── 私有方法 ─── */

void MainWindow::createMenus() {
    QMenu* fm=menuBar()->addMenu("文件(&F)"); QAction* a;
    a=fm->addAction("新建图文件");a->setShortcut(QKeySequence(DEF_SHORTCUT_NEW_FILE));connect(a,&QAction::triggered,this,&MainWindow::onNewFile);
    a=fm->addAction("打开图文件");a->setShortcut(QKeySequence(DEF_SHORTCUT_OPEN_FILE));connect(a,&QAction::triggered,this,&MainWindow::onOpenFile);
    a=fm->addAction("保存");a->setShortcut(QKeySequence(DEF_SHORTCUT_SAVE));connect(a,&QAction::triggered,this,&MainWindow::onSave);
    a=fm->addAction("另存为");a->setShortcut(QKeySequence(DEF_SHORTCUT_SAVE_AS));connect(a,&QAction::triggered,this,&MainWindow::onSaveAs);
    fm->addSeparator(); a=fm->addAction("重命名");a->setShortcut(QKeySequence(DEF_SHORTCUT_RENAME));connect(a,&QAction::triggered,this,&MainWindow::onRename);
    a=fm->addAction("关闭");a->setShortcut(QKeySequence(DEF_SHORTCUT_CLOSE_TAB));connect(a,&QAction::triggered,this,&MainWindow::onCloseTab);
    QMenu* bm=menuBar()->addMenu("批量操作(&B)");
    a=bm->addAction("选中连通分量");a->setShortcut(QKeySequence(DEF_SHORTCUT_SELECT_COMP));connect(a,&QAction::triggered,this,&MainWindow::onSelectConnectedComponent);
    a=bm->addAction("全选图");a->setShortcut(QKeySequence(DEF_SHORTCUT_SELECT_ALL));connect(a,&QAction::triggered,this,&MainWindow::onSelectAll);
    bm->addSeparator();
    a=bm->addAction("清空图");a->setShortcut(QKeySequence(DEF_SHORTCUT_CLEAR_GRAPH));connect(a,&QAction::triggered,this,&MainWindow::onClearGraph);
    a=bm->addAction("清空边");a->setShortcut(QKeySequence(DEF_SHORTCUT_CLEAR_EDGES));connect(a,&QAction::triggered,this,&MainWindow::onClearEdges);
    a=bm->addAction("重排序修复");a->setShortcut(QKeySequence(DEF_SHORTCUT_RESORT));connect(a,&QAction::triggered,this,&MainWindow::onResort);
    a=bm->addAction("补全为完全图");connect(a,&QAction::triggered,this,&MainWindow::onCompleteGraph);
    QMenu* cm=menuBar()->addMenu("检查(&C)");
    a=cm->addAction("检查图结构合法性");connect(a,&QAction::triggered,this,&MainWindow::onCheckValidity);
    a=cm->addAction("检测自环");connect(a,&QAction::triggered,this,&MainWindow::onCheckSelfLoop);
    a=cm->addAction("检测环");connect(a,&QAction::triggered,this,&MainWindow::onCheckCycle);
    a=cm->addAction("检测孤立结点");connect(a,&QAction::triggered,this,&MainWindow::onCheckIsolated);
    a=cm->addAction("检测结点可达性");connect(a,&QAction::triggered,this,&MainWindow::onCheckReachability);
    QMenu* am=menuBar()->addMenu("算法演示(&A)");
    a=am->addAction("DFS遍历演示");a->setShortcut(QKeySequence(DEF_SHORTCUT_ALGO_DFS));connect(a,&QAction::triggered,this,&MainWindow::onRunDFS);
    a=am->addAction("BFS遍历演示");a->setShortcut(QKeySequence(DEF_SHORTCUT_ALGO_BFS));connect(a,&QAction::triggered,this,&MainWindow::onRunBFS);
    a=am->addAction("Dijkstra单源最短路径演示");a->setShortcut(QKeySequence(DEF_SHORTCUT_ALGO_DIJKSTRA));connect(a,&QAction::triggered,this,&MainWindow::onRunDijkstra);
    a=am->addAction("Bellman-Ford单源最短路径演示");a->setShortcut(QKeySequence(DEF_SHORTCUT_ALGO_BF));connect(a,&QAction::triggered,this,&MainWindow::onRunBellmanFord);
    QMenu* pm=menuBar()->addMenu("性能(&P)");
    QMenu* pfm=pm->addMenu("物理计算帧率");
    QActionGroup* fpsGroup=new QActionGroup(pfm);fpsGroup->setExclusive(true);
    int curPhysFps=m_settings->physicsFps();
    int fpsValues[]={20,30,45,60,75,90,120};
    for(int fv:fpsValues){QAction* fa=pfm->addAction(QString("%1 FPS").arg(fv));fa->setCheckable(true);fa->setData(fv);fpsGroup->addAction(fa);if(fv==curPhysFps)fa->setChecked(true);connect(fa,&QAction::triggered,this,[this,fv](){onSetPhysicsFps(fv);});}
    QMenu* rfm=pm->addMenu("画面渲染帧率");
    m_renderFpsGroup=new QActionGroup(rfm);m_renderFpsGroup->setExclusive(true);
    int curRndFps=m_settings->renderFps();
    for(int fv:fpsValues){QAction* fa=rfm->addAction(QString("%1 FPS").arg(fv));fa->setCheckable(true);fa->setData(fv);m_renderFpsGroup->addAction(fa);m_renderFpsActions.append(fa);if(fv==curRndFps)fa->setChecked(true);connect(fa,&QAction::triggered,this,[this,fv](){onSetRenderFps(fv);});}
    updateRenderFpsMenu();
    a=menuBar()->addAction("物理(&Y)");connect(a,&QAction::triggered,this,&MainWindow::onPhysicsConfig);
    QMenu* hm=menuBar()->addMenu("帮助(&H)");
    a=hm->addAction("关于");a->setShortcut(QKeySequence(DEF_SHORTCUT_ABOUT));connect(a,&QAction::triggered,this,&MainWindow::onAbout);
    a=hm->addAction("操作教程");a->setShortcut(QKeySequence(DEF_SHORTCUT_TUTORIAL));connect(a,&QAction::triggered,this,&MainWindow::onTutorial);
    QAction* da=hm->addAction("调试模式");da->setShortcut(QKeySequence(DEF_SHORTCUT_DEBUG));da->setCheckable(true);da->setChecked(DEBUG_MODE.load());connect(da,&QAction::triggered,this,&MainWindow::onToggleDebug);
}

void MainWindow::connectSignals() {
    connect(m_canvas,&GraphCanvas::nodeClicked,this,&MainWindow::onCanvasNodeClicked);
    connect(m_canvas,&GraphCanvas::edgeClicked,this,&MainWindow::onCanvasEdgeClicked);
    connect(m_canvas,&GraphCanvas::canvasRightClicked,this,&MainWindow::onCanvasRightClicked);
    connect(m_canvas,&GraphCanvas::canvasDoubleClicked,this,&MainWindow::onCanvasDoubleClicked);
    connect(m_canvas,&GraphCanvas::edgeCreatedByDrag,this,&MainWindow::onEdgeCreatedByDrag);
    connect(m_canvas,&GraphCanvas::nodeMoveFinished,this,&MainWindow::onNodeMoveFinished);
    connect(m_canvas,&GraphCanvas::physicsToggled,this,[this](bool paused){statusBar()->showMessage(paused?"物理模拟已暂停":"物理模拟已恢复",2000);});
    connect(m_canvas,&GraphCanvas::keyPressed,this,&MainWindow::onCanvasKeyPressed);
    connect(m_menuManager,&ContextMenuManager::deleteNodeRequested,this,&MainWindow::onDeleteNode);
    connect(m_menuManager,&ContextMenuManager::deleteEdgeRequested,this,&MainWindow::onDeleteEdge);
    connect(m_menuManager,&ContextMenuManager::deleteSelectedRequested,this,&MainWindow::onDeleteSelected);
    connect(m_menuManager,&ContextMenuManager::edgeWeightEditRequested,this,&MainWindow::onEditEdgeWeight);
    connect(m_menuManager,&ContextMenuManager::setStartNodeRequested,this,&MainWindow::onSetStartNode);
    connect(m_menuManager,&ContextMenuManager::setEndNodeRequested,this,&MainWindow::onSetEndNode);
    connect(m_menuManager,&ContextMenuManager::cancelStartNodeRequested,this,&MainWindow::onCancelStartNode);
    connect(m_menuManager,&ContextMenuManager::cancelEndNodeRequested,this,&MainWindow::onCancelEndNode);
    connect(m_menuManager,&ContextMenuManager::moveModeRequested,this,&MainWindow::onMoveModeStart);
    connect(m_menuManager,&ContextMenuManager::addNodeRequested,this,&MainWindow::onAddNode);
    connect(m_menuManager,&ContextMenuManager::trackCenterRequested,this,&MainWindow::onTrackCenter);
    connect(m_algoRunner,&AlgorithmRunner::stepExecuted,this,&MainWindow::onAlgorithmStep);
    connect(m_algoRunner,&AlgorithmRunner::finished,this,&MainWindow::onAlgorithmFinished);
    connect(m_algoRunner,&AlgorithmRunner::error,this,&MainWindow::onAlgorithmError);
    connect(m_tabBar,&TabBarWidget::tabClicked,this,&MainWindow::onTabClicked);
    connect(m_tabBar,&TabBarWidget::tabDoubleClicked,this,&MainWindow::onTabDoubleClicked);
    connect(m_tabBar,&TabBarWidget::tabContextMenu,this,&MainWindow::onTabContextMenu);
    connect(m_tabBar,&TabBarWidget::tabsReordered,this,&MainWindow::onTabsReordered);
}

GraphDocument* MainWindow::currentDocument() const { return (m_currentDocIndex<0||m_currentDocIndex>=m_documents.size())?nullptr:m_documents[m_currentDocIndex]; }

void MainWindow::addTab(const QString& name,const QString& path) {
    QString n=name;if(name=="未命名"){int idx=1;for(GraphDocument* d:m_documents){if(d->fileName().startsWith("未命名"))idx++;}if(idx>1)n=QString("未命名 %1").arg(idx);}
    GraphDocument* doc=new GraphDocument(this); doc->setFileName(n); doc->setFilePath(path);
    connect(doc,&GraphDocument::layoutProgress,this,[this](int pct){if(pct<100)showStatusProgress(pct,"正在布局...");else hideStatusProgress();});
    m_documents.append(doc); m_tabBar->addTab(n); switchTab(m_documents.size()-1);
}

void MainWindow::removeTab(int index) {
    if(index<0||index>=m_documents.size())return; delete m_documents[index]; m_documents.removeAt(index); m_tabBar->removeTab(index);
    m_canvasStates.remove(index);
    if(m_documents.isEmpty()){m_currentDocIndex=-1;m_canvas->setDocument(nullptr);}else{if(index<=m_currentDocIndex)m_currentDocIndex=qMax(0,m_currentDocIndex-1);switchTab(m_currentDocIndex);}
}

void MainWindow::switchTab(int index) {
    if(index<0||index>=m_documents.size()||index==m_currentDocIndex)return;
    if(m_currentDocIndex>=0){
        TabCanvasState s;s.zoomLevel=m_canvas->zoomLevel();
        s.scrollX=m_canvas->horizontalScrollBar()->value();s.scrollY=m_canvas->verticalScrollBar()->value();
        m_canvasStates[m_currentDocIndex]=s;}
    m_currentDocIndex=index; m_tabBar->selectTab(index);
    GraphDocument* doc=m_documents[index]; m_canvas->setDocument(doc); m_canvas->rebuildScene();
    if(m_canvasStates.contains(index)){
        TabCanvasState& s=m_canvasStates[index];m_canvas->setZoomLevel(s.zoomLevel);
        m_canvas->horizontalScrollBar()->setValue(s.scrollX);m_canvas->verticalScrollBar()->setValue(s.scrollY);}
    m_canvas->infoOverlay()->refresh(doc); m_canvas->minimapOverlay()->syncWithCanvas(m_canvas);
}

void MainWindow::showStatusProgress(int pct,const QString& label){pct=qBound(0,pct,100);double f=pct/100.0;statusBar()->setStyleSheet(QString("QStatusBar{border-top:4px solid;border-top-color:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #5294e2,stop:%1 #5294e2,stop:%2 #333,stop:1 #333);color:%3;}").arg(f).arg(f+0.001).arg(sbColor(DEF_SBAR_PROGRESS)));statusBar()->showMessage(label);}
void MainWindow::hideStatusProgress(){statusBar()->setStyleSheet("");statusBar()->clearMessage();}
void MainWindow::showError(const QString& msg) {
    statusBar()->showMessage(msg,5000); statusBar()->setStyleSheet(QString("QStatusBar{color:%1;}").arg(sbColor(DEF_SBAR_ERROR)));
    QTimer::singleShot(5000,[this](){statusBar()->setStyleSheet("");});
    m_flashColor="red";m_errorFlashCount=0; m_errorFlashTimer->start(200); QApplication::beep();
}
void MainWindow::showCheckResult(const QString& msg,bool ok){
    statusBar()->showMessage(msg,4000);statusBar()->setStyleSheet(QString("QStatusBar{color:%1;}").arg(ok?sbColor(DEF_SBAR_SUCCESS):sbColor(DEF_SBAR_ERROR)));
    QTimer::singleShot(4000,[this](){statusBar()->setStyleSheet("");});
    m_flashColor=ok?"green":"red";m_errorFlashCount=0;m_errorFlashTimer->start(200);
}

/* ─── 文件 ─── */

void MainWindow::onNewFile() { addTab("未命名"); COUT_DEBUG("新建"); }

void MainWindow::onOpenFile() {
    QString fp=m_fileService->getOpenFileName(this,m_appDir); if(fp.isEmpty())return;
    for(int i=0;i<m_documents.size();i++){if(m_documents[i]->filePath()==fp){switchTab(i);return;}}
    GraphDocument* doc=m_fileService->loadFromFile(fp,this);
    if(!doc){QMessageBox mb(this);mb.setIcon(QMessageBox::Critical);mb.setWindowTitle("加载失败");mb.setText("JSON解析错误或字段缺失");mb.addButton("确定",QMessageBox::AcceptRole);mb.exec();statusBar()->showMessage("加载失败: "+fp,5000);return;}
    connect(doc,&GraphDocument::layoutProgress,this,[this](int pct){if(pct<100)showStatusProgress(pct,"正在布局...");else hideStatusProgress();});
    m_documents.append(doc); m_tabBar->addTab(doc->fileName());
    if(doc->hasNegativeEdge())statusBar()->showMessage("⚠ 图中存在负权边，Dijkstra算法将不可用",5000);
    switchTab(m_documents.size()-1); COUT_DEBUG("打开:"+doc->fileName()); statusBar()->showMessage("已加载: "+doc->fileName(),3000);
}

void MainWindow::onSave() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    if(doc->filePath().isEmpty()){onSaveAs();return;}
    if(m_fileService->saveToFile(doc,doc->filePath())){m_tabBar->renameTab(m_currentDocIndex,doc->fileName());statusBar()->showMessage("已保存: "+doc->filePath(),3000);COUT_DEBUG("保存:"+doc->filePath());}
    else{QMessageBox mb(this);mb.setIcon(QMessageBox::Critical);mb.setWindowTitle("保存失败");mb.setText("无法写入文件");mb.addButton("确定",QMessageBox::AcceptRole);mb.exec();}
}

void MainWindow::onSaveAs() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    QString fp=m_fileService->getSaveFileName(this,m_appDir,doc->fileName()); if(fp.isEmpty())return;
    doc->setFileName(QFileInfo(fp).completeBaseName());
    if(m_fileService->saveToFile(doc,fp)){m_tabBar->renameTab(m_currentDocIndex,doc->fileName());statusBar()->showMessage("已保存: "+fp,3000);COUT_DEBUG("另存为:"+fp);}
    else{QMessageBox mb(this);mb.setIcon(QMessageBox::Critical);mb.setWindowTitle("保存失败");mb.setText("无法写入文件");mb.addButton("确定",QMessageBox::AcceptRole);mb.exec();}
}

void MainWindow::onRename() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    bool ok;QString name=QInputDialog::getText(this,"重命名",QString("图名称(当前:%1):").arg(doc->fileName()),QLineEdit::Normal,doc->fileName(),&ok);
    if(ok&&!name.isEmpty()){doc->setFileName(name);m_tabBar->renameTab(m_currentDocIndex,name);statusBar()->showMessage(QString("已重命名:%1").arg(name),2000);}
}
void MainWindow::onCloseTab() {
    int idx=m_currentDocIndex; if(idx<0)return; GraphDocument* doc=m_documents[idx];
    if(doc->isModified()){QMessageBox mb(this);mb.setIcon(QMessageBox::Question);mb.setWindowTitle("未保存");mb.setText("是否保存？");
        QPushButton* saveBtn=mb.addButton("保存",QMessageBox::AcceptRole);mb.addButton("不保存",QMessageBox::RejectRole);QPushButton* cancelBtn=mb.addButton("取消",QMessageBox::RejectRole);
        mb.exec();if(mb.clickedButton()==cancelBtn)return;if(mb.clickedButton()==saveBtn)onSave();}
    COUT_DEBUG("关闭标签:"+doc->fileName()); removeTab(idx);
}

/* ─── 批量 ─── */

void MainWindow::onClearGraph() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    QMessageBox mb(this);mb.setIcon(QMessageBox::Question);mb.setWindowTitle("确认");mb.setText("确定要清空整个图吗？");
    QPushButton* yesBtn=mb.addButton("是",QMessageBox::AcceptRole);mb.addButton("否",QMessageBox::RejectRole);mb.exec();
    if(mb.clickedButton()!=yesBtn)return;
    doc->clear(); m_canvas->rebuildScene(); COUT_DEBUG("清空图");
}

void MainWindow::onClearEdges() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    QMessageBox mb(this);mb.setIcon(QMessageBox::Question);mb.setWindowTitle("确认");mb.setText("确定要清空所有边吗？");
    QPushButton* yesBtn=mb.addButton("是",QMessageBox::AcceptRole);mb.addButton("否",QMessageBox::RejectRole);mb.exec();
    if(mb.clickedButton()!=yesBtn)return;
    doc->clearAllEdges(); m_canvas->rebuildScene(); COUT_DEBUG("清空边");
}

void MainWindow::onResort() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    doc->resort(); m_canvas->rebuildScene(); statusBar()->showMessage("重排序后起终点已重置",3000); COUT_DEBUG("重排序修复");
}
void MainWindow::onCompleteGraph() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    doc->completeToCompleteGraph(); m_canvas->rebuildScene();
    statusBar()->showMessage("已补全为完全图",3000); COUT_DEBUG("补全完全图");
}

void MainWindow::onSelectConnectedComponent() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    auto nids=doc->selectedNodeIds(); auto eids=doc->selectedEdgeKeys();
    if(nids.isEmpty()&&eids.isEmpty()){showError("请先选中一个结点或一条边");return;}
    _usdint_ startNode=0;
    if(!nids.isEmpty())startNode=nids[0];
    else startNode=eids[0].first;
    QList<_usdint_> component;
    QMap<_usdint_,bool> visited;
    QList<_usdint_> queue; queue.append(startNode); visited[startNode]=true;
    while(!queue.isEmpty()){_usdint_ u=queue.takeFirst(); component.append(u);
        for(_usdint_ v:doc->adjacency(u)){if(!visited.contains(v)){visited[v]=true;queue.append(v);}}}
    doc->clearSelection();
    for(_usdint_ id:component) doc->selectNode(id,true);
    statusBar()->showMessage(QString("已选中连通分量: %1 个结点").arg(component.size()),3000);
    COUT_DEBUG("选中连通分量:"+QString::number(component.size()));
}

void MainWindow::onSelectAll() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    doc->clearSelection();
    _usdint_ mid=doc->maxId();
    for(_usdint_ i=1;i<=mid;i++){if(doc->nodeValid(i))doc->selectNode(i,true);}
    for(_usdint_ i=1;i<=mid;i++){if(!doc->nodeValid(i))continue;
        for(_usdint_ j:doc->adjacencyFrom(i)){if(doc->nodeValid(j))doc->selectEdge(EdgeKey(i,j),true);}}
    statusBar()->showMessage("已全选",3000);
    COUT_DEBUG("全选图");
}

/* ─── 检查 ─── */

void MainWindow::onCheckValidity() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    _usdint_ mid=doc->maxId(); QList<_usdint_> badNodes; QList<EdgeKey> badEdges; QStringList msgs;
    for(_usdint_ i=1;i<=mid;i++){if(!doc->nodeValid(i)){badNodes.append(i);msgs.append(QString("缺失结点:%1").arg(i));}}
    for(_usdint_ i=1;i<=mid;i++){if(!doc->nodeValid(i))continue;for(_usdint_ j:doc->adjacencyFrom(i)){if(!doc->nodeValid(j)){badEdges.append(EdgeKey(i,j));msgs.append(QString("边%1→%2指向无效").arg(i).arg(j));}}}
    if(msgs.isEmpty()){showCheckResult("图结构合法 ✓",true);}
    else{m_canvas->flashItems(badNodes,badEdges,4000);showCheckResult(msgs.join("; "),false);}
    COUT_DEBUG("检查合法性");
}

void MainWindow::onCheckSelfLoop() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    _usdint_ mid=doc->maxId(); QList<_usdint_> loopNodes; QList<EdgeKey> loopEdges;
    for(_usdint_ i=1;i<=mid;i++){if(doc->nodeValid(i)&&doc->edgeValid(i,i)){loopNodes.append(i);loopEdges.append(EdgeKey(i,i));}}
    if(loopNodes.isEmpty()){showCheckResult("无自环 ✓",true);}
    else{m_canvas->flashItems(loopNodes,loopEdges,4000);showCheckResult(QString("发现 %1 个自环").arg(loopNodes.size()),false);}
    COUT_DEBUG("检测自环");
}

void MainWindow::onCheckCycle() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    _usdint_ mid=doc->maxId(); QList<_usdint_> cycleNodes; QList<EdgeKey> cycleEdges;
    QMap<_usdint_,int> color; for(_usdint_ i=1;i<=mid;i++)if(doc->nodeValid(i))color[i]=0;
    QMap<_usdint_,_usdint_> parent; bool found=false;
    function<void(_usdint_)> dfs=[&](_usdint_ u){if(found)return;color[u]=1;
        for(_usdint_ v:doc->adjacencyFrom(u)){if(!doc->nodeValid(v))continue;
            if(color[v]==0){parent[v]=u;dfs(v);if(found)return;}
            else if(color[v]==1){found=true;
                _usdint_ cur=u;cycleNodes.append(v);cycleEdges.append(EdgeKey(u,v));
                while(cur!=v){cycleNodes.prepend(cur);cycleEdges.prepend(EdgeKey(parent[cur],cur));cur=parent[cur];}
                return;}}color[u]=2;};
    for(_usdint_ i=1;i<=mid;i++)if(doc->nodeValid(i)&&color[i]==0){dfs(i);if(found)break;}
    if(!found){showCheckResult("无环 \u2713",true);}
    else{m_canvas->flashItems(cycleNodes,cycleEdges,4000);showCheckResult("检测到环——已高亮参与环的结点和边",false);}
    COUT_DEBUG("检测环");
}

void MainWindow::onCheckIsolated() {
    GraphDocument* doc=currentDocument(); if(!doc)return; QList<_usdint_> iso;
    _usdint_ mid=doc->maxId();
    for(_usdint_ i=1;i<=mid;i++)if(doc->nodeValid(i)&&doc->outDeg(i)==0&&doc->inDeg(i)==0)iso.append(i);
    if(iso.isEmpty()){showCheckResult("无孤立结点 ✓",true);}
    else{m_canvas->flashItems(iso,{},4000);QString s;for(auto i:iso)s+=QString::number(i)+" ";showCheckResult(QString("孤立结点(%1个): %2").arg(iso.size()).arg(s),false);}
    COUT_DEBUG("检测孤立结点");
}

void MainWindow::onCheckReachability() {
    GraphDocument* doc=currentDocument(); if(!doc)return;
    auto sids=doc->selectedNodeIds(); if(sids.size()!=2||sids[0]==sids[1]){showError("可达性:请选中两个不同结点");return;}
    _usdint_ a=sids[0],b=sids[1];
    auto pathFwd=doc->dijkstra(a,b);
    if(doc->hasNegativeEdge()){
        const int INF=1e9;_usdint_ nc=doc->nodeCount(),mid=doc->maxId();
        QMap<_usdint_,int> dist;QMap<_usdint_,_usdint_> pre;
        for(_usdint_ i=1;i<=mid;i++){if(doc->nodeValid(i)){dist[i]=INF;pre[i]=0;}}dist[a]=0;
        for(_usdint_ iter=0;iter<nc-1;iter++){bool ch=false;
            for(_usdint_ u=1;u<=mid;u++){if(!doc->nodeValid(u)||dist[u]==INF)continue;
                for(_usdint_ v:doc->adjacencyFrom(u)){if(!doc->nodeValid(v))continue;int w=doc->edgeWeight(u,v);
                    if(dist[u]+w<dist[v]){dist[v]=dist[u]+w;pre[v]=u;ch=true;}}}if(!ch)break;}
        if(dist[b]!=INF){pathFwd.clear();_usdint_ cur=b;while(cur!=a){pathFwd.push_back(cur);cur=pre[cur];}pathFwd.push_back(a);reverse(pathFwd.begin(),pathFwd.end());}
    }
    auto bfsRev=doc->BFS(b);bool revReach=false;
    for(_usdint_ id:bfsRev){if(id==a){revReach=true;break;}}
    bool fwdOk=!pathFwd.empty();
    if(!fwdOk&&!revReach){showCheckResult(QString("%1 与 %2 互相不可达").arg(a).arg(b),false);}
    else if(fwdOk&&!revReach){
        QList<EdgeKey> pathEdges;for(size_t i=0;i<pathFwd.size()-1;i++)pathEdges.append(EdgeKey(pathFwd[i],pathFwd[i+1]));
        QList<_usdint_> pn;for(auto id:pathFwd)pn.append(id);
        m_canvas->flashItems(pn,pathEdges,4000);showCheckResult(QString("%1\u2192%2 单向可达(最短%3步)").arg(a).arg(b).arg(pn.size()-1),true);
    }else if(!fwdOk&&revReach){
        showCheckResult(QString("%1\u2190%2 反向单向可达").arg(a).arg(b),true);
    }else{
        QList<EdgeKey> pathEdges;for(size_t i=0;i<pathFwd.size()-1;i++)pathEdges.append(EdgeKey(pathFwd[i],pathFwd[i+1]));
        QList<_usdint_> pn;for(auto id:pathFwd)pn.append(id);
        m_canvas->flashItems(pn,pathEdges,4000);showCheckResult(QString("%1\u2194%2 双向可达(最短%3步)").arg(a).arg(b).arg(pn.size()-1),true);
    }
    COUT_DEBUG("检测可达性");
}

void MainWindow::onRunDFS() {m_algoFinished=false;
    GraphDocument* doc=currentDocument(); if(!doc)return; auto sids=doc->selectedNodeIds(); if(sids.size()!=1){showError("DFS:请选中唯一一个结点");return;}
    m_canvas->clearAlgorithmHighlights(); m_algoRunner->start(AlgorithmRunner::Algorithm::DFS,doc,sids[0]); COUT_DEBUG("DFS演示:"+QString::number(sids[0]));
}

void MainWindow::onRunBFS() {m_algoFinished=false;
    GraphDocument* doc=currentDocument(); if(!doc)return; auto sids=doc->selectedNodeIds(); if(sids.size()!=1){showError("BFS:请选中唯一一个结点");return;}
    m_canvas->clearAlgorithmHighlights(); m_algoRunner->start(AlgorithmRunner::Algorithm::BFS,doc,sids[0]); COUT_DEBUG("BFS演示:"+QString::number(sids[0]));
}

void MainWindow::onRunDijkstra() {m_algoFinished=false;
    GraphDocument* doc=currentDocument(); if(!doc)return;
    _usdint_ s=doc->startNode(),e=doc->endNode();
    if(s==0||e==0||s==e){showError("Dijkstra:请先设定起点和终点");return;}
    if(doc->hasNegativeEdge()){showError("Dijkstra:图中存在负权边无法执行");return;}
    if(doc->dijkstra(s,e).empty()){showError("Dijkstra:不可达");return;}
    m_canvas->clearAlgorithmHighlights(); m_algoRunner->start(AlgorithmRunner::Algorithm::Dijkstra,doc,s,e); COUT_DEBUG(QString("Dijkstra:%1→%2").arg(s).arg(e));
}

void MainWindow::onRunBellmanFord() {m_algoFinished=false;
    GraphDocument* doc=currentDocument(); if(!doc)return;
    _usdint_ s=doc->startNode(),e=doc->endNode();
    if(s==0||e==0||s==e){showError("Bellman-Ford:请先设定起点和终点");return;}
    m_canvas->clearAlgorithmHighlights(); m_algoRunner->start(AlgorithmRunner::Algorithm::BellmanFord,doc,s,e); COUT_DEBUG(QString("Bellman-Ford:%1→%2").arg(s).arg(e));
}

void MainWindow::onAlgorithmStep(int step,int total,const QList<_usdint_>& h,const QList<_usdint_>& f,const QList<EdgeKey>& eh,const QList<EdgeKey>& ef){m_algoFinished=false;for(auto id:f)m_canvas->fadeNode(id);for(auto id:h)m_canvas->highlightNode(id,QColor(DEF_ALGO_STEP));for(const EdgeKey& ek:ef)m_canvas->fadeEdge(ek.first,ek.second);for(const EdgeKey& ek:eh)m_canvas->highlightEdge(ek.first,ek.second,QColor(DEF_ALGO_STEP));statusBar()->showMessage(QString("步 %1/%2").arg(step).arg(total),1000);}
void MainWindow::onAlgorithmFinished(const vector<_usdint_>& p){
    if(p.empty()){
        /* BFS/DFS：保留最终高亮状态，等待用户操作恢复 */
    } else {
        /* 单源路径：展示最优路径（绿色结点 + 蓝色边） */
        m_canvas->clearAlgorithmHighlights();
        for(auto id:p)m_canvas->highlightNode(id,QColor(DEF_ALGO_FINISH));
        for(size_t i=0;i+1<p.size();i++){EdgeKey ek(p[i],p[i+1]);GraphDocument* doc=currentDocument();if(doc&&doc->edgeValid(ek.first,ek.second)){m_canvas->flashItems({},{ek},999999,QColor(DEF_ALGO_PATH));}}
    }
    m_algoFinished=true;statusBar()->showMessage("演示完成——双击空白或按Enter恢复",0);
}
void MainWindow::onAlgorithmError(const QString& msg){showError(msg);}

/* ─── 帮助 ─── */

void MainWindow::onAbout(){QMessageBox mb(this);mb.setIcon(QMessageBox::Information);mb.setWindowTitle("关于");mb.setText("图可视化 Graph-Visual v1.0.0\n基于Qt的图数据结构可视化桌面应用");mb.addButton("确定",QMessageBox::AcceptRole);mb.exec();}
void MainWindow::onTutorial(){QString p=m_appDir+DEF_PATH_TUTORIAL;if(QFile::exists(p))QDesktopServices::openUrl(QUrl::fromLocalFile(p));else{QMessageBox mb(this);mb.setIcon(QMessageBox::Information);mb.setWindowTitle("教程");mb.setText("操作教程文件未找到");mb.addButton("确定",QMessageBox::AcceptRole);mb.exec();}}

void MainWindow::onToggleDebug() {
    DEBUG_MODE=!DEBUG_MODE.load(); m_settings->setDebugMode(DEBUG_MODE); m_settings->save();
    statusBar()->showMessage(DEBUG_MODE.load()?"调试模式已开启":"调试模式已关闭",3000); COUT_DEBUG(DEBUG_MODE.load()?"调试模式开启":"调试模式关闭");
    QMenu* hm=menuBar()->actions().last()->menu(); if(hm){for(QAction* a:hm->actions()){if(a->shortcut()==QKeySequence(DEF_SHORTCUT_DEBUG)){a->setChecked(DEBUG_MODE.load());break;}}}
}

void MainWindow::onSetPhysicsFps(int fps){m_canvas->setPhysicsFps(fps);m_settings->setPhysicsFps(fps);m_settings->save();updateRenderFpsMenu();statusBar()->showMessage(QString("物理帧率: %1 FPS").arg(fps),3000);}

void MainWindow::onSetRenderFps(int fps){m_settings->setRenderFps(fps);m_settings->save();statusBar()->showMessage(QString("渲染帧率: %1 FPS").arg(fps),3000);}

void MainWindow::onPhysicsConfig(){
    QJsonObject cur=m_settings->allPhysicsParams();
    PhysicsDialog dlg(cur,this);
    if(dlg.exec()!=QDialog::Accepted)return;
    QJsonObject np=dlg.getParams();
    m_settings->setAllPhysicsParams(np);m_settings->save();
    m_canvas->loadPhysicsParams(m_settings);
    statusBar()->showMessage("物理参数已应用",3000);
}

void MainWindow::updateRenderFpsMenu(){int physFps=m_settings->physicsFps();for(QAction* a:m_renderFpsActions){int fv=a->data().toInt();a->setEnabled(fv<=physFps);if(a->isChecked()&&fv>physFps){a->setChecked(false);QAction* first=m_renderFpsActions.first();for(QAction* fa:m_renderFpsActions){if(fa->data().toInt()<=physFps){fa->setChecked(true);m_settings->setRenderFps(fa->data().toInt());m_settings->save();break;}}break;}}}

void MainWindow::onTrackCenter(){m_canvas->centerOnGraphCenter();}

/* ─── 画布 ─── */

void MainWindow::onCanvasKeyPressed(int key,Qt::KeyboardModifiers mods){
    if(key==DEF_KEY_WASD_Q&&mods==Qt::ControlModifier){
        QPoint gp=m_canvas->mapFromGlobal(QCursor::pos());
        QPointF sp=m_canvas->mapToScene(gp);
        onAddNode(sp);return;
    }
    if(key==DEF_KEY_WASD_D&&mods==Qt::ControlModifier){
        onDeleteSelected();return;
    }
    if((key==DEF_KEY_ALGO_RESTORE)&&m_algoFinished){
        m_canvas->clearAlgorithmHighlights();m_algoFinished=false;statusBar()->showMessage("已恢复",2000);return;
    }
    if(key==DEF_KEY_ALGO_STEP&&m_algoRunner->isRunning()){
        if(m_algoRunner->isPaused())m_algoRunner->resume();
        else m_algoRunner->pause();
        statusBar()->showMessage(m_algoRunner->isPaused()?"已暂停":"继续演示",2000);
        return;
    }
    if(key==DEF_KEY_NEXT_NODE||key==DEF_KEY_PREV_NODE){
        GraphDocument* doc=currentDocument();if(!doc)return;
        auto nids=doc->selectedNodeIds();if(nids.size()!=1)return;
        _usdint_ cur=nids[0];if(!doc->nodeValid(cur))return;
        vector<_usdint_> adj=doc->adjacency(cur);
        QList<_usdint_> cands;
        for(_usdint_ a:adj){if(a!=1)cands.append(a);}
        if(cands.isEmpty()){statusBar()->showMessage("没有可切换的相邻结点",3000);return;}
        int pos=cands.indexOf(cur);
        int next;
        if(key==DEF_KEY_NEXT_NODE){
            next=(pos+1)%cands.size();
        }else{
            next=(pos-1+cands.size())%cands.size();
        }
        doc->selectNode(cands[next],false);
        m_canvas->centerOnNode(cands[next]);
        statusBar()->showMessage(QString("切换到结点 %1").arg(cands[next]),2000);
    }
}

void MainWindow::onCanvasNodeClicked(_usdint_ id,Qt::KeyboardModifiers mods){GraphDocument* doc=currentDocument();if(doc)doc->selectNode(id,mods&Qt::ControlModifier);}
void MainWindow::onCanvasEdgeClicked(EdgeKey ek,Qt::KeyboardModifiers mods){GraphDocument* doc=currentDocument();if(doc)doc->selectEdge(ek,mods&Qt::ControlModifier);}

void MainWindow::onCanvasRightClicked(QPointF sp) {
    GraphDocument* doc=currentDocument(); if(!doc||m_canvas->interactMode()==GraphCanvas::InteractMode::NodeMoving)return;
    QPoint spg=m_canvas->mapToGlobal(m_canvas->mapFromScene(sp));
    QGraphicsItem* it=m_canvas->findItemAt(sp);
    if(it&&it->type()==QGraphicsEllipseItem::Type){
        _usdint_ nid=it->data(0).toUInt();
        if(doc->nodeValid(nid)&&doc->isNodeSelected(nid)){m_menuManager->showNodeMenu(nid,spg,doc);return;}
    }else if(it&&it->type()==QGraphicsLineItem::Type){
        EdgeKey ek=it->data(0).value<EdgeKey>();
        if(doc->edgeValid(ek.first,ek.second)&&doc->isEdgeSelected(ek)){m_menuManager->showEdgeMenu(ek,spg);return;}
    }
    auto nids=doc->selectedNodeIds();auto eids=doc->selectedEdgeKeys();
    if(!nids.isEmpty()||!eids.isEmpty()){
        QMenu menu(this);
        QAction* addAct=menu.addAction("新增结点");
        QAction* delSel=menu.addAction("删除选中");
        QAction* trackAct=menu.addAction("追踪回图中心");
        QAction* act=menu.exec(spg);
        if(act==addAct)m_menuManager->addNodeRequested(sp);
        else if(act==delSel)onDeleteSelected();
        else if(act==trackAct)m_canvas->centerOnGraphCenter();
        return;
    }
    m_menuManager->showCanvasMenu(sp,spg);
}

void MainWindow::onCanvasDoubleClicked(){if(m_algoFinished){m_canvas->clearAlgorithmHighlights();m_algoFinished=false;statusBar()->showMessage("已恢复",2000);return;}GraphDocument* doc=currentDocument();if(doc)doc->clearSelection();}
void MainWindow::onEdgeCreatedByDrag(_usdint_ f,_usdint_ t){GraphDocument* doc=currentDocument();if(!doc||!doc->nodeValid(f)||!doc->nodeValid(t)||f==t||doc->edgeValid(f,t))return;doc->addEdge(f,t,1);statusBar()->showMessage(QString("新增边:%1→%2").arg(f).arg(t),2000);COUT_DEBUG(QString("新增边:%1→%2").arg(f).arg(t));}

/* ─── 右键菜单 ─── */

void MainWindow::onDeleteNode(_usdint_ id){GraphDocument* doc=currentDocument();if(!doc)return;bool n1=(id==1);doc->deleteNode(id,true);if(n1)statusBar()->showMessage("1号结点已删除已重新布局",3000);m_canvas->rebuildScene();COUT_DEBUG("删除结点:"+QString::number(id));}
void MainWindow::onDeleteEdge(EdgeKey ek){GraphDocument* doc=currentDocument();if(!doc)return;doc->deleteEdge(ek.first,ek.second);m_canvas->rebuildScene();COUT_DEBUG(QString("删除边:%1→%2").arg(ek.first).arg(ek.second));}
void MainWindow::onDeleteSelected(){GraphDocument* doc=currentDocument();if(!doc)return;auto nids=doc->selectedNodeIds();auto eids=doc->selectedEdgeKeys();if(nids.isEmpty()&&eids.isEmpty())return;for(_usdint_ id:nids)onDeleteNode(id);for(const EdgeKey& ek:eids)onDeleteEdge(ek);COUT_DEBUG(QString("删除选中: %1结点 %2边").arg(nids.size()).arg(eids.size()));}
void MainWindow::onEditEdgeWeight(EdgeKey ek){GraphDocument* doc=currentDocument();if(!doc)return;bool ok;int w=QInputDialog::getInt(this,"编辑边权重",QString("权重(%1→%2):").arg(ek.first).arg(ek.second),doc->edgeWeight(ek.first,ek.second),-9999,9999,1,&ok);if(ok){doc->modifyEdge(ek.first,ek.second,w);m_canvas->rebuildScene();if(doc->hasNegativeEdge())statusBar()->showMessage("⚠图中存在负权边Dijkstra算法将不可用",5000);COUT_DEBUG(QString("修改边权重:%1→%2 w=%3").arg(ek.first).arg(ek.second).arg(w));}}
void MainWindow::onSetStartNode(_usdint_ id){GraphDocument* doc=currentDocument();if(doc){if(doc->endNode()==id)doc->setEndNode(0);doc->setStartNode(id);COUT_DEBUG("设置起点:"+QString::number(id));}}
void MainWindow::onCancelStartNode(){GraphDocument* doc=currentDocument();if(doc){doc->setStartNode(0);COUT_DEBUG("取消起点");}}
void MainWindow::onSetEndNode(_usdint_ id){GraphDocument* doc=currentDocument();if(doc){if(doc->startNode()==id)doc->setStartNode(0);doc->setEndNode(id);COUT_DEBUG("设置终点:"+QString::number(id));}}
void MainWindow::onCancelEndNode(){GraphDocument* doc=currentDocument();if(doc){doc->setEndNode(0);COUT_DEBUG("取消终点");}}


void MainWindow::onMoveModeStart() {
    m_canvas->setInteractMode(GraphCanvas::InteractMode::NodeMoving); statusBar()->showMessage("移动模式:按住M,左键拖动结点,松开M退出",0); COUT_DEBUG("进入移动模式");
}
void MainWindow::onNodeMoveFinished(){m_canvas->setInteractMode(GraphCanvas::InteractMode::Normal);GraphDocument* doc=currentDocument();if(doc){doc->enforceMinSpacing();doc->clearSelection();}m_canvas->rebuildScene();statusBar()->showMessage("已退出移动模式",2000);COUT_DEBUG("退出移动模式");}
void MainWindow::onAddNode(QPointF sp){GraphDocument* doc=currentDocument();if(!doc)return;_usdint_ id=doc->addNode(sp);m_canvas->rebuildScene();COUT_DEBUG("新增结点:"+QString::number(id));}

/* ─── 标签 ─── */

void MainWindow::onTabClicked(int index){switchTab(index);}
void MainWindow::onTabDoubleClicked(int index){
    if(index<0||index>=m_documents.size())return;
    GraphDocument* doc=m_documents[index];
    bool ok;QString name=QInputDialog::getText(this,"重命名",QString("图名称(当前:%1):").arg(doc->fileName()),QLineEdit::Normal,doc->fileName(),&ok);
    if(ok&&!name.isEmpty()){doc->setFileName(name);m_tabBar->renameTab(index,name);statusBar()->showMessage(QString("已重命名为:%1").arg(name),2000);}
}
void MainWindow::onTabContextMenu(int index,QPoint globalPos){
    if(index<0||index>=m_documents.size())return;
    QMenu menu(this);
    QAction* closeAct=menu.addAction("关闭标签页");
    QAction* act=menu.exec(globalPos);
    if(act==closeAct){m_currentDocIndex=index;onCloseTab();}
}
void MainWindow::onTabsReordered(int f,int t){if(f<0||t<0||f>=m_documents.size()||t>=m_documents.size())return;m_documents.swapItemsAt(f,t);if(m_currentDocIndex==f)m_currentDocIndex=t;else if(m_currentDocIndex==t)m_currentDocIndex=f;}

/* ─── 错误 ─── */

void MainWindow::onErrorFlashTick(){m_errorFlashCount++;if(m_errorFlashCount>=6){m_errorFlashTimer->stop();m_canvas->setStyleSheet("");return;}QString c=m_flashColor.isEmpty()?"red":m_flashColor;m_canvas->setStyleSheet(m_errorFlashCount%2==0?QString("QGraphicsView{border:2px solid %1;}").arg(c):"");}

/* ─── 关闭 ─── */

void MainWindow::closeEvent(QCloseEvent* event) {
    QList<int> unsaved; for(int i=0;i<m_documents.size();i++)if(m_documents[i]->isModified())unsaved.append(i);
    if(!unsaved.isEmpty()){
        QString d; for(int i:unsaved)d+=m_documents[i]->fileName()+"\n";
        QMessageBox mb(this);mb.setWindowTitle("未保存");mb.setText("以下文件未保存:");mb.setInformativeText(d+"\n是否保存？");
        QPushButton* saveBtn=mb.addButton("保存",QMessageBox::AcceptRole);mb.addButton("不保存",QMessageBox::RejectRole);QPushButton* cancelBtn=mb.addButton("取消",QMessageBox::RejectRole);
        mb.exec();if(mb.clickedButton()==cancelBtn){event->ignore();return;}
        if(mb.clickedButton()==saveBtn){for(int i:unsaved){m_currentDocIndex=i;onSave();}}
    }
    m_settings->setWindowGeometry(geometry()); m_settings->save();
    DebugLogger::instance().flush(); event->accept();
}
