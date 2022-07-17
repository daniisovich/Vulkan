set compiler_path="../../../vendor/vulkan/Bin/glslc.exe"

%compiler_path% glsl/shader.vert -o spir-v/vert.spv
%compiler_path% glsl/shader.frag -o spir-v/frag.spv