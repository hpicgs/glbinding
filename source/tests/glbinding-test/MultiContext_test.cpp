
#include <gmock/gmock.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glbinding/AbstractFunction.h>
#include <glbinding/Version.h>
#include <glbinding/Binding.h>
#include <glbinding/glbinding.h>
#include <glbinding-aux/ContextInfo.h>

#include <glbinding/gl/gl.h>

using namespace gl;
using namespace glbinding;
using namespace aux;

class MultiContext_test : public testing::Test
{
public:
};

namespace 
{
    void error(int /*errnum*/, const char * /*errmsg*/)
    {
        FAIL();
    }
}

TEST_F(MultiContext_test, Test)
{
    int success = glfwInit();

    EXPECT_EQ(1, success);

    glfwSetErrorCallback(error);

    glfwWindowHint(GLFW_VISIBLE, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, false); // does not work for all opengl drivers
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window1 = glfwCreateWindow(320, 240, "", nullptr, nullptr);

    EXPECT_NE(nullptr, window1);

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, false);

    GLFWwindow * window2 = glfwCreateWindow(320, 240, "", nullptr, nullptr);

    EXPECT_NE(nullptr, window2);

    glfwMakeContextCurrent(window1);
    glbinding::initialize(0, [](const char * name) {
        return glfwGetProcAddress(name);
    });

#ifdef  SYSTEM_WINDOWS
    EXPECT_EQ(Version(3, 2), ContextInfo::version());
#elif defined(SYSTEM_DARWIN)
    EXPECT_EQ(Version(4, 1), ContextInfo::version());
    EXPECT_EQ(nullptr, Binding::DispatchCompute.address());
#else // Linux
    EXPECT_EQ(Version(3, 2), ContextInfo::version());
    EXPECT_NE(nullptr, Binding::DispatchCompute.address());
#endif

    glfwMakeContextCurrent(window2);
    glbinding::initialize(1, [](const char * name) {
           return glfwGetProcAddress(name);
    });

    Binding::releaseContext(1);
    glfwMakeContextCurrent(window1);
    Binding::releaseContext(2);

    glfwTerminate();
}
