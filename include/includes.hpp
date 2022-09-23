#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "setup_model.hpp"
#include "renderSSAA.hpp"
#include "output_quad.hpp"
#include "shadow_maps.hpp"