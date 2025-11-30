#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

class Scene;

class ImGuiLayer
{
public:
	void renderGUI(Scene& scene);
};

