#include "rov_gl_exec.hpp"
#include "rov_gl.hpp"
#include <lib/bits.hpp>
#include <lib/color.hpp>
#include <lib/bench.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>
#include <math/mat/mat4.hpp>


#ifdef ROV_GL4_VR_PROTO
#include <string>
#include <openvr.h>

struct Vector2
{
  float x;
  float y;

  // ctors
  Vector2() : x(0), y(0) {};
  Vector2(float x, float y) : x(x), y(y) {};

  // utils functions
  void        set(float x, float y);
  float       length() const;                         //
  float       distance(const Vector2& vec) const;     // distance between two vectors
  Vector2&    normalize();                            //
  float       dot(const Vector2& vec) const;          // dot product
  bool        equal(const Vector2& vec, float e) const; // compare with epsilon

                                                        // operators
  Vector2     operator-() const;                      // unary operator (negate)
  Vector2     operator+(const Vector2& rhs) const;    // add rhs
  Vector2     operator-(const Vector2& rhs) const;    // subtract rhs
  Vector2&    operator+=(const Vector2& rhs);         // add rhs and update this object
  Vector2&    operator-=(const Vector2& rhs);         // subtract rhs and update this object
  Vector2     operator*(const float scale) const;     // scale
  Vector2     operator*(const Vector2& rhs) const;    // multiply each element
  Vector2&    operator*=(const float scale);          // scale and update this object
  Vector2&    operator*=(const Vector2& rhs);         // multiply each element and update this object
  Vector2     operator/(const float scale) const;     // inverse scale
  Vector2&    operator/=(const float scale);          // scale and update this object
  bool        operator==(const Vector2& rhs) const;   // exact compare, no epsilon
  bool        operator!=(const Vector2& rhs) const;   // exact compare, no epsilon
  bool        operator<(const Vector2& rhs) const;    // comparison for sort
  float       operator[](int index) const;            // subscript operator v[0], v[1]
  float&      operator[](int index);                  // subscript operator v[0], v[1]

  friend Vector2 operator*(const float a, const Vector2 vec);
  friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
};



///////////////////////////////////////////////////////////////////////////////
// 3D vector
///////////////////////////////////////////////////////////////////////////////
struct Vector3
{
  float x;
  float y;
  float z;

  // ctors
  Vector3() : x(0), y(0), z(0) {};
  Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

  // utils functions
  void        set(float x, float y, float z);
  float       length() const;                         //
  float       distance(const Vector3& vec) const;     // distance between two vectors
  Vector3&    normalize();                            //
  float       dot(const Vector3& vec) const;          // dot product
  Vector3     cross(const Vector3& vec) const;        // cross product
  bool        equal(const Vector3& vec, float e) const; // compare with epsilon

                                                        // operators
  Vector3     operator-() const;                      // unary operator (negate)
  Vector3     operator+(const Vector3& rhs) const;    // add rhs
  Vector3     operator-(const Vector3& rhs) const;    // subtract rhs
  Vector3&    operator+=(const Vector3& rhs);         // add rhs and update this object
  Vector3&    operator-=(const Vector3& rhs);         // subtract rhs and update this object
  Vector3     operator*(const float scale) const;     // scale
  Vector3     operator*(const Vector3& rhs) const;    // multiplay each element
  Vector3&    operator*=(const float scale);          // scale and update this object
  Vector3&    operator*=(const Vector3& rhs);         // product each element and update this object
  Vector3     operator/(const float scale) const;     // inverse scale
  Vector3&    operator/=(const float scale);          // scale and update this object
  bool        operator==(const Vector3& rhs) const;   // exact compare, no epsilon
  bool        operator!=(const Vector3& rhs) const;   // exact compare, no epsilon
  bool        operator<(const Vector3& rhs) const;    // comparison for sort
  float       operator[](int index) const;            // subscript operator v[0], v[1]
  float&      operator[](int index);                  // subscript operator v[0], v[1]

  friend Vector3 operator*(const float a, const Vector3 vec);
  friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);
};



///////////////////////////////////////////////////////////////////////////////
// 4D vector
///////////////////////////////////////////////////////////////////////////////
struct Vector4
{
  float x;
  float y;
  float z;
  float w;

  // ctors
  Vector4() : x(0), y(0), z(0), w(0) {};
  Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

  // utils functions
  void        set(float x, float y, float z, float w);
  float       length() const;                         //
  float       distance(const Vector4& vec) const;     // distance between two vectors
  Vector4&    normalize();                            //
  float       dot(const Vector4& vec) const;          // dot product
  bool        equal(const Vector4& vec, float e) const; // compare with epsilon

                                                        // operators
  Vector4     operator-() const;                      // unary operator (negate)
  Vector4     operator+(const Vector4& rhs) const;    // add rhs
  Vector4     operator-(const Vector4& rhs) const;    // subtract rhs
  Vector4&    operator+=(const Vector4& rhs);         // add rhs and update this object
  Vector4&    operator-=(const Vector4& rhs);         // subtract rhs and update this object
  Vector4     operator*(const float scale) const;     // scale
  Vector4     operator*(const Vector4& rhs) const;    // multiply each element
  Vector4&    operator*=(const float scale);          // scale and update this object
  Vector4&    operator*=(const Vector4& rhs);         // multiply each element and update this object
  Vector4     operator/(const float scale) const;     // inverse scale
  Vector4&    operator/=(const float scale);          // scale and update this object
  bool        operator==(const Vector4& rhs) const;   // exact compare, no epsilon
  bool        operator!=(const Vector4& rhs) const;   // exact compare, no epsilon
  bool        operator<(const Vector4& rhs) const;    // comparison for sort
  float       operator[](int index) const;            // subscript operator v[0], v[1]
  float&      operator[](int index);                  // subscript operator v[0], v[1]

  friend Vector4 operator*(const float a, const Vector4 vec);
  friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);
};



// fast math routines from Doom3 SDK
inline float invSqrt(float x)
{
  float xhalf = 0.5f * x;
  int i = *(int*)&x;          // get bits for floating value
  i = 0x5f3759df - (i >> 1);    // gives initial guess
  x = *(float*)&i;            // convert bits back to float
  x = x * (1.5f - xhalf*x*x); // Newton step
  return x;
}



///////////////////////////////////////////////////////////////////////////////
// inline functions for Vector2
///////////////////////////////////////////////////////////////////////////////
inline Vector2 Vector2::operator-() const {
  return Vector2(-x, -y);
}

inline Vector2 Vector2::operator+(const Vector2& rhs) const {
  return Vector2(x + rhs.x, y + rhs.y);
}

inline Vector2 Vector2::operator-(const Vector2& rhs) const {
  return Vector2(x - rhs.x, y - rhs.y);
}

inline Vector2& Vector2::operator+=(const Vector2& rhs) {
  x += rhs.x; y += rhs.y; return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& rhs) {
  x -= rhs.x; y -= rhs.y; return *this;
}

inline Vector2 Vector2::operator*(const float a) const {
  return Vector2(x*a, y*a);
}

inline Vector2 Vector2::operator*(const Vector2& rhs) const {
  return Vector2(x*rhs.x, y*rhs.y);
}

inline Vector2& Vector2::operator*=(const float a) {
  x *= a; y *= a; return *this;
}

inline Vector2& Vector2::operator*=(const Vector2& rhs) {
  x *= rhs.x; y *= rhs.y; return *this;
}

inline Vector2 Vector2::operator/(const float a) const {
  return Vector2(x / a, y / a);
}

inline Vector2& Vector2::operator/=(const float a) {
  x /= a; y /= a; return *this;
}

inline bool Vector2::operator==(const Vector2& rhs) const {
  return (x == rhs.x) && (y == rhs.y);
}

inline bool Vector2::operator!=(const Vector2& rhs) const {
  return (x != rhs.x) || (y != rhs.y);
}

inline bool Vector2::operator<(const Vector2& rhs) const {
  if (x < rhs.x) return true;
  if (x > rhs.x) return false;
  if (y < rhs.y) return true;
  if (y > rhs.y) return false;
  return false;
}

inline float Vector2::operator[](int index) const {
  return (&x)[index];
}

inline float& Vector2::operator[](int index) {
  return (&x)[index];
}

inline void Vector2::set(float x, float y) {
  this->x = x; this->y = y;
}

inline float Vector2::length() const {
  return sqrtf(x*x + y*y);
}

inline float Vector2::distance(const Vector2& vec) const {
  return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y));
}

inline Vector2& Vector2::normalize() {
  //@@const float EPSILON = 0.000001f;
  float xxyy = x*x + y*y;
  //@@if(xxyy < EPSILON)
  //@@    return *this;

  //float invLength = invSqrt(xxyy);
  float invLength = 1.0f / sqrtf(xxyy);
  x *= invLength;
  y *= invLength;
  return *this;
}

inline float Vector2::dot(const Vector2& rhs) const {
  return (x*rhs.x + y*rhs.y);
}

inline bool Vector2::equal(const Vector2& rhs, float epsilon) const {
  return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
}

inline Vector2 operator*(const float a, const Vector2 vec) {
  return Vector2(a*vec.x, a*vec.y);
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
  os << "(" << vec.x << ", " << vec.y << ")";
  return os;
}
// END OF VECTOR2 /////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// inline functions for Vector3
///////////////////////////////////////////////////////////////////////////////
inline Vector3 Vector3::operator-() const {
  return Vector3(-x, -y, -z);
}

inline Vector3 Vector3::operator+(const Vector3& rhs) const {
  return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline Vector3 Vector3::operator-(const Vector3& rhs) const {
  return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline Vector3& Vector3::operator+=(const Vector3& rhs) {
  x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& rhs) {
  x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
}

inline Vector3 Vector3::operator*(const float a) const {
  return Vector3(x*a, y*a, z*a);
}

inline Vector3 Vector3::operator*(const Vector3& rhs) const {
  return Vector3(x*rhs.x, y*rhs.y, z*rhs.z);
}

inline Vector3& Vector3::operator*=(const float a) {
  x *= a; y *= a; z *= a; return *this;
}

inline Vector3& Vector3::operator*=(const Vector3& rhs) {
  x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
}

inline Vector3 Vector3::operator/(const float a) const {
  return Vector3(x / a, y / a, z / a);
}

inline Vector3& Vector3::operator/=(const float a) {
  x /= a; y /= a; z /= a; return *this;
}

inline bool Vector3::operator==(const Vector3& rhs) const {
  return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}

inline bool Vector3::operator!=(const Vector3& rhs) const {
  return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

inline bool Vector3::operator<(const Vector3& rhs) const {
  if (x < rhs.x) return true;
  if (x > rhs.x) return false;
  if (y < rhs.y) return true;
  if (y > rhs.y) return false;
  if (z < rhs.z) return true;
  if (z > rhs.z) return false;
  return false;
}

inline float Vector3::operator[](int index) const {
  return (&x)[index];
}

inline float& Vector3::operator[](int index) {
  return (&x)[index];
}

inline void Vector3::set(float x, float y, float z) {
  this->x = x; this->y = y; this->z = z;
}

inline float Vector3::length() const {
  return sqrtf(x*x + y*y + z*z);
}

inline float Vector3::distance(const Vector3& vec) const {
  return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z));
}

inline Vector3& Vector3::normalize() {
  //@@const float EPSILON = 0.000001f;
  float xxyyzz = x*x + y*y + z*z;
  //@@if(xxyyzz < EPSILON)
  //@@    return *this; // do nothing if it is ~zero vector

  //float invLength = invSqrt(xxyyzz);
  float invLength = 1.0f / sqrtf(xxyyzz);
  x *= invLength;
  y *= invLength;
  z *= invLength;
  return *this;
}

inline float Vector3::dot(const Vector3& rhs) const {
  return (x*rhs.x + y*rhs.y + z*rhs.z);
}

inline Vector3 Vector3::cross(const Vector3& rhs) const {
  return Vector3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
}

inline bool Vector3::equal(const Vector3& rhs, float epsilon) const {
  return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon && fabs(z - rhs.z) < epsilon;
}

inline Vector3 operator*(const float a, const Vector3 vec) {
  return Vector3(a*vec.x, a*vec.y, a*vec.z);
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
  os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return os;
}
// END OF VECTOR3 /////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// inline functions for Vector4
///////////////////////////////////////////////////////////////////////////////
inline Vector4 Vector4::operator-() const {
  return Vector4(-x, -y, -z, -w);
}

inline Vector4 Vector4::operator+(const Vector4& rhs) const {
  return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

inline Vector4 Vector4::operator-(const Vector4& rhs) const {
  return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

inline Vector4& Vector4::operator+=(const Vector4& rhs) {
  x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this;
}

inline Vector4& Vector4::operator-=(const Vector4& rhs) {
  x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this;
}

inline Vector4 Vector4::operator*(const float a) const {
  return Vector4(x*a, y*a, z*a, w*a);
}

inline Vector4 Vector4::operator*(const Vector4& rhs) const {
  return Vector4(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
}

inline Vector4& Vector4::operator*=(const float a) {
  x *= a; y *= a; z *= a; w *= a; return *this;
}

inline Vector4& Vector4::operator*=(const Vector4& rhs) {
  x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this;
}

inline Vector4 Vector4::operator/(const float a) const {
  return Vector4(x / a, y / a, z / a, w / a);
}

inline Vector4& Vector4::operator/=(const float a) {
  x /= a; y /= a; z /= a; w /= a; return *this;
}

inline bool Vector4::operator==(const Vector4& rhs) const {
  return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}

inline bool Vector4::operator!=(const Vector4& rhs) const {
  return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
}

inline bool Vector4::operator<(const Vector4& rhs) const {
  if (x < rhs.x) return true;
  if (x > rhs.x) return false;
  if (y < rhs.y) return true;
  if (y > rhs.y) return false;
  if (z < rhs.z) return true;
  if (z > rhs.z) return false;
  if (w < rhs.w) return true;
  if (w > rhs.w) return false;
  return false;
}

inline float Vector4::operator[](int index) const {
  return (&x)[index];
}

inline float& Vector4::operator[](int index) {
  return (&x)[index];
}

inline void Vector4::set(float x, float y, float z, float w) {
  this->x = x; this->y = y; this->z = z; this->w = w;
}

inline float Vector4::length() const {
  return sqrtf(x*x + y*y + z*z + w*w);
}

inline float Vector4::distance(const Vector4& vec) const {
  return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z) + (vec.w - w)*(vec.w - w));
}

inline Vector4& Vector4::normalize() {
  //NOTE: leave w-component untouched
  //@@const float EPSILON = 0.000001f;
  float xxyyzz = x*x + y*y + z*z;
  //@@if(xxyyzz < EPSILON)
  //@@    return *this; // do nothing if it is zero vector

  //float invLength = invSqrt(xxyyzz);
  float invLength = 1.0f / sqrtf(xxyyzz);
  x *= invLength;
  y *= invLength;
  z *= invLength;
  return *this;
}

inline float Vector4::dot(const Vector4& rhs) const {
  return (x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w);
}

inline bool Vector4::equal(const Vector4& rhs, float epsilon) const {
  return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon &&
    fabs(z - rhs.z) < epsilon && fabs(w - rhs.w) < epsilon;
}

inline Vector4 operator*(const float a, const Vector4 vec) {
  return Vector4(a*vec.x, a*vec.y, a*vec.z, a*vec.w);
}

inline std::ostream& operator<<(std::ostream& os, const Vector4& vec) {
  os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
  return os;
}
// END OF VECTOR4 /////////////////////////////////////////////////////////////


namespace {


  bool vr_loaded = false;
  vr::IVRSystem *m_pHMD = nullptr;
  vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];

  std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
  {
    uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
    if (unRequiredBufferLen == 0)
      return "";

    char *pchBuffer = new char[unRequiredBufferLen];
    unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
    std::string sResult = pchBuffer;
    delete[] pchBuffer;
    return sResult;
  }

  struct FramebufferDesc
  {
    GLuint m_nDepthBufferId;
    GLuint m_nRenderTextureId;
    GLuint m_nRenderFramebufferId;
    GLuint m_nResolveTextureId;
    GLuint m_nResolveFramebufferId;
  };
  FramebufferDesc leftEyeDesc;
  FramebufferDesc rightEyeDesc;

  struct VertexDataWindow
  {
    Vector2 position;
    Vector2 texCoord;

    VertexDataWindow(const Vector2 & pos, const Vector2 tex) : position(pos), texCoord(tex) {	}
  };

  size_t m_uiCompanionWindowIndexSize = 0;
  GLuint m_unCompanionWindowVAO;
  GLuint m_glCompanionWindowIDVertBuffer;
  GLuint m_glCompanionWindowIDIndexBuffer;
  uint32_t m_nCompanionWindowWidth = 800;
  uint32_t m_nCompanionWindowHeight = 600;
  GLuint m_unCompanionWindowProgramID;
  
  uint32_t m_nRenderWidth, m_nRenderHeight;
  
  bool m_rbShowTrackedDevice[vr::k_unMaxTrackedDeviceCount];


  GLuint CompileGLShader(const char *pchShaderName, const char *pchVertexShader, const char *pchFragmentShader)
  {
    GLuint unProgramID = glCreateProgram();

    GLuint nSceneVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(nSceneVertexShader, 1, &pchVertexShader, NULL);
    glCompileShader(nSceneVertexShader);

    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(nSceneVertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE)
    {
      printf("%s - Unable to compile vertex shader %d!\n", pchShaderName, nSceneVertexShader);
      glDeleteProgram(unProgramID);
      glDeleteShader(nSceneVertexShader);
      return 0;
    }
    glAttachShader(unProgramID, nSceneVertexShader);
    glDeleteShader(nSceneVertexShader); // the program hangs onto this once it's attached

    GLuint  nSceneFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(nSceneFragmentShader, 1, &pchFragmentShader, NULL);
    glCompileShader(nSceneFragmentShader);

    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(nSceneFragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE)
    {
      printf("%s - Unable to compile fragment shader %d!\n", pchShaderName, nSceneFragmentShader);
      glDeleteProgram(unProgramID);
      glDeleteShader(nSceneFragmentShader);
      return 0;
    }

    glAttachShader(unProgramID, nSceneFragmentShader);
    glDeleteShader(nSceneFragmentShader); // the program hangs onto this once it's attached

    glLinkProgram(unProgramID);

    GLint programSuccess = GL_TRUE;
    glGetProgramiv(unProgramID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE)
    {
      printf("%s - Error linking program %d!\n", pchShaderName, unProgramID);
      glDeleteProgram(unProgramID);
      return 0;
    }

    glUseProgram(unProgramID);
    glUseProgram(0);

    return unProgramID;
  }

} // anon ns - VR


#endif


namespace ROV_Internal {


void
ogl_exec(
  rovGLData *rov_gl_data,
  rovData *rov_data)
{
  BENCH_SCOPED_CPU(OglExec);

  #ifdef ROV_GL4_VR_PROTO

  if (!vr_loaded)
  {
    m_unCompanionWindowProgramID = CompileGLShader(
      "CompanionWindow",

      // vertex shader
      "#version 410 core\n"
      "layout(location = 0) in vec4 position;\n"
      "layout(location = 1) in vec2 v2UVIn;\n"
      "noperspective out vec2 v2UV;\n"
      "void main()\n"
      "{\n"
      "	v2UV = v2UVIn;\n"
      "	gl_Position = position;\n"
      "}\n",

      // fragment shader
      "#version 410 core\n"
      "uniform sampler2D mytexture;\n"
      "noperspective in vec2 v2UV;\n"
      "out vec4 outputColor;\n"
      "void main()\n"
      "{\n"
      "		outputColor = texture(mytexture, v2UV);\n"
      "}\n"
    );

    vr::EVRInitError eError = vr::VRInitError_None;
    m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

    if (eError != vr::VRInitError_None)
    {
      m_pHMD = NULL;
      char buf[1024];

      assert(false);
    }

    auto m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
    if (!m_pRenderModels)
    {
      m_pHMD = NULL;
      vr::VR_Shutdown();

      assert(false);
    }

    std::string m_strDriver = "No Driver";
    std::string m_strDisplay = "No Display";

    m_strDriver = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
    m_strDisplay = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

    vr::EVRInitError peError = vr::VRInitError_None;

    if (!vr::VRCompositor())
    {
      assert(false);
    }

    {
      uint32_t width, height;
      m_pHMD->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);

      auto CreateFrameBuffer = [](int nWidth, int nHeight, FramebufferDesc &framebufferDesc)
      {
        glGenFramebuffers(1, &framebufferDesc.m_nRenderFramebufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nRenderFramebufferId);

        glGenRenderbuffers(1, &framebufferDesc.m_nDepthBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, nWidth, nHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);

        glGenTextures(1, &framebufferDesc.m_nRenderTextureId);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, nWidth, nHeight, true);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId, 0);

        glGenFramebuffers(1, &framebufferDesc.m_nResolveFramebufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nResolveFramebufferId);

        glGenTextures(1, &framebufferDesc.m_nResolveTextureId);
        glBindTexture(GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId, 0);

        // check FBO status
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
          return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
      };

      CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, leftEyeDesc);
      CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, rightEyeDesc);
    }

    {
      std::vector<VertexDataWindow> vVerts;

      // left eye verts
      vVerts.push_back(VertexDataWindow(Vector2(-1, -1), Vector2(0, 1)));
      vVerts.push_back(VertexDataWindow(Vector2(0, -1), Vector2(1, 1)));
      vVerts.push_back(VertexDataWindow(Vector2(-1, 1), Vector2(0, 0)));
      vVerts.push_back(VertexDataWindow(Vector2(0, 1), Vector2(1, 0)));

      // right eye verts
      vVerts.push_back(VertexDataWindow(Vector2(0, -1), Vector2(0, 1)));
      vVerts.push_back(VertexDataWindow(Vector2(1, -1), Vector2(1, 1)));
      vVerts.push_back(VertexDataWindow(Vector2(0, 1), Vector2(0, 0)));
      vVerts.push_back(VertexDataWindow(Vector2(1, 1), Vector2(1, 0)));

      GLushort vIndices[] = { 0, 1, 3,   0, 3, 2,   4, 5, 7,   4, 7, 6 };
      m_uiCompanionWindowIndexSize = _countof(vIndices);

      glGenVertexArrays(1, &m_unCompanionWindowVAO);
      glBindVertexArray(m_unCompanionWindowVAO);

      glGenBuffers(1, &m_glCompanionWindowIDVertBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, m_glCompanionWindowIDVertBuffer);
      glBufferData(GL_ARRAY_BUFFER, vVerts.size() * sizeof(VertexDataWindow), &vVerts[0], GL_STATIC_DRAW);

      glGenBuffers(1, &m_glCompanionWindowIDIndexBuffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glCompanionWindowIDIndexBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiCompanionWindowIndexSize * sizeof(GLushort), &vIndices[0], GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataWindow), (void *)offsetof(VertexDataWindow, position));

      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataWindow), (void *)offsetof(VertexDataWindow, texCoord));

      glBindVertexArray(0);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    vr_loaded = true;
  }

  // Process SteamVR events
  vr::VREvent_t event;
  while (m_pHMD->PollNextEvent(&event, sizeof(event)))
  {
    switch (event.eventType)
    {
    case vr::VREvent_TrackedDeviceActivated:
    {
      //SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
      printf("Device %u attached. Setting up render model.\n", event.trackedDeviceIndex);
    }
    break;
    case vr::VREvent_TrackedDeviceDeactivated:
    {
      printf("Device %u detached.\n", event.trackedDeviceIndex);
    }
    break;
    case vr::VREvent_TrackedDeviceUpdated:
    {
      printf("Device %u updated.\n", event.trackedDeviceIndex);
    }
    break;
    }
  }

  // Process SteamVR controller state
  for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
  {
    vr::VRControllerState_t state;
    if (m_pHMD->GetControllerState(unDevice, &state, sizeof(state)))
    {
      //m_rbShowTrackedDevice[unDevice] = state.ulButtonPressed == 0;
    }
  }

  vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

  static int m_iValidPoseCount = 0;
  static std::string m_strPoseClasses = "";
  static char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];
  static math::mat4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
  math::mat4 hmd_view;

  for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
  {
    if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
    {
      m_iValidPoseCount++;
      

      const float f[] {
        m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[0][0], m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[1][0], m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[2][0], 0.0,
        m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[0][1], m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[1][1], m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[2][1], 0.0,
        m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[0][2], m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[1][2], m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[2][2], 0.0,
        m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[0][3], m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[1][3], m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking.m[2][3], 1.0f
      };

      m_rmat4DevicePose[nDevice] = math::mat4_init(f);

      if (m_rDevClassChar[nDevice] == 0)
      {
        switch (m_pHMD->GetTrackedDeviceClass(nDevice))
        {
        case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
        case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
        case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
        case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
        case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
        default:                                       m_rDevClassChar[nDevice] = '?'; break;
        }
      }
      m_strPoseClasses += m_rDevClassChar[nDevice];
    }
  }

  if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
  {
    hmd_view = math::mat4_get_inverse(m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd]);
    //m_mat4HMDPose.invert();
  }

  #endif

  #ifdef GL_HAS_VAO
  glBindVertexArray(rov_gl_data->vao);
  #endif

  glUseProgram(0);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  
  #ifdef ROV_GL4_VR_PROTO
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_MULTISAMPLE);

    for(uint32_t e = 0; e < 2; ++e)
    {

      if (e == 0)
      {
        glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
        glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);

      }
      else
      {
        glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
        glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
      }
   
  #endif

  /*
    For each renderpass.
  */
  const size_t pass_count = rov_data->rov_render_passes.size();
  const rovRenderPass *passes = rov_data->rov_render_passes.data();
  
  for(size_t p = 0; p < pass_count; ++p)
  {
    const rovRenderPass &rp = passes[p];
  
    GLbitfield cl_flags = 0;
    if(rp.clear_flags & rovClearFlag_Color) { cl_flags |= GL_COLOR_BUFFER_BIT; }
    if(rp.clear_flags & rovClearFlag_Depth) { cl_flags |= GL_DEPTH_BUFFER_BIT; }

    const float cl_color[4]
    {
      lib::color::get_channel_1f(rp.clear_color),
      lib::color::get_channel_2f(rp.clear_color),
      lib::color::get_channel_3f(rp.clear_color),
      lib::color::get_channel_4f(rp.clear_color),
    };

    glClearColor(cl_color[0], cl_color[1], cl_color[2], cl_color[3]);
    glClear(cl_flags);
    glEnable(GL_DEPTH_TEST);
    //glViewport(rp.viewport[0], rp.viewport[1], rp.viewport[2], rp.viewport[3]);

    #ifdef ROV_GL4_VR_PROTO
    math::mat4 proj = math::mat4_init(rp.proj);
    if (e == 0)
    {
      vr::HmdMatrix44_t mat = m_pHMD->GetProjectionMatrix(vr::Eye_Left, 0.1f, 100.f);
      const float data[] = {
        mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
        mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
        mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
        mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
      };
      proj = math::mat4_init(data);
    }
    else
    {
      vr::HmdMatrix44_t mat = m_pHMD->GetProjectionMatrix(vr::Eye_Right, 0.1f, 100.f);
      const float data[] = {
        mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
        mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
        mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
        mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
      };
      proj = math::mat4_init(data);
    }
    #else
    const math::mat4 proj = math::mat4_init(rp.proj);
    #endif

    #ifdef ROV_GL4_PROTO
    const math::mat4 view = math::mat4_init(rp.view);
    #else
    const math::mat4 view = hmd_view;
    #endif

    const math::mat4 view_proj = math::mat4_multiply(view, proj);

    /*
      For each material in the pass.
    */
    size_t dc_index = 0;
    
    BENCH_PUSH_GPU(MeshRender); 
    
    GLuint last_shd = 0;

    const size_t mat_count = rp.materials.size();

    for(size_t k = 0; k < mat_count; ++k)
//    for(auto &mat : rp.materials)
    {
      auto &mat = rp.materials[k];
    
      if(mat.draw_calls == 0)
      {
        continue;
      }
    
      // Pull Material Info Out
      const uint32_t color   = lib::bits::upper32(mat.material);

      const float colorf[4] {
        math::to_float(lib::bits::first8(color)) / 255.f,
        math::to_float(lib::bits::second8(color)) / 255.f,
        math::to_float(lib::bits::third8(color)) / 255.f,
        math::to_float(lib::bits::forth8(color)) / 255.f,
      };

      const uint32_t details = lib::bits::lower32(mat.material);

      const uint8_t shader = rovShader_Lit;//lib::bits::first8(details);
      const uint8_t texture_01 = lib::bits::second8(details);
      const uint8_t texture_02 = lib::bits::third8(details);
      const uint8_t texture_03 = lib::bits::forth8(details);
      
      const rovGLMeshProgram shd = rov_gl_data->rov_mesh_programs[shader];
      

      /*
        Bind the shader
      */
      if(shd.program != last_shd)
      {
        last_shd = shd.program;
        glUseProgram(shd.program);
      }
      
      /*
        Load the textures and buffers
      */
      {
        uint32_t texture_slots = 0;
        
        /*
          Texture Maps
        */
        const uint8_t texture_maps[] = { texture_01, texture_02, texture_03 };
        
        for(uint8_t t = 0; t < rov_max_texture_maps; ++t)
        {
          if(texture_maps[t] && shd.uni_tex[t] != -1)
          {
            const size_t texture_index = texture_maps[t]  - 1;

            if(texture_index < rov_gl_data->rov_textures.size())
            {
              const rovGLTexture tex = rov_gl_data->rov_textures[texture_index];
              
              glActiveTexture(GL_TEXTURE0 + texture_slots);
              glBindTexture(GL_TEXTURE_2D, tex.gl_id);
              
              glUniform1i(shd.uni_tex[t], texture_slots);
              
              ++texture_slots;
            }
          }
        }
        
        /*
          Buffers
        */
        if(shd.uni_buffer_01 != -1)
        {
          const GLuint slot = texture_slots + 1; // Unsure why, if I bind to slot 0 we get 1282's.
          
          const size_t l_index = rp.light_buffer - 1;
          const rovGLLightPack light_pack = rov_gl_data->light_buffers[l_index];
          
          glUniform1i(shd.uni_light_count, light_pack.count);
          
          glActiveTexture(GL_TEXTURE0 + slot);
          glBindTexture(GL_TEXTURE_1D, light_pack.gl_id);
          
          glUniform1i(shd.uni_buffer_01, slot);
          
          ++texture_slots;
        }
      }
      
      /*
        For each draw call in the material.
      */
      for(uint32_t i = 0; i < mat.draw_calls; ++i)
      {
        auto &dc = rp.draw_calls[dc_index++];

        rovGLMesh vbo;
        
        if(dc.mesh > 0)
        {
          vbo = rov_gl_data->rov_meshes[dc.mesh - 1];
          glBindBuffer(GL_ARRAY_BUFFER, vbo.gl_id);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
          vbo = rovGLMesh{};
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          
          continue;
        }
        
        /*
          Load input format
        */
        {
          size_t pointer = 0;

          if(shd.vs_in_pos >= 0)
          {
            glEnableVertexAttribArray(shd.vs_in_pos);
            glVertexAttribPointer(
              shd.vs_in_pos,
              3,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }

          pointer += (sizeof(float) * 3);

          if(shd.vs_in_norm >= 0)
          {
            glEnableVertexAttribArray(shd.vs_in_norm);
            glVertexAttribPointer(
              shd.vs_in_norm,
              3,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }

          pointer += (sizeof(float) * 3);

          if(shd.vs_in_uv)
          {
            glEnableVertexAttribArray(shd.vs_in_uv);
            glVertexAttribPointer(
              shd.vs_in_uv,
              2,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }
        }
        
        /*
          Apply Matrices and other uniforms.
        */
        {
          

          #ifdef ROV_GL4_VR_PROTO
          math::mat4 world = math::mat4_init(dc.world);

          
          if (e == 0)
          {
            vr::HmdMatrix34_t matEyeRight = m_pHMD->GetEyeToHeadTransform(vr::Eye_Left);
            const float data[] {
              matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
              matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
              matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
              matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
            };


            math::mat4 other = math::mat4_init(data);
            world = math::mat4_multiply(world, other);
          }
          else
          {
            vr::HmdMatrix34_t matEyeRight = m_pHMD->GetEyeToHeadTransform(vr::Eye_Right);
            const float data[]{
              matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
              matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
              matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
              matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
            };

            math::mat4 other = math::mat4_init(data);
            world = math::mat4_multiply(world, other);
          }
          #else
          const math::mat4 world = math::mat4_init(dc.world);
          #endif

          const math::mat4 wvp_mat  = math::mat4_multiply(world, view, proj);
//          const math::vec4 pos      = math::mat4_multiply(math::vec4_init(0,0,0,1), math::mat4_get_inverse(view));
          const math::mat4 mv_mat   = math::mat4_multiply(world, view);
          const math::mat4 norm_mat = math::mat4_get_transpose(math::mat4_get_inverse(mv_mat));
  
          glUniformMatrix4fv(shd.uni_wvp,     1, GL_FALSE, math::mat4_get_data(wvp_mat));
          glUniformMatrix4fv(shd.uni_view,    1, GL_FALSE, math::mat4_get_data(view));
          glUniformMatrix4fv(shd.uni_world,   1, GL_FALSE, math::mat4_get_data(world));
          glUniformMatrix4fv(shd.uni_normal,  1, GL_FALSE, math::mat4_get_data(norm_mat));
          glUniformMatrix4fv(shd.uni_wv,      1, GL_FALSE, math::mat4_get_data(mv_mat));
          glUniform3fv(shd.uni_eye,           1, rp.eye_position);
          glUniform4fv(shd.uni_color,         1, colorf);
        }
        
        /*
          Draw
        */
        glDrawArrays(GL_TRIANGLES, 0, vbo.vertex_count);
        
        /*
          Error Check
        */
        const GLuint err = glGetError();
        if(err)
        {
          LOG_ERROR("GL Error - Mesh Drawing")
        }
      }
    } // For amts
    
    BENCH_POP_GPU;

    // Line Renderer
    #ifdef GL_HAS_GEO_SHD
    {
//      BENCH_SCOPED_GPU(DebugLineRender);
    
      glUseProgram(rov_gl_data->rov_line_programs[0].program);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      
      // -- Draw the Batches Of Lines -- //
      const size_t batches = (rp.line_draw_calls.size() / 32) + 1;

      for(size_t i = 0; i < batches; ++i)
      {
        // -- Load Camera -- //
        glUniformMatrix4fv(
          rov_gl_data->rov_line_programs[0].uni_wvp,
          1,
          GL_FALSE,
          math::mat4_get_data(view_proj)
        );

        const size_t count(
          ((i + 1) == batches) ?
            rp.line_draw_calls.size() % 32 : 32
        );

        const uint32_t batch_start = i * 32;

        glUniform3fv(
          rov_gl_data->rov_line_programs[0].uni_line_buffer,
          count * 3,
          (float*)&rp.line_draw_calls[batch_start]
        );

        glDrawArrays(GL_POINTS, 0, count);
      }
    }
    #endif
  }

#ifdef ROV_GL4_VR_PROTO
  
    if (e == 0)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glDisable(GL_MULTISAMPLE);

      glBindFramebuffer(GL_READ_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, leftEyeDesc.m_nResolveFramebufferId);

      glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR);

      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    }
    else
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glDisable(GL_MULTISAMPLE);

      glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEyeDesc.m_nResolveFramebufferId);

      glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR);

      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    }

    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, m_nCompanionWindowWidth, m_nCompanionWindowHeight);

    glBindVertexArray(m_unCompanionWindowVAO);
    glUseProgram(m_unCompanionWindowProgramID);

    // render left eye (first half of index array )
    glBindTexture(GL_TEXTURE_2D, leftEyeDesc.m_nResolveTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glDrawElements(GL_TRIANGLES, m_uiCompanionWindowIndexSize / 2, GL_UNSIGNED_SHORT, 0);

    // render right eye (second half of index array )
    glBindTexture(GL_TEXTURE_2D, rightEyeDesc.m_nResolveTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glDrawElements(GL_TRIANGLES, m_uiCompanionWindowIndexSize / 2, GL_UNSIGNED_SHORT, (const void *)(uintptr_t)(m_uiCompanionWindowIndexSize));

    glBindVertexArray(0);
    glUseProgram(0);

    vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)leftEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
    vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)rightEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

    glFinish();

#endif

  #ifdef GL_HAS_VAO
  glBindVertexArray(0);
  #endif
}


} // ns
