#include "include/GestureDetector.h"

//-------------------------------------------------------------------------------- 
// gestureDetector.cpp 
//-------------------------------------------------------------------------------- 
namespace argeo
{

	//-------------------------------------------------------------------------------- 
	// includes 
	//-------------------------------------------------------------------------------- 

	//-------------------------------------------------------------------------------- 
	// GestureDetector 
	//-------------------------------------------------------------------------------- 
	GestureDetector::GestureDetector()
	{
		dp_factor_ = 1.f;
	}

	void GestureDetector::SetConfiguration(AConfiguration* config)
	{
		dp_factor_ = 160.f / AConfiguration_getDensity(config);
	}

	//-------------------------------------------------------------------------------- 
	// TapDetector 
	//-------------------------------------------------------------------------------- 
	GESTURE_STATE TapDetector::Detect(const AInputEvent* motion_event)
	{
		if (AMotionEvent_getPointerCount(motion_event) > 1)
		{
			//Only support single touch 
			return false;
		}

		int32_t action = AMotionEvent_getAction(motion_event);
		unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
		switch (flags)
		{
		case AMOTION_EVENT_ACTION_DOWN:
			down_pointer_id_ = AMotionEvent_getPointerId(motion_event, 0);
			down_x_ = AMotionEvent_getX(motion_event, 0);
			down_y_ = AMotionEvent_getY(motion_event, 0);
			break;
		case AMOTION_EVENT_ACTION_UP:
		{
			int64_t eventTime = AMotionEvent_getEventTime(motion_event);
			int64_t downTime = AMotionEvent_getDownTime(motion_event);
			if (eventTime - downTime <= TAP_TIMEOUT)
			{
				if (down_pointer_id_ == AMotionEvent_getPointerId(motion_event, 0))
				{
					float x = AMotionEvent_getX(motion_event, 0) - down_x_;
					float y = AMotionEvent_getY(motion_event, 0) - down_y_;
					if (x * x + y * y < TOUCH_SLOP * TOUCH_SLOP * dp_factor_)
					{
						return GESTURE_STATE_ACTION;
					}
				}
			}
			break;
		}
		}
		return GESTURE_STATE_NONE;
	}

	//-------------------------------------------------------------------------------- 
	// DoubletapDetector 
	//-------------------------------------------------------------------------------- 
	GESTURE_STATE DoubletapDetector::Detect(const AInputEvent* motion_event)
	{
		if (AMotionEvent_getPointerCount(motion_event) > 1)
		{
			//Only support single double tap 
			return false;
		}

		bool tap_detected = tap_detector_.Detect(motion_event);

		int32_t action = AMotionEvent_getAction(motion_event);
		unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
		switch (flags)
		{
		case AMOTION_EVENT_ACTION_DOWN:
		{
			int64_t eventTime = AMotionEvent_getEventTime(motion_event);
			if (eventTime - last_tap_time_ <= DOUBLE_TAP_TIMEOUT)
			{
				float x = AMotionEvent_getX(motion_event, 0) - last_tap_x_;
				float y = AMotionEvent_getY(motion_event, 0) - last_tap_y_;
				if (x * x + y * y < DOUBLE_TAP_SLOP * DOUBLE_TAP_SLOP * dp_factor_)
				{
					return GESTURE_STATE_ACTION;
				}
			}
			break;
		}
		case AMOTION_EVENT_ACTION_UP:
			if (tap_detected)
			{
				last_tap_time_ = AMotionEvent_getEventTime(motion_event);
				last_tap_x_ = AMotionEvent_getX(motion_event, 0);
				last_tap_y_ = AMotionEvent_getY(motion_event, 0);
			}
			break;
		}
		return GESTURE_STATE_NONE;
	}

	void DoubletapDetector::SetConfiguration(AConfiguration* config)
	{
		dp_factor_ = 160.f / AConfiguration_getDensity(config);
		tap_detector_.SetConfiguration(config);
	}

	//-------------------------------------------------------------------------------- 
	// PinchDetector 
	//-------------------------------------------------------------------------------- 

	int32_t PinchDetector::FindIndex(const AInputEvent* event, int32_t id)
	{
		int32_t count = AMotionEvent_getPointerCount(event);
		for (uint32_t i = 0; i < count; ++i)
		{
			if (id == AMotionEvent_getPointerId(event, i))
				return i;
		}
		return -1;
	}

	GESTURE_STATE PinchDetector::Detect(const AInputEvent* event)
	{
		GESTURE_STATE ret = GESTURE_STATE_NONE;
		int32_t action = AMotionEvent_getAction(event);
		uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;
		event_ = event;

		int32_t count = AMotionEvent_getPointerCount(event);
		switch (flags)
		{
		case AMOTION_EVENT_ACTION_DOWN:
			vec_pointers_.push_back(AMotionEvent_getPointerId(event, 0));
			break;
		case AMOTION_EVENT_ACTION_POINTER_DOWN:
		{
			int32_t iIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
				>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
			vec_pointers_.push_back(AMotionEvent_getPointerId(event, iIndex));
			if (count == 2)
			{
				//Start new pinch 
				ret = GESTURE_STATE_START;
			}
		}
		break;
		case AMOTION_EVENT_ACTION_UP:
			vec_pointers_.pop_back();
			break;
		case AMOTION_EVENT_ACTION_POINTER_UP:
		{
			int32_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
				>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
			int32_t released_pointer_id = AMotionEvent_getPointerId(event, index);

			std::vector<int32_t>::iterator it = vec_pointers_.begin();
			std::vector<int32_t>::iterator it_end = vec_pointers_.end();
			int32_t i = 0;
			for (; it != it_end; ++it, ++i)
			{
				if (*it == released_pointer_id)
				{
					vec_pointers_.erase(it);
					break;
				}
			}

			if (i <= 1)
			{
				//Reset pinch or drag 
				if (count != 2)
				{
					//Start new pinch 
					ret = GESTURE_STATE_START | GESTURE_STATE_END;
				}
			}
		}
		break;
		case AMOTION_EVENT_ACTION_MOVE:
			switch (count)
			{
			case 1:
				break;
			default:
				//Multi touch 
				ret = GESTURE_STATE_MOVE;
				break;
			}
			break;
		case AMOTION_EVENT_ACTION_CANCEL:
			break;
		}

		return ret;
	}

	bool PinchDetector::GetPointers(vec2& v1, vec2& v2)
	{
		if (vec_pointers_.size() < 2)
			return false;

		int32_t index = FindIndex(event_, vec_pointers_[0]);
		if (index == -1)
			return false;

		float x = AMotionEvent_getX(event_, index);
		float y = AMotionEvent_getY(event_, index);

		index = FindIndex(event_, vec_pointers_[1]);
		if (index == -1)
			return false;

		float x2 = AMotionEvent_getX(event_, index);
		float y2 = AMotionEvent_getY(event_, index);

		v1 = vec2(x, y);
		v2 = vec2(x2, y2);

		return true;
	}

	//-------------------------------------------------------------------------------- 
	// DragDetector 
	//-------------------------------------------------------------------------------- 

	int32_t DragDetector::FindIndex(const AInputEvent* event, int32_t id)
	{
		int32_t count = AMotionEvent_getPointerCount(event);
		for (uint32_t i = 0; i < count; ++i)
		{
			if (id == AMotionEvent_getPointerId(event, i))
				return i;
		}
		return -1;
	}

	GESTURE_STATE DragDetector::Detect(const AInputEvent* event)
	{
		GESTURE_STATE ret = GESTURE_STATE_NONE;
		int32_t action = AMotionEvent_getAction(event);
		int32_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
			>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;
		event_ = event;

		int32_t count = AMotionEvent_getPointerCount(event);
		switch (flags)
		{
		case AMOTION_EVENT_ACTION_DOWN:
			vec_pointers_.push_back(AMotionEvent_getPointerId(event, 0));
			ret = GESTURE_STATE_START;
			break;
		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			vec_pointers_.push_back(AMotionEvent_getPointerId(event, index));
			break;
		case AMOTION_EVENT_ACTION_UP:
			vec_pointers_.pop_back();
			ret = GESTURE_STATE_END;
			break;
		case AMOTION_EVENT_ACTION_POINTER_UP:
		{
			int32_t released_pointer_id = AMotionEvent_getPointerId(event, index);

			std::vector<int32_t>::iterator it = vec_pointers_.begin();
			std::vector<int32_t>::iterator it_end = vec_pointers_.end();
			int32_t i = 0;
			for (; it != it_end; ++it, ++i)
			{
				if (*it == released_pointer_id)
				{
					vec_pointers_.erase(it);
					break;
				}
			}

			if (i <= 1)
			{
				//Reset pinch or drag 
				if (count == 2)
				{
					ret = GESTURE_STATE_START;
				}
			}
			break;
		}

		case AMOTION_EVENT_ACTION_MOVE:
			switch (count)
			{
			case 1:
				//Drag 
				ret = GESTURE_STATE_MOVE;
				break;
			default:
				break;
			}
			break;
		case AMOTION_EVENT_ACTION_CANCEL:
			break;
		}

		return ret;
	}

	bool DragDetector::GetPointer(vec2& v)
	{
		if (vec_pointers_.size() < 1)
			return false;

		int32_t iIndex = FindIndex(event_, vec_pointers_[0]);
		if (iIndex == -1)
			return false;

		float x = AMotionEvent_getX(event_, iIndex);
		float y = AMotionEvent_getY(event_, iIndex);

		v = vec2(x, y);

		return true;
	}

}   //namespace ndkHelper