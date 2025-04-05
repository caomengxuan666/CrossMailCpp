#include "../CrossMailCpp/smtp.h"

int main() {
    // 创建 EmailWrapper 实例
    EmailWrapper email_wrapper("email.yaml", 10);

    // 硬编码 HTML 内容
    std::string hard_coded_html = R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>测试邮件</title>
        </head>
        <body>
            <h1>Hello!这是一个测试邮件</h1>
            <p>这是邮件的内容部分。</p>
        </body>
        </html>
        )";

    // 收件人信息
    std::string recipient = "xxx@qq.com";
    std::string title = "HTML测试邮件";

    try {
        // 设置硬编码的 HTML 内容
        std::string html_content = email_wrapper.set_html_content(hard_coded_html);
        // 发送邮件
        email_wrapper.send_email(recipient, html_content, title);
        std::cout << "邮件发送成功！" << std::endl;
    } catch (const EmailException &e) {
        std::cerr << "邮件操作失败: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "发生错误: " << e.what() << std::endl;
    }
}