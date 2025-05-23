/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorLight.h
 *  Last Change: Automatically updated
 */

#pragma once

#include "Renderer/vantorShader.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace vantor::Graphics
{
    class LightException : public QuarkException
    {
            using QuarkException::QuarkException;
    };

    struct Attenuation
    {
            float constant;
            float linear;
            float quadratic;
    };

    // TODO: Change this to [0, 0, 1].
    constexpr Attenuation DEFAULT_ATTENUATION = {1.0f, 0.09f, 0.032f};
    constexpr glm::vec3   DEFAULT_DIFFUSE     = glm::vec3(0.5f, 0.5f, 0.5f);
    constexpr glm::vec3   DEFAULT_SPECULAR    = glm::vec3(1.0f, 1.0f, 1.0f);

    constexpr float DEFAULT_INNER_ANGLE = glm::radians(10.5f);
    constexpr float DEFAULT_OUTER_ANGLE = glm::radians(19.5f);

    enum class LightType
    {
        DIRECTIONAL_LIGHT,
        POINT_LIGHT,
        SPOT_LIGHT,
    };

    class LightRegistry;

    class Light
    {
        public:
            virtual ~Light()                       = default;
            virtual LightType getLightType() const = 0;

            void setUseViewTransform(bool useViewTransform) { useViewTransform_ = useViewTransform; }

            friend LightRegistry;

        protected:
            void setLightIdx(unsigned int lightIdx)
            {
                lightIdx_    = lightIdx;
                uniformName_ = getUniformName(lightIdx);
            }

            void checkState()
            {
                if (hasViewDependentChanged_ && !hasViewBeenApplied_)
                {
                    throw LightException("ERROR::LIGHT::VIEW_CHANGED\n"
                                         "Light state changed without "
                                         "re-applying view transform.");
                }
            }

            void resetChangeDetection()
            {
                hasViewDependentChanged_ = false;
                hasLightChanged_         = false;
                hasViewBeenApplied_      = false;
            }

            virtual std::string getUniformName(unsigned int lightIdx) const                        = 0;
            virtual void        updateUniforms(vantor::Graphics::Renderer::Shader::Shader &shader) = 0;
            virtual void        applyViewTransform(const glm::mat4 &view)                          = 0;

            unsigned int lightIdx_;
            std::string  uniformName_;

            bool useViewTransform_        = true;
            bool hasViewDependentChanged_ = true;
            bool hasLightChanged_         = true;

            bool hasViewBeenApplied_ = false;
    };

    class ViewSource
    {
        public:
            virtual glm::mat4 getViewTransform() const = 0;
    };

    class LightRegistry : public UniformSource
    {
        public:
            virtual ~LightRegistry() = default;
            void addLight(std::shared_ptr<Light> light);
            void setViewSource(std::shared_ptr<ViewSource> viewSource) { viewSource_ = viewSource; }
            void updateUniforms(vantor::Graphics::Renderer::Shader::Shader &shader);
            void applyViewTransform(const glm::mat4 &view);
            void setUseViewTransform(bool useViewTransform);

        private:
            unsigned int directionalCount_ = 0;
            unsigned int pointCount_       = 0;
            unsigned int spotCount_        = 0;

            std::shared_ptr<ViewSource>         viewSource_;
            std::vector<std::shared_ptr<Light>> lights_;
    };

    class DirectionalLight : public Light
    {
        public:
            DirectionalLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
                             glm::vec3 diffuse   = DEFAULT_DIFFUSE,
                             // TODO: Change this to default to whatever the diffuse was.
                             glm::vec3 specular = DEFAULT_SPECULAR);

            LightType getLightType() const { return LightType::DIRECTIONAL_LIGHT; }

            glm::vec3 getDirection() const { return direction_; }
            void      setDirection(glm::vec3 direction)
            {
                direction_               = direction;
                hasViewDependentChanged_ = true;

            private:
                glm::vec3 position_;
                glm::vec3 viewPosition_;

                glm::vec3 diffuse_;
                glm::vec3 specular_;

                Attenuation attenuation_;
            };

            class SpotLight : public Light
            {
                public:
                    SpotLight(glm::vec3   position    = glm::vec3(0.0f, 1.0f, 0.0f),
                              glm::vec3   direction   = glm::vec3(0.0f, -1.0f, 0.0f),
                              float       innerAngle  = DEFAULT_INNER_ANGLE,
                              float       outerAngle  = DEFAULT_OUTER_ANGLE,
                              glm::vec3   diffuse     = DEFAULT_DIFFUSE,
                              glm::vec3   specular    = DEFAULT_SPECULAR,
                              Attenuation attenuation = DEFAULT_ATTENUATION);

                    LightType getLightType() const { return LightType::SPOT_LIGHT; }

                    glm::vec3 getPosition() const { return position_; }
                    void      setPosition(glm::vec3 position)
                    {
                        position_                = position;
                        hasViewDependentChanged_ = true;
                    }
                    glm::vec3 getDirection() const { return direction_; }
                    void      setDirection(glm::vec3 direction)
                    {
                        direction_               = direction;
                        hasViewDependentChanged_ = true;
                    }
                    glm::vec3 getDiffuse() const { return diffuse_; }
                    void      setDiffuse(glm::vec3 diffuse)
                    {
                        diffuse_         = diffuse;
                        hasLightChanged_ = true;
                    }
                    glm::vec3 getSpecular() const { return specular_; }
                    void      setSpecular(glm::vec3 specular)
                    {
                        specular_        = specular;
                        hasLightChanged_ = true;
                    }
                    Attenuation getAttenuation() const { return attenuation_; }
                    void        setAttenuation(Attenuation attenuation)
                    {
                        attenuation_     = attenuation;
                        hasLightChanged_ = true;
                    }

                protected:
                    std::string getUniformName(unsigned int lightIdx) const { return "vantor_spotLights[" + std::to_string(lightIdx) + "]"; }
                    void        updateUniforms(vantor::Graphics::Renderer::Shader::Shader &shader);
                    void        applyViewTransform(const glm::mat4 &view);

                private:
                    glm::vec3 position_;
                    glm::vec3 viewPosition_;
                    glm::vec3 direction_;
                    glm::vec3 viewDirection_;

                    float innerAngle_;
                    float outerAngle_;

                    glm::vec3 diffuse_;
                    glm::vec3 specular_;

                    Attenuation attenuation_;
            };
    }
} // namespace vantor::Graphics