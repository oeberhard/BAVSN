/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#define JNI_CLASS_MEMBERS(METHOD, STATICMETHOD, FIELD, STATICFIELD) \
 METHOD (layout,        "layout",        "(IIII)V") \
 METHOD (requestRender, "requestRender", "()V") \

DECLARE_JNI_CLASS (OpenGLView, JUCE_ANDROID_ACTIVITY_CLASSPATH "$OpenGLView");
#undef JNI_CLASS_MEMBERS

extern jobject createOpenGLView (ComponentPeer*);

//==============================================================================
class OpenGLContext::NativeContext
{
public:
    NativeContext (Component& comp,
                   const OpenGLPixelFormat& pixelFormat,
                   void* /*contextToShareWith*/)
        : component (comp),
          isInsideGLCallback (false)
    {
        {
            const ScopedLock sl (contextListLock);
            glView = GlobalRef (createOpenGLView (component.getPeer()));
            contextList.add (this);
        }

        updateWindowPosition (component.getTopLevelComponent()
                                ->getLocalArea (&component, component.getLocalBounds()));
    }

    ~NativeContext()
    {
        {
            const ScopedLock sl (contextListLock);
            contextList.removeFirstMatchingValue (this);
        }

        android.activity.callVoidMethod (JuceAppActivity.deleteView, glView.get());
        glView.clear();
    }

    void initialiseOnRenderThread() {}
    void shutdownOnRenderThread() {}

    bool makeActive() const noexcept            { return isInsideGLCallback; }
    bool isActive() const noexcept              { return isInsideGLCallback; }
    static void deactivateCurrentContext()      {}

    void swapBuffers() const noexcept           {}
    bool setSwapInterval (const int)            { return false; }
    int getSwapInterval() const                 { return 0; }

    bool createdOk() const noexcept             { return getRawContext() != nullptr; }
    void* getRawContext() const noexcept        { return glView.get(); }
    GLuint getFrameBufferID() const noexcept    { return 0; }

    void updateWindowPosition (const Rectangle<int>& bounds)
    {
        if (lastBounds != bounds)
        {
            lastBounds = bounds;
            glView.callVoidMethod (OpenGLView.layout,
                                   bounds.getX(), bounds.getY(),
                                   bounds.getRight(), bounds.getBottom());
        }
    }

    void triggerRepaint()
    {
        glView.callVoidMethod (OpenGLView.requestRender);
    }

    //==============================================================================
    void contextCreatedCallback();
    void contextChangedSize() {}
    void renderCallback();

    //==============================================================================
    static NativeContext* findContextFor (JNIEnv* env, jobject glView)
    {
        const ScopedLock sl (contextListLock);

        for (int i = contextList.size(); --i >= 0;)
        {
            NativeContext* const c = contextList.getUnchecked(i);

            if (env->IsSameObject (c->glView.get(), glView))
                return c;
        }

        return nullptr;
    }

    static NativeContext* getActiveContext() noexcept
    {
        const ScopedLock sl (contextListLock);

        for (int i = contextList.size(); --i >= 0;)
        {
            NativeContext* const c = contextList.getUnchecked(i);

            if (c->isInsideGLCallback)
                return c;
        }

        return nullptr;
    }

    struct Locker { Locker (NativeContext&) {} };

    Component& component;

private:
    GlobalRef glView;
    Rectangle<int> lastBounds;
    bool isInsideGLCallback;

    typedef Array<NativeContext*> ContextArray;
    static CriticalSection contextListLock;
    static ContextArray contextList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext);
};

CriticalSection OpenGLContext::NativeContext::contextListLock;
OpenGLContext::NativeContext::ContextArray OpenGLContext::NativeContext::contextList;

//==============================================================================
bool OpenGLHelpers::isContextActive()
{
    return OpenGLContext::NativeContext::getActiveContext() != nullptr;
}

//==============================================================================
#define GL_VIEW_CLASS_NAME    JUCE_JOIN_MACRO (JUCE_ANDROID_ACTIVITY_CLASSNAME, _00024OpenGLView)

JUCE_JNI_CALLBACK (GL_VIEW_CLASS_NAME, contextCreated, void, (JNIEnv* env, jobject view))
{
    threadLocalJNIEnvHolder.getOrAttach();

    OpenGLContext::NativeContext* const context = OpenGLContext::NativeContext::findContextFor (env, view);
    jassert (context != nullptr);

    if (context != nullptr)
        context->contextCreatedCallback();
}

JUCE_JNI_CALLBACK (GL_VIEW_CLASS_NAME, contextChangedSize, void, (JNIEnv* env, jobject view))
{
    OpenGLContext::NativeContext* const context = OpenGLContext::NativeContext::findContextFor (env, view);

    if (context != nullptr)
        context->contextChangedSize();
}

JUCE_JNI_CALLBACK (GL_VIEW_CLASS_NAME, render, void, (JNIEnv* env, jobject view))
{
    OpenGLContext::NativeContext* const context = OpenGLContext::NativeContext::findContextFor (env, view);

    if (context != nullptr)
        context->renderCallback();
}
