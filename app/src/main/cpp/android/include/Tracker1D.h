#pragma once

/*
* Tracker1D
*
* Copyright (c) 2014 Renato Villone
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <cmath>

namespace argeo
{
	/**
	* Kalman filter tracking in one dimension.
	*/
	class Tracker1D 
	{

		// Settings

		/**
		* Time step
		*/
	private:
		double mt, mt2, mt2d2, mt3d2, mt4d4;

		/**
		* Process noise covariance
		*/
		double mQa, mQb, mQc, mQd;

		/**
		* Estimated state
		*/
		double mXa, mXb;

		/**
		* Estimated covariance
		*/
		double mPa, mPb, mPc, mPd;

	public:
		/**
		* Creates a tracker.
		*
		* @param timeStep Delta time between predictions. Usefull to calculate speed.
		* @param processNoise Standard deviation to calculate noise covariance from.
		*/
		Tracker1D(
			double time_step, 
			double process_noise) 
		{
			// Lookup time step
			mt = time_step;
			mt2 = mt * mt;
			mt2d2 = mt2 / 2.0;
			mt3d2 = mt2 * mt / 2.0;
			mt4d4 = mt2 * mt2 / 4.0;

			// Process noise covariance
			double n2 = process_noise * process_noise;
			mQa = n2 * mt4d4;
			mQb = n2 * mt3d2;
			mQc = mQb;
			mQd = n2 * mt2;

			// Estimated covariance
			mPa = mQa;
			mPb = mQb;
			mPc = mQc;
			mPd = mQd;
		}

		/**
		* Reset the filter to the given state.
		* <p>
		* Should be called after creation, unless position and velocity are assumed to be both zero.
		*
		* @param position
		* @param velocity
		* @param noise
		*/
		void set_state(
			double position, 
			double velocity, 
			double noise)
		{

			// State vector
			mXa = position;
			mXb = velocity;

			// Covariance
			double n2 = noise * noise;
			mPa = n2 * mt4d4;
			mPb = n2 * mt3d2;
			mPc = mPb;
			mPd = n2 * mt2;
		}

		/**
		* Update (correct) with the given measurement.
		*
		* @param position
		* @param noise
		*/
		void update(
			double position, 
			double noise) 
		{

			double r = noise * noise;

			//  y   =  z   -   H  . x
			double y = position - mXa;

			// S = H.P.H' + R
			double s = mPa + r;
			double si = 1.0 / s;

			// K = P.H'.S^(-1)
			double Ka = mPa * si;
			double Kb = mPc * si;

			// x = x + K.y
			mXa = mXa + Ka * y;
			mXb = mXb + Kb * y;

			// P = P - K.(H.P)
			double Pa = mPa - Ka * mPa;
			double Pb = mPb - Ka * mPb;
			double Pc = mPc - Kb * mPa;
			double Pd = mPd - Kb * mPb;

			mPa = Pa;
			mPb = Pb;
			mPc = Pc;
			mPd = Pd;
		}

		/**
		* Predict state.
		*
		* @param acceleration Should be 0 unless there's some sort of control input (a gas pedal, for instance).
		*/
		void predict(double acceleration) 
		{

			// x = F.x + G.u
			mXa = mXa + mXb * mt + acceleration * mt2d2;
			mXb = mXb + acceleration * mt;

			// P = F.P.F' + Q
			double Pdt = mPd * mt;
			double FPFtb = mPb + Pdt;
			double FPFta = mPa + mt * (mPc + FPFtb);
			double FPFtc = mPc + Pdt;
			double FPFtd = mPd;

			mPa = FPFta + mQa;
			mPb = FPFtb + mQb;
			mPc = FPFtc + mQc;
			mPd = FPFtd + mQd;
		}

		/**
		* @return Estimated position.
		*/
		double get_position() { return mXa; }

		/**
		* @return Estimated velocity.
		*/
		double get_velocity() { return mXb; }

		/**
		* @return Accuracy
		*/
		double get_accuracy() { return std::sqrt(mPd / mt2); }
	};
}
