#pragma once

#include "vk-context.hpp"

namespace gt::vk
{
    void createSwapChain(VulkanContext &context, int32_t frameBufferWidth, int32_t frameBufferHeight);
    void destroySwapChain(const VulkanContext &c_context);
}