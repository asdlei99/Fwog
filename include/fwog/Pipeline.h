#pragma once
#include <fwog/BasicTypes.h>
#include <fwog/detail/Flags.h>
#include <span>
#include <optional>

namespace Fwog
{
  class Shader;

  struct InputAssemblyState
  {
    PrimitiveTopology topology  = PrimitiveTopology::TRIANGLE_LIST;
    bool primitiveRestartEnable = false;
  };

  struct VertexInputBindingDescription
  {
    uint32_t location; // glEnableVertexArrayAttrib + glVertexArrayAttribFormat
    uint32_t binding;  // glVertexArrayAttribBinding
    Format format;     // glVertexArrayAttribFormat
    uint32_t offset;   // glVertexArrayAttribFormat
  };

  struct VertexInputState
  {
    std::span<const VertexInputBindingDescription> vertexBindingDescriptions = {};
  };

  // TODO: see what rasterization state can be dynamic instead
  struct RasterizationState
  {
    bool depthClampEnable         = false;
    PolygonMode polygonMode       = PolygonMode::FILL;
    CullMode cullMode             = CullMode::BACK;
    FrontFace frontFace           = FrontFace::COUNTERCLOCKWISE;
    bool depthBiasEnable          = false;
    float depthBiasConstantFactor = 0;
    float depthBiasSlopeFactor    = 0;
    //float depthBiasClamp; // no equivalent core OpenGL function
    float lineWidth               = 1; // glLineWidth
    float pointSize               = 1; // glPointSize
  };

  struct DepthState
  {
    bool depthTestEnable     = true;            // gl{Enable, Disable}(GL_DEPTH_TEST)
    bool depthWriteEnable    = true;            // glDepthMask(depthWriteEnable)
    CompareOp depthCompareOp = CompareOp::LESS; // glDepthFunc
    //bool depthBoundsTestEnable; // no equivalent core OpenGL function
    //float minDepthBounds;       // ???
    //float maxDepthBounds;       // ???

  };

  struct StencilOpState
  {
    StencilOp passOp      = StencilOp::KEEP;   // glStencilOp (dppass)
    StencilOp failOp      = StencilOp::KEEP;   // glStencilOp (sfail)
    StencilOp depthFailOp = StencilOp::KEEP;   // glStencilOp (dpfail)
    CompareOp compareOp   = CompareOp::ALWAYS; // glStencilFunc (func)
    uint32_t compareMask  = 0;                 // glStencilFunc (mask)
    uint32_t writeMask    = 0;                 // glStencilMask
    uint32_t reference    = 0;                 // glStencilFunc (ref)
  };

  struct StencilState
  {
    bool stencilTestEnable = false;
    StencilOpState front   = {};
    StencilOpState back    = {};
  };

  struct ColorBlendAttachmentState      // glBlendFuncSeparatei + glBlendEquationSeparatei
  {
    bool blendEnable = false;                                           // if false, blend factor = one?
    BlendFactor srcColorBlendFactor    = BlendFactor::ONE;              // srcRGB
    BlendFactor dstColorBlendFactor    = BlendFactor::ZERO;             // dstRGB
    BlendOp colorBlendOp               = BlendOp::ADD;                  // modeRGB
    BlendFactor srcAlphaBlendFactor    = BlendFactor::ONE;              // srcAlpha
    BlendFactor dstAlphaBlendFactor    = BlendFactor::ZERO;             // dstAlpha
    BlendOp alphaBlendOp               = BlendOp::ADD;                  // modeAlpha
    ColorComponentFlags colorWriteMask = ColorComponentFlag::RGBA_BITS; // glColorMaski
  };

  struct ColorBlendState
  {
    bool logicOpEnable                                     = false;          // gl{Enable, Disable}(GL_COLOR_LOGIC_OP)
    LogicOp logicOp                                        = LogicOp::COPY;  // glLogicOp(logicOp)
    std::span<const ColorBlendAttachmentState> attachments = {};             // glBlendFuncSeparatei + glBlendEquationSeparatei
    float blendConstants[4]                                = { 0, 0, 0, 0 }; // glBlendColor
  };

  struct GraphicsPipelineInfo
  {
    const Shader* vertexShader            = nullptr;
    const Shader* fragmentShader          = nullptr;
    InputAssemblyState inputAssemblyState = {};
    VertexInputState vertexInputState     = {};
    RasterizationState rasterizationState = {};
    DepthState depthState                 = {};
    StencilState stencilState             = {};
    ColorBlendState colorBlendState       = {};
    // Multisample state omitted (stretch goal)
    // Tessellation state omitted (stretch goal)
  };

  struct ComputePipelineInfo
  {
    const Shader* shader;
  };

  struct GraphicsPipeline
  {
    auto operator<=>(const GraphicsPipeline&) const = default;
    uint64_t id;
  };
  struct ComputePipeline
  {
    auto operator<=>(const ComputePipeline&) const = default;
    uint64_t id;
  };

  std::optional<GraphicsPipeline> CompileGraphicsPipeline(const GraphicsPipelineInfo& info);
  bool DestroyGraphicsPipeline(GraphicsPipeline pipeline);

  std::optional<ComputePipeline> CompileComputePipeline(const ComputePipelineInfo& info);
  bool DestroyComputePipeline(ComputePipeline pipeline);
}