#pragma once

// Base
#include "Application.h"
#include "Engine.h"

// Logging
#include "utils/Debug.h"

// Input Method
#include "input/InputManager.h"

// Graphics And Vertex
#include "graphics/ShaderProgram.h"
#include "graphics/GraphicsAPI.h"
#include "graphics/VertexLayout.h"
#include "graphics/Texture.h"

// Rendering
#include "render/Material.h"
#include "render/Mesh.h"
#include "render/RenderQueue.h"

// Scene And GameObject
#include "scene/GameObject.h"
#include "scene/Scene.h"
#include "scene/Component.h"

// Components
#include "scene/components/MeshComponent.h"
#include "scene/components/CameraComponent.h"
#include "scene/components/PlayerControllerComponent.h"
#include "scene/components/LightComponent.h"
#include "scene/components/AnimationComponent.h"
#include "scene/components/PhysicsComponent.h"

// IO
#include "io/FileSystem.h"

// Physics
#include "physics/PhysicsManager.h"
#include "physics/Collider.h"
#include "physics/RigidBody.h"