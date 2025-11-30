#include "ImGuiLayer.h"
#include "Constants.h"

#include "Scene.h"

void ImGuiLayer::renderGUI(Scene& scene)
{
    ImGui::Begin("Scene Control Panel");


    glm::vec3 clearColor = scene.getClearColor();
    if (ImGui::ColorEdit3("Background Color", glm::value_ptr(clearColor)))
    {
        scene.setClearColor(clearColor);
    }

    glm::vec3 lightColor = scene.getLightColor();
    if (ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor)))
    {
        scene.setLightColor(lightColor);
    }

    glm::vec3 cubeColor = scene.getCubeColor();
	if (ImGui::ColorEdit3("Cube Color", glm::value_ptr(cubeColor)))
    {
        scene.setCubeColor(cubeColor);
    }

    bool isMoving = scene.getIsMovingLight();
    if (ImGui::Checkbox("Moving Light", &isMoving))
    {
        scene.setIsMovingLight(isMoving);
    }

    float lightSpeed = scene.getLightSpeed();
    if (ImGui::SliderFloat("Light Speed", &lightSpeed, 0.0f, 5.0f))
    {
        scene.setLightSpeed(lightSpeed);
    }


    float mixValue = scene.getMixValue();
    if (ImGui::SliderFloat("Texture Mix", &mixValue, RenderConfig::minMixValue, RenderConfig::maxMixValue));
    {
        scene.setMixValue(mixValue);
    }

    float rotateSpeed = scene.getRotateSpeed();
    if (ImGui::SliderFloat("Cube Rotate Speed", &rotateSpeed, RenderConfig::minRotationSpeed, RenderConfig::maxRotationSpeed))
    {
        scene.setRotateSpeed(rotateSpeed);
    }

    size_t numCubes = scene.getCubePositions().size();
    ImGui::Text("Total Cubes: %zu", numCubes);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate,
        ImGui::GetIO().Framerate);

    ImGui::End();

}
