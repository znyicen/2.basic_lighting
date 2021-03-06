// ## 注意事项 ##
// 1. 对filesystem.h的修改：
//    #include "root_directory.h" - 原本的代码
//    const char* logl_root = "C:/LearnOpenGL-master"; // 设定根目录
// 2. 需要关闭工程的 C/C++ SDL检查 特性
// 3. 对工程连接SOIL.lib
// ## 注意事项 ##

//// I 库头 ////
#include <iostream>
#include <cmath>

// GLEW
// GLEW 能自动识别你的平台所支持的全部OpenGL高级扩展涵数
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
// OpenGL 窗口管理、分辨率切换、键盘、鼠标以及游戏手柄、定时器输入、线程创建等等。
#include <GLFW/glfw3.h>

// Other Libs
// 图像读取库
#include <SOIL.h>
// GLM 数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include <learnopengl/shader.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/Camera.h>

//// II 函数原型 ////
// 键盘输入回调函数
void key_callback(GLFWwindow* window, int key, int scancode,
	int action, int mode);
// 鼠标输入回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// 鼠标滚动回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 输出入理函数
void do_movement();

//// III 全局变量 ////
// 窗口尺寸
const GLuint WIDTH = 800, HEIGHT = 600;

// 摄影机变量
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// 灯光属性
glm::vec3 lightPos(1.2f, 0.5f, 2.0f);

// 帧耗时
GLfloat deltaTime = 0.0f; // 当前帧到上一帧所耗时间
GLfloat lastFrame = 0.0f; // 上一帧

//// IV 主程序 ////
int main()
{
	std::cout << "进入GLFW环境, OpenGL版本：3.3。" << std::endl;
	// 1.0 初始化GLFW
	glfwInit();
	// 1.1 设定GLFW程序基本模式
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 2.0 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	// 2.1 将窗口加入openGL环境
	glfwMakeContextCurrent(window);

	// 设置所需的回调函数 
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 3.0 初始化GLEW
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 不显示光标
	glewExperimental = GL_TRUE;                                  // 使用现代化技术
	glewInit();

	// 4.0 设置视口
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// 5.0 编写并编译着色器程序
	Shader lightingShader("basic_lighting.vert", "basic_lighting.frag");
	Shader lampShader("lamp.vert", "lamp.frag");


	// 6.0 设置顶点数据
	GLfloat vertices[] = {
		// 位置               // 法向量
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// 6.1 创建数据对象
	GLuint VBO, VAO;		    // 顶点缓存对象，顶点数组对象
	glGenVertexArrays(1, &VAO);	// 生成缓存ID
	glGenBuffers(1, &VBO);		// 生成缓存ID

	// 6.2 绑定数据对象
	// 处理VAO
	glBindVertexArray(VAO);

	// 处理VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 处理EBO
	// ...

	// 6.3 解释顶点数据（数据的：位置、数位、格式，？，步长，偏移量）
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 法线属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)
		(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 6.4 解绑 VAO
	glBindVertexArray(0);

	// 为灯创建一个VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为容器(物体)的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置灯立方体的顶点属性指针(仅设置灯的顶点数据)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// 7.0 游戏循环
	while (!glfwWindowShouldClose(window))
	{
		// 7.1 计算当前帧的deltaTime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 7.2 检测输入事件
		glfwPollEvents();
		// 按键处理
		do_movement();

		// 7.3 渲染
		// 7.31 清空颜色缓冲区
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除深度缓冲区

		// 7.32 使用纹理单元绑定纹理
		// ...

		// 7.33 激活渲染器
		// 照明着色器
		lightingShader.Use();

		// 获取灯光着色器参数地址
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

		// 设置参数
		// 固有色
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);// 我们所熟悉的珊瑚红
		// 灯光
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);  // 依旧把光源设置为白色
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		// 7.34 创建摄影机/视图变换
		// 定义视图矩阵
		glm::mat4 view;
		// 使用摄像机类创建观察矩阵,并将其传递给视图矩阵
		view = camera.GetViewMatrix();
		// 创建投影矩阵
		glm::mat4 projection;
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// 获取各个矩阵在着色器的uniform的地址
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// 把矩阵传递给着色器 
		// 参数1：uniform的地址(Location)
		// 参数2：将要发送多少个矩阵，目前是1
		// 参数3：询问我们我们是否希望对我们的矩阵进行置换(Transpose)，也就是说交换我们矩阵的行和列。
		// OpenGL开发者通常使用一种内部矩阵布局叫做以列为主顺序的(Column-major Ordering)布局。
		// GLM已经是用以列为主顺序定义了它的矩阵，所以并不需要置换矩阵，我们填GL_FALSE。
		// 参数4：是实际的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望的那种，
		// 因此我们要先用GLM的自带的函数value_ptr来变换这些数据。
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// 7.35 开始绘制
		// 绑VAO
		glBindVertexArray(VAO);

		// 计算每个对象的模型矩阵，并把它传给着色器
		// 我们建议您在组合矩阵时，先进行缩放操作，
		// 然后是旋转，最后才是平移，否则它们会(消极地)互相影响。
		// 定义模型矩阵
		glm::mat4 model;
		// 将矩阵传入着色器
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// 7.36 然后我们使用glDrawArrays来画立方体，这一次总共有36个顶点。
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// 解绑
		glBindVertexArray(0);

		// 7.37 激活灯光着色器
		lampShader.Use();

		// 处理 Uniform
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// 模型矩阵
		model = glm::mat4();
		// 平移
		model = glm::translate(model, lightPos);
		// 缩放
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// 7.38 绘制灯光对象
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// 交换屏幕缓冲区
		glfwSwapBuffers(window);
	}
	// 8.0 释放资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &lightVAO);

	glfwTerminate();
	return 0;

}

//// V 函数实现 ////
// 输入回调函数实现
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// 键位状态记录(待住函数中一并处理，则可支持多键位同事处理）
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// 键位状态处理
void do_movement()
{
	// 摄像机控制
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05; // 鼠标灵敏度
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}