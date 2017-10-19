#include "include/ArgeoTest.h"
#include "Device.h"
#include "InputManager.h"
#include "BillboardGraphics.h"

namespace argeo
{
	namespace jni
	{
		ArgeoTest::ArgeoTest()
		{ }

		ArgeoTest::~ArgeoTest()
		{ }

		void ArgeoTest::on_start()
		{
			InputManager::add_gesture_listener(this);
		}

		void ArgeoTest::on_stop()
		{
			InputManager::remove_gesture_listener(this);
		}

		void ArgeoTest::load_resources()
		{
			ArgeoNativeActivity::load_resources();
		}

		void ArgeoTest::unload_resources()
		{
			ArgeoNativeActivity::unload_resources();
		}

		bool ArgeoTest::on_tap_event(const TouchEvent& args)
		{
			Viewer* viewer  = get_viewer();

			viewer->execute_on_render_loop([&, args](Context* context)
			{
				vec3d point = get_viewer()->pick_position(vec2d(
					args.point.x,
					args.point.y));
				
				if (!std::isnan(point.x) &&
					!std::isnan(point.y) &&
					!std::isnan(point.z))
				{
					Entity* entity = new Entity(
						"epe",
						new BillboardGraphics(
							"icons/032.png"));

					entity->set_position(point);
					viewer->get_entities()->add(entity);
				}

			});

			//IPickable* object = view->get_scene()->pick_primitive(vec2(
			//	args.point.x,
			//	args.point.y));

			return true;
		}
	}
}