module;
#include "vulkan/vulkan.h" 

export module Aether:VulkanDebug;
import :Vector;
import <string>;

export namespace Aether
{
	namespace VulkanDebug
	{
		extern bool logToFile;
		extern std::string logFileName;

		// Default debug callback
		VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessageCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		// Load debug function pointers and set debug callback
		void setupDebugging(VkInstance instance);
		// Clear debug callback
		void freeDebugCallback(VkInstance instance);
		// Used to populate a VkDebugUtilsMessengerCreateInfoEXT with our example messenger function and desired flags
		void setupDebugingMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugUtilsMessengerCI);
		void log(std::string message);


		// Wrapper for the VK_EXT_debug_utils extension
		// These can be used to name Vulkan objects for debugging tools like RenderDoc
		void setup(VkInstance instance);
		void cmdBeginLabel(VkCommandBuffer cmdbuffer, std::string caption, float4 color);
		void cmdEndLabel(VkCommandBuffer cmdbuffer);
	};

};