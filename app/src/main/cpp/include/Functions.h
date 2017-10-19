#pragma once

#include <string>

namespace argeo
{
    /**
     * DOC_TBA
     *
     * @name czm_columbusViewMorph
     * @glslFunction
     */
	static const std::string columbus_view_morph =
			R"(
vec4 og_columbusViewMorph(vec4 position2D, vec4 position3D, float time)
{
    // Just linear for now.
    vec3 p = mix(position2D.xyz, position3D.xyz, time);
    return vec4(p, 1.0);
}
)";


    /**
     * Computes the fraction of a Web Wercator rectangle at which a given geodetic latitude is located.
     *
     * @name czm_latitudeToWebMercatorFraction
     * @glslFunction
     *
     * @param {float} latitude The geodetic latitude, in radians.
     * @param {float} southMercatorY The Web Mercator coordinate of the southern boundary of the rectangle.
     * @param {float} oneOverMercatorHeight The total height of the rectangle in Web Mercator coordinates.
     *
     * @returns {float} The fraction of the rectangle at which the latitude occurs.  If the latitude is the southern
     *          boundary of the rectangle, the return value will be zero.  If it is the northern boundary, the return
     *          value will be 1.0.  Latitudes in between are mapped according to the Web Mercator projection.
     */
    static const std::string latitude_to_web_mercator_fraction =
            R"(
float og_latitudeToWebMercatorFraction(float latitude, float southMercatorY, float oneOverMercatorHeight)
{
    float sinLatitude = sin(latitude);
    float mercatorY = 0.5 * log((1.0 + sinLatitude) / (1.0 - sinLatitude));
    return (mercatorY - southMercatorY) * oneOverMercatorHeight;
}
)";

	/**
	* Determines if a time interval is empty.
	*
	* @name czm_isEmpty
	* @glslFunction
	*
	* @param {czm_raySegment} interval The interval to test.
	*
	* @returns {bool} <code>true</code> if the time interval is empty; otherwise, <code>false</code>.
	*
	* @example
	* bool b0 = czm_isEmpty(czm_emptyRaySegment);      // true
	* bool b1 = czm_isEmpty(czm_raySegment(0.0, 1.0)); // false
	* bool b2 = czm_isEmpty(czm_raySegment(1.0, 1.0)); // false, contains 1.0.
	*/
	static const std::string is_empty =
R"(
bool og_isEmpty(og_raySegment interval)
{
    return (interval.stop < 0.0);
}
)";

	static const std::string ray_ellipsoid_intersection_interval =
		R"(
og_raySegment og_rayEllipsoidIntersectionInterval(og_ray ray, og_ellipsoid ellipsoid)
{
	const og_raySegment og_emptyRaySegment = og_raySegment(-og_infinity, -og_infinity);
	const og_raySegment og_fullRaySegment  = og_raySegment(0.0, og_infinity);

    // ray and ellipsoid center in eye coordinates.  radii in model coordinates.
    vec3 q = ellipsoid.inverseRadii * (og_inverseModelViewMatrix * vec4(ray.origin, 1.0)).xyz;
    vec3 w = ellipsoid.inverseRadii * (og_inverseModelViewMatrix * vec4(ray.direction, 0.0)).xyz;
    q      = q - ellipsoid.inverseRadii * (og_inverseModelViewMatrix * vec4(ellipsoid.center, 1.0)).xyz;
    
    float q2 = dot(q, q);
    float qw = dot(q, w);
    
	// Outside ellipsoid.
    if (q2 > 1.0) 
    {
		// Looking outward or tangent (0 intersections).
        if (qw >= 0.0) 
        {
            return og_emptyRaySegment;
        }
		// qw < 0.0.
        else 
        {
            float qw2 = qw * qw;
            float difference = q2 - 1.0; // Positively valued.
            float w2 = dot(w, w);
            float product = w2 * difference;
            
            if (qw2 < product) // Imaginary roots (0 intersections).
            {
                return og_emptyRaySegment;     
            }   
            else if (qw2 > product) // Distinct roots (2 intersections).
            {
                float discriminant = qw * qw - product;
                float temp = -qw + sqrt(discriminant); // Avoid cancellation.
                float root0 = temp / w2;
                float root1 = difference / temp;
                if (root0 < root1)
                {
                    og_raySegment i = og_raySegment(root0, root1);
                    return i;
                }
                else
                {
                    og_raySegment i = og_raySegment(root1, root0);
                    return i;
                }
            }
            else // qw2 == product.  Repeated roots (2 intersections).
            {
                float root = sqrt(difference / w2);
                og_raySegment i = og_raySegment(root, root);
                return i;
            }
        }
    }
    else if (q2 < 1.0) // Inside ellipsoid (2 intersections).
    {
        float difference = q2 - 1.0; // Negatively valued.
        float w2 = dot(w, w);
        float product = w2 * difference; // Negatively valued.
        float discriminant = qw * qw - product;
        float temp = -qw + sqrt(discriminant); // Positively valued.
        og_raySegment i = og_raySegment(0.0, temp / w2);
        return i;
    }
    else // q2 == 1.0. On ellipsoid.
    {
        if (qw < 0.0) // Looking inward.
        {
            float w2 = dot(w, w);
            og_raySegment i = og_raySegment(0.0, -qw / w2);
            return i;
        }
        else // qw >= 0.0.  Looking outward or tangent.
        {
            return og_emptyRaySegment;
        }
    }

}
)";

	/**
	* Returns the WGS84 ellipsoid, with its center at the origin of world coordinates, in eye coordinates.
	*
	* @name og_getWgs84EllipsoidEC
	* @glslFunction
	* @returns {og_ellipsoid} The WGS84 ellipsoid, with its center at the origin of world coordinates, in eye coordinates.
	* @see Ellipsoid.WGS84
	* @example
	* og_ellipsoid ellipsoid = og_getWgs84EllipsoidEC();
	*/
	static const std::string get_wgs84_ellipsoid_ec =
		R"(
og_ellipsoid og_getWgs84EllipsoidEC()
{
    const vec3 radii  = vec3(6378137.0, 6378137.0, 6356752.314245);
    vec3 inverseRadii = vec3(1.0 / radii.x, 1.0 / radii.y, 1.0 / radii.z);
    vec3 inverseRadiiSquared = inverseRadii * inverseRadii;
    og_ellipsoid temp        = og_ellipsoid(og_viewMatrix[3].xyz, radii, inverseRadii, inverseRadiiSquared);

    return temp;
}
)";

	/**
	* Procedural anti-aliasing by blurring two colors that meet at a sharp edge.
	*
	* @name og_antialias
	* @glslFunction
	*
	* @param {vec4} color1 The color on one side of the edge.
	* @param {vec4} color2 The color on the other side of the edge.
	* @param {vec4} currentcolor The current color, either <code>color1</code> or <code>color2</code>.
	* @param {float} dist The distance to the edge in texture coordinates.
	* @param {float} [fuzzFactor=0.1] Controls the blurriness between the two colors.
	* @returns {vec4} The anti-aliased color.
	*
	* @example
	* // GLSL declarations
	* vec4 og_antialias(vec4 color1, vec4 color2, vec4 currentColor, float dist, float fuzzFactor);
	* vec4 og_antialias(vec4 color1, vec4 color2, vec4 currentColor, float dist);
	*
	* // get the color for a material that has a sharp edge at the line y = 0.5 in texture space
	* float dist = abs(textureCoordinates.t - 0.5);
	* vec4 currentColor = mix(bottomColor, topColor, step(0.5, textureCoordinates.t));
	* vec4 color = og_antialias(bottomColor, topColor, currentColor, dist, 0.1);
	*/
	static const std::string antialias =
R"(
vec4 og_antialias(vec4 color1, vec4 color2, vec4 currentColor, float dist, float fuzzFactor)
{
    float val1 = clamp(dist / fuzzFactor, 0.0, 1.0);
    float val2 = clamp((dist - 0.5) / fuzzFactor, 0.0, 1.0);
    val1 = val1 * (1.0 - val2);
    val1 = val1 * val1 * (3.0 - (2.0 * val1));
    val1 = pow(val1, 0.5); //makes the transition nicer
    
    vec4 midColor = (color1 + color2) * 0.5;
    return mix(midColor, currentColor, val1);
}

vec4 og_antialias(vec4 color1, vec4 color2, vec4 currentColor, float dist)
{
    return og_antialias(color1, color2, currentColor, dist, 0.1);
}

)";

	/**
	* Compares <code>left</code> and <code>right</code> componentwise. Returns <code>true</code>
	* if they are within <code>epsilon</code> and <code>false</code> otherwise. The inputs
	* <code>left</code> and <code>right</code> can be <code>float</code>s, <code>vec2</code>s,
	* <code>vec3</code>s, or <code>vec4</code>s.
	*
	* @name og_equalsEpsilon
	* @glslFunction
	*
	* @param {} left The first vector.
	* @param {} right The second vector.
	* @param {float} epsilon The epsilon to use for equality testing.
	* @returns {bool} <code>true</code> if the components are within <code>epsilon</code> and <code>false</code> otherwise.
	*
	* @example
	* // GLSL declarations
	* bool og_equalsEpsilon(float left, float right, float epsilon);
	* bool og_equalsEpsilon(vec2 left, vec2 right, float epsilon);
	* bool og_equalsEpsilon(vec3 left, vec3 right, float epsilon);
	* bool og_equalsEpsilon(vec4 left, vec4 right, float epsilon);
	*/
	static const std::string equals_epsilon =
R"(
bool og_equalsEpsilon(vec4 left, vec4 right, float epsilon) {
	return all(lessThanEqual(abs(left - right), vec4(epsilon)));
}

bool og_equalsEpsilon(vec3 left, vec3 right, float epsilon) {
	return all(lessThanEqual(abs(left - right), vec3(epsilon)));
}

bool og_equalsEpsilon(vec2 left, vec2 right, float epsilon) {
	return all(lessThanEqual(abs(left - right), vec2(epsilon)));
}

bool og_equalsEpsilon(float left, float right, float epsilon) {
	return (abs(left - right) <= epsilon);
}
)";


	/**
	* Transforms a position from eye to window coordinates.  The transformation
	* from eye to clip coordinates is done using {@link og_projectionMatrix}.
	* The transform from normalized device coordinates to window coordinates is
	* done using {@link og_viewportTransformationMatrix}, which assumes a depth range
	* of <code>near = 0</code> and <code>far = 1</code>.
	* <br /><br />
	* This transform is useful when there is a need to manipulate window coordinates
	* in a vertex shader as done by {@link BillboardCollection}.
	*
	* @name og_eyeToWindowCoordinates
	* @glslFunction
	*
	* @param {vec4} position The position in eye coordinates to transform.
	*
	* @returns {vec4} The transformed position in window coordinates.
	*
	* @see og_modelToWindowCoordinates
	* @see og_projectionMatrix
	* @see og_viewportTransformationMatrix
	* @see BillboardCollection
	*
	* @example
	* vec4 positionWC = og_eyeToWindowCoordinates(positionEC);
	*/

	static const std::string eye_to_window_coordinates =
R"(
vec4 og_eyeToWindowCoordinates(vec4 positionEC)
{
	vec4 q = og_projectionMatrix * positionEC;                 // clip coordinates
	q.xyz /= q.w;                                               // normalized device coordinates
	q.xyz = (og_viewportTransformationMatrix * vec4(q.xyz, 1.0)).xyz; // window coordinates
	return q;
}
)";


	/**
	* Transforms a position from model to window coordinates.  The transformation
	* from model to clip coordinates is done using {@link og_modelViewProjectionMatrix}.
	* The transform from normalized device coordinates to window coordinates is
	* done using {@link og_viewportTransformationMatrix}, which assumes a depth range
	* of <code>near = 0</code> and <code>far = 1</code>.
	* <br /><br />
	* This transform is useful when there is a need to manipulate window coordinates
	* in a vertex shader as done by {@link BillboardCollection}.
	* <br /><br />
	* This function should not be confused with {@link og_viewportOrthographic},
	* which is an orthographic projection matrix that transforms from window
	* coordinates to clip coordinates.
	*
	* @name og_modelToWindowCoordinates
	* @glslFunction
	*
	* @param {vec4} position The position in model coordinates to transform.
	*
	* @returns {vec4} The transformed position in window coordinates.
	*
	* @see og_eyeToWindowCoordinates
	* @see og_modelViewProjectionMatrix
	* @see og_viewportTransformationMatrix
	* @see og_viewportOrthographic
	* @see BillboardCollection
	*
	* @example
	* vec4 positionWC = og_modelToWindowCoordinates(positionMC);
	*/
	static const std::string model_to_window_coordinates =
R"(
vec4 og_modelToWindowCoordinates(vec4 position)
{
	vec4 q = og_modelViewProjectionMatrix * position;                // clip coordinates
	q.xyz /= q.w;                                                // normalized device coordinates
	q.xyz  = (og_viewportTransformationMatrix * vec4(q.xyz, 1.0)).xyz; // window coordinates
	return q;
}
)";

	static const std::string pack_float_to_vec4 =
R"(
vec4 og_packFloatToVec4(const in float value)
{
	vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * value;
    enc  = fract(enc);
    enc -= enc.yzww * vec4(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 0.0);
    return enc;
}
)";

	static const std::string unpack_float_from_vec4 =
R"(
float og_unpackFloatFromVec4(const in vec4 value)
{
	return dot(value, vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 160581375.0));
}
)";

	/**
	* Translates a position (or any <code>vec3</code>) that was encoded with {@link EncodedCartesian3},
	* and then provided to the shader as separate <code>high</code> and <code>low</code> bits to
	* be relative to the eye.  As shown in the example, the position can then be transformed in eye
	* or clip coordinates using {@link og_modelViewRelativeToEyeMatrix} or {@link og_modelViewProjectionRelativeToEye},
	* respectively.
	* <p>
	* This technique, called GPU RTE, eliminates jittering artifacts when using large coordinates as
	* described in {@link http://blogs.agi.com/insight3d/index.php/2008/09/03/precisions-precisions/|Precisions, Precisions}.
	* </p>
	*
	* @name og_translateRelativeToEye
	* @glslFunction
	*
	* @param {vec3} high The position's high bits.
	* @param {vec3} low The position's low bits.
	* @returns {vec3} The position translated to be relative to the camera's position.
	*
	* @example
	* attribute vec3 positionHigh;
	* attribute vec3 positionLow;
	*
	* void main()
	* {
	*   vec4 p = og_translateRelativeToEye(positionHigh, positionLow);
	*   gl_Position = og_modelViewProjectionRelativeToEye * p;
	* }
	*
	* @see og_modelViewRelativeToEyeMatrix
	* @see og_modelViewProjectionRelativeToEye
	* @see og_computePosition
	* @see EncodedCartesian3
	*/
	static const std::string translate_relative_to_eye =
R"(
vec4 og_translateRelativeToEye(vec3 high, vec3 low)
{
	vec3 highDifference = high - og_cameraEyeHighMC;
	vec3 lowDifference  = low  - og_cameraEyeLowMC;

	return vec4(highDifference + lowDifference, 1.0);
}
)";

	static const std::string denormalize_float =
R"(
float og_denormalizeFloat(const in float value, const in float max, const in float min)
{
	return value * (max - min) + min;
}
)";

	static const std::string normalize_float =
R"(
float og_normalizeFloat(const in float value, const in float max, const in float min)
{
	return (value - min) / (max - min);
}
)";

	static const std::string compute_position =
R"(
vec4 og_computePosition(vec3 high, vec3 low)
{
	return vec4(high + low, 1.0);
}
)";

	/**
	* Transforms a position from window to eye coordinates.
	* The transform from window to normalized device coordinates is done using components
	* of (@link og_viewport} and {@link og_viewportTransformationMatrix} instead of calculating
	* the inverse of <code>og_viewportTransformationMatrix</code>. The transformation from
	* normalized device coordinates to clip coordinates is done using <code>positionWC.w</code>,
	* which is expected to be the scalar used in the perspective divide. The transformation
	* from clip to eye coordinates is done using {@link og_inverseProjectionMatrix}.
	*
	* @name og_windowToEyeCoordinates
	* @glslFunction
	*
	* @param {vec4} fragmentCoordinate The position in window coordinates to transform.
	*
	* @returns {vec4} The transformed position in eye coordinates.
	*
	* @see og_modelToWindowCoordinates
	* @see og_eyeToWindowCoordinates
	* @see og_inverseProjectionMatrix
	* @see og_viewport
	* @see og_viewportTransformationMatrix
	*
	* @example
	* vec4 positionEC = og_windowToEyeCoordinates(gl_FragCoord);
	*/
	static const std::string window_to_eye_coordinates =
R"(
vec4 og_windowToEyeCoordinates(vec4 fragmentCoordinate)
{
	float x = 2.0 * (fragmentCoordinate.x - og_viewport.x) / og_viewport.z - 1.0;
	float y = 2.0 * (fragmentCoordinate.y - og_viewport.y) / og_viewport.w - 1.0;
	float z = (fragmentCoordinate.z - og_viewportTransformationMatrix[3][2]) / og_viewportTransformationMatrix[2][2];
	vec4 q = vec4(x, y, z, 1.0);
	q /= fragmentCoordinate.w;
	q = og_inverseProjectionMatrix * q;
	return q;
}
)";


	static const std::string polyline_common =
R"(

void clipLineSegmentToNearPlane(
	vec3 p0,
	vec3 p1,
	out vec4 positionWC,
	out bool clipped,
	out bool culledByNearPlane)
{
	culledByNearPlane = false;
	clipped			  = false;
    
	vec3 p1ToP0	    = p1 - p0;
	float magnitude = length(p1ToP0);
	vec3 direction  = normalize(p1ToP0);

	float endPoint0Distance = -(og_currentFrustum.x + p0.z);
	float denominator	    = -direction.z;
    
	if (endPoint0Distance < 0.0 && abs(denominator) < og_epsilon7)
	{
		culledByNearPlane = true;
	}
	else if (endPoint0Distance < 0.0 && abs(denominator) > og_epsilon7)
	{
		float t = (og_currentFrustum.x + p0.z) / denominator;
		if (t < 0.0 || t > magnitude)
		{
			culledByNearPlane = true;
		}
		else
		{
			p0 = p0 + t * direction;
			clipped = true;
		}
	}
    
	positionWC = og_eyeToWindowCoordinates(vec4(p0, 1.0));
}

vec4 getPolylineWindowCoordinates(
	vec4 position, 
	vec4 previous, 
	vec4 next, 
	float expandDirection, 
	float width,
	bool usePrevious) 
{
	vec4 endPointWC, p0, p1;
	bool culledByNearPlane, clipped;
    
	vec4 positionEC = og_modelViewRelativeToEyeMatrix * position;
	vec4 prevEC     = og_modelViewRelativeToEyeMatrix * previous;
	vec4 nextEC     = og_modelViewRelativeToEyeMatrix * next;
    
	clipLineSegmentToNearPlane(prevEC.xyz, positionEC.xyz, p0, clipped, culledByNearPlane);
	clipLineSegmentToNearPlane(nextEC.xyz, positionEC.xyz, p1, clipped, culledByNearPlane);

	clipLineSegmentToNearPlane(
			positionEC.xyz,
			usePrevious ? prevEC.xyz : nextEC.xyz, 
			endPointWC,
			clipped,
			culledByNearPlane);
    
	if (culledByNearPlane)
	{
		return vec4(0.0, 0.0, 0.0, 1.0);
	}
    
	vec2 prevWC = normalize(p0.xy - endPointWC.xy);
	vec2 nextWC = normalize(p1.xy - endPointWC.xy);
    
	float expandWidth = width * 0.5;
	vec2 direction;

	if (og_equalsEpsilon(previous.xyz, position.xyz, og_epsilon1) ||
	    og_equalsEpsilon(prevWC, -nextWC, og_epsilon1))
	{
		direction = vec2(-nextWC.y, nextWC.x);
	}
	else if (og_equalsEpsilon(next.xyz, position.xyz, og_epsilon1) ||
			 clipped)
	{
		direction = vec2(prevWC.y, -prevWC.x);
	}
	else
	{
		vec2 normal = vec2(-nextWC.y, nextWC.x);
		direction   = normalize((nextWC + prevWC) * 0.5);
		if (dot(direction, normal) < 0.0)
		{
			direction = -direction;
		}

		// The sine of the angle between the two vectors is given by the formula
		//         |a x b| = |a||b|sin(theta)
		// which is
		//     float sinAngle = length(cross(vec3(direction, 0.0), vec3(nextWC, 0.0)));
		// Because the z components of both vectors are zero, the x and y coordinate will be zero.
		// Therefore, the sine of the angle is just the z component of the cross product.

		float sinAngle = abs(direction.x * nextWC.y - direction.y * nextWC.x);
		expandWidth    = clamp(expandWidth / sinAngle, 0.0, width * 2.0);
	}

	vec2 offset = direction * expandDirection * expandWidth * og_highResolutionSnapScale;

	return vec4(
		 endPointWC.xy + offset,
		-endPointWC.z,
		 1.0);
}

)";


		/**
		* An og_material with default values. Every material's og_getMaterial
		* should use this default material as a base for the material it returns.
		* The default normal value is given by materialInput.normalEC.
		*
		* @name og_getDefaultMaterial
		* @glslFunction
		*
		* @param {og_materialInput} input The input used to construct the default material.
		*
		* @returns {og_material} The default material.
		*
		* @see og_materialInput
		* @see og_material
		* @see og_getMaterial
		*/
		static const std::string get_default_material =
R"(

og_material og_getDefaultMaterial(og_materialInput materialInput)
{
	og_material material;
	material.diffuse = vec3(0.0);
	material.specular  = vec3(0.0);
	material.shininess = 1.0;
	material.normal = materialInput.normalEC;
	material.emission = vec3(0.0);
	material.alpha = 1.0;
	return material;
}

)";

		/**
		* Creates a matrix that transforms vectors from tangent space to eye space.
		*
		* @name og_tangentToEyeSpaceMatrix
		* @glslFunction
		*
		* @param {vec3} normalEC The normal vector in eye coordinates.
		* @param {vec3} tangentEC The tangent vector in eye coordinates.
		* @param {vec3} binormalEC The binormal vector in eye coordinates.
		*
		* @returns {mat3} The matrix that transforms from tangent space to eye space.
		*
		* @example
		* mat3 tangentToEye = og_tangentToEyeSpaceMatrix(normalEC, tangentEC, binormalEC);
		* vec3 normal = tangentToEye * texture2D(normalMap, st).xyz;
		*/
		static const std::string tangent_to_eye_space_matrix =
R"(

mat3 og_tangentToEyeSpaceMatrix(vec3 normalEC, vec3 tangentEC, vec3 binormalEC)
{
    vec3 normal   = normalize(normalEC);
    vec3 tangent  = normalize(tangentEC);
    vec3 binormal = normalize(binormalEC);
    return mat3(tangent.x,  tangent.y,  tangent.z,
                binormal.x, binormal.y, binormal.z,
                normal.x,   normal.y,   normal.z);
}

)";


		/**
		* Calculates the intensity of diffusely reflected light.
		*
		* @name og_getLambertDiffuse
		* @glslFunction
		*
		* @param {vec3} lightDirectionEC Unit vector pointing to the light source in eye coordinates.
		* @param {vec3} normalEC The surface normal in eye coordinates.
		*
		* @returns {float} The intensity of the diffuse reflection.
		*
		* @see og_phong
		*
		* @example
		* float diffuseIntensity = og_getLambertDiffuse(lightDirectionEC, normalEC);
		* float specularIntensity = og_getSpecular(lightDirectionEC, toEyeEC, normalEC, 200);
		* vec3 color = (diffuseColor * diffuseIntensity) + (specularColor * specularIntensity);
		*/
		static const std::string get_lambert_diffuse =
R"(
float og_getLambertDiffuse(vec3 lightDirectionEC, vec3 normalEC)
{
    return max(dot(lightDirectionEC, normalEC), 0.0);
}
)";

		/**
		* Calculates the specular intensity of reflected light.
		*
		* @name og_getSpecular
		* @glslFunction
		*
		* @param {vec3} lightDirectionEC Unit vector pointing to the light source in eye coordinates.
		* @param {vec3} toEyeEC Unit vector pointing to the eye position in eye coordinates.
		* @param {vec3} normalEC The surface normal in eye coordinates.
		* @param {float} shininess The sharpness of the specular reflection.  Higher values create a smaller, more focused specular highlight.
		*
		* @returns {float} The intensity of the specular highlight.
		*
		* @see og_phong
		*
		* @example
		* float diffuseIntensity = og_getLambertDiffuse(lightDirectionEC, normalEC);
		* float specularIntensity = og_getSpecular(lightDirectionEC, toEyeEC, normalEC, 200);
		* vec3 color = (diffuseColor * diffuseIntensity) + (specularColor * specularIntensity);
		*/
		static const std::string get_specular =
R"(
float og_getSpecular(vec3 lightDirectionEC, vec3 toEyeEC, vec3 normalEC, float shininess)
{
    vec3 toReflectedLight = reflect(-lightDirectionEC, normalEC);
    float specular = max(dot(toReflectedLight, toEyeEC), 0.0);
    return pow(specular, shininess);
}
)";

		/**
		* Computes a color using the Phong lighting model.
		*
		* @name og_phong
		* @glslFunction
		*
		* @param {vec3} toEye A normalized vector from the fragment to the eye in eye coordinates.
		* @param {og_material} material The fragment's material.
		*
		* @returns {vec4} The computed color.
		*
		* @example
		* vec3 positionToEyeEC = // ...
		* og_material material = // ...
		* gl_FragColor = og_phong(normalize(positionToEyeEC), material);
		*
		* @see og_getMaterial
		*/
		static const std::string phong =
			R"(

float og_private_getLambertDiffuseOfMaterial(vec3 lightDirectionEC, og_material material)
{
    return og_getLambertDiffuse(lightDirectionEC, material.normal);
}

float og_private_getSpecularOfMaterial(vec3 lightDirectionEC, vec3 toEyeEC, og_material material)
{
    return og_getSpecular(lightDirectionEC, toEyeEC, material.normal, material.shininess);
}

vec4 og_phong(vec3 toEye, vec3 sunPosition, og_material material)
{
    // Diffuse from directional light sources at eye (for top-down)
    float diffuse = og_private_getLambertDiffuseOfMaterial(sunPosition, material);

    // Specular from sun and pseudo-moon
    float specular = og_private_getSpecularOfMaterial(sunPosition, toEye, material);

	vec3 materialDiffuse = material.diffuse * 0.5;
	vec3 ambient = materialDiffuse;

    vec3 color   = ambient + material.emission;
    color += materialDiffuse   * diffuse;


#ifdef HAS_SPECULAR
    color += material.specular * specular;
#endif

    return vec4(color, material.alpha);
}

)";

}

