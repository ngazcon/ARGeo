#pragma once

#include "LocationManager.h"

#include <string>
#include <vector>
#include <unordered_set>

namespace argeo
{
    enum class GPSState : unsigned char
    {
        Standing = 0,
        Moving   = 1
    };

    class GPSListener : public LocationListener
    {

    public:
        GPSListener();
        ~GPSListener();

        bool add_listener(LocationListener* listener);
        bool remove_listener(LocationListener* listener);

        void on_location_changed(const LocationChangedEventArgs& location);
        void on_provider_enabled(const std::string& provider);
        void on_provider_disabled(const std::string& provider);
        void on_status_changed(const std::string& provider, const int& status);

        void start();
        void stop();
        void start_network_provider();
        void stop_network_provider();

        GPSState get_state();

        void set_network_standing_frec(const int& value);
        int  get_network_standing_frec() const;

        void set_network_moving_frec(const int& value);
        int  get_network_moving_frec() const;

        void set_gps_moving_frec(const int& value);
        int  get_gps_moving_frec() const;

        void set_gps_standing_frec(const int& value);
        int  get_gps_standing_frec() const;

        void set_moving_window(const int& value);
        int  get_moving_window() const;

        void set_standing_window(const int& value);
        int  get_standing_window() const;

        void set_initial_samples(const int& value);
        int  get_initial_samples() const;

        void set_accuracy_delta(const int& value);
        int  get_accuracy_delta() const;

        void force_location_updates();


        double get_accuracy() const;
        double get_speed() const;
        std::string get_provider() const;

    private:
        void notify_listeners(const LocationChangedEventArgs& location);

        bool is_better_location(
                const LocationChangedEventArgs& location,
                const LocationChangedEventArgs& current_best_location);

        bool is_moving();
        void switch_state();
        void next_index();
        void update_moving_locations(const LocationChangedEventArgs& location);

        void request_updates();

        void write_to_file();

    private:
        int m_gps_standing_frec;
        int m_gps_moving_frec;
        int m_network_standing_frec ;
        int m_network_moving_frec;
        int m_moving_window; // expressed in milliseconds
        int m_standing_window; // expressed in milliseconds
        int m_initial_samples;
        int m_moving_samples;
        int m_accuracy_delta;

        int m_best_moving_locations_index;

        bool m_force_update_location;
        bool m_initialised;
        bool m_only_gps;
        bool m_state_standing; // By default is moving, so the initialization will use the short time window

        LocationChangedEventArgs m_best_location;

        std::unordered_set<LocationListener*> m_listeners;

        std::vector<LocationChangedEventArgs> m_best_moving_locations;
        std::vector<LocationChangedEventArgs> m_locations;
        std::vector<LocationChangedEventArgs> m_best_locations;
    };
}


