#pragma once

#include "Scene.h"
#include "Bitmap.h"

namespace argeo
{
	class SnapshotListener
	{
	public:

		virtual void on_snapshot_available(
			Bitmap* snapshot) = 0;

		virtual void on_ar_buffer_snapshot_available(
			Bitmap* reality_snapshot,
			Bitmap* virtuality_snapshot,
			Bitmap* mixed_reality_snapshot) = 0;

		virtual void on_ttar_snapshot_available(
			Bitmap* reality_snapshot,
			Bitmap* ttar_snapshot,
			Camera* camera) = 0;
	};
}
