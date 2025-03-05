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
        allocInfo.commandBufferCount = 1;

        gtAssert(vkAllocateCommandBuffers(context.device, &allocInfo, &context.commandBuffer) == VK_SUCCESS);
    }

    static void
        recordCommand(VulkanContext& context, uint32_t image)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        gtAssert(vkBeginCommandBuffer(context.commandBuffer, &beginInfo) == VK_SUCCESS);

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

        vkCmdBeginRenderPass(context.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(context.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, context.pipeline);

        VkViewport viewport{};
        viewport.x        = 0.0f;
        viewport.y        = 0.0f;
        viewport.width    = static_cast<float>(context.extent.width);
        viewport.height   = static_cast<float>(context.extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(context.commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = context.extent;
        vkCmdSetScissor(context.commandBuffer, 0, 1, &scissor);

        vkCmdDraw(context.commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(context.commandBuffer);

        gtAssert(vkEndCommandBuffer(context.commandBuffer) == VK_SUCCESS);
    }

    void
        render(VulkanContext& context)
    {
        vkWaitForFences(context.device, 1, &context.inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(context.device, 1, &context.inFlightFence);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(context.device, context.swapChain, UINT64_MAX, context.imageAvailableSemaphore,
                              VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(context.commandBuffer, 0);

        recordCommand(context, imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore          waitSemaphores[] = {context.imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submitInfo.waitSemaphoreCount         = 1;
        submitInfo.pWaitSemaphores            = waitSemaphores;
        submitInfo.pWaitDstStageMask          = waitStages;
        submitInfo.commandBufferCount         = 1;
        submitInfo.pCommandBuffers            = &context.commandBuffer;

        VkSemaphore signalSemaphores[]  = {context.renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = signalSemaphores;

        gtAssert(vkQueueSubmit(context.graphicsQueue, 1, &submitInfo, context.inFlightFence) == VK_SUCCESS);

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