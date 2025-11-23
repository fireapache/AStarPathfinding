#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "application.h"
#include "renderer.h"
#include "fixinclude.h"

// Window dimensions
const int VIEWPORT_WIDTH = 421;
const int VIEWPORT_HEIGHT = 411;
const int UI_PANEL_WIDTH = 180;
const int WINDOW_WIDTH = VIEWPORT_WIDTH + UI_PANEL_WIDTH;
const int WINDOW_HEIGHT = VIEWPORT_HEIGHT;

// Mouse state for dragging
static bool isDragging = false;
static int dragStartX = 0;
static int dragStartY = 0;
static int mouseX = 0;
static int mouseY = 0;

// Application state
static Application* app = nullptr;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Let ImGui handle mouse input first
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            isDragging = true;
            dragStartX = mouseX;
            dragStartY = mouseY;
        }
        else if (action == GLFW_RELEASE)
        {
            if (app && isDragging)
            {
                app->handleMouseClick(mouseX, mouseY, true, dragStartX, dragStartY);
            }
            isDragging = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Adjust for UI panel offset
    mouseX = (int)xpos - UI_PANEL_WIDTH;
    mouseY = (int)ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Let ImGui handle scroll first
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;

    // Mouse wheel to step through algorithm
    if (yoffset != 0 && app)
    {
        app->showNextStep();
    }
}

int main(int argc, char* argv[])
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // GL 2.1 + GLSL 120
    const char* glsl_version = "#version 120";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "A* Pathfinding", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Set up callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Initialize renderer and application
    Renderer renderer;
    if (!renderer.initialize())
    {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }

    app = new Application(&renderer);

    // UI state
    static int mapWidth = 25;
    static int mapHeight = 25;
    static int mapScale = 2;
    static bool mapBuilt = false;
    static int selectedHeuristic = 0; // 0 = Euclidean, 1 = Manhattan

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Left panel UI
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2((float)UI_PANEL_WIDTH, (float)WINDOW_HEIGHT));
            ImGui::Begin("Functions", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            ImGui::Text("Map Size:");
            ImGui::PushItemWidth(80);
            if (ImGui::InputInt("Width", &mapWidth))
            {
                if (mapWidth < 1) mapWidth = 1;
                if (mapWidth > 100) mapWidth = 100;
                mapBuilt = false;
            }
            if (ImGui::InputInt("Height", &mapHeight))
            {
                if (mapHeight < 1) mapHeight = 1;
                if (mapHeight > 100) mapHeight = 100;
                mapBuilt = false;
            }
            ImGui::PopItemWidth();

            ImGui::Spacing();

            if (ImGui::Button(mapBuilt ? "Rebuild Map" : "Build Map", ImVec2(150, 0)))
            {
                app->buildMap(mapWidth, mapHeight, mapScale);
                mapBuilt = true;
            }

            ImGui::Spacing();

            ImGui::Text("Scale:");
            ImGui::PushItemWidth(80);
            if (ImGui::InputInt("##Scale", &mapScale))
            {
                if (mapScale < 1) mapScale = 1;
                if (mapScale > 4) mapScale = 4;
                mapBuilt = false;
            }
            ImGui::PopItemWidth();

            ImGui::Separator();

            ImGui::BeginDisabled(!mapBuilt);
            if (ImGui::Button("Show Path", ImVec2(150, 0)))
            {
                app->showPath();
            }

            if (ImGui::Button("Show Step", ImVec2(150, 0)))
            {
                app->showNextStep();
            }

            ImGui::EndDisabled();

            ImGui::Separator();

            ImGui::Text("Heuristic:");
            if (ImGui::RadioButton("Euclidean", &selectedHeuristic, 0))
            {
                app->setHeuristics(euclidean);
            }
            if (ImGui::RadioButton("Manhattan", &selectedHeuristic, 1))
            {
                app->setHeuristics(manhattan);
            }

            ImGui::Separator();

            ImGui::Text("Tools:");

            if (ImGui::Button("Start [S]", ImVec2(150, 0)))
            {
                app->setToSetState(TOSETSTART);
            }

            if (ImGui::Button("End [E]", ImVec2(150, 0)))
            {
                app->setToSetState(TOSETEND);
            }

            if (ImGui::Button("Way [W]", ImVec2(150, 0)))
            {
                app->setToSetState(TOSETWAY);
            }

            if (ImGui::Button("Wall [#]", ImVec2(150, 0)))
            {
                app->setToSetState(TOSETWALL);
            }

            if (ImGui::Button("Dirt [D]", ImVec2(150, 0)))
            {
                app->setToSetState(TOSETDIRT);
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Text("Current Tool:");
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f),
                app->getToSetState() == TOSETSTART ? "Start" :
                app->getToSetState() == TOSETEND ? "End" :
                app->getToSetState() == TOSETWAY ? "Way" :
                app->getToSetState() == TOSETWALL ? "Wall" :
                app->getToSetState() == TOSETDIRT ? "Dirt" : "Unknown");

            ImGui::End();
        }

        // Rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the grid in the right viewport area
        if (app->isMapBuilt())
        {
            // Set viewport for grid rendering
            glViewport(UI_PANEL_WIDTH, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

            renderer.renderGrid(app->getMap(), app->getMapWidth(), app->getMapHeight(),
                              UI_PANEL_WIDTH, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

            // Reset viewport for ImGui
            glViewport(0, 0, display_w, display_h);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    delete app;
    renderer.cleanup();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
