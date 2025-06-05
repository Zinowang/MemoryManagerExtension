//
//  SeriaOOMDetector.h
//  SeriaOOMDetector
//
//  Created by macminim2 on 2023/10/12.
//


#include <Foundation/Foundation.h>
#include <malloc/malloc.h>
#include <os/lock.h>
@interface SeriaOOMDetector : NSObject
+(void) StartMallocLogger;
+(void) EndMallocLogger;
@end

#ifdef __cplusplus
extern "C" {
#endif
typedef void (malloc_logger_t)(uint32_t type, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t result, uint32_t num_hot_frames_to_skip);
    
extern malloc_logger_t* malloc_logger;

void common_stack_logger(uint32_t type, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t result, uint32_t backtrace_to_skip);

typedef void(malloc_record_t)(vm_address_t load_address,int depth, uintptr_t size, vm_address_t** stack);
#ifdef __cplusplus
}
#endif

