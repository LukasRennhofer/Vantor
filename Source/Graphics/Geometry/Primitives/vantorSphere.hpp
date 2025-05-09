/*
*    				~ Vantor ~
*
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 25.04.2025
*
* File: vantorSphere.hpp
* Last Change:
*/

#pragma once

#include "../../RenderDevice/vantorRenderDevice.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for glm::two_pi

#include <cmath>

namespace vantor::Graphics::Geometry::Primitives {
    class Sphere : public vantor::Graphics::RenderDevice::Mesh
    {
    public:
        Sphere(unsigned int xSegments, unsigned int ySegments)
        {
            for (unsigned int y = 0; y <= ySegments; ++y)
            {
                for (unsigned int x = 0; x <= xSegments; ++x)
                {
                    float xSegment = static_cast<float>(x) / static_cast<float>(xSegments);
                    float ySegment = static_cast<float>(x) / static_cast<float>(ySegments);
                    float xPos = std::cos(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());
                    float yPos = std::cos(ySegment * glm::pi<float>());
                    float zPos = std::sin(xSegment * glm::two_pi<float>()) * std::sin(ySegment * glm::pi<float>());

                    Positions.push_back(glm::vec3(xPos, yPos, zPos));
                    UV.push_back(glm::vec2(xSegment, ySegment));
                    Normals.push_back(glm::vec3(xPos, yPos, zPos));
                }
            }

            bool oddRow = false;
            for (int y = 0; y < ySegments; ++y)
            {
                for (int x = 0; x < xSegments; ++x)
                {
                    Indices.push_back((y + 1) * (xSegments + 1) + x);
                    Indices.push_back(y       * (xSegments + 1) + x);
                    Indices.push_back(y       * (xSegments + 1) + x + 1);

                    Indices.push_back((y + 1) * (xSegments + 1) + x);
                    Indices.push_back(y       * (xSegments + 1) + x + 1);
                    Indices.push_back((y + 1) * (xSegments + 1) + x + 1);
                }
            }

            Topology = TRIANGLES;
            Finalize();
        }
    };
}