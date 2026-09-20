
// 示例：如何使用 yunet_trt 核心库做摄像头实时人脸 + 双眼 ROI 检测

// 核心库只提供两个东西：
//   1) YuNetTRT        —— 封装 TensorRT 引擎的构建/加载与推理
//   2) computeEyeRois  —— 由 5 点关键点推算左右眼 ROI

// 使用步骤：

//   detector.init(onnxPath, enginePath);              // 首次构建引擎，之后直接加载


//       f.box;          // 人脸框 (x, y, w, h)
//       f.landmarks[i]; // 5 个关键点
//       f.score;        // 置信度
//       const std::array<cv::Rect2f, 2> eyes = computeEyeRois(f);  // 左右眼 ROI


// 本文件把上述流程接到 OpenCV 摄像头上，仅作演示；核心推理都在库里。


#include "yunet_trt.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>

namespace {

bool isFile(const std::string& p) {
    struct stat st {};
    return ::stat(p.c_str(), &st) == 0 && (st.st_mode & S_IFREG);
}

bool isDir(const std::string& p) {
    struct stat st {};
    return ::stat(p.c_str(), &st) == 0 && (st.st_mode & S_IFDIR);
}

// 相对路径回退：先在当前目录找，找不到再试 ../ 和 ../../，
std::string resolvePath(const std::string& path) {
    if (path.empty() || path[0] == '/')
        return path;                                   // Linux 绝对路径
    if (path.size() > 1 && path[1] == ':')
        return path;                                   // Windows 绝对路径
    if (isFile(path))
        return path;
    const std::size_t slash = path.find_last_of('/');
    const std::string dir = (slash == std::string::npos) ? std::string(".") : path.substr(0, slash);
    if (isDir(dir))
        return path;                                   // 父目录存在，保持原样
    const std::array<const char*, 2> prefixes{"..", "../.."};
    for (const char* prefix : prefixes) {
        const std::string candidate = std::string(prefix) + "/" + path;
        if (isFile(candidate) || isDir(std::string(prefix) + "/" + dir))
            return candidate;
    }
    return path;
}

void printUsage(const char* exe) {
    std::cout
        << "用法: " << exe << " [选项]\n"
        << "选项:\n"
        << "  --onnx <path>    ONNX 模型 (默认 models/face_detection_yunet_2023mar.onnx)\n"
        << "  --engine <path>  序列化引擎 (默认 models/yunet_fp16.engine)\n"
        << "  --camera <int>   摄像头编号 (默认 0)\n"
        << "  --width <int>    采集宽度 (默认 1280)\n"
        << "  --height <int>   采集高度 (默认 720)\n"
        << "  --backend <s>    摄像头后端 auto|dshow|msmf|v4l2 (默认 auto)\n"
        << "  --score <float>  置信度阈值 (默认 0.9)\n"
        << "  --nms <float>    NMS IoU 阈值 (默认 0.3)\n"
        << "  --topk <int>     NMS 前保留候选框数量 (默认 5000)\n"
        << "  --log-every <n>  每 n 帧打印一次检测结果，0 表示不打印 (默认 0)\n"
        << "  --eye-scale <f>  眼框边长 = eye-scale * 双眼间距 (默认 0.6)\n"
        << "  --no-eyes        不画左右眼 ROI\n"
        << "  --no-fp16        关闭 FP16\n"
        << "  --rebuild        强制重新构建引擎\n"
        << "  --headless       不显示窗口 (自检用)\n"
        << "  --max-frames <n> 处理 n 帧后退出，0 表示一直运行 (默认 0)\n"
        << "  --snapshot <p>   保存一张带标注的截图\n";
}

void drawResults(cv::Mat& image, const std::vector<FaceResult>& faces,
                 bool drawEyes, float eyeScale) {
    const std::array<cv::Scalar, 5> landmarkColors{
        cv::Scalar(255, 0, 0), cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0),
        cv::Scalar(255, 0, 255), cv::Scalar(0, 255, 255)};
    for (const auto& f : faces) {
        cv::rectangle(image,
                      cv::Rect(cv::Point(static_cast<int>(f.box.x), static_cast<int>(f.box.y)),
                               cv::Point(static_cast<int>(f.box.x + f.box.width),
                                         static_cast<int>(f.box.y + f.box.height))),
                      cv::Scalar(0, 255, 0), 2);
        for (std::size_t n = 0; n < f.landmarks.size(); ++n)
            cv::circle(image,
                       cv::Point(static_cast<int>(f.landmarks[n].x),
                                 static_cast<int>(f.landmarks[n].y)),
                       2, landmarkColors[n], -1);
        cv::putText(image, cv::format("%.2f", f.score),
                    cv::Point(static_cast<int>(f.box.x),
                              std::max(12, static_cast<int>(f.box.y) - 4)),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);

        if (drawEyes) {
            const std::array<cv::Rect2f, 2> rois = computeEyeRois(f, eyeScale);
            const std::array<cv::Scalar, 2> eyeColors{cv::Scalar(255, 0, 0),
                                                      cv::Scalar(0, 0, 255)};
            const std::array<const char*, 2> eyeNames{"R", "L"};
            for (std::size_t e = 0; e < rois.size(); ++e) {
                const cv::Rect r(static_cast<int>(rois[e].x), static_cast<int>(rois[e].y),
                                 static_cast<int>(rois[e].width),
                                 static_cast<int>(rois[e].height));
                cv::rectangle(image, r, eyeColors[e], 2);
                cv::putText(image, eyeNames[e], cv::Point(r.x, std::max(12, r.y - 4)),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, eyeColors[e], 1);
            }
        }
    }
}

}

int main(int argc, char** argv) {
    std::string onnxPath = "models/face_detection_yunet_2023mar.onnx";
    std::string enginePath = "models/yunet_fp16.engine";
    int cameraIndex = 0;
    int width = 1280;
    int height = 720;
    std::string backend = "auto";
    float score = 0.6f;
    float nms = 0.3f;
    int topk = 5000;
    int logEvery = 0;
    float eyeScale = 0.6f;
    bool drawEyes = true;
    bool fp16 = true;
    bool rebuild = false;
    bool headless = false;
    int maxFrames = 0;
    std::string snapshot;

    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];
        const auto value = [&]() -> const char* {
            if (i + 1 >= argc) {
                std::cerr << "选项缺少参数: " << argument << '\n';
                std::exit(2);
            }
            return argv[++i];
        };
        if (argument == "--onnx") onnxPath = value();
        else if (argument == "--engine") enginePath = value();
        else if (argument == "--camera") cameraIndex = std::stoi(value());
        else if (argument == "--width") width = std::stoi(value());
        else if (argument == "--height") height = std::stoi(value());
        else if (argument == "--backend") backend = value();
        else if (argument == "--score") score = std::stof(value());
        else if (argument == "--nms") nms = std::stof(value());
        else if (argument == "--topk") topk = std::stoi(value());
        else if (argument == "--log-every") logEvery = std::stoi(value());
        else if (argument == "--eye-scale") eyeScale = std::stof(value());
        else if (argument == "--no-eyes") drawEyes = false;
        else if (argument == "--no-fp16") fp16 = false;
        else if (argument == "--rebuild") rebuild = true;
        else if (argument == "--headless") headless = true;
        else if (argument == "--max-frames") maxFrames = std::stoi(value());
        else if (argument == "--snapshot") snapshot = value();
        else if (argument == "--help" || argument == "-h") {
            printUsage(argv[0]);
            return 0;
        } else {
            std::cerr << "未知参数: " << argument << '\n';
            printUsage(argv[0]);
            return 1;
        }
    }

    onnxPath = resolvePath(onnxPath);
    enginePath = resolvePath(enginePath);

    YuNetTRT detector;
    if (!detector.init(onnxPath, enginePath, fp16, rebuild)) {
        std::cerr << "初始化 YuNet TensorRT 失败\n"
                  << "  请确认模型文件存在，例如在项目根目录运行: ./build/yunet_camera_demo\n"
                  << "  或显式指定: --onnx <path.onnx> --engine <path.engine>\n";
        return 1;
    }

    int api = cv::CAP_ANY;
    if (backend == "dshow") api = cv::CAP_DSHOW;
    else if (backend == "msmf") api = cv::CAP_MSMF;
    else if (backend == "v4l2") api = cv::CAP_V4L2;

    cv::VideoCapture cap(cameraIndex, api);
    if (!cap.isOpened()) {
        std::cerr << "无法打开摄像头 " << cameraIndex << "\n";
        return 1;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    const char* win = "YuNet TensorRT camera  [q/ESC]=quit  [s]=save";
    if (!headless)
        cv::namedWindow(win, cv::WINDOW_NORMAL);

    std::cout << "按 q 或 ESC 退出，s 保存截图。\n";
    int frameIdx = 0;
    while (true) {
        cv::Mat frame;
        if (!cap.read(frame) || frame.empty()) {
            std::cerr << "读取摄像头帧失败\n";
            break;
        }
        ++frameIdx;

        const std::vector<FaceResult> faces = detector.detect(frame, score, nms, topk);
        if (logEvery > 0 && frameIdx % logEvery == 0) {
            float best = 0.f;
            for (const auto& f : faces)
                best = std::max(best, f.score);
            std::cout << "[frame " << frameIdx << "] faces=" << faces.size()
                      << " best=" << best << '\n';
        }
        drawResults(frame, faces, drawEyes, eyeScale);

        if (!headless) {
            cv::imshow(win, frame);
            const int k = cv::waitKey(1) & 0xFF;
            if (k == 27 || k == 'q')
                break;
            if (k == 's') {
                const std::string out = snapshot.empty() ? "yunet_cam_snapshot.jpg" : snapshot;
                cv::imwrite(out, frame);
                std::cout << "已保存 " << out << "\n";
            }
        } else if (!snapshot.empty() && frameIdx == 1) {
            cv::imwrite(snapshot, frame);
        }

        if (maxFrames > 0 && frameIdx >= maxFrames)
            break;
    }

    cap.release();
    if (!headless)
        cv::destroyAllWindows();
    std::cout << "处理帧数: " << frameIdx << "\n";
    return 0;
}
