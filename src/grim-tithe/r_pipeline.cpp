#include <array>
#include <filesystem>

#include "m_utilities.hpp"
#include "r_pipeline.hpp"

namespace gt::renderer
{
    using namespace gt;

    static void
        createComputeDescriptorSetLayout(VulkanContext &context)
    {
        std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
        layoutBindings[0].binding            = 0;
        layoutBindings[0].descriptorCount    = 1;
        layoutBindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        layoutBindings[0].pImmutableSamplers = nullptr;
        layoutBindings[0].stageFlags         = VK_SHADER_STAGE_COMPUTE_BIT;

        layoutBindings[1].binding            = 1;
        layoutBindings[1].descriptorCount    = 1;
        layoutBindings[1].descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        layoutBindings[1].pImmutableSamplers = nullptr;
        layoutBindings[1].stageFlags         = VK_SHADER_STAGE_COMPUTE_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 2;
        layoutInfo.pBindings    = layoutBindings.data();

        gtAssert(vkCreateDescriptorSetLayout(context.device, &layoutInfo, nullptr,
                                             &context.computeDescriptorSetLayout) == VK_SUCCESS);
    }

    static void
        compileShader(const char *c_filePath, const char *c_fileName, const char *c_compiledFileName)
    {
        const char *c_vulkanSdk = std::getenv("VULKAN_SDK");
        gtAssert(c_vulkanSdk != nullptr);

        char command[CHAR_MAX] = {0};

        misc::concatString(&(command[0]), c_vulkanSdk, sizeof(command));
        misc::concatString(&(command[0]), "/Bin/glslc.exe ", sizeof(command));
        misc::concatString(&(command[0]), c_filePath, sizeof(command));
        misc::concatString(&(command[0]), c_fileName, sizeof(command));
        misc::concatString(&(command[0]), " -o ", sizeof(command));
        misc::concatString(&(command[0]), c_filePath, sizeof(command));
        misc::concatString(&(command[0]), c_compiledFileName, sizeof(command));

        int32_t result = std::system(command);
        gtAssert(result == 0);
    }

    static VkShaderModule
        createShader(const VulkanContext &c_context, const char *c_filePath, const char *c_fileName,
                     const char *c_compiledFileName)
    {
        compileShader(c_filePath, c_fileName, c_compiledFileName);

        char compiledShaderPath[CHAR_MAX] = {0};

        misc::concatString(&compiledShaderPath[0], c_filePath, sizeof(compiledShaderPath));
        misc::concatString(&compiledShaderPath[0], c_compiledFileName, sizeof(compiledShaderPath));

        std::vector<char> fileData{};
        misc::readFile(fileData, compiledShaderPath);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = fileData.size();
        createInfo.pCode    = reinterpret_cast<const uint32_t *>(fileData.data());

        VkShaderModule module = VK_NULL_HANDLE;
        gtAssert(vkCreateShaderModule(c_context.device, &createInfo, nullptr, &module) == VK_SUCCESS);

        return module;
    }

    void
        createPipeline(VulkanContext &context)
    {
        const char *c_shaderPath = "resources/shaders/";

        VkShaderModule vertShaderModule = createShader(context, c_shaderPath, "mainShader.vert", "vert.spv");
        VkShaderModule fragShaderModule = createShader(context, c_shaderPath, "mainShader.frag", "frag.spv");

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName  = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName  = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        // not loading any data yet
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount   = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;

        // good ol triangles
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x        = 0.0f;
        viewport.y        = 0.0f;
        viewport.width    = static_cast<float>(context.extent.width);
        viewport.height   = static_cast<float>(context.extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = context.extent;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports    = &viewport;
        viewportState.scissorCount  = 1;
        viewportState.pScissors     = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable        = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;

        // TODO - look into if VK_POLYGON_MODE_LINE would be better
        rasterizer.polygonMode     = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth       = 1.0f;
        rasterizer.cullMode        = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace       = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable  = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable     = VK_FALSE;
        colorBlending.logicOp           = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount   = 1;
        colorBlending.pAttachments      = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount         = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        gtAssert(vkCreatePipelineLayout(context.device, &pipelineLayoutInfo, nullptr, &context.layout) == VK_SUCCESS);

        std::vector<VkDynamicState>      dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates    = dynamicStates.data();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount          = 2;
        pipelineInfo.pStages             = shaderStages;
        pipelineInfo.pVertexInputState   = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState      = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState   = &multisampling;
        pipelineInfo.pColorBlendState    = &colorBlending;
        pipelineInfo.pDynamicState       = &dynamicState;
        pipelineInfo.layout              = context.layout;
        pipelineInfo.renderPass          = context.renderPass;
        pipelineInfo.subpass             = 0;

        gtAssert(vkCreateGraphicsPipelines(context.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
                                           &context.pipeline) == VK_SUCCESS);

        vkDestroyShaderModule(context.device, fragShaderModule, nullptr);
        vkDestroyShaderModule(context.device, vertShaderModule, nullptr);
    }

    void
        createComputePipeline(VulkanContext &context)
    {
        createComputeDescriptorSetLayout(context);

        const char *c_shaderPath = "resources/shaders/";

        VkShaderModule computeShaderModule = createShader(context, c_shaderPath, "mainShader.comp", "comp.spv");

        VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
        computeShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        computeShaderStageInfo.stage  = VK_SHADER_STAGE_COMPUTE_BIT;
        computeShaderStageInfo.module = computeShaderModule;
        computeShaderStageInfo.pName  = "main";

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts    = &context.computeDescriptorSetLayout;

        gtAssert(vkCreatePipelineLayout(context.device, &pipelineLayoutInfo, nullptr, &context.computePipelineLayout) ==
                 VK_SUCCESS);

        VkComputePipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineInfo.layout = context.computePipelineLayout;
        pipelineInfo.stage  = computeShaderStageInfo;

        gtAssert(vkCreateComputePipelines(context.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
                                          &context.computePipeline) == VK_SUCCESS);

        vkDestroyShaderModule(context.device, computeShaderModule, nullptr);
    }

    void
        destroyPipeline(const VulkanContext &c_context)
    {
        vkDestroyPipeline(c_context.device, c_context.pipeline, nullptr);
        vkDestroyPipelineLayout(c_context.device, c_context.layout, nullptr);
    }

    void
        destroyComputePipeline(const VulkanContext &c_context)
    {
        vkDestroyPipeline(c_context.device, c_context.computePipeline, nullptr);
        vkDestroyPipelineLayout(c_context.device, c_context.computePipelineLayout, nullptr);
    }

    void
        createRenderPass(VulkanContext &context)
    {
        // just a color buffer should do it
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format         = context.format;
        colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments    = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments    = &colorAttachment;
        renderPassInfo.subpassCount    = 1;
        renderPassInfo.pSubpasses      = &subpass;

        VkSubpassDependency dependency{};
        dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass          = 0;
        dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask       = 0;
        dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies   = &dependency;

        gtAssert(vkCreateRenderPass(context.device, &renderPassInfo, nullptr, &context.renderPass) == VK_SUCCESS);
    }

    void
        destroyRenderPass(const VulkanContext &c_context)
    {
        vkDestroyRenderPass(c_context.device, c_context.renderPass, nullptr);
    }

    void
        destroyDescriptorSetLayout(const VulkanContext &c_context)
    {
        vkDestroyDescriptorSetLayout(c_context.device, c_context.computeDescriptorSetLayout, nullptr);
    }

} // namespace gt::renderer
