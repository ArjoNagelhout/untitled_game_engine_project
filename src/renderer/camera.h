//
// Created by Arjo Nagelhout on 16/12/2023.
//

#ifndef BORED_ENGINE_CAMERA_H
#define BORED_ENGINE_CAMERA_H

#include "math/matrix.h"

#include "graphics/device.h"
#include "graphics/buffer.h"

namespace renderer
{
	enum class CameraProjection
	{
		Perspective,
		Orthographic,
	};

	// a camera is a simple piece of data that does not do anything intelligent,
	// nor does it exist in a hierarchy. This makes refactoring the transform hierarchy easier
	//
	// it only generates a frustum from the set properties
	class Camera
	{
	public:
		explicit Camera(graphics::IDevice* pDevice);
		~Camera();

		//
		[[nodiscard]] CameraProjection getCameraProjection() const;

		//
		void setCameraProjection(CameraProjection _cameraProjection);

		//
		[[nodiscard]] float getAspectRatio() const;

		//
		void setAspectRatio(float _aspectRatio);

		//
		[[nodiscard]] float getFieldOfView() const;

		//
		void setFieldOfView(float _fieldOfView);

		// get buffer
		[[nodiscard]] graphics::IBuffer* getCameraDataBuffer() const;

	private:
		math::mat4 worldSpaceTransform = math::mat4::identity;
		CameraProjection cameraProjection{CameraProjection::Perspective};
		float aspectRatio{1.0f}; // width / height
		float fieldOfView{30.0f}; // in degrees
		float zNear{0.1f};
		float zFar{1000.0f};

		std::unique_ptr<graphics::IBuffer> pBuffer;
	};
}

#endif //BORED_ENGINE_CAMERA_H
