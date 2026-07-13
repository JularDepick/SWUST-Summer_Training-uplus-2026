# 实验二：带用户界面的数据结构实现

# 作者
- 姓名：李文芳
- 学号：5120252354
- 班级：卓软2501班
- 指导老师：杨春明
- 邮箱：1724834368@qq.com
- GitHub：[JularDepick](https://github.com/JularDepick)

# 实验环境
- 硬件配置：
  - 运存：16GB
  - CPU：Intel(R) Core(TM) i7-14650HX 2.2GHz
  - GPU：NVIDIA GeForce RTX 5060 Laptop GPU
- 操作系统：Windows_x64
- 编程语言：C++
- IDE：Qt 6.11.1

## 文件结构

```
src/
├── CMakeLists.txt        # Cmake列表
├── main.cpp              # 入口
├── mainwindow.h          # 主窗口声明
├── mainwindow.cpp        # 主窗口实现
├── mainwindow.ui         # UI骨架布局
├── indexGraph.hpp        # 图数据结构引擎
├── components.hpp        # 全部业务模块
├── defines.hpp           # 全局可自定义宏
├── Graph-Visual_zh_CN.ts # 中文本地化
├── 操作教程.md            # 操作教程
└── README.md             # 本文件
```

## 架构

```
main.cpp → mainwindow.h/.cpp → components.hpp → indexGraph.hpp
```

## 快捷键

所有快捷键在 `defines.hpp` 中定义为全局宏，可统一修改。

| 快捷键 | 宏名 | 功能 |
|--------|------|------|
| Ctrl+N | `DEF_SHORTCUT_NEW_FILE` | 新建 |
| Ctrl+O | `DEF_SHORTCUT_OPEN_FILE` | 打开 |
| Ctrl+S | `DEF_SHORTCUT_SAVE` | 保存 |
| Ctrl+Shift+S | `DEF_SHORTCUT_SAVE_AS` | 另存为 |
| Ctrl+Shift+N | `DEF_SHORTCUT_RENAME` | 重命名 |
| Ctrl+W | `DEF_SHORTCUT_CLOSE_TAB` | 关闭标签页 |
| Ctrl+A | `DEF_SHORTCUT_SELECT_COMP` | 选中连通分量 |
| Ctrl+Shift+A | `DEF_SHORTCUT_SELECT_ALL` | 全选图 |
| Ctrl+Shift+C | `DEF_SHORTCUT_CLEAR_GRAPH` | 清空图 |
| Ctrl+Shift+E | `DEF_SHORTCUT_CLEAR_EDGES` | 清空边 |
| Ctrl+Shift+R | `DEF_SHORTCUT_RESORT` | 重排序修复 |
| Ctrl+Q | `DEF_SHORTCUT_ADD_NODE` | 新增结点 |
| Ctrl+D | `DEF_SHORTCUT_DELETE_SEL` | 删除选中 |
| Ctrl+Shift+T | `DEF_SHORTCUT_TRACK_CENTER` | 追踪回图中心 |
| Ctrl+Alt+D | `DEF_SHORTCUT_ALGO_DFS` | DFS演示 |
| Ctrl+Alt+B | `DEF_SHORTCUT_ALGO_BFS` | BFS演示 |
| Ctrl+Alt+A | `DEF_SHORTCUT_ALGO_DIJKSTRA` | Dijkstra演示 |
| Ctrl+Alt+E | `DEF_SHORTCUT_ALGO_BF` | Bellman-Ford演示 |
| F11 | `DEF_SHORTCUT_FULLSCREEN` | 全屏 |
| F1 | `DEF_SHORTCUT_ABOUT` | 关于 |
| F2 | `DEF_SHORTCUT_TUTORIAL` | 教程 |
| F3 | `DEF_SHORTCUT_DEBUG` | 调试模式 |

## 操作键

| 按键 | 宏名 | 功能 |
|------|------|------|
| M | `DEF_KEY_MOVE_MODE` | 移动模式切换 |
| Z | `DEF_KEY_BIDIR_DRAG` | 按住创建双向边 |
| Tab | `DEF_KEY_PHYSICS_TOGGLE` | 物理暂停/恢复 |
| Space | `DEF_KEY_ALGO_STEP` | 算法演示暂停/继续 |
| Enter | `DEF_KEY_ALGO_RESTORE` | 演示恢复 |
| . | `DEF_KEY_NEXT_NODE` | 下一相邻结点 |
| , | `DEF_KEY_PREV_NODE` | 上一相邻结点 |
| W/A/S/D | `DEF_KEY_WASD_*` | 画布平移（带加速度） |
| Q | `DEF_KEY_WASD_Q` | Ctrl+Q 新增结点 |
