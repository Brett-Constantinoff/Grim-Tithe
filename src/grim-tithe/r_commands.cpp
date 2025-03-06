#include "m_utilities.hpp"

#include "r_commands.hpp"
#include "r_utilities.hpp"

namespace gt::renderer
{
    void
        createCommandPool(VulkanContext& context)
    {
        QueueFamilyIndices indices = getQueueFamilies(context.physicalDevice, context.surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = indices.graphicsFamily;

        gtAssert(vkCreateCommandPool(context.device, &poolInfo, nullptr, &context.commandPool) == VK_SUCCESS);
    }

    void
        destroyCommandPool(const VulkanContext& c_context)
    {
        vkDestroyCommandPool(c_context.device, c_context.commandPool, nullptr);
    }

    void
        createCommandBuffer(VulkanContext& context)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool        = context.commandPool;
        allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = context.c_framesInFlight;

        gtAssert(vkAllocateCommandBuffers(context.device, &allocInfo, &context.commandBuffers[0]) == VK_SUCCESS);
    }

    static void
        recordCommand(VulkanContext& context, uint32_t image, uint32_t currentFrame)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        gtAssert(vkBeginCommandBuffer(context.commandBuffers[currentFrame], &beginInfo) == VK_SUCCESS);

        VkClearValue clearValues[1]{}; 
        clearValues[0].color = {0.3f, 0.0f, 0.6f, 1.0f};

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass  = context.renderPass;
        renderPassInfo.framebuffer = context.framebuffers[image];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = context.extent;
        renderPassInfo.clearValueCount   = 1; 
        renderPassInfo.pClearValues      = clearValues;

        vkCmdBeginRenderPass(context.commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(context.commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipeline);

        VkViewport viewport{};
        viewport.x        = 0.0f;
        viewport.y        = 0.0f;
        viewport.width    = static_cast<float>(context.extent.width);
        viewport.height   = static_cast<float>(context.extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(context.commandBuffers[currentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = context.extent;
        vkCmdSetScissor(context.commandBuffers[currentFrame], 0, 1, &scissor);

        vkCmdDraw(context.commandBuffers[currentFrame], 3, 1, 0, 0);

        vkCmdEndRenderPass(context.commandBuffers[currentFrame]);

        gtAssert(vkEndCommandBuffer(context.commandBuffers[currentFrame]) == VK_SUCCESS);
    }

    void
        render(VulkanContext& context, uint32_t currentFrame)
    {
        vkWaitForFences(context.device, 1, &context.inFlightFence[currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(context.device, 1, &context.inFlightFence[currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(context.device, context.swapChain, UINT64_MAX, context.imageAvailableSemaphore[currentFrame],
                              VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(context.commandBuffers[currentFrame], 0);

        recordCommand(context, imageIndex, currentFrame);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore          waitSemaphores[] = {context.imageAvailableSemaphore[currentFrame]};
        VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submitInfo.waitSemaphoreCount         = 1;
        submitInfo.pWaitSemaphores            = waitSemaphores;
        submitInfo.pWaitDstStageMask          = waitStages;
        submitInfo.commandBufferCount         = 1;
        submitInfo.pCommandBuffers            = &context.commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[]  = {context.renderFinishedSemaphore[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = signalSemaphores;

        gtAssert(vkQueueSubmit(context.graphicsQueue, 1, &submitInfo, context.inFlightFence[currentFrame]) == VK_SUCCESS);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = signalSemaphores;

        VkSwapchainKHR swapChains[] = {context.swapChain};
        presentInfo.swapchainCount  = 1;
        presentInfo.pSwapchains     = swapChains;
        presentInfo.pImageIndices   = &imageIndex;

        vkQueuePresentKHR(context.presentQueue, &presentInfo);
    }
} // namespace gt::renderer