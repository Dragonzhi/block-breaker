
**砖治不服 | Shut Up & Brick!**

*一、依赖环境与配置要求*

•	操作系统：Windows 7 及以上

•	开发环境：Visual Studio 2022 及以上

•	编译器：支持 C++11 标准

•	第三方库：

•	EasyX 图形库（EasyX 官网）

请提前安装 EasyX 并配置到 Visual Studio 的包含目录和库目录。

*二、软件功能介绍*

本软件为一个关卡制打砖块小游戏，主要功能包括：

•	关卡选择界面，支持 6 个关卡选择

•	每个关卡下方显示该关卡的历史最高分

•	右侧显示所有关卡的总分

•	进入关卡后进行游戏，游戏结束后可刷新最高分

•	分数数据自动保存和读取

*三、操作流程*

1.	启动程序

运行可执行文件，进入关卡选择界面。

2.	选择关卡

点击任意关卡按钮进入对应关卡。

3.	进行游戏

按照游戏规则操作，完成关卡。

4.	查看分数

游戏结束后，系统会自动保存本关最高分。返回关卡选择界面可查看分数更新。

5.	退出程序

关闭窗口即可，分数会自动保存。

*四、输入输出格式*

•	输入：

•	鼠标点击进行关卡选择和游戏操作

•	键盘操作（R：显示碰撞盒；S：开启无敌模式（将不计入成绩）；T：关闭无敌模式；ESC：退出当前场景；Space：暂停游戏）

•	输出：

•	关卡选择界面显示每关最高分和总分

•	游戏界面显示当前分数

•	分数数据保存在本地 highscores.dat 文件中，自动读写，无需手动操作

*五、注意事项*

•	请确保 highscores.dat 文件有读写权限，首次运行会自动创建

•	若分数显示异常，请检查 EasyX 是否正确安装，或删除 highscores.dat 重新生成

![image](https://github.com/user-attachments/assets/b8c2d508-8494-4430-8e66-6b413ee7c050)
![image](https://github.com/user-attachments/assets/c92e02de-2ce0-44de-84bf-cc8bf6d350d0)
![image](https://github.com/user-attachments/assets/957a4ce2-f18f-46be-98c2-79c343153c81)
![image](https://github.com/user-attachments/assets/a026a425-4385-4e32-830b-32724402bb36)
