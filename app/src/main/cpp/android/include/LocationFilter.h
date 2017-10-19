#pragma once

#include "LocationManager.h"

namespace argeo
{
	class LocationFilter
	{
	public:
		LocationFilter()
			: current_best_location(LocationChangedEventArgs())
		{ }


		void set_current_best_location(const LocationChangedEventArgs& value)
		{
			current_best_location = value;
		}
		
		/** Determines whether one Location reading is better than the current Location fix
		* @param location The new Location that you want to evaluate
		* @param currentBestLocation The current Location fix, to which you want to compare the new one
		*/
		bool is_better_location(const LocationChangedEventArgs& location)
		{
			// Check whether the new location fix is newer or older
			std::int64_t time_delta = location.time - current_best_location.time;
			bool is_significantly_newer = time_delta >  TWO_MINUTES;
			bool is_significantly_older = time_delta < -TWO_MINUTES;
			bool is_newer = time_delta > 0;

			// If it's been more than two minutes since the current location, use the new location
			// because the user has likely moved
			if (is_significantly_newer) 
			{
				return true;
				// If the new location is more than two minutes older, it must be worse
			}
			else if (is_significantly_older) 
			{
				return false;
			}

			// Check whether the new location fix is more or less accurate
			int  accuracy_delta = (int)(location.accuracy - current_best_location.accuracy);
			bool is_less_accurate = accuracy_delta > 0;
			bool is_more_accurate = accuracy_delta < 0;
			bool is_significantly_less_accurate = accuracy_delta > 200;

			// Check if the old and new location are from the same provider
			bool is_from_same_provider = is_same_provider(
				location.provider,
				current_best_location.provider);

			// Determine location quality using a combination of timeliness and accuracy
			if (is_more_accurate) 
			{
				return true;
			}
			else if (is_newer && !is_less_accurate)
			{
				return true;
			}
			else if (is_newer && !is_significantly_less_accurate && is_from_same_provider) 
			{
				return true;
			}

			return false;
		}

	private:
		/** Checks whether two providers are the same */
		bool is_same_provider(
			const std::string& provider1,
			const std::string& provider2)
		{
			if (provider1.empty())
			{
				return provider2.empty();
			}

			return provider1.compare(provider2) == 0;
		}

	private:
		const int TWO_MINUTES = 1000 * 60 * 2;
		LocationChangedEventArgs current_best_location;

	};
}