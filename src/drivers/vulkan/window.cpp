#include "drivers/vulkan/window.hpp"

#include <vkbootstrap/VkBootstrap.h>

#include "drivers/vulkan/commands.hpp"
#include "drivers/vulkan/initializers.hpp"
#include "drivers/vulkan/memory.hpp"
namespace hydra {
    namespace drivers {
        namespace vlkn {
            Window::Window(BasicData* basicDataPtr, VkSurfaceKHR surface, vk::Extent2D extent)
            {
                m_basicDataPtr = basicDataPtr;
                m_windowData.surface = std::make_unique<vk::raii::SurfaceKHR>(*basicDataPtr->instance, surface);
                m_windowData.extent = extent;
                vkb::SwapchainBuilder swapchainBuilder{**m_basicDataPtr->physicalDevice, **m_basicDataPtr->device, surface};
                // **m_windowData.surface->
                vkb::Swapchain vkbSwapchain = swapchainBuilder
                                                  .use_default_format_selection()
                                                  // use vsync present mode
                                                  .set_desired_present_mode(VK_PRESENT_MODE_FIFO_RELAXED_KHR)
                                                  .set_desired_extent(m_windowData.extent.width, m_windowData.extent.height)

                                                  .build()
                                                  .value();

                m_windowData.swapchain = std::make_unique<vk::raii::SwapchainKHR>(*m_basicDataPtr->device, vkbSwapchain.swapchain);
                m_windowData.swapchainImageFormat = vk::Format(vkbSwapchain.image_format);
                // m_windowData.swapchainImages = m_windowData.swapchain->getImages();
                for (const auto& img : m_windowData.swapchain->getImages()) {
                    auto swapImgViewCI = init::imageViewCreateInfo(m_windowData.swapchainImageFormat, img, vk::ImageAspectFlagBits::eColor);
                    m_windowData.swapchainImageViews.push_back(m_basicDataPtr->device->createImageView(swapImgViewCI));
                }

                // RAWIMAGE
                {
                    auto imgCI = init::imageCreateInfo(m_windowData.swapchainImageFormat,
                                                       vk::ImageUsageFlagBits::eColorAttachment |
                                                           vk::ImageUsageFlagBits::eTransferSrc |
                                                           vk::ImageUsageFlagBits::eSampled,
                                                       {m_windowData.extent.width, m_windowData.extent.height, 1});
                    imgCI.mipLevels = 1;
                    imgCI.arrayLayers = 1;
                    imgCI.initialLayout = vk::ImageLayout::ePreinitialized;
                    imgCI.imageType = vk::ImageType::e2D;
                    imgCI.tiling = vk::ImageTiling::eOptimal;
                    auto imgViewCI = init::imageViewCreateInfo(m_windowData.swapchainImageFormat, {}, vk::ImageAspectFlagBits::eColor);

                    m_windowData.rawImage = std::make_unique<AllocatedImage>(std::move(m_basicDataPtr->memoryAllocator->allocateImage(imgCI, imgViewCI)));
                }

                // DEPTH STENCIL
                {
                    auto dsCI = init::depthStencilImageCreateInfo(1024, 1024);
                    auto dsViewCI = init::imageViewCreateInfo(dsCI.format, {}, vk::ImageAspectFlagBits::eDepth);

                    m_windowData.depthStencil = std::make_unique<AllocatedImage>(std::move(m_basicDataPtr->memoryAllocator->allocateImage(dsCI, dsViewCI)));
                }

                // RENDERPASS
                {
                    vk::RenderPassCreateInfo rpCI{};

                    std::vector<vk::SubpassDescription> subpassDescrs;

                    std::vector<vk::SubpassDependency> subpassDeps;
                    std::vector<vk::AttachmentDescription> attchDescrs;
                    std::vector<vk::AttachmentReference> attchRefs;
                    vk::AttachmentReference depthStencilAttachmentRef;
                    {
                        vk::AttachmentDescription attchDescr{};
                        attchDescr.format = m_windowData.swapchainImageFormat;
                        attchDescr.loadOp = vk::AttachmentLoadOp::eClear;
                        attchDescr.storeOp = vk::AttachmentStoreOp::eStore;
                        attchDescr.finalLayout = vk::ImageLayout::ePresentSrcKHR;
                        attchDescr.samples = vk::SampleCountFlagBits::e1;
                        attchDescrs.push_back(attchDescr);

                        attchDescr = vk::AttachmentDescription{};
                        attchDescr.format = vk::Format::eD24UnormS8Uint;
                        attchDescr.loadOp = vk::AttachmentLoadOp::eClear;
                        attchDescr.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
                        attchDescr.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
                        attchDescr.samples = vk::SampleCountFlagBits::e1;
                        attchDescrs.push_back(attchDescr);

                        rpCI.attachmentCount = attchDescrs.size();
                        rpCI.pAttachments = attchDescrs.data();
                    }
                    {
                        depthStencilAttachmentRef = {(uint32_t)1, vk::ImageLayout::eDepthStencilAttachmentOptimal};
                        attchRefs.push_back({(uint32_t)0, vk::ImageLayout::eColorAttachmentOptimal});

                        vk::SubpassDescription subpassDescr{};
                        subpassDescr.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
                        subpassDescr.pColorAttachments = attchRefs.data();
                        subpassDescr.colorAttachmentCount = attchRefs.size();
                        subpassDescr.pDepthStencilAttachment = &depthStencilAttachmentRef;

                        subpassDescrs.push_back(subpassDescr);

                        rpCI.subpassCount = subpassDescrs.size();
                        rpCI.pSubpasses = subpassDescrs.data();
                    }
                    {
                        vk::SubpassDependency subpassDep{};
                        subpassDep.srcSubpass = VK_SUBPASS_EXTERNAL;
                        subpassDep.dstSubpass = 0;
                        subpassDep.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                        subpassDep.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                        subpassDep.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
                        subpassDeps.push_back(std::move(subpassDep));

                        rpCI.dependencyCount = subpassDeps.size();
                        rpCI.pDependencies = subpassDeps.data();
                    }
                    m_renderData.renderPass = std::make_unique<vk::raii::RenderPass>(*m_basicDataPtr->device, rpCI);
                }

                // FRAMEBUFFERS
                for (const auto& image : m_windowData.swapchainImageViews) {
                    auto attch = std::vector<vk::ImageView>{*image, m_windowData.depthStencil->imageView()};
                    vk::FramebufferCreateInfo fbCI{};
                    fbCI.renderPass = **m_renderData.renderPass;
                    fbCI.pAttachments = attch.data();
                    fbCI.attachmentCount = attch.size();
                    fbCI.width = m_windowData.extent.width;
                    fbCI.height = m_windowData.extent.height;
                    fbCI.layers = 1;

                    m_windowData.framebuffers.push_back({*m_basicDataPtr->device, fbCI});
                }

                for (uint32_t i = 0; i < m_basicDataPtr->bufferedFrames; i++) {
                    vk::FenceCreateInfo fCI;
                    fCI.flags = vk::FenceCreateFlagBits::eSignaled;
                    typedef vk::CommandPoolCreateFlagBits cpbits;
                    vk::SemaphoreCreateInfo semCI;
                    m_renderData.framesData.push_back(
                        FrameData{
                            .commandBuffer = {m_basicDataPtr->commandBufferAllocator->allocateCommandBuffer({}, cpbits::eResetCommandBuffer)},
                            .fence = {*m_basicDataPtr->device, fCI},
                            .presentSemaphore = {*m_basicDataPtr->device, semCI},
                            .renderSemaphore = {*m_basicDataPtr->device, semCI},
                        });
                }

                typedef vk::CommandPoolCreateFlagBits cpbits;

                m_renderData.contextCommandBuffer = std::make_unique<vk::raii::CommandBuffer>(m_basicDataPtr->commandBufferAllocator->allocateCommandBuffer({}, cpbits::eResetCommandBuffer));

                vk::FenceCreateInfo fci;
                fci.flags = vk::FenceCreateFlagBits::eSignaled;
                m_renderData.contextFence = std::make_unique<vk::raii::Fence>(m_basicDataPtr->device->createFence(fci));
            }
            void Window::draw()
            {
                auto umax = std::numeric_limits<uint64_t>::max();

                auto* frame = &m_renderData.framesData[m_renderData.currentFrame % m_basicDataPtr->bufferedFrames];

                {
                    std::vector<vk::Fence> fences{*frame->fence};
                    if (m_basicDataPtr->device->waitForFences(fences, 1, 1000000000) != vk::Result::eSuccess) {}
                    m_basicDataPtr->device->resetFences(fences);
                }
                frame->commandBuffer.reset();

                vk::AcquireNextImageInfoKHR acqImgInfo{};
                acqImgInfo.swapchain = **m_windowData.swapchain;

                acqImgInfo.semaphore = *frame->presentSemaphore;
                acqImgInfo.timeout = umax;
                acqImgInfo.deviceMask = 1;

                auto [result, imageIndex] = m_basicDataPtr->device->acquireNextImage2KHR(acqImgInfo);
                if (result != vk::Result::eSuccess) {
                    return;
                }

                vk::ClearDepthStencilValue clearDepthValue{1.0f, 0};
                std::array<vk::ClearValue, 2> clearColours{vk::ClearValue(std::array<float, 4>{0.75f, 0.75f, 0.75f, 1}), clearDepthValue};
                vk::RenderPassBeginInfo rpbi;
                rpbi.renderPass = **m_renderData.renderPass;
                rpbi.framebuffer = *m_windowData.framebuffers[imageIndex];
                rpbi.renderArea = vk::Rect2D{{0, 0}, m_windowData.extent};
                rpbi.clearValueCount = (uint32_t)clearColours.size();
                rpbi.pClearValues = clearColours.data();

                vk::CommandBufferBeginInfo bi{};
                frame->commandBuffer.begin(bi);
                frame->commandBuffer.beginRenderPass(rpbi, vk::SubpassContents::eInline);
                // COMMAND BUFFER
                frame->commandBuffer.endRenderPass();
                frame->commandBuffer.end();
                vk::PipelineStageFlags waitStages = vk::PipelineStageFlagBits::eColorAttachmentOutput;
                {
                    vk::SubmitInfo submit;

                    submit.waitSemaphoreCount = 1;
                    submit.pWaitSemaphores = &*frame->presentSemaphore;
                    submit.pWaitDstStageMask = &waitStages;
                    submit.commandBufferCount = 1;
                    submit.pCommandBuffers = &*frame->commandBuffer;
                    submit.signalSemaphoreCount = 1;
                    submit.pSignalSemaphores = &*frame->renderSemaphore;
                    m_basicDataPtr->graphicsQueue->submit({submit}, *frame->fence);
                }
                std::vector<vk::SwapchainKHR> swapchains{**m_windowData.swapchain};
                vk::PresentInfoKHR presentInfo;

                presentInfo.pSwapchains = swapchains.data();
                presentInfo.swapchainCount = swapchains.size();
                presentInfo.pImageIndices = &imageIndex;
                presentInfo.waitSemaphoreCount = 1;
                presentInfo.pWaitSemaphores = &*frame->renderSemaphore;

                try {
                    if (m_basicDataPtr->graphicsQueue->presentKHR(presentInfo) != vk::Result::eSuccess) {}
                }
                catch (const vk::OutOfDateKHRError) {
                }
                m_renderData.currentFrame++;
            }

        }  // namespace vlkn
    }      // namespace drivers
}  // namespace hydra