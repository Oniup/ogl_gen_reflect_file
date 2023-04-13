#ifndef __TEST_GAME_COMPONENTS_HPP__
#define __TEST_GAME_COMPONENTS_HPP__

#include "ogl/ogl.hpp"

namespace game_test {

    struct HealthComponent {
        OGL_SERIALIZABLE_OBJECT()

        int health{ 175 };
        int shield{ 25 };
    }

    struct PlayerComponent {
        OGL_SERIALIZABLE_OBJECT()

        float move_speed{ 20.0f };

        OGL_NO_SERIALIZE()
        ogl::ColliderComponent* component{ nullptr };

        OGL_NO_REFLECT()
        glm::vec3 move_direction{};
    };

    struct EnemyComponent {
        OGL_SERIALIZABLE_OBJECT()

        float move_speed{ 20.0f };
        int damage{ 27 };

        OGL_NO_SERIALIZE()
        ogl::TransformComponent* target{ nullptr };
        OGL_NO_SERIALIZE()
        ogl::ColliderComponent* component{ nullptr };

        OGL_NO_REFLECT()
        glm::vec3 move_direction{};
    }

}

#endif