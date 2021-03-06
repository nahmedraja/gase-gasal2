#ifndef __GASAL_H__
#define __GASAL_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "/usr/local/cuda/targets/x86_64-linux/include/cuda_runtime.h"


#ifndef HOST_MALLOC_SAFETY_FACTOR
#define HOST_MALLOC_SAFETY_FACTOR 5
#endif


enum comp_start{
	WITH_START,
	WITHOUT_START
};

enum algo_type{
	LOCAL,
	GLOBAL,
	SEMI_GLOBAL
};


//stream data
typedef struct {
	uint8_t *unpacked_query_batch;
	uint8_t *unpacked_target_batch;
	uint32_t *packed_query_batch;
	uint32_t *packed_target_batch;
	uint32_t *query_batch_offsets;
	uint32_t *target_batch_offsets;
	uint32_t *query_batch_lens;
	uint32_t *target_batch_lens;
	uint8_t *host_unpacked_query_batch;
	uint8_t *host_unpacked_target_batch;
	uint32_t *host_query_batch_offsets;
	uint32_t *host_target_batch_offsets;
	uint32_t *host_query_batch_lens;
	uint32_t *host_target_batch_lens;
	int32_t *aln_score;
	int32_t *query_batch_end;
	int32_t *target_batch_end;
	int32_t *query_batch_start;
	int32_t *target_batch_start;
	int32_t *host_aln_score;
	int32_t *host_query_batch_end;
	int32_t *host_target_batch_end;
	int32_t *host_query_batch_start;
	int32_t *host_target_batch_start;
	uint32_t gpu_max_query_batch_bytes;
	uint32_t gpu_max_target_batch_bytes;
	uint32_t host_max_query_batch_bytes;
	uint32_t host_max_target_batch_bytes;
	uint32_t gpu_max_n_alns;
	uint32_t host_max_n_alns;
	cudaStream_t str;
	int is_free;

} gasal_gpu_storage_t;

//vector of streams
typedef struct {
	int n;
	gasal_gpu_storage_t *a;
}gasal_gpu_storage_v;


//match/mismatch and gap penalties
typedef struct{
	int32_t match;
	int32_t mismatch;
	int32_t gap_open;
	int32_t gap_extend;
} gasal_subst_scores;



#ifdef __cplusplus
extern "C" {
#endif


void gasal_aln(gasal_gpu_storage_t *gpu_storage, const uint8_t *query_batch, const uint32_t *query_batch_offsets, const uint32_t *query_batch_lens, const uint8_t *target_batch, const uint32_t *target_batch_offsets, const uint32_t *target_batch_lens,   const uint32_t actual_query_batch_bytes, const uint32_t actual_target_batch_bytes, const uint32_t actual_n_alns, int32_t *host_aln_score, int32_t *host_query_batch_start, int32_t *host_target_batch_start, int32_t *host_query_batch_end, int32_t *host_target_batch_end,  int algo, int start);

void gasal_gpu_mem_alloc(gasal_gpu_storage_t *gpu_storage, int gpu_max_query_batch_bytes, int gpu_max_target_batch_bytes, int gpu_max_n_alns, int algo, int start);

void gasal_gpu_mem_free(gasal_gpu_storage_t *gpu_storage);

void gasal_copy_subst_scores(gasal_subst_scores *subst);

gasal_gpu_storage_v gasal_init_gpu_storage_v(int n_streams);

void gasal_aln_async(gasal_gpu_storage_t *gpu_storage, const uint32_t actual_query_batch_bytes, const uint32_t actual_target_batch_bytes, const uint32_t actual_n_alns, int algo, int start);

int gasal_is_aln_async_done(gasal_gpu_storage_t *gpu_storage);

void gasal_init_streams(gasal_gpu_storage_v *gpu_storage_vec, int host_max_query_batch_bytes,  int gpu_max_query_batch_bytes,  int host_max_target_batch_bytes, int gpu_max_target_batch_bytes, int host_max_n_alns, int gpu_max_n_alns, int algo, int start);

void gasal_destroy_streams(gasal_gpu_storage_v *gpu_storage_vec);

void gasal_destroy_gpu_storage_v(gasal_gpu_storage_v *gpu_storage_vec);




#ifdef __cplusplus
}
#endif

#endif
