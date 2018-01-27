#include <fiber.hpp>
#include <boost_context/fcontext.h>
#include <config.hpp>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>


#ifdef __clang__

#ifdef __x86_64__

#ifdef __APPLE__
#define APPLE_X64
#endif

#endif

#elif defined(_MSC_VER)

#ifdef _M_X64
#define WIN_X64
#elif defined(_M_IX86)
#define WIN_X86
#endif

#endif


#ifdef WIN_X86

//__declspec(naked) void jump_fcontext(fcontext_t *from, fcontext_t to, void *arg)
//{
//__asm {
//  
//    ; prepare stack
//    lea  esp, [esp-02ch]
//
//    ; save MMX control- and status-word
//    stmxcsr  [esp]
//    ; save x87 control-word
//    fnstcw  [esp+04h]
//
//    assume  fs:nothing
//    ; load NT_TIB into ECX
//    mov  edx, fs:[018h]
//    assume  fs:error
//    ; load fiber local storage
//    mov  eax, [edx+010h]
//    mov  [esp+08h], eax
//    ; load current deallocation stack
//    mov  eax, [edx+0e0ch]
//    mov  [esp+0ch], eax
//    ; load current stack limit
//    mov  eax, [edx+08h]
//    mov  [esp+010h], eax
//    ; load current stack base
//    mov  eax, [edx+04h]
//    mov  [esp+014h], eax
//    ; load current SEH exception list
//    mov  eax, [edx]
//    mov  [esp+018h], eax
//
//    mov  [esp+01ch], edi  ; save EDI 
//    mov  [esp+020h], esi  ; save ESI 
//    mov  [esp+024h], ebx  ; save EBX 
//    mov  [esp+028h], ebp  ; save EBP 
//
//    ; store ESP (pointing to context-data) in arg 1
//	mov eax, [esp+030h]
//	mov [eax], esp
//
//    ; second arg of jump_fcontext() == fcontext to jump to
//    mov  ecx, [esp+034h]
//	; store arg 3 (data) in EAX
//	mov eax, [esp+038h]
//    
//    ; restore ESP (pointing to context-data) from ECX
//    mov  esp, ecx
//
//    ; restore MMX control- and status-word
//    ldmxcsr  [esp]
//    ; restore x87 control-word
//    fldcw  [esp+04h]
//
//    assume  fs:nothing
//    ; load NT_TIB into EDX
//    mov  edx, fs:[018h]
//    assume  fs:error
//    ; restore fiber local storage
//    mov  ecx, [esp+08h]
//    mov  [edx+010h], ecx
//    ; restore current deallocation stack
//    mov  ecx, [esp+0ch]
//    mov  [edx+0e0ch], ecx
//    ; restore current stack limit
//    mov  ecx, [esp+010h]
//    mov  [edx+08h], ecx
//    ; restore current stack base
//    mov  ecx, [esp+014h]
//    mov  [edx+04h], ecx
//    ; restore current SEH exception list
//    mov  ecx, [esp+018h]
//    mov  [edx], ecx
//
//    mov  ecx, [esp+02ch]  ; restore EIP
//
//    mov  edi, [esp+01ch]  ; restore EDI 
//    mov  esi, [esp+020h]  ; restore ESI 
//    mov  ebx, [esp+024h]  ; restore EBX 
//    mov  ebp, [esp+028h]  ; restore EBP 
//
//    ; prepare stack
//    lea  esp, [esp+030h]
//
//    ; jump to context
//    jmp ecx
//jump_fcontext ENDP
//END
//
//}
//}

//__declspec(naked) fcontext_t make_fcontext(void * sp, size_t size, void(*func)(void *))
//{
//__asm {
//
//  .386
//  .XMM
//  .model flat, c
//  ; standard C library function
//  _exit PROTO, value:SDWORD
//  .code
//
//  make_fcontext PROC
//  ; first arg of make_fcontext() == top of context - stack
//  mov  eax, [esp + 04h]
//
//  ; reserve space for first argument of context - function
//  ; EAX might already point to a 16byte border
//  lea  eax, [eax - 08h]
//
//  ; shift address in EAX to lower 16 byte boundary
//  and  eax, -16
//
//  ; reserve space for context - data on context - stack
//  ; on context - function entry : (ESP - 0x4) % 8 == 0
//  ; additional space is required for SEH
//  lea  eax, [eax - 044h]
//
//  ; save MMX control - and status - word
//  stmxcsr[eax]
//  ; save x87 control - word
//  fnstcw[eax + 04h]
//
//  ; first arg of make_fcontext() == top of context - stack
//  mov  ecx, [esp + 04h]
//  ; save top address of context stack as 'base'
//  mov[eax + 014h], ecx
//  ; second arg of make_fcontext() == size of context - stack
//  mov  edx, [esp + 08h]
//  ; negate stack size for LEA instruction(== substraction)
//  neg  edx
//  ; compute bottom address of context stack(limit)
//  lea  ecx, [ecx + edx]
//  ; save bottom address of context - stack as 'limit'
//  mov[eax + 010h], ecx
//  ; save bottom address of context - stack as 'dealloction stack'
//  mov[eax + 0ch], ecx
//  ; set fiber - storage to zero
//  xor  ecx, ecx
//  mov[eax + 08h], ecx
//
//  ; third arg of make_fcontext() == address of context - function
//  ; stored in EBX
//  mov  ecx, [esp + 0ch]
//  mov[eax + 024h], ecx
//
//  ; compute abs address of label trampoline
//  mov  ecx, trampoline
//  ; save address of trampoline as return-address for context - function
//  ; will be entered after calling jump_fcontext() first time
//  mov[eax + 02ch], ecx
//
//  ; compute abs address of label finish
//  mov  ecx, finish
//  ; save address of finish as return-address for context - function in EBP
//  ; will be entered after context - function returns
//  mov[eax + 028h], ecx
//
//  ; traverse current seh chain to get the last exception handler installed by Windows
//  ; note that on Windows Server 2008 and 2008 R2, SEHOP is activated by default
//  ; the exception handler chain is tested for the presence of ntdll.dll!FinalExceptionHandler
//  ; at its end by RaiseException all seh - handlers are disregarded if not present and the
//  ; program is aborted
//  assume  fs : nothing
//  ; load NT_TIB into ECX
//  mov  ecx, fs:[0h]
//  assume  fs : error
//
//  walk :
//  ; load 'next' member of current SEH into EDX
//    mov  edx, [ecx]
//    ; test if 'next' of current SEH is last(== 0xffffffff)
//    inc  edx
//    jz  found
//    dec  edx
//    ; exchange content; ECX contains address of next SEH
//    xchg edx, ecx
//    ; inspect next SEH
//    jmp  walk
//
//    found :
//  ; load 'handler' member of SEH == address of last SEH handler installed by Windows
//    mov  ecx, [ecx + 04h]
//    ; save address in ECX as SEH handler for context
//    mov[eax + 03ch], ecx
//    ; set ECX to - 1
//    mov  ecx, 0ffffffffh
//    ; save ECX as next SEH item
//    mov[eax + 038h], ecx
//    ; load address of next SEH item
//    lea  ecx, [eax + 038h]
//    ; save next SEH
//    mov[eax + 018h], ecx
//
//    ret; return pointer to context - data
//
//    trampoline :
//  ; move data for entering context - function
//    mov[esp], eax
//    push ebp
//    ; jump to context - function
//    jmp ebx
//
//    finish :
//  ; exit code is zero
//    xor  eax, eax
//    mov[esp], eax
//    ; exit application
//    call  _exit
//    hlt
//    make_fcontext ENDP
//    END
//}
//}


#endif


#ifdef WIN_X64

__asm__(

  "; standard C library function\n\t"
  "EXTERN  _exit:PROC\n\t"
  ".code\n\t"
  "\n\t"
  "; generate function table entry in .pdata and unwind information in\n\t"
  "make_fcontext PROC FRAME\n\t"
  "; .xdata for a function's structured exception handling unwind behavior\n\t"
  ".endprolog\n\t"
  "\n\t"
  "; first arg of make_fcontext() == top of context-stack\n\t"
  "mov  rax, rcx\n\t"
  "\n\t"
  "; shift address in RAX to lower 16 byte boundary\n\t"
  "; == pointer to fcontext_t and address of context stack\n\t"
  "and  rax, -16\n\t"
  "\n\t"
  "; reserve space for context-data on context-stack\n\t"
  "; on context-function entry: (RSP -0x8) % 16 == 0\n\t"
  "sub  rax, 0140h\n\t"
  "\n\t"
  "; first arg of make_fcontext() == top of context-stack\n\t"
  "; save top address of context stack as 'base\n\t'\n\t"
  "mov  [rax+0c8h], rcx\n\t"
  "; second arg of make_fcontext() == size of context-stack\n\t"
  "; negate stack size for LEA instruction (== substraction)\n\t"
  "neg  rdx\n\t"
  "; compute bottom address of context stack (limit)\n\t"
  "lea  rcx, [rcx+rdx]\n\t"
  "; save bottom address of context stack as 'limit'\n\t"
  "mov  [rax+0c0h], rcx\n\t"
  "; save address of context stack limit as 'dealloction stack'\n\t"
  "mov  [rax+0b8h], rcx\n\t"
  "; set fiber-storage to zero\n\t"
  "xor  rcx, rcx\n\t"
  "mov  [rax+0b0h], rcx\n\t"
  "\n\t"
  "; save MMX control- and status - word\n\t"
  "stmxcsr  [rax+0a0h]\n\t"
  "; save x87 control-word\n\t"
  "fnstcw  [rax+0a4h]\n\t"
  "\n\t"
  "; third arg of make_fcontext() == address of context-function\n\t"
  "mov  [rax+0100h], r8\n\t"
  "\n\t"
  "; compute abs address of label trampoline\n\t"
  "lea  rcx, trampoline\n\t"
  "; save address of trampoline as return-address for context-function\n\t"
  "; will be entered after calling jump_fcontext() first time\n\t"
  "mov  [rax+0118h], rcx\n\t"
  "\n\t"
  "; compute abs address of label finish\n\t"
  "lea  rcx, finish\n\t"
  "; save address of finish as return-address for context-function in RBP\n\t"
  "; will be entered after context-function returns \n\t"
  "mov  [rax+0108h], rcx\n\t"
  "\n\t"
  "ret ; return pointer to context-data\n\t"
  "\n\t"
  "\n\t"
  "trampoline:\n\t"
  "; store return address on stack\n\t"
  "; fix stack alignment\n\t"
  "push rbp\n\t"
  "; jump to context-function\n\t"
  "jmp rbx\n\t"
  "\n\t"
  "finish:\n\t"
  "; exit code is zero\n\t"
  "xor  rcx, rcx\n\t"
  "; exit application\n\t"
  "call  _exit\n\t"
  "hlt\n\t"
  "make_fcontext ENDP\n\t"
  "END\n\t"
)


__asm__(
  "\n\t"
  ".code\n\t"
  "\n\t"
  "jump_fcontext PROC FRAME\n\t"
  ".endprolog\n\t"
  "\n\t"
  "; prepare stack‭\n\t"
  "lea rsp, [rsp-0118h]\n\t"
  "\n\t"
  "; save XMM storage\n\t"
  "movaps  [rsp], xmm6\n\t"
  "movaps  [rsp+010h], xmm7\n\t"
  "movaps  [rsp+020h], xmm8\n\t"
  "movaps  [rsp+030h], xmm9\n\t"
  "movaps  [rsp+040h], xmm10\n\t"
  "movaps  [rsp+050h], xmm11\n\t"
  "movaps  [rsp+060h], xmm12\n\t"
  "movaps  [rsp+070h], xmm13\n\t"
  "movaps  [rsp+080h], xmm14\n\t"
  "movaps  [rsp+090h], xmm15\n\t"
  "; save MMX control- and status-word\n\t"
  "stmxcsr  [rsp+0a0h]\n\t"
  "; save x87 control-word\n\t"
  "fnstcw  [rsp+0a4h]\n\t"
  "\n\t"
  "; load NT_TIB\n\t"
  "mov  r10,  gs:[030h]\n\t"
  "; save fiber local storage\n\t"
  "mov  rax, [r10+018h]\n\t"
  "mov  [rsp+0b0h], rax\n\t"
  "; save current deallocation stack\n\t"
  "mov  rax, [r10+01478h]\n\t"
  "mov  [rsp+0b8h], rax\n\t"
  "; save current stack limit\n\t"
  "mov  rax, [r10+010h]\n\t"
  "mov  [rsp+0c0h], rax\n\t"
  "; save current stack base\n\t"
  "mov  rax,  [r10+08h]\n\t"
  "mov  [rsp+0c8h], rax\n\t"
  "\n\t"
  "mov [rsp+0d0h], r12  ; save R12\n\t"
  "mov [rsp+0d8h], r13  ; save R13\n\t"
  "mov [rsp+0e0h], r14  ; save R14\n\t"
  "mov [rsp+0e8h], r15  ; save R15\n\t"
  "mov [rsp+0f0h], rdi  ; save RDI\n\t"
  "mov [rsp+0f8h], rsi  ; save RSI\n\t"
  "mov [rsp+0100h], rbx  ; save RBX\n\t"
  "mov [rsp+0108h], rbp  ; save RBP\n\t"
  "\n\t"
  "; preserve RSP (pointing to context-data) in RCX\n\t"
  "mov  [rcx], rsp\n\t"
  "\n\t"
  "; restore RSP (pointing to context-data) from RDX\n\t"
  "mov  rsp, rdx\n\t"
  "\n\t"
  "; restore XMM storage\n\t"
  "movaps  xmm6, [rsp]\n\t"
  "movaps  xmm7, [rsp+010h]\n\t"
  "movaps  xmm8, [rsp+020h]\n\t"
  "movaps  xmm9, [rsp+030h]\n\t"
  "movaps  xmm10, [rsp+040h]\n\t"
  "movaps  xmm11, [rsp+050h]\n\t"
  "movaps  xmm12, [rsp+060h]\n\t"
  "movaps  xmm13, [rsp+070h]\n\t"
  "movaps  xmm14, [rsp+080h]\n\t"
  "movaps  xmm15, [rsp+090h]\n\t"
  "; restore MMX control- and status-word\n\t"
  "ldmxcsr  [rsp+0a0h]\n\t"
  "; save x87 control-word\n\t"
  "fldcw   [rsp+0a4h]\n\t"
  "\n\t"
  "; load NT_TIB\n\t"
  "mov  r10,  gs:[030h]\n\t"
  "; restore fiber local storage\n\t"
  "mov  rax, [rsp+0b0h]\n\t"
  "mov  [r10+018h], rax\n\t"
  "; restore current deallocation stack\n\t"
  "mov  rax, [rsp+0b8h]\n\t"
  "mov  [r10+01478h], rax\n\t"
  "; restore current stack limit\n\t"
  "mov  rax, [rsp+0c0h]\n\t"
  "mov  [r10+010h], rax\n\t"
  "; restore current stack base\n\t"
  "mov  rax, [rsp+0c8h]\n\t"
  "mov  [r10+08h], rax\n\t"
  "\n\t"
  "mov r12, [rsp+0d0h]  ; restore R12\n\t"
  "mov r13, [rsp+0d8h]  ; restore R13\n\t"
  "mov r14, [rsp+0e0h]  ; restore R14\n\t"
  "mov r15, [rsp+0e8h]  ; restore R15\n\t"
  "mov rdi, [rsp+0f0h]  ; restore RDI\n\t"
  "mov rsi, [rsp+0f8h]  ; restore RSI\n\t"
  "mov rbx, [rsp+0100h]  ; restore RBX\n\t"
  "mov rbp, [rsp+0108h]  ; restore RBP\n\t"
  "\n\t"
  "; prepare stack\n\t"
  "lea rsp, [rsp+0118h]\n\t"
  "\n\t"
  "; Set the third arg (data) as the first arg of the context function\n\t"
  "mov  rcx,  r8\n\t"
  "\n\t"
  "; load return-address\n\t"
  "pop r10\n\t"
  "\n\t"
  "; indirect jump to context\n\t"
  "jmp  r10\n\t"
  "\n\t"
  "ret\n\t"
  "\n\t"
  "jump_fcontext ENDP\n\t"
  "END\n\t"
)

#endif


/*
  Apple - Intel 64
*/
#ifdef APPLE_X64

__asm__(
".text\n\t"
".globl _jump_fcontext\n\t"
".align 8\n\t"
"_jump_fcontext:\n\t"
    "leaq  -0x38(%rsp), %rsp\n\t"

    "stmxcsr  (%rsp)     /* save MMX control- and status-word */\n\t"
    "fnstcw   0x4(%rsp)  /* save x87 control-word */\n\t"

    "movq  %r12, 0x8(%rsp)  /* save R12 */\n\t"
    "movq  %r13, 0x10(%rsp)  /* save R13 */\n\t"
    "movq  %r14, 0x18(%rsp)  /* save R14 */\n\t"
    "movq  %r15, 0x20(%rsp)  /* save R15 */\n\t"
    "movq  %rbx, 0x28(%rsp)  /* save RBX */\n\t"
    "movq  %rbp, 0x30(%rsp)  /* save RBP */\n\t"

    "/*  store RSP (pointing to context-data) in address given by RDI (first arg passed to jump_fcontext) */\n\t"
    "movq  %rsp, (%rdi)\n\t"

    "/* restore RSP (pointing to context-data) from RSI */\n\t"
    "movq  %rsi, %rsp\n\t"

    "movq  0x38(%rsp), %r8  /* restore return-address */\n\t"

    "ldmxcsr  (%rsp)     /* restore MMX control- and status-word */\n\t"
    "fldcw    0x4(%rsp)  /* restore x87 control-word */\n\t"

    "movq  0x8(%rsp), %r12  /* restore R12 */\n\t"
    "movq  0x10(%rsp), %r13  /* restore R13 */\n\t"
    "movq  0x18(%rsp), %r14  /* restore R14 */\n\t"
    "movq  0x20(%rsp), %r15  /* restore R15 */\n\t"
    "movq  0x28(%rsp), %rbx  /* restore RBX */\n\t"
    "movq  0x30(%rsp), %rbp  /* restore RBP */\n\t"

    "leaq  0x40(%rsp), %rsp /* prepare stack */\n\t"

    "/* pass data (third arg passed to jump_fcontext) as first arg passed to context function */\n\t"
    "movq  %rdx, %rdi\n\t"

    "/* indirect jump to context */\n\t"
    "jmp  *%r8\n\t"
);


__asm__(
".text\n\t"
".globl _make_fcontext\n\t"
".align 8\n\t"
"_make_fcontext:\n\t"
    "/* first arg of make_fcontext() == top of context-stack */\n\t"
    "movq  %rdi, %rax\n\t"

    "/* shift address in RAX to lower 16 byte boundary */\n\t"
    "andq  $-16, %rax\n\t"

    "/* reserve space for context-data on context-stack */\n\t"
    "/* on context-function entry: (RSP -0x8) % 16 == 0 */\n\t"
    "leaq  -0x40(%rax), %rax\n\t"

    "/* third arg of make_fcontext() == address of context-function */\n\t"
    "/* stored in RBX */\n\t"
    "movq  %rdx, 0x28(%rax)\n\t"

    "/* save MMX control- and status-word */\n\t"
    "stmxcsr  (%rax)\n\t"
    "/* save x87 control-word */\n\t"
    "fnstcw   0x4(%rax)\n\t"

    "/* compute abs address of label trampoline */\n\t"
    "leaq  trampoline(%rip), %rcx\n\t"
    "/* save address of trampoline as return-address for context-function */\n\t"
    "/* will be entered after calling jump_fcontext() first time */\n\t"
    "movq  %rcx, 0x38(%rax)\n\t"

    "/* compute abs address of label finish */\n\t"
    "leaq  finish(%rip), %rcx\n\t"
    "/* save address of finish as return-address for context-function */\n\t"
    "/* will be entered after context-function returns */\n\t"
    "movq  %rcx, 0x30(%rax)\n\t"

    "ret /* return pointer to context-data */\n\t"

"trampoline:\n\t"
    "/* store return address on stack */\n\t"
    "/* fix stack alignment */\n\t"
    "push %rbp\n\t"
    "/* jump to context-function */\n\t"
    "jmp *%rbx\n\t"

"finish:\n\t"
    "/* exit code is zero */\n\t"
    "xorq  %rdi, %rdi\n\t"
    "/* exit application */\n\t"
    "call  __exit\n\t"
    "hlt\n\t"
);


#endif



/* ------------------------------------------------------- [ Fiber Types ] -- */


struct roa_fiber
{
  void *stack;
  size_t sys_page_size;
  size_t stack_size;
  fcontext_t context;
  void *arg;
  
  unsigned job_id;
};


/* ---------------------------------------------------- [ Fiber Lifetime ] -- */


void
roa_fiber_create(struct roa_fiber **fiber, roa_fiber_func func, void *arg)
{
  const int fiber_bytes = sizeof(struct roa_fiber);
  struct roa_fiber *new_fiber = (struct roa_fiber*)roa_zalloc(fiber_bytes);
  
  if(func != 0)
  {
    int stack_size = 524288;

    int bytes = stack_size + stack_size + stack_size;
    
    new_fiber->stack = roa_zalloc(bytes);
    char *start = ((char*)new_fiber->stack) + stack_size;
    new_fiber->context = make_fcontext(start, stack_size, func);
    
    new_fiber->arg = arg;
  }
  else
  {
    fcontext_t local = NULL;
    new_fiber->context = local;
  }

  *fiber = new_fiber;
}


void
roa_fiber_destroy(struct roa_fiber *fiber)
{
  if(fiber->stack)
  {
    roa_free(fiber->stack);
    fiber->stack = NULL;
  }
  
  if(fiber)
  {
    roa_free(fiber);
  }
  
  fiber->context = NULL;
}


/* -------------------------------------------------- [ Fiber Attributes ] -- */


void
roa_fiber_set_user_id(struct roa_fiber *fiber, unsigned user_id)
{
  /* param check */
  ROA_ASSERT(fiber);

  fiber->job_id = user_id;
}


unsigned
roa_fiber_get_user_id(const struct roa_fiber *fiber)
{
  /* param check */
  ROA_ASSERT(fiber);
  
  return fiber->job_id;
}


/* ----------------------------------------------------- [ Fiber Actions ] -- */


void
roa_fiber_switch(struct roa_fiber *this_fiber, struct roa_fiber *new_fiber)
{
  jump_fcontext(&this_fiber->context, new_fiber->context, new_fiber->arg);
}
