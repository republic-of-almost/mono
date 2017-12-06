#include <fiber.hpp>
#include <boost_context/fcontext.h>
#include <config.hpp>


#ifdef __clang__
#ifdef __x86_64__

#ifdef _APPLE_
#define APPLE_X64
#endif

#endif

#elif defined(_MSC_VER)

#ifdef _M_X64
#define WIN_X64
#endif

#endif


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


struct optio_fiber
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
optio_fiber_create(struct optio_fiber **fiber, optio_fiber_func func, void *arg)
{
  const int fiber_bytes = sizeof(struct optio_fiber);
  struct optio_fiber *new_fiber = (struct optio_fiber*)FIBER_MALLOC(fiber_bytes);
  FIBER_MEMZERO(new_fiber, fiber_bytes);
  
  if(func != 0)
  {
    int stack_size = 524288;

    int bytes = stack_size + stack_size + stack_size;
    
    new_fiber->stack = FIBER_MALLOC(bytes);
    FIBER_MEMZERO(new_fiber->stack, bytes);
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
optio_fiber_destroy(struct optio_fiber *fiber)
{
  if(fiber->stack)
  {
    FIBER_FREE(fiber->stack);
    fiber->stack = NULL;
  }
  
  if(fiber)
  {
    FIBER_FREE(fiber);
  }
  
  fiber->context = NULL;
}


/* -------------------------------------------------- [ Fiber Attributes ] -- */


void
optio_fiber_set_user_id(struct optio_fiber *fiber, unsigned user_id)
{
  /* param check */
  FIBER_ASSERT(fiber);

  fiber->job_id = user_id;
}


unsigned
optio_fiber_get_user_id(const struct optio_fiber *fiber)
{
  /* param check */
  FIBER_ASSERT(fiber);
  
  return fiber->job_id;
}


/* ----------------------------------------------------- [ Fiber Actions ] -- */


void
optio_fiber_switch(struct optio_fiber *this_fiber, struct optio_fiber *new_fiber)
{
  jump_fcontext(&this_fiber->context, new_fiber->context, new_fiber->arg);
}
