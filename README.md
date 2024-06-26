# StudentManage
一个前后端分离的学生成绩管理系统。后端数据库用 C 实现，前端用户界面用 Web 实现。支持 Windows、Linux 。前端还未布设服务器，只能直接通过 HTML 访问。

## Build
要在 `Windows` 中使用 `make`，推荐 [Chocolatey](https://chocolatey.org/install). 安装完 Chocolatey 后：
```bash
choco install make
make
make reload
```
如果要 debug, 用 `make debug` 代替 `make`  

## 生成测试数据
本项目使用 [Google Gemini](https://ai.google.dev/) 辅助生成测试数据. 开始前先申请 [Google Gemini API key](https://ai.google.dev/) 并运行 `GetAwards.py`, `GetEssays.py`, `GetProjects.py` and `GetCourses.py` ( 顺序无所谓 ) 并跟随提示完成与 AI 的对话。绝大多数情况下只需要无脑按空格并在生成的样本量足够大时回复 `exit` 结束对话。完成对话后分别在各个生成的文件（`Courses.json` `__Award.json` `__Essay.json` `__Project.json`）中检查并手动将测试数据（JSON objects）组织到一个 `list` 中：
```
[{},{},{},...]
```
然后就可以运行 `GetTestData.py` 来生成最后的 `TestData.json` 和 `Teachers.json` 了。

## Usage
### C 后端
首次运行服务器前, 先运行 `reload.exe` 来由 `TestData.json` 初始化数据库，初始化后就可以运行服务器了：
```bash
reload.exe
StudentManage.exe
```
服务器默认监听 8080 端口。注意如果直接更改了 `TestData.json` 的内容需要重新初始化数据库。

### Web 前端
打开 `UI/login.html` 即可访问用户界面。