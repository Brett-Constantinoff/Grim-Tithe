#pragma once

#include "vk-context.hpp"

namespace gt::vk
{
    void createPipeline(VulkanContext &context);
    void destroyPipeline(const VulkanContext &c_context);
    void createRenderPass(VulkanContext &context);
    void destroyRenderPass(const VulkanContext &c_context);
} // namespace gt::vk