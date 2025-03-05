#pragma once

#include "r_context.hpp"

namespace gt::renderer
{
    void createPipeline(VulkanContext &context);
    void destroyPipeline(const VulkanContext &c_context);
    void createRenderPass(VulkanContext &context);
    void destroyRenderPass(const VulkanContext &c_context);
} // namespace gt::renderer