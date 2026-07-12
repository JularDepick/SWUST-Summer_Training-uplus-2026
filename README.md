<div align="center">

# SWUST-Summer_Training-uplus-2026

[![Version](https://img.shields.io/badge/Version-v26.7.11--n1-red)](https://github.com/JularDepick/SWUST-Summer_Training-uplus-2026/tree/v26.7.11-n1)
[![Copyright](https://img.shields.io/badge/Copyright-JularDepick-0066AA)](./COPYRIGHT)
[![License](https://img.shields.io/badge/License-MIT-yellow)](./LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows-blue)](./)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=cplusplus&logoColor=white)
![Git](https://img.shields.io/badge/Git-F05032?style=flat&logo=git&logoColor=white)

</div>

2026年SWUST卓越班暑期培训个人实践内容。

# 实验环境
- 硬件配置：
  - 运行内存：16GB
  - CPU：Intel(R) Core(TM) i7-14650HX 2.2GHz
  - GPU：NVIDIA GeForce RTX 5060 Laptop GPU
- 操作系统：Windows_x64
- 工具环境：
  - 版本管理：git version 2.54.0.windows.1
- 编程语言：C++
- 编辑器：Dev-C++ 5.11 (**ANSI字符集**)
- 编译器：g++ (tdm64-1) 10.3.0
- 编译选项：`-std=c++17 -s -DNDEBUG` (关闭`-Ox`优化避免耗时测试失效,实际使用头文件产物时可以开启)


# 实验列表
- [实验一：高级数据结构实现](./experiments/01/README.md)


# 常见问题

## 源代码在 VS Code 等编辑器中无法正常显示中文字符？

> Dev-C++ 5.11 创建的源代码文件使用的是 `ANSI` 字符集, 可能无法被使用 `UTF-8` 字符集的编辑器识别

如遇到该问题请**在备份全量文件后**运行修复脚本 `scripts/fixCharset-2-UTF-8.py` ,回退操作则运行 `scripts/fixCharset-2-ANSI.py`
