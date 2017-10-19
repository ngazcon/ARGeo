package vyglab.argeo.jni;

import android.location.Location;
import android.location.LocationListener;
import android.os.Bundle;
import android.hardware.GeomagneticField;

public class ArgeoLocationListener implements LocationListener
{
	private long mNativeObject;

	public ArgeoLocationListener(long nativeObject)
	{
		this.mNativeObject = nativeObject;
	}

	public void onLocationChanged(Location location)
	{
		GeomagneticField geomagnetic_field = new GeomagneticField(
				(float) location.getLatitude(),
				(float) location.getLongitude(),
				(float) location.getAltitude(),
				(long)	location.getTime());


		locationChanged(
				this.mNativeObject,
				location.getLatitude(),
				location.getLongitude(),
				location.getAltitude(),
				location.getAccuracy(),
				location.getSpeed(),
				location.getTime(),
				location.getProvider(),
				geomagnetic_field.getDeclination(),
				location.hasAltitude(),
				location.hasSpeed());
	}

	public void onProviderDisabled(String provider)
	{
		providerDisabled(
				this.mNativeObject,
				provider);
	}


	public void onProviderEnabled(String provider)
	{
		providerEnabled(
				this.mNativeObject,
				provider);
	}

	public void onStatusChanged(String provider, int status, Bundle extras)
	{
		statusChanged(
				this.mNativeObject,
				provider,
				status,
				extras);
	}

	// List of methods that will be linked in the c++ application
	// using JNI's registerNatives
	public static native void locationChanged(
			long   listener,
			double latitude,
			double longitude,
			double altitude,
			double accuracy,
			double speed,
			long   time,
			String provider,
			double geomagnetic_declination,
			boolean hasAltitude,
			boolean hasSpeed);

	public static native void providerDisabled(
			long   listener,
			String provider);

	public static native void providerEnabled(
			long   listener,
			String provider);

	public static native void statusChanged(
			long   listener,
			String provider,
			int    status,
			Bundle bundle);

}
