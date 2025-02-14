/* Простой пример, демонстрирующий использование ImGui для создания окна с несколькими элементами:
Что происходит в этом коде: 
    Инициализация GLFW : Создается окно с помощью GLFW.
    Настройка ImGui : Конфигурируется контекст ImGui и связывается с GLFW и OpenGL.
    Главный цикл : Каждый кадр обновляется содержимое GUI, вызывая функцию renderGui().
    Окончание работы : После завершения работы очищаются все ресурсы. */

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // GLFW - библиотека для работы с окном и OpenGL

// Функция отрисовки GUI
void renderGui() {
    static float value = 0.0f; // Статическая переменная для хранения значения слайдера
    static bool checkboxValue = true;

    // Начало нового кадра ImGui
    ImGui::NewFrame();

    // Создание окна
    ImGui::Begin("Пример ImGui", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // Добавление элементов в окно
    ImGui::Text("Добро пожаловать в ImGui!");
    ImGui::SliderFloat("Слайдер", &value, 0.0f, 1.0f); // Слайдер
    ImGui::Checkbox("Чекбокс", &checkboxValue);        // Чекбокс
    if (ImGui::Button("Нажми меня")) {                 // Кнопка
        value = 0.0f; // Сброс значения слайдера при нажатии кнопки
    }

    // Завершение окна
    ImGui::End();
}

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "Пример ImGui", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Включение V-Sync

    // Инициализация ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Темная тема

    // Настройка платформы и рендера для GLFW и OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Главный цикл приложения
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Начало нового кадра OpenGL
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка GUI
        renderGui();

        // Отображение ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Обмен буферами
        glfwSwapBuffers(window);
    }

    // Очистка ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Очистка GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
