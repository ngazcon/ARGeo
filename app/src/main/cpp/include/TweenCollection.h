#pragma once

#include "EasingFunction.h"

#include <memory>
#include <functional>
#include <tween.h>

namespace argeo
{
    class TweenCollection;
    struct Tween
    {
        friend class TweenCollection;

        std::function<void()> cancel;

    private:
        tween::Tween m_internal_tween;
    };

    class TweenCollection
    {
    public:

        ~TweenCollection()
        {
            remove_all();
        }

        template <typename T>
        Tween* add(
            T& start_object,
            T& stop_object,
            const double& duration,
            const double& delay,
            const EasingFunction& easing_function,
            std::function<void(T&)> update_callback,
            std::function<void()> complete_callback  = nullptr,
            std::function<void()> cancelled_callback = nullptr)
        {
            if (duration < 0.0)
            {
                throw std::invalid_argument("duration must be positive.");
            }

            Tween* result = new Tween();
            m_collection.insert(result);

            if (duration == 0.0)
            {
                complete_callback();
                return result;
            }

            result->cancel = cancelled_callback;

            result->m_internal_tween = tween::Tween::make()
                .fromTo(start_object, stop_object)
                .delay(delay)
                .onUpdate(update_callback)
                .onComplete(complete_callback)
                .seconds(duration);

            switch (easing_function)
            {
                case EasingFunction::EaseInBack:
                    result->m_internal_tween = result->m_internal_tween.easeinback();
                    break;
                case EasingFunction::EaseInsine:
                    result->m_internal_tween = result->m_internal_tween.easeinsine();
                    break;
                case EasingFunction::EaseOutBack:
                    result->m_internal_tween = result->m_internal_tween.easeoutback();
                    break;
                case EasingFunction::EaseOutsine:
                    result->m_internal_tween = result->m_internal_tween.easeoutsine();
                    break;
                case EasingFunction::Linear:
                    result->m_internal_tween = result->m_internal_tween.linear();
                    break;
                case EasingFunction::Repeating:
                    result->m_internal_tween = result->m_internal_tween.repeating();
                    break;
                case EasingFunction::Sine:
                    result->m_internal_tween = result->m_internal_tween.sine();
                    break;
                case EasingFunction::SmoothStep:
                    result->m_internal_tween = result->m_internal_tween.smoothstep();
                    break;
            }

            return result;
        }


        bool remove(Tween* tween)
        {
            if (tween == nullptr)
            {
                return false;
            }

            if (m_collection.find(tween) != m_collection.end())
            {
                tween->m_internal_tween.cancel();

                if(tween->cancel != nullptr)
                {
                    tween->cancel();
                }

                m_collection.erase(tween);
                delete tween;

                return true;
            }

            return false;

        }

        void remove_all()
        {
            std::unordered_set<Tween*> cached_tweens = m_collection;
            for (auto tween : cached_tweens)
            {
                remove(tween);
            }
        }

        void update(const std::time_t& timestamp)
        {
            tween::Tween::updateTweens(timestamp - m_last_update);

            m_last_update = timestamp;
        }

    private:
        std::unordered_set<Tween*> m_collection;
        std::time_t m_last_update;

    };
}