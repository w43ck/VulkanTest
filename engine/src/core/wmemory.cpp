#include "wmemory.hpp"
#include "logger.hpp"
#include "../platform/platform.hpp"

#include <vector>
#include <stdio.h>

struct memory_stats {
	u64 total_allocated;
	u64 tagged_allocations[static_cast<u64>(mem_tag::MEMORY_TAG_MAX_TAGS)];
};

std::vector<std::wstring_view> mem_tag_strings = {
	L"UNKNOWN    ",
	L"ARRAY      ",
	L"DARRAY     ",
	L"DICT       ",
	L"RING_QUEUE ",
	L"BST        ",
	L"STRING     ",
	L"APPLICATION",
	L"JOB        ",
	L"TEXTURE    ",
	L"MAT_INST   ",
	L"RENDERER   ",
	L"GAME       ",
	L"TRANSFORM  ",
	L"ENTITY     ",
	L"ENTITY_NODE",
	L"SCENE      "
};

static memory_stats stats;

void initialize_memory() {
	platform_zero_memory(&stats, sizeof(stats));
}

void shutdown_memory() {
	
}

void * wallocate(u64 size, mem_tag tag) {
	if(tag == mem_tag::MEMORY_TAG_UNKNOWN) {
		WWARN(L"wallocate() called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
	}
	
	stats.total_allocated += size;
	stats.tagged_allocations[static_cast<u64>(tag)] += size;
	
	void * block = platfrom_allocate(size, false);
	platform_zero_memory(block, size);
	return block;
}

void wfree(void * block, u64 size, mem_tag tag) {
	if(tag == mem_tag::MEMORY_TAG_UNKNOWN) {
		WWARN(L"wfree() called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
	}
	
	stats.total_allocated -= size;
	stats.tagged_allocations[static_cast<u64>(tag)] -= size;
	platform_free(block, false);
}

void * wzero_memory(void * block, u64 size) {
	return platform_zero_memory(block, size);
}

void * wcopy_memory(void * dest, const void * source, u64 size) {
	return platform_copy_memory(dest, source, size);
}

void * wset_memory(void * dest, i32 value, u64 size) {
	return platform_set_memory(dest, value, size);
}

wchar_t * get_memory_usage_str() {
	const u64 gib = 1024 * 1024 * 1024;
	const u64 mib = 1024 * 1024;
	const u64 kib = 1024;
	
	wchar_t buffer[8000] = L"System memory use (tagged):\n";
	u64 offset = wcslen(buffer);
	for(u32 i = 0; i < static_cast<u32>(mem_tag::MEMORY_TAG_MAX_TAGS); ++i) {
		wchar_t unit[4] = L"XiB";
		float amount = 1.f;
		if(stats.tagged_allocations[i] >= gib) {
			unit[0] = L'G';
			amount = stats.tagged_allocations[i] / gib;
		} else if(stats.tagged_allocations[i] >= mib) {
			unit[0] = L'M';
			amount = stats.tagged_allocations[i] / mib;
		} else if(stats.tagged_allocations[i] >= kib) {
			unit[0] = L'K';
			amount = stats.tagged_allocations[i] / kib;
		} else {
			unit[0] = L'B';
			unit[1] = 0;
			amount = stats.tagged_allocations[i];
		}
		
		i32 length = swprintf(buffer + offset, 8000, L"  %s: %.2f%s\n", mem_tag_strings[i].data(), amount, unit);
		offset += length;
	}
	
	wchar_t * out_string = _wcsdup(buffer);
	return out_string;
}