#ifndef CODEX_INCLUDED
#define CODEX_INCLUDED


#ifdef _cplusplus
extern "C" {
#endif


typedef struct codex_ctx * codex_ctx_t;
typedef int CODEX_RESULT;
typedef int CODEX_DATA_TYPE;


typedef enum codex_data_type {
        CODEX_DATA_UINT,
        CODEX_DATA_UINT2,
        CODEX_DATA_UINT3,
        CODEX_DATA_UINT4,

        CODEX_DATA_INT,
        CODEX_DATA_INT2,
        CODEX_DATA_INT3,
        CODEX_DATA_INT4,

        CODEX_DATA_FLOAT,
        CODEX_DATA_FLOAT2,
        CODEX_DATA_FLOAT3,
        CODEX_DATA_FLOAT4,

        CODEX_DATA_DOUBLE,
        CODEX_DATA_DOUBLE2,
        CODEX_DATA_DOUBLE3,
        CODEX_DATA_DOUBLE4,

        CODEX_DATA_BOOL,
        CODEX_DATA_STRING,

        CODEX_DATA_PTR,

} CODEX_DATA_TYPE;


/* ---------------------------------------------------------- [ Lifetime ] -- */


CODEX_RESULT
codex_create(
        codex_ctx_t *ctx);


CODEX_RESULT
codex_destroy(
        codex_ctx_t *ctx);


/* ----------------------------------------------------------- [ Objects ] -- */


CODEX_RESULT
codex_object_create(
        codex_ctx_t ctx,
        uint32_t **out_ids,
        int count);


CODEX_RESULT
codex_object_destroy(
        codex_ctx_t ctx,
        uint32_t *ids,
        int count);


/* --------------------------------------------------------- [ Data Type ] -- */


CODEX_RESULT
codex_data_type_create(
        const char *name,
        CODEX_DATA_TYPE data_type,
        uint32_t *out_id);


/* -------------------------------------------------------------- [ Data ] -- */


CODEX_RESULT
codex_data_set(
        uint32_t object_id,
        uint32_t data_id,
        void *data);


CODEX_RESULT
codex_data_get(
        uint32_t object_id,
        uint32_t data_id,
        void *data);



#ifdef _cplusplus
} /* extern */
#endif


/* inc guard */
#endif
