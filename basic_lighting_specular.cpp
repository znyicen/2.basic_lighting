// ## ע������ ##
// 1. ��filesystem.h���޸ģ�
//    #include "root_directory.h" - ԭ���Ĵ���
//    const char* logl_root = "C:/LearnOpenGL-master"; // �趨��Ŀ¼
// 2. ��Ҫ�رչ��̵� C/C++ SDL��� ����
// 3. �Թ�������SOIL.lib
// ## ע������ ##

//// I ��ͷ ////
#include <iostream>
#include <cmath>

// GLEW
// GLEW ���Զ�ʶ�����ƽ̨��֧�ֵ�ȫ��OpenGL�߼���չ����
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
// OpenGL ���ڹ����ֱ����л������̡�����Լ���Ϸ�ֱ�����ʱ�����롢�̴߳����ȵȡ�
#include <GLFW/glfw3.h>

// Other Libs
// ͼ���ȡ��
#include <SOIL.h>
// GLM ��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include <learnopengl/shader.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/Camera.h>

//// II ����ԭ�� ////
// ��������ص�����
void key_callback(GLFWwindow* window, int key, int scancode,
	int action, int mode);
// �������ص�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// �������ص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// ���������
void do_movement();

//// III ȫ�ֱ��� ////
// ���ڳߴ�
const GLuint WIDTH = 800, HEIGHT = 600;

// ��Ӱ������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// �ƹ�����
glm::vec3 lightPos(1.2f, 0.5f, 2.0f);

// ֡��ʱ
GLfloat deltaTime = 0.0f; // ��ǰ֡����һ֡����ʱ��
GLfloat lastFrame = 0.0f; // ��һ֡

//// IV ������ ////
int main()
{
	std::cout << "����GLFW����, OpenGL�汾��3.3��" << std::endl;
	// 1.0 ��ʼ��GLFW
	glfwInit();
	// 1.1 �趨GLFW�������ģʽ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 2.0 �������ڶ���
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	// 2.1 �����ڼ���openGL����
	glfwMakeContextCurrent(window);

	// ��������Ļص����� 
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 3.0 ��ʼ��GLEW
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // ����ʾ���
	glewExperimental = GL_TRUE;                                  // ʹ���ִ�������
	glewInit();

	// 4.0 �����ӿ�
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// 5.0 ��д��������ɫ������
	Shader lightingShader("basic_lighting.vert", "basic_lighting.frag");
	Shader lampShader("lamp.vert", "lamp.frag");


	// 6.0 ���ö�������
	GLfloat vertices[] = {
		// λ��               // ������
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

	// 6.1 �������ݶ���
	GLuint VBO, VAO;		    // ���㻺����󣬶����������
	glGenVertexArrays(1, &VAO);	// ���ɻ���ID
	glGenBuffers(1, &VBO);		// ���ɻ���ID

	// 6.2 �����ݶ���
	// ����VAO
	glBindVertexArray(VAO);

	// ����VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ����EBO
	// ...

	// 6.3 ���Ͷ������ݣ����ݵģ�λ�á���λ����ʽ������������ƫ������
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// ��������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)
		(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 6.4 ��� VAO
	glBindVertexArray(0);

	// Ϊ�ƴ���һ��VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ����(����)��VBO�������Ѿ���������ȷ�������嶥������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���õ�������Ķ�������ָ��(�����õƵĶ�������)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// 7.0 ��Ϸѭ��
	while (!glfwWindowShouldClose(window))
	{
		// 7.1 ���㵱ǰ֡��deltaTime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 7.2 ��������¼�
		glfwPollEvents();
		// ��������
		do_movement();

		// 7.3 ��Ⱦ
		// 7.31 �����ɫ������
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �����Ȼ�����

		// 7.32 ʹ������Ԫ������
		// ...

		// 7.33 ������Ⱦ��
		// ������ɫ��
		lightingShader.Use();

		// ��ȡ�ƹ���ɫ��������ַ
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

		// ���ò���
		// ����ɫ
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);// ��������Ϥ��ɺ����
		// �ƹ�
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);  // ���ɰѹ�Դ����Ϊ��ɫ
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		// 7.34 ������Ӱ��/��ͼ�任
		// ������ͼ����
		glm::mat4 view;
		// ʹ��������ഴ���۲����,�����䴫�ݸ���ͼ����
		view = camera.GetViewMatrix();
		// ����ͶӰ����
		glm::mat4 projection;
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// ��ȡ������������ɫ����uniform�ĵ�ַ
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// �Ѿ��󴫵ݸ���ɫ�� 
		// ����1��uniform�ĵ�ַ(Location)
		// ����2����Ҫ���Ͷ��ٸ�����Ŀǰ��1
		// ����3��ѯ�����������Ƿ�ϣ�������ǵľ�������û�(Transpose)��Ҳ����˵�������Ǿ�����к��С�
		// OpenGL������ͨ��ʹ��һ���ڲ����󲼾ֽ�������Ϊ��˳���(Column-major Ordering)���֡�
		// GLM�Ѿ���������Ϊ��˳���������ľ������Բ�����Ҫ�û�����������GL_FALSE��
		// ����4����ʵ�ʵľ������ݣ�����GLM�����ǰ����ǵľ��󴢴�ΪOpenGL��ϣ�������֣�
		// �������Ҫ����GLM���Դ��ĺ���value_ptr���任��Щ���ݡ�
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// 7.35 ��ʼ����
		// ��VAO
		glBindVertexArray(VAO);

		// ����ÿ�������ģ�;��󣬲�����������ɫ��
		// ���ǽ���������Ͼ���ʱ���Ƚ������Ų�����
		// Ȼ������ת��������ƽ�ƣ��������ǻ�(������)����Ӱ�졣
		// ����ģ�;���
		glm::mat4 model;
		// ����������ɫ��
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// 7.36 Ȼ������ʹ��glDrawArrays���������壬��һ���ܹ���36�����㡣
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// ���
		glBindVertexArray(0);

		// 7.37 ����ƹ���ɫ��
		lampShader.Use();

		// ���� Uniform
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// ģ�;���
		model = glm::mat4();
		// ƽ��
		model = glm::translate(model, lightPos);
		// ����
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// 7.38 ���Ƶƹ����
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// ������Ļ������
		glfwSwapBuffers(window);
	}
	// 8.0 �ͷ���Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &lightVAO);

	glfwTerminate();
	return 0;

}

//// V ����ʵ�� ////
// ����ص�����ʵ��
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// ��λ״̬��¼(��ס������һ���������֧�ֶ��λͬ�´���
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// ��λ״̬����
void do_movement()
{
	// ���������
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

	GLfloat sensitivity = 0.05; // ���������
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}