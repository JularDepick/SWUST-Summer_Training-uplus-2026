# 实验一：高级数据结构实现

# 实验环境
参见 [项目主README #实验环境](../../README.md#实验环境)

# 目录结构
```
experiments/01/
├── docs/  # 实验文档
│   ├── 实验一：高级数据结构实现.docx  # 实验报告DOCX文档
│   └── 实验一：高级数据结构实现.pdf
├── src/  # 源码目录
│   ├── assist.hpp              # 工具类（TIMER计时器、rdint随机数）
│   ├── indexGraph.hpp          # 有向图类（邻接表+反向邻接表+边集，Dijkstra/Bellman-Ford/BFS/DFS）
│   ├── README.md               # 源码介绍文档
│   └── testexp/                # 测试与基准评测
│       ├── genTestdata.cpp     # 测试数据生成器
│       ├── runner@reporter.cpp # 性能基准测试（build/查询/遍历/路径算法）
│       ├── collector.cpp       # 数据采集器，输出横向对比.md
│       ├── input/              # genTestdata.cpp 生成的测试数据（.in.txt）
│       ├── report/             # runner@reporter.cpp生成的基准测试报告（.out.txt）
│       └── docsmd/             # collector.cpp采集后的横向对比文档（.md）
├── pretest/ # 预先试验（非正式）
└── README.md  # 本介绍文档
```

