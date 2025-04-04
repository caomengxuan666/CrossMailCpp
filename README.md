# CrossMailCpp

CrossMailCpp 是一个基于 C++ 和 Python 的跨语言的SMTP邮件发送工具。通过结合 C++ 的高性能和 Python 的灵活性，该项目提供了一个简单易用的且非常之轻量级的接口来发送电子邮件。相比于其他大型的C/C++SMTP库，具有轻量级的特点。

---

## 功能特性

- **跨语言支持**: 使用 C++ 调用 Python 的邮件服务模块。
- **异步发送**: 支持通过 Python 的 `asyncio` 实现异步邮件发送。
- **HTML 内容支持**: 可加载 HTML 文件或直接设置硬编码的 HTML 内容作为邮件正文。
- **配置驱动**: 通过 YAML 配置文件管理邮件服务参数。

---

## 快速开始

### 环境要求

- **C++ 编译器**: GCC 或 Clang（支持 C++17、C++20）。
- **CMake**: 版本 >= 3.12。
- **Python**: 版本 >= 3.6，并安装 `pybind11`。
- **依赖**: 安装 `requirements.txt` 中列出的 Python 包。

### 构建步骤

1. 克隆项目：
   ```bash
   git clone https://github.com/caomengxuan666/CrossMailCpp.git
   cd CrossMailCpp
   ```

2. 安装Python依赖
    ```bash
    pip install -r requirements.txt
    ```

3. 配置和构建项目

### 使用方式

#### 配置邮件服务
编辑 email.yaml 文件以设置邮件服务器的相关参数，例如 SMTP 地址、端口、用户名和密码。

示例配置
```yaml
smtp:
  host: "smtp.example.com"
  port: 587
  username: "your-email@example.com"
  password: "your-password"
```

### 发送邮件
在 C++ 中调用 EmailWrapper 类的接口发送邮件：

```cpp
#include "email.h"

int main() {
    EmailWrapper email;

    // 加载 HTML 文件内容
    std::string html_content = email.load_html_content("path/to/email.html");

    // 设置邮件内容
    std::string recipient = "recipient@example.com";
    std::string title = "Test Email";
    std::string content = html_content;

    // 发送邮件
    email.send_email(recipient, content, title);

    return 0;
}
```

你也可以设置成硬编码的内容来替代我们的HTML文件的方式，在我们的实例中已经给出

## 注意事项
- Python 环境 : 确保 Python 环境已正确配置，并且 email_server.py 能正常运行。如果在linux下出现了GLIBC版本错误
你可以使用
```bash ln -s /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /home/cmx/miniconda3/lib/libstdc++.so.6```
来确保你conda/minconda的环境链接到最新的GLIBC库。

- YAML 配置 : 修改 email.yaml 文件以匹配你的邮件服务器配置。
- 错误处理 : 在实际使用中，请根据需要扩展错误处理逻辑。

## 贡献
欢迎贡献代码或提出改进建议！本项目处于开发阶段，欢迎大家参与改进！

## 许可证
本项目采用 MIT 许可证。详情请参阅 LICENSE 文件。