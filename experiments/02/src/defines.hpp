#pragma once
/* defines.hpp — 全局可自定义宏*/

/* ─── 结点 ─── */
#define DEF_NODE_RADIUS       20.0    /* 结点半径 */
#define DEF_NODE_FILL         0x4A, 0x90, 0xD9   /* 填充色 #4A90D9 */
#define DEF_NODE_BORDER       0x78, 0x78, 0x78   /* 边框色 #787878 */
#define DEF_NODE_TEXT_COLOR   0xFF, 0xFF, 0xFF   /* 编号文字色 #FFFFFF */

/* ─── 边 ─── */
#define DEF_EDGE_COLOR        0x66, 0x66, 0x66   /* 边颜色 #666666 */
#define DEF_ARROW_SIZE        10.0    /* 箭头长度 */
#define DEF_ARROW_WIDTH       5.0     /* 箭头宽度 */
#define DEF_BIDIR_OFFSET      5.0     /* 双向边偏移量 */

/* ─── 选中高亮 ─── */
#define DEF_SELECT_COLOR      0x00, 0xAA, 0xFF   /* 选中色 #00AAFF */
#define DEF_SELECT_WIDTH      3       /* 选中边框宽度 */

/* ─── 算法演示 ─── */
#define DEF_ALGO_STEP         0xFF, 0x33, 0x33   /* 当前步 #FF3333 */
#define DEF_ALGO_FADE         0xFF, 0x88, 0x88   /* 已访问 #FF8888 */
#define DEF_ALGO_FINISH       0x4C, 0xAF, 0x50   /* 完成结点 #4CAF50 */
#define DEF_ALGO_PATH         0x21, 0x96, 0xF3   /* 最终路径边 #2196F3 */
#define DEF_ALGO_SELECT       0x55, 0x55, 0x55   /* 选中闪烁 #555555 */

/* ─── 检查闪烁 ─── */
#define DEF_FLASH_OK          0x4C, 0xAF, 0x50   /* 通过 #4CAF50 */
#define DEF_FLASH_ERR         0xFF, 0x33, 0x33   /* 异常 #FF3333 */

/* ─── 状态栏文字颜色 ─── */
#define DEF_SBAR_NORMAL       0x1C, 0x1C, 0x1C   /* 普通提醒 */
#define DEF_SBAR_SUCCESS      0x15, 0x90, 0x1A   /* 成功/绿色 */
#define DEF_SBAR_ERROR        0xC6, 0x28, 0x28   /* 错误/红色 */
#define DEF_SBAR_WARNING      0xD4, 0x8B, 0x0A   /* 警告/黄色 */
#define DEF_SBAR_PROGRESS     0x52, 0x94, 0xE2   /* 进度文字/蓝色 */

/* ─── 画布 ─── */
#define DEF_CANVAS_BG         0xFF, 0xFF, 0xFF   /* 画布背景 #FFFFFF */

/* ─── 浅色主题 ─── */
#define DEF_THEME_WINDOW      0xF3, 0xF3, 0xF3   /* 窗口背景 */
#define DEF_THEME_BAR         0xE1, 0xE1, 0xE1   /* 菜单栏背景 */
#define DEF_THEME_BORDER      0xCC, 0xCC, 0xCC   /* 边框色 */
#define DEF_THEME_TEXT        0x33, 0x33, 0x33   /* 文字色 */
#define DEF_THEME_HOVER       0x00, 0x78, 0xD4   /* 悬停色 */
#define DEF_THEME_PANEL       0xDA, 0xDA, 0xDA   /* 面板背景 */
#define DEF_THEME_INPUT_BG    0xFF, 0xFF, 0xFF   /* 输入框背景 */
#define DEF_THEME_INPUT_TEXT  0x22, 0x22, 0x22   /* 输入框文字 */

/* ─── 悬浮文本 ─── */
#define DEF_TOOLTIP_BG        0x33, 0x33, 0x33   /* 提示框背景 */
#define DEF_TOOLTIP_TEXT      0xEE, 0xEE, 0xEE   /* 提示框文字 */
#define DEF_TOOLTIP_BORDER    0x55, 0x55, 0x55   /* 提示框边框 */

/* ─── 起点/终点标签 ─── */
#define DEF_START_BG          0x2E, 0x7D, 0x32   /* 起点背景 */
#define DEF_START_TEXT        0xA5, 0xD6, 0xA7   /* 起点文字 */
#define DEF_END_BG            0xC6, 0x28, 0x28   /* 终点背景 */
#define DEF_END_TEXT          0xEF, 0x9A, 0x9A   /* 终点文字 */

/* ─── 缩略图 ─── */
#define DEF_MINIMAP_BG        0x1e,0x1e,0x1e     /* 背景色 */
#define DEF_MINIMAP_EDGE      0x64,0x64,0x64     /* 边颜色 */
#define DEF_MINIMAP_NODE      0x34,0x98,0xdb     /* 结点颜色 */
#define DEF_MINIMAP_VP        255,0,0            /* 视口指示色 */
#define DEF_MINIMAP_SCALE     0.15               /* 缩放比例 */

/* ─── 力导向布局 ─── */
/* ─── 力导向布局（派生自物理参数，保持一致） ─── */
#define DEF_LAYOUT_SPACING    DEF_PHYSICS_IDEAL       /* 结点理想间距 */
#define DEF_LAYOUT_REPULSE    DEF_PHYSICS_K_REP       /* 斥力系数 */
#define DEF_LAYOUT_ATTRACT    (DEF_PHYSICS_K_UNI*0.5) /* 引力系数(弱于物理,避免初始过猛) */
#define DEF_LAYOUT_CENTER     DEF_PHYSICS_K_COMP      /* 向心力系数 */
#define DEF_LAYOUT_DAMPING    0.9                     /* 布局阻尼(比物理稍高,快速收敛) */
#define DEF_LAYOUT_MIN_DIST   DEF_PHYSICS_MIN_DIST    /* 最小距离钳制 */
#define DEF_LAYOUT_OVERLAP    (DEF_PHYSICS_MIN_DIST-10)/* 重叠判定距离 */
#define DEF_LAYOUT_SPACING_MIN DEF_PHYSICS_MIN_DIST   /* 结点中心最小间距 */

/* ─── 物理模拟 ─── */
#define DEF_PHYSICS_FPS        120    /* 物理计算每秒帧数 */
#define DEF_PHYSICS_RENDER_FPS 30     /* 画面渲染每秒帧数 */
#define DEF_PHYSICS_K_REP      3500.0 /* 斥力系数 */
#define DEF_PHYSICS_K_UNI      0.06   /* 单向边拉力系数(sqrt) */
#define DEF_PHYSICS_K_BI       0.008  /* 双向边弹簧系数 */
#define DEF_PHYSICS_K_COMP     0.005  /* 分量引力（有全局排斥力制衡，只需防无限漂移） */
#define DEF_PHYSICS_GAMMA_L    0.15   /* 布局阶段阻力系数 */
#define DEF_PHYSICS_GAMMA_D    0.45   /* 动态维持阶段阻力系数（抑制切向振荡） */
#define DEF_PHYSICS_V_MAX      2.0    /* 速度上限 */
#define DEF_PHYSICS_STOP_VEL   0.3    /* 布局收敛阈值 */
#define DEF_PHYSICS_IDLE_VEL   0.05   /* 物理自动暂停阈值 */
#define DEF_PHYSICS_IDEAL      110.0  /* 相邻结点理想间距下界(死区低端) */
#define DEF_PHYSICS_MIN_DIST   65.0   /* 全局最小结点间距 */
#define DEF_PHYSICS_MAX_DIST   800.0  /* 全局最大结点间距(纯安全网,平时不应触发) */
#define DEF_PHYSICS_CUTOFF     225.0  /* 斥力截止距离 */
#define DEF_PHYSICS_G_UNIV     25.0   /* 万有引力(F=G/ds, 1/r衰减, 远距主导) */
#define DEF_PHYSICS_POS_MIN   -1e6    /* 结点坐标下界 */
#define DEF_PHYSICS_POS_MAX    1e6    /* 结点坐标上界 */

/* ─── 操作键 ─── */
#define DEF_KEY_MOVE_MODE       Qt::Key_M      /* 移动模式切换 */
#define DEF_KEY_BIDIR_DRAG      Qt::Key_Z      /* 双向边拖拽 */
#define DEF_KEY_WASD_W          Qt::Key_W      /* 上移 */
#define DEF_KEY_WASD_A          Qt::Key_A      /* 左移 */
#define DEF_KEY_WASD_S          Qt::Key_S      /* 下移 */
#define DEF_KEY_WASD_D          Qt::Key_D      /* 右移 */
#define DEF_KEY_WASD_Q          Qt::Key_Q      /* 新增结点(Ctrl+Q) */
#define DEF_KEY_PHYSICS_TOGGLE  Qt::Key_Tab    /* 物理暂停切换 */
#define DEF_KEY_ALGO_STEP       Qt::Key_Space  /* 演示暂停/继续 */
#define DEF_KEY_ALGO_RESTORE    Qt::Key_Return /* 演示恢复 */
#define DEF_KEY_NEXT_NODE       Qt::Key_Period /* 下一相邻结点 */
#define DEF_KEY_PREV_NODE       Qt::Key_Comma  /* 上一相邻结点 */

/* ─── 快捷键 ─── */
#define DEF_SHORTCUT_NEW_FILE      "Ctrl+N"        /* 新建 */
#define DEF_SHORTCUT_OPEN_FILE     "Ctrl+O"        /* 打开 */
#define DEF_SHORTCUT_SAVE          "Ctrl+S"        /* 保存 */
#define DEF_SHORTCUT_SAVE_AS       "Ctrl+Shift+S"  /* 另存为 */
#define DEF_SHORTCUT_RENAME        "Ctrl+Shift+N"  /* 重命名 */
#define DEF_SHORTCUT_CLOSE_TAB     "Ctrl+W"        /* 关闭标签 */
#define DEF_SHORTCUT_SELECT_COMP   "Ctrl+A"        /* 选中连通分量 */
#define DEF_SHORTCUT_SELECT_ALL    "Ctrl+Shift+A"  /* 全选 */
#define DEF_SHORTCUT_CLEAR_GRAPH   "Ctrl+Shift+C"  /* 清空图 */
#define DEF_SHORTCUT_CLEAR_EDGES   "Ctrl+Shift+E"  /* 清空边 */
#define DEF_SHORTCUT_RESORT        "Ctrl+Shift+R"  /* 重排序 */
#define DEF_SHORTCUT_ADD_NODE      "Ctrl+Q"        /* 新增结点 */
#define DEF_SHORTCUT_DELETE_SEL    "Ctrl+D"        /* 删除选中 */
#define DEF_SHORTCUT_TRACK_CENTER  "Ctrl+Shift+T"  /* 追踪回图中心 */
#define DEF_SHORTCUT_ALGO_DFS      "Ctrl+Alt+D"    /* DFS演示 */
#define DEF_SHORTCUT_ALGO_BFS      "Ctrl+Alt+B"    /* BFS演示 */
#define DEF_SHORTCUT_ALGO_DIJKSTRA "Ctrl+Alt+A"    /* Dijkstra演示 */
#define DEF_SHORTCUT_ALGO_BF       "Ctrl+Alt+E"    /* Bellman-Ford演示 */
#define DEF_SHORTCUT_FULLSCREEN    "F11"            /* 全屏 */
#define DEF_SHORTCUT_ABOUT         "F1"             /* 关于 */
#define DEF_SHORTCUT_TUTORIAL      "F2"             /* 教程 */
#define DEF_SHORTCUT_DEBUG         "F3"             /* 调试模式 */

/* ─── 文件路径 ─── */
#define DEF_PATH_SETTINGS     "/settings.json"     /* 设置文件 */
#define DEF_PATH_LOGS         "logs"               /* 日志目录 */
#define DEF_PATH_TUTORIAL     "/操作教程.md"        /* 教程文件 */
#define DEF_FILE_FILTER       "图文件 (*.indexgraph.json)"           /* 保存过滤器 */
#define DEF_FILE_FILTER_ALL   "图文件 (*.indexgraph.json);;所有文件 (*)" /* 打开过滤器 */
#define DEF_FILE_EXT          ".indexgraph.json"   /* 默认扩展名 */

/* ─── 调试日志 ─── */
#define DEF_LOG_HEADER        "Graph-Visual 调试日志"   /* 日志文件头 */
#define DEF_CRASH_HEADER      "=== Graph-Visual 崩溃日志 ===" /* 崩溃日志头 */
