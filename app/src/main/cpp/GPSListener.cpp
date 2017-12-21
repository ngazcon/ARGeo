#include "GPSListener.h"

#include <iomanip>
#include <sstream>
#include <EmbeddedResources.h>

namespace argeo
{
    GPSListener::GPSListener()
    : m_gps_standing_frec(500) // 4000
    , m_gps_moving_frec(100) // 1000
    , m_network_standing_frec(500) // 4000
    , m_network_moving_frec(100) // 1000
    , m_moving_window(1000 * 1) // 1000 * 2
    , m_standing_window(1000 * 0.5) // 1000 * 5
    , m_initial_samples(2) // 10
    , m_moving_samples(3)
    , m_accuracy_delta(10)
    , m_best_moving_locations_index(0)
    , m_initialised(false)
    , m_force_update_location(false)
    , m_only_gps(true)
    , m_state_standing(false)
    { }

    GPSListener::~GPSListener()
    { }

    void GPSListener::on_location_changed(const LocationChangedEventArgs& location)
    {
        bool location_changed = false;

        // Called when a new location is found by the network location provider.
        if (!m_initialised)
        {
            if (m_locations.size() == m_initial_samples)
            {
                LocationChangedEventArgs best_initial = m_locations[0];
                for (int i = 1; i < m_initial_samples; i++)
                {
                    if (best_initial.accuracy > m_locations[i].accuracy)
                    {
                        best_initial = m_locations[i];
                    }
                }

                m_best_location = best_initial;
                m_best_locations.push_back(m_best_location);
                update_moving_locations(m_best_location);
                m_initialised    = true;
                location_changed = true;
            }
            else
            {
                m_locations.push_back(location);
            }
        }
        else
        {
            if (m_force_update_location)
            {
                m_force_update_location = false;
                m_best_location = location;
                m_best_locations.push_back(m_best_location);
                update_moving_locations(m_best_location);
                location_changed = true;
            }
            else
            {
                if (is_better_location(location, m_best_location))
                {
                    m_best_location = location;
                    m_best_locations.push_back(m_best_location);
                    update_moving_locations(m_best_location);
                    location_changed = true;
                }
            }
        }

        if (location_changed)
        {
            notify_listeners(m_best_location);
            //write_to_file();
        }
    }

    void GPSListener::notify_listeners(const LocationChangedEventArgs& location)
    {
        for(LocationListener* listener : m_listeners)
        {
            listener->on_location_changed(location);
        }
    }

    void GPSListener::on_provider_enabled(const std::string& provider)
    { }

    void GPSListener::on_provider_disabled(const std::string& provider)
    { }

    void GPSListener::on_status_changed(const std::string& provider, const int& status)
    { }

    bool GPSListener::add_listener(LocationListener* listener)
    {
        if (m_listeners.find(listener) == m_listeners.end())
        {
            m_listeners.emplace(listener);
            return true;
        }
        return false;

    }

    bool GPSListener::remove_listener(LocationListener* listener)
    {
        return m_listeners.emplace(listener).second;
    }

    void GPSListener::request_updates()
    {
        int window;

        if (m_state_standing)
        {
            window = m_gps_standing_frec;
        }
        else
        {
            window = m_gps_moving_frec;
        }

        LocationManager::get_instance().request_location_updates(
                "gps",
                window,
                0,
                this);

        if (!m_only_gps)
        {
            if (m_state_standing)
            {
                window = m_network_standing_frec;
            }
            else
            {
                window = m_network_moving_frec;
            }

            LocationManager::get_instance().request_location_updates(
                    "network",
                    window,
                    0,
                    this);
        }
    }

    void GPSListener::start()
    {
        request_updates();
    }

    void GPSListener::stop()
    {
        LocationManager::get_instance().remove_updates(this);
    }

    void GPSListener::start_network_provider()
    {
        if (m_only_gps)
        {
            m_only_gps = false;
            stop();
            start();
        }
    }

    void GPSListener::stop_network_provider()
    {
        if (!m_only_gps)
        {
            m_only_gps = true;
            stop();
            start();
        }
    }

    void GPSListener::switch_state()
    {
        stop();
        m_state_standing = !m_state_standing;
        request_updates();
    }

    void GPSListener::next_index()
    {
        m_best_moving_locations_index = (m_best_moving_locations_index + 1) % m_moving_samples;
    }

    void GPSListener::update_moving_locations(const LocationChangedEventArgs& location)
    {
        if (m_best_moving_locations.size() < m_moving_samples)
        {
            m_best_moving_locations.push_back(location);
        }
        else
        {
            m_best_moving_locations[m_best_moving_locations_index] = location;
        }
        next_index();
    }

    GPSState GPSListener::get_state()
    {
        return is_moving() ? GPSState::Moving : GPSState::Standing;
    }

    bool GPSListener::is_moving()
    {
        bool moving = false;
        int size    = m_best_moving_locations.size();

        if (size >= m_moving_samples)
        {
            int   i = 0;
            bool  keep_searching = true;
            while(keep_searching && (i<size))
            {
                keep_searching = m_best_moving_locations.at(i).speed > 0;
                i++;
            }
            moving = keep_searching;
        }

        if ((m_state_standing && moving) || (!m_state_standing && !moving) )
        {
            switch_state();
        }
        return  moving;
    }

    bool GPSListener::is_better_location(
            const LocationChangedEventArgs& location,
            const LocationChangedEventArgs& current_best_location)
    {
        // Check whether the new location fix is newer or older
        bool is_better = false;
        long time_delta = location.time - current_best_location.time;
        bool elapsed_window;

        if (is_moving())
        {
            elapsed_window = time_delta > m_moving_window;

        }
        else
        {
            elapsed_window = time_delta > m_standing_window;
        }

        // Check whether the new location fix is more or less accurate
        int  accuracy_delta = (int) (location.accuracy - current_best_location.accuracy);
        bool is_more_accurate = accuracy_delta < 0;
        bool is_significantly_less_accurate = accuracy_delta > m_accuracy_delta;

        // Determine location quality using a combination of timeliness and accuracy
        if (is_more_accurate)
        {
            is_better = true; // Ignore the time window because it is more accurate
        }
        else if (elapsed_window && !is_significantly_less_accurate && (current_best_location.distance_to(location) > current_best_location.accuracy))
        {
            is_better = true; // need update due to time, but only if it's outside acc boundary (avoid "leaps")
        }
        else if (elapsed_window && is_significantly_less_accurate && (location.distance_to(current_best_location) > location.accuracy) )
        {
            is_better = true; // need update due time and because it is possible the user already moved (distance is too long)
        }

        return is_better;
    }

    void GPSListener::set_network_standing_frec(const int& value)
    {
        m_network_standing_frec = value;
    }

    int  GPSListener::get_network_standing_frec() const
    {
        return m_network_standing_frec;
    }

    void GPSListener::set_network_moving_frec(const int& value)
    {
        m_network_moving_frec = value;
    }

    int  GPSListener::get_network_moving_frec() const
    {
        return m_network_moving_frec;
    }

    void GPSListener::set_gps_moving_frec(const int& value)
    {
        m_gps_moving_frec = value;
    }

    int  GPSListener::get_gps_moving_frec() const
    {
        return m_gps_moving_frec;
    }

    void GPSListener::set_gps_standing_frec(const int& value)
    {
        m_gps_standing_frec = value;
    }

    int  GPSListener::get_gps_standing_frec() const
    {
        return m_gps_standing_frec;
    }

    void GPSListener::set_moving_window(const int& value)
    {
        m_moving_window = value;
    }

    int  GPSListener::get_moving_window() const
    {
        return m_moving_window;
    }

    void GPSListener::set_standing_window(const int& value)
    {
        m_standing_window = value;
    }

    int  GPSListener::get_standing_window() const
    {
        return m_standing_window;
    }

    void GPSListener::set_initial_samples(const int& value)
    {
        m_initial_samples = value;
    }

    int  GPSListener::get_initial_samples() const
    {
        return m_initial_samples;
    }

    void GPSListener::set_accuracy_delta(const int& value)
    {
        m_accuracy_delta = value;
    }

    int  GPSListener::get_accuracy_delta() const
    {
        return m_accuracy_delta;
    }

    void GPSListener::force_location_updates()
    {
        m_force_update_location = true;
    }

    void GPSListener::write_to_file()
    {
        std::stringstream sbuf;
        sbuf << "var markers = \n[ ";
        LocationChangedEventArgs l;
        for (unsigned int i = 0; i < m_locations.size() - 1; i++)
        {
            l = m_locations[i];
            sbuf << "[";
            sbuf << std::setprecision(6) << ArgeoMath::to_degrees(l.location.get_latitude());
            sbuf << ",";
            sbuf << std::setprecision(6) << ArgeoMath::to_degrees(l.location.get_longitude());
            sbuf << ",\'";
            sbuf << std::setprecision(6) << l.accuracy;
            sbuf << ",";
            sbuf << l.provider;
            sbuf << ",";
            sbuf << l.time;
            sbuf << "\'],\n";
        }

        l = m_locations[m_locations.size() - 1];
        sbuf << "[";
        sbuf << std::setprecision(6) << ArgeoMath::to_degrees(l.location.get_latitude());
        sbuf << ",";
        sbuf << std::setprecision(6) << ArgeoMath::to_degrees(l.location.get_longitude());
        sbuf << ",\'";
        sbuf << std::setprecision(6) << l.accuracy;
        sbuf << ",";
        sbuf << l.provider;
        sbuf << ",";
        sbuf << l.time;
        sbuf << "\'] ];\n";

        sbuf << "var best_markers = \n[ ";
        for (unsigned int i = 0; i < m_best_locations.size() - 1; i++)
        {
            l = m_best_locations[i];
            sbuf << "[";
            sbuf << std::setprecision(6) << ArgeoMath::to_degrees(l.location.get_latitude());
            sbuf << ",";
            sbuf << std::setprecision(6) << ArgeoMath::to_degrees(l.location.get_longitude());
            sbuf << ",\'";
            sbuf << std::setprecision(6) << l.accuracy;
            sbuf << ",";
            sbuf << l.provider;
            sbuf << ",";
            sbuf << l.time;
            sbuf << "\'],\n";
        }

        l = m_best_locations[m_best_locations.size() - 1];
        sbuf << "[";
        sbuf << std::setprecision(6) << ArgeoMath::to_degrees(l.location.get_latitude());
        sbuf << ",";
        sbuf << std::setprecision(6) << ArgeoMath::to_degrees(l.location.get_longitude());
        sbuf << ",\'";
        sbuf << std::setprecision(6) << l.accuracy;
        sbuf << ",";
        sbuf << l.provider;
        sbuf << ",";
        sbuf << l.time;
        sbuf << "\'] ];\n";

        std::string s = sbuf.str();
        if(!EmbeddedResources::resource_exists("test/gps.txt"))
        {
            EmbeddedResources::create_resource_path("test/gps.txt");
        }
        FILE* file = EmbeddedResources::resource_open("test/gps.txt", "wb");
        ::fwrite(s.c_str(), sizeof(char), s.length(), file);
        EmbeddedResources::resource_close(file);
    }


}