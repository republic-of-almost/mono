#ifndef CODEX_CTX_INCLUDED_FAAA1F32_3DFA_4DC1_A758_E5AAD5DC1435
#define CODEX_CTX_INCLUDED_FAAA1F32_3DFA_4DC1_A758_E5AAD5DC1435


struct data_type {
        uint32_t type;
        const char *name;

        int *updated_callbacks;
        int *created_callbacks;
        int *destroyed_callbacks;
};


struct object_data {
        union data {
                unsigned char byte;
                short short_int;
                unsigned short short_uint;
                int integer;
                unsigned uinteger;
                float floating;
                double dfloating;
                void *data;
        };
        int data_size;
};


struct object_data {
        int object_write_lock;
        uint32_t *data_ids;
        struct object_data *data;
};


struct codex_ctx {
        uint32_t object_counter;
        int read_write_lock;

        uint32_t *object_ids;
        struct object_data *object_data;
};


/* inc guard */
#endif
