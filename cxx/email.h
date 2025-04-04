/**
 * @FilePath     : /CrossMailCpp/cxx/email.h
 * @Description  :  
 * @Author       : caomengxuan666 2507560089@qq.com
 * @Version      : 0.0.1
 * @LastEditors  : caomengxuan666 2507560089@qq.com
 * @LastEditTime : 2025-04-04 23:32:24
 * @Copyright    : PESONAL DEVELOPER CMX., Copyright (c) 2025.
**/
#include <fstream>
#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <sstream>

namespace py = pybind11;

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

// 全局管理 Python 解释器的初始化和销毁
class PyInterpreterManager {
public:
    PyInterpreterManager() {
        py::initialize_interpreter();
    }

    ~PyInterpreterManager() {
        py::finalize_interpreter();
    }
};

// 封装 EmailServer 类的功能
class DLL_EXPORT EmailWrapper {
public:
    EmailWrapper() {
        // 导入 Python 模块
        email_module = py::module_::import("email_server");

        // 创建 EmailServer 实例
        email_server = email_module.attr("EmailServer")();
    }

    // 调用 send_email 方法
    void send_email(const std::string &recv, const std::string &content, const std::string &title) {
        try {
            py::object asyncio = py::module_::import("asyncio");
            py::object send_email_coroutine = email_server.attr("send_email")(recv, content, title);


            asyncio.attr("run")(send_email_coroutine);
        } catch (const py::error_already_set &e) {
            std::cerr << "Python Error: " << e.what() << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "C++ Error: " << e.what() << std::endl;
        }
    }

    // 加载 HTML 文件内容
    std::string load_html_content(const std::string &html_file_path) {
        std::ifstream file(html_file_path);
        if (!file.is_open()) {
            throw std::runtime_error("无法打开 HTML 文件: " + html_file_path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // 设置硬编码的 HTML 内容
    std::string set_html_content(const std::string &html_content) {
        return html_content;// 直接返回硬编码的 HTML 内容
    }

private:
    py::object email_module;// 保存对 Python 模块的引用
    py::object email_server;// 存储 Python 对象
};

PYBIND11_MODULE(email_wrapper, m) {
    py::class_<EmailWrapper>(m, "EmailWrapper")
            .def(py::init<>())                                         // 构造函数
            .def("send_email", &EmailWrapper::send_email)              // 暴露 send_email 方法
            .def("load_html_content", &EmailWrapper::load_html_content)// 暴露 load_html_content 方法
            .def("set_html_content", &EmailWrapper::set_html_content); // 暴露 set_html_content 方法
}


// 全局管理 Python 解释器
static PyInterpreterManager interpreter_manager;