#pragma once

#include "r_context.hpp"

namespace gt::renderer
{
    void createSwapChain(VulkanContext &context, int32_t frameBufferWidth, int32_t frameBufferHeight);
    void destroySwapChain(const VulkanContext &c_context);
    void createImageViews(VulkanContext& context);
    void destroyImageViews(const VulkanContext &c_context);
    void createFramebuffers(VulkanContext &context);
    void destroyFramebuffers(const VulkanContext& c_context);
} // namespace gt::renderer