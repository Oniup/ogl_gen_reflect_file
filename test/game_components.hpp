/****************************************************************************/
/* game_components.hpp                                                      */
/* This file is part of: ogl_gen_reflection_file                            */
/****************************************************************************/
/* Copyright 2023 Ewan Robson                                               */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the "License");          */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an "AS IS" BASIS,        */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/****************************************************************************/

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