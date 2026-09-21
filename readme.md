# 机器视觉实验（学生版）

本目录是《机器视觉实验手册（lab1-8）》配套的**学生版**代码包。

`lab_code/` 下的
`lab1`、`lab2`、`lab5`、`lab6` 四个实验工程里留有待补全的 `TODO(labN)` 挖空；

其余部分
（`common/` 框架、各实验的界面与线程骨架）都是完整可用的，补全时**不要修改 `common/`**。

## 目录结构

```text
visual_lab/
├─ 机器视觉实验手册（lab1-8）.docx   实验原理、步骤与验收要求
└─ lab_code/
   ├─ common/          公共分层代码（core / runtime / qt / opencv），完整框架，勿改
   ├─ lab1/            实验：图像与视频显示（TODO 练习）
   ├─ lab2/            实验：Stage/Pipeline 管线与 Sobel 边缘检测（TODO 练习）
   ├─ lab5/            实验：Amsler 视物变形测量（TODO 练习）
   ├─ lab6/            实验：注视点检测（TODO 练习）
   ├─ text_image/      实验用测试图片
   └─ yunet_trt_cpp/   YuNet + TensorRT 人脸/眼睛检测库
```

## 代码分层

四个实验共用同一套分层，建议先读结构再写代码：

| 层 | 位置 | 职责 |
|---|---|---|
| 核心 | `common/core` | 纯 C++17：`Frame` / `Stage` / `BoundedQueue` / `StageRegistry`，不依赖 Qt |
| 执行 | `common/runtime`、各实验的 worker | 全工程唯一的建线程点：`WorkerThread`、串行 `Pipeline`、采集/测量循环 |
| 表示 | 各实验的 `*_viewmodel.*` | QObject 适配层：Mat→QImage、状态→信号；Qt 与 OpenCV 只在这里交界 |
| 视图 | 各实验的 `mainwindow.*`、`amsler_widget.*` | 只渲染与转发输入，不含算法、不建线程 |

要点：

- 图像以不可变的 `std::shared_ptr<const labcore::Frame>` 在层间传递，数据本身无需加锁；
- 采集/处理回调运行在工作线程，不能访问 `QWidget/QPixmap`，显示一律走 `labqt::toQImage()` + 最新帧槽（`LatestImageSlot`）；
- 算法参数用 `setParams()` 热更新，只有增删算法节点才需要重建管线；
- 新增一个 Stage = 新增一个文件 + 注册一次 + 装配一行。

## TODO（待补全位置）

挖空只出现在 `lab1`、`lab2`、`lab5`、`lab6`，**以代码里的 `TODO(labN)` 注释为准**；
每个函数的声明处（`.h`）都写有实现要点，先读声明，再补实现（`.cpp`）。

| 实验 | 文件 | 待实现 |
|---|---|---|
| lab1 | `camera_worker.cpp` | 采集循环中的镜像与帧发布 |
| lab1 | `display_viewmodel.{h,cpp}` | 图片读取、摄像头启停 |
| lab2 | `capture_source.{h,cpp}` | 源阶段读一帧 |
| lab2 | `sobel_stage.{h,cpp}` | 灰度 + Sobel 边缘检测 |
| lab2 | `pipeline_viewmodel.{h,cpp}` | 串行管线组装与启动 |
| lab5 | `amsler_test.{h,cpp}` | Amsler 测试状态机（开始 / 回答推进） |
| lab6 | `eye_detection.{h,cpp}` | ROI 二值化寻找瞳孔 |
| lab6 | `measurement_worker.{h,cpp}` | 子线程一次测量 |


## 构建与运行
Linux / Jetson Nano（Qt 5 + OpenCV，以 lab2 为例）

有两种构建运行方式，任选其一即可：

**方式一：使用 Qt Creator（图形界面）**
直接双击 `lab2.pro` 文件，或用 Qt Creator 打开对应的 `.pro` 文件，然后点击构建并运行即可。

**方式二：使用命令行（bash）**

```bash
cd lab_code/lab2
mkdir build && cd build
qmake ../lab2.pro && make -j4 && ./lab2
```

lab1 / lab5 / lab6 同理，把路径与工程名换成对应的即可。


## 开始实验

1. 按《机器视觉实验手册（lab1-8）》的安装说明与实验步骤准备环境；
2. 通读对应实验的代码分层，再定位 `TODO(labN)` 的实现要点；
3. 每完成一个实验，按手册给出的界面操作与预期现象进行验收。

## 实验验收
1. 助教核验每个实验完成情况；
2. 助教随机提问考查实验知识掌握情况。

## 知识点清单

按四个主题分类：C++（含并发与线程、架构与设计模式）、Qt、OpenCV、深度学习推理部署。括号内是典型落点，便于对照代码。

### 一、C++（含并发与线程、架构与设计模式）

#### 1.1 语言基础

- 值/移动语义、拷贝消除、`std::move`（`Frame`、`cv::Mat` 传递）
- RAII 所有权、`std::unique_ptr` / `shared_ptr` / `weak_ptr`
- 自定义删除器（`TrtDestroy` / `CudaFree`）
- 不可变共享：`std::shared_ptr<const labcore::Frame>`
- `std::optional`（`FrameSource::next()` 的“有无值”）
- `std::function` 回调、lambda（`Pipeline::setOutput`、worker 回调）
- `enum class`（`PixelFormat`、`Phase`、`Answer`）
- `constexpr` / `const` 正确性（`kPi`、`kStrides`）
- `std::array` / `std::vector` / `std::string` / `QString` 互转
- 模板与泛型（Stage 接口、删除器模板 `operator()(T*)`）
- `noexcept`、异常处理（捕获 `cv::Exception`）
- PIMPL（`YuNetTRT::Impl` 隐藏 TensorRT/CUDA 依赖）

#### 1.2 并发与线程

- `std::thread` 生命周期与 `join`（`WorkerThread`）
- `std::mutex` / `lock_guard` / `unique_lock`（`setParams` 与 `process` 互斥）
- `std::condition_variable` + 有界阻塞队列（`BoundedQueue`）
- 生产者–消费者与背压、丢帧统计（`Pipeline` fps/dropped）
- `std::atomic` 停止标志、协作式取消（`m_stopRequested`）
- 带超时等待、超时返回 `nullopt`（`MeasurementWorker::measureOnce`）

#### 1.3 架构与设计模式

- 分层架构：core（纯算法）/ runtime（执行）/ qt（表示）
- MVVM：view ↔ view-model ↔（worker/算法）
- 注册表/插件模式（`StageRegistry`、`register_labN_stages`）
- 策略模式（`Stage` 接口，多算法实现）
- 管道–过滤器 / 责任链（`Pipeline` 串行 stage）
- 借用语义 vs 所有权（`Pipeline::setSource` 借用 source）
- 依赖注入（`setLocator`、locator 传给 worker）
- 观察者（信号槽、`std::function` 回调）

### 二、Qt

- 信号/槽与 `connect`（`frameChanged`、`modeChanged`、`fixationValidChanged`）
- `QMainWindow` / `QWidget` / `QLabel` / `QPushButton` / `QComboBox` / `QSlider` / `QCheckBox`
- `QTimer`（~30Hz 拉帧、统计定时、刺激闪烁）
- `QStackedWidget` 参数页切换（lab4）
- 自定义控件与事件处理（`mybutton` 的 mouse/paintEvent）
- `QPainter` 绘制：`paintEvent`、`QPen/QBrush`、网格与刺激（Amsler）
- `QImage` / `QPixmap` / `QIcon`；`cv::Mat ↔ QImage`（`labqt::toQImage`）
- 跨线程显示：GUI 只在主线程，工作线程经槽/LatestImageSlot 投递
- `QString` / `QStringLiteral` / `fromLocal8Bit`、`qgetenv`（无头钩子）

### 三、OpenCV

- `cv::Mat` 内存模型、引用计数、`clone()` 与 ROI 视图
- 格式与通道：BGR/BGRA/Gray、`split/merge`、1/3/4 通道分支
- 颜色空间：`cvtColor`（`BGR2GRAY`、`BGRA2GRAY`）
- 滤波：`GaussianBlur`、邻域均值（眩光）
- 边缘：`Sobel`（`dx/dy/ksize`，lab2）
- 阈值：`threshold`（`THRESH_BINARY_INV`，瞳孔/眩光）
- 轮廓：`findContours`（`RETR_TREE`/`CHAIN_APPROX_SIMPLE`）、`contourArea`、`boundingRect`
- 几何：`cv::Rect`、ROI 裁剪与相交、局部坐标 → 全图坐标
- 数据类型/深度：`convertTo`（`CV_32F`）
- 逐元素运算：`cv::min`（视野缺失掩膜）、`addWeighted`（叠加）
- 缩放：`resize`（效果缩略图）
- 视频采集：`VideoCapture`、`CAP_ANY`、`read/isOpened/release`
- 图像 IO：`imread`（`IMREAD_UNCHANGED`）、`imwrite`

### （可选）四、深度学习推理部署（YuNet + TensorRT + CUDA）

#### 4.1 模型与 TensorRT 构建/加载

- ONNX 解析：`nvonnxparser::IParser::parseFromFile`
- 显式 batch：`createNetworkV2(EXPLICIT_BATCH)`
- `IBuilder` / `IBuilderConfig` / `INetworkDefinition`
- 精度与工作区：`kFLOAT`/`kHALF`（FP16）、`setMaxWorkspaceSize`
- 动态形状与优化配置：`createOptimizationProfile`、`setBindingDimensions`
- 引擎序列化/缓存：`serialize` → 文件、`deserialize` 复用（避免重建）
- 运行时对象：`IRuntime` / `ICudaEngine` / `IExecutionContext`

#### 4.2 推理执行与前后处理

- 执行：`enqueueV2(pointers, stream)` + 同步
- 输入预处理：letterbox 缩放/填充、归一化、NCHW blob、H2D
- 输出解码：多尺度 stride 8/16/32 的 cls/obj/bbox/kps、先验框解码
- 后处理：置信度阈值、`iou` + `nms` 去重
- 业务映射：人脸框 → 眼 ROI（`computeEyeRois`）、逐帧跟踪
- 输出回读：D2H、类型/维度解析（`getBinding*`）



#### 编译

```
cd ~/Desktop/Machine-Vision-Course-Design
QMAKE=/usr/lib/qt5/bin/qmake
source config/load_visual_lab_config.sh

for l in lab1 lab2 lab3 lab4 lab5 lab6 lab7 lab8; do
  grep -q '^[[:space:]]*DESTDIR' "$l/$l.pro" \
    || printf '\nDESTDIR = build\n' >> "$l/$l.pro"
  [ -L "$l/$l" ] || { rm -f "$l/$l"; ln -s "build/$l" "$l/$l"; }
  mkdir -p "$l/build"
  echo "=== $l ==="
  (cd "$l" && make distclean >/dev/null 2>&1; \
   rm -f .qmake.stash Makefile; \
   "$QMAKE" "$l.pro" && make -j"$(nproc)") \
   || echo ">>> $l 编译失败"
done
```