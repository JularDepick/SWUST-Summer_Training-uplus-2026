# 实验一：高级数据结构实现

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
- 编辑器：Dev-C++ 5.11 (**ANSI字符集**)
- 编译器：g++ (tdm64-1) 10.3.0
- 编译选项：`-std=c++17 -s -DNDEBUG` (关闭`-Ox`优化避免耗时测试失效)

# 目录结构
```
dist/
├── assist.hpp              # 工具类（TIMER计时器、rdint随机数）
├── indexGraph.hpp          # 有向图类（邻接表+反向邻接表+边集，Dijkstra/Bellman-Ford/BFS/DFS）
├── README.md               # 本介绍文档
└── testexp/                # 测试与基准评测
    ├── genTestdata.cpp     # 测试数据生成器
    ├── runner@reporter.cpp # 性能基准测试（build/查询/遍历/路径算法）
    ├── collector.cpp       # 数据采集器，输出横向对比.md
    ├── input/              # genTestdata.cpp 生成的测试数据（.in.txt）
    ├── report/             # runner@reporter.cpp生成的基准测试报告（.out.txt）
    └── docsmd/             # collector.cpp采集后的横向对比文档（.md）
```

# 快速测试

> **提示**：项目已自带完整的测试产出文件（`input/`、`report/`、`docsmd/`），可直接用于分析，无需重新跑全流程。如需重新生成，按下方流程操作。

## 运行路线
```
genTestdata.cpp  →  runner@reporter.cpp  →  collector.cpp
    (生成数据)         (主基准测试)           (数据采集汇总)
```

## 各程序说明

### 1. genTestdata.cpp — 测试数据生成器
- **输入**：无（参数硬编码在main中）
- **输出**：`testexp/input/<编号>.in.txt`，每个文件包含 `n m`、`m` 行 `from to weight`、空行、`wl`、`wr`
- **行为**：生成50组测试数据，覆盖不同规模（n=100~2000）和权重范围（[1,1]~[-100,100]）

### 2. runner@reporter.cpp — 主性能基准
- **输入**：`testexp/input/<编号>.in.txt`
- **输出**：`testexp/report/<编号>.out.txt`，包含15项计时数据 + n/m/wl/wr元信息
- **行为**：依次测试 build、endouble、getdegs、areacount、adjion(3种)、DFS、BFS、Dijkstra(50对)、Bellman-Ford(50对)、clear，累加total_dt

### 3. collector.cpp — 数据采集器
- **输入**：`testexp/report/*.out.txt`
- **输出**：`testexp/docsmd/<指标名>.md`，横向对比所有测试用例
- **行为**：解析报告文件，按指标分类输出，格式包含 n/m/w[wl,wr] 元信息
