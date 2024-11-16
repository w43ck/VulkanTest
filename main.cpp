#include <vulkan/vulkan.hpp>

void InitVulkan() {
	
	static vk::DynamicLoader dL;
	
	static bool uninitialized = 1;
	if(uninitialized) {
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dL.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		static vk::DispatchLoaderDynamic dispatcher(vkGetInstanceProcAddr);
		uninitialized = 0;
	}	
}