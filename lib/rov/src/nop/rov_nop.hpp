#ifdef ROV_NOP

#ifndef ROV_NOP_INCLUDED_6DB81942_8B8E_49D4_9ABD_6F1AE1DE0A85
#define ROV_NOP_INCLUDED_6DB81942_8B8E_49D4_9ABD_6F1AE1DE0A85


namespace ROV_Internal {


struct rovNopData
{
  
};


// ------------------------------------------------------------- [ Lifetime ] --


void
nop_init() {}

void
nop_exec() {}

void
nop_destroy() {}


// ------------------------------------------------------------ [ Resources ] --


bool
nop_createTexture() { return true; }

bool
nop_createMesh() { return true; }

bool
nop_createIndex() { return true; }

bool
nop_createLights() { return true; }

bool
nop_updateLights() { return true; }

bool
nop_createRenderTarget() { return true; }


} // ns


#endif // inc guard

#endif // nop
