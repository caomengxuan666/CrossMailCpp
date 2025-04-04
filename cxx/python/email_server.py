# email_server.py
import os
import yaml
import asyncio
from email.mime.text import MIMEText
from email.header import Header
import aiosmtplib
import sys

# Windows/Linux兼容性处理
if sys.platform == 'win32':
    asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())
else:
    try:
        import uvloop
        uvloop.install()
    except ImportError:
        pass


class EmailServer:
    def __init__(self):
        # 嵌入配置文件路径
        self.config_path = os.path.join(os.path.dirname(__file__), 'email.yaml')
        
        # 加载配置文件
        with open(self.config_path, 'r') as f:
            config = yaml.safe_load(f)
        
        self.from_addr = config['email-server']['sender']['address']
        self.password = config['email-server']['sender']['password']
        self.smtp_server = config['email-server']['server']
        self.smtp_port = config['email-server']['port']

    def set_email_content(self, content, msg_type='html'):
        self.email_content = MIMEText(content, msg_type, 'utf-8')

    def set_email_title(self, title):
        self.email_title = title

    async def send_email(self, recv, content, title):
        # 构建邮件内容
        self.set_email_content(content)
        self.set_email_title(title)

        msg = self.email_content
        msg['From'] = self.from_addr  # 发件人地址
        msg['To'] = recv  # 直接收件人地址
        msg['Subject'] = Header(title, 'utf-8')  # 添加编码参数

        try:
            # 使用 aiosmtplib 异步发送邮件
            smtp_client = aiosmtplib.SMTP(
                hostname=self.smtp_server,
                port=self.smtp_port,
                use_tls=True
            )
            await smtp_client.connect()
            await smtp_client.login(self.from_addr, self.password)
            await smtp_client.sendmail(self.from_addr, [recv], msg.as_string())
            print("邮件发送成功！")
        except aiosmtplib.SMTPAuthenticationError as e:
            print(f"认证失败错误: {e}")
        except Exception as e:
            print(f"其他错误: {str(e)}")
        finally:
            # 关闭连接
            if smtp_client.is_connected:
                await smtp_client.quit()

# 示例用法
if __name__ == "__main__":
    async def main():
        email_server = EmailServer()
        await email_server.send_email(
            recv="xxxxx@qq.com",
            content="测试邮件发送功能",
            title="测试"
        )

    asyncio.run(main())