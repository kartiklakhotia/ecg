#ifndef  __storage_h___
#define  __storage_h___
#include "app_defines.h"

#define __hf_mem__(hf_id, sigma_id, index)  hf_storage[(hf_id*MEM_SIZE_PER_HF) + (sigma_id*NSAMPLES) + index]
#define __dotp_mem__(hf_id, sigma_id)       dotp_storage[(hf_id*NSIGMAS) + sigma_id]


#endif
