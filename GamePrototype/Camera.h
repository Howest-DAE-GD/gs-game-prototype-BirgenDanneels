#pragma once
class Camera
{
private:

	Rectf m_CameraRect;	//Can also just be a Poin2f
	Rectf m_ObjectRect; //In this rect the object can move around
	Rectf m_OriginalOR;
	Rectf m_OriginalCR;
	float m_Scale;

public:

	explicit Camera(float windowWidth, float windowHeight, const Rectf& objectRect, float scale);
	explicit Camera();
	~Camera();

	void TranslateAndScaleCamera() const;
	void UpdateTranslationCamera(const Point2f& posObject);

	Rectf GetCameraRect() const;
	Rectf GetObjectRect() const;
	void SetScale(float scale);
};

