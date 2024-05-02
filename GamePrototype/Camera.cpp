#include "pch.h"
#include "Camera.h"

Camera::Camera(float windowWidth, float windowHeight, const Rectf& objectRect, float scale)
    :m_ObjectRect{ objectRect },
    m_Scale{ scale }
{
    m_CameraRect = Rectf{ 0,0,windowWidth / scale, windowHeight / scale };
}

Camera::Camera()
    :Camera(0.f, 0.f, Rectf{ 0,0,0,0 }, 0.f)
{
}

Camera::~Camera()
{

}

void Camera::TranslateAndScaleCamera() const
{
    glTranslatef(-m_CameraRect.left * m_Scale, -m_CameraRect.bottom * m_Scale, 0);
    glScaled(m_Scale, m_Scale, 1);
}

void Camera::UpdateTranslationCamera(const Point2f& posObject)
{
    //Check X bounds

    if (posObject.x < m_ObjectRect.left)
    {
        float cameraOffset{ m_ObjectRect.left - posObject.x };

        m_CameraRect.left -= cameraOffset;
        m_ObjectRect.left -= cameraOffset;
    }
    else if (posObject.x > m_ObjectRect.left + m_ObjectRect.width)
    {
        float cameraOffset{ posObject.x - m_ObjectRect.left - m_ObjectRect.width };

        m_CameraRect.left += cameraOffset;
        m_ObjectRect.left += cameraOffset;
    }


    //Check Y bounds

    if (posObject.y < m_ObjectRect.bottom)
    {
        float cameraOffset{ m_ObjectRect.bottom - posObject.y };

        m_CameraRect.bottom -= cameraOffset;
        m_ObjectRect.bottom -= cameraOffset;
    }
    else if (posObject.y > m_ObjectRect.bottom + m_ObjectRect.height)
    {
        float cameraOffset{ posObject.y - m_ObjectRect.bottom - m_ObjectRect.height };

        m_CameraRect.bottom += cameraOffset;
        m_ObjectRect.bottom += cameraOffset;
    }
}

Rectf Camera::GetCameraRect() const
{
    return m_CameraRect;
}

Rectf Camera::GetObjectRect() const
{
    return m_ObjectRect;
}
