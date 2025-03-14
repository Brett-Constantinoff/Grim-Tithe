#pragma once

#include "r_context.hpp"

namespace gt::renderer
{
    void createCommandPool(VulkanContext &context);
    void destroyCommandPool(const VulkanContext& context);
    void createCommandBuffer(VulkanContext& context);
    void render(VulkanContext &context, uint32_t currentFrame);
} // namespace gt::renderer