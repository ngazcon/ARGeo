#pragma once

namespace argeo
{
	static const char globe_clipmap_vs[] =
		R"(

precision highp float;

in vec2 position;

out vec3 fsPositionEC;
out vec3 fsPositionToEye;

out vec2 fsImageryTextureCoordinates;
out vec2 fsHeightFineTextureCoordinates;
out vec2 fsHeightCoarseTextureCoordinates;

out float fsAlpha;
out float fsHeight;

uniform sampler2D og_texture0;
uniform sampler2D og_texture1;

uniform vec2 u_patchOriginInClippedLevel;
uniform vec2 u_levelScaleFactor;
uniform vec2 u_levelZeroWorldScaleFactor;
uniform vec2 u_levelOffsetFromWorldOrigin;

uniform float u_heightExaggeration;
uniform float u_oneOverClipmapSize;

uniform bool u_useBlendRegions;
uniform vec2 u_fineLevelOriginInCoarse;
uniform vec2 u_viewPosInClippedLevel;
uniform vec2 u_unblendedRegionSize;
uniform vec2 u_oneOverBlendedRegionSize;
uniform vec2 u_fineTextureOrigin;
uniform vec2 u_oneOverBlockSize;

uniform vec3 u_globeRadiiSquared;

uniform vec2 u_terrainToImageryResolutionRatio;
uniform vec2 u_terrainOffsetInImagery;
uniform vec2 u_oneOverImagerySize;

uniform vec4 u_tileRectangle;

#ifdef ENABLE_DOUBLE_PRECISION
uniform vec3 u_center3DHigh;
uniform vec3 u_center3DLow;
#else
uniform vec3 u_center3D;
#endif

uniform vec2 u_maxMinHeights;
uniform vec2 u_southAndNorthLatitude;
uniform vec2 u_southMercatorYAndOneOverHeight;

uniform mat4 u_modifiedModelViewMatrix;
uniform mat4 u_modifiedModelViewProjectionMatrix;


vec4  getPosition(vec3 position, float height, vec2 textureCoordinates);
float get2DYPositionFraction(vec2 textureCoordinates);

// http://andrewthall.org/papers/df64_qf128.pdf
float times_frc(float a, float b)
{
  return mix(0.0, a * b, b != 0.0 ? 1.0 : 0.0);
}

float plus_frc(float a, float b)
{
  return mix(a, a + b, b != 0.0 ? 1.0 : 0.0);
}

float minus_frc(float a, float b)
{
  return mix(a, a - b, b != 0.0 ? 1.0 : 0.0);
}

// Double emulation based on GLSL Mandelbrot Shader by Henry Thasler (www.thasler.org/blog)
//
// Emulation based on Fortran-90 double-single package. See http://crd.lbl.gov/~dhbailey/mpdist/
// Substract: res = ds_add(a, b) => res = a + b
vec2 add(vec2 dsa, vec2 dsb)
{
  vec2 dsc;
  float t1, t2, e;
  t1 = plus_frc(dsa.x, dsb.x);
  e  = minus_frc(t1, dsa.x);
  t2 = plus_frc(plus_frc(plus_frc(minus_frc(dsb.x, e), minus_frc(dsa.x, minus_frc(t1, e))), dsa.y), dsb.y);
  dsc.x = plus_frc(t1, t2);
  dsc.y = minus_frc(t2, minus_frc(dsc.x, t1));
  return dsc;
}

// Substract: res = ds_sub(a, b) => res = a - b
vec2 sub(vec2 dsa, vec2 dsb)
{
  vec2 dsc;
  float e, t1, t2;
  t1 = minus_frc(dsa.x, dsb.x);
  e  = minus_frc(t1, dsa.x);
  t2 = minus_frc(plus_frc(plus_frc(minus_frc(minus_frc(0.0, dsb.x), e), minus_frc(dsa.x, minus_frc(t1, e))), dsa.y), dsb.y);
  dsc.x = plus_frc(t1, t2);
  dsc.y = minus_frc(t2, minus_frc(dsc.x, t1));
  return dsc;
}


// Compare: res = -1 if a < b
//              = 0 if a == b
//              = 1 if a > b
float cmp(vec2 dsa, vec2 dsb)
{
  if (dsa.x < dsb.x)
  {
    return -1.0;
  }

  if (dsa.x > dsb.x)
  {
    return 1.0;
  }

  if (dsa.y < dsb.y)
  {
    return -1.0;
  }

  if (dsa.y > dsb.y)
  {
    return 1.0;
  }

  return 0.0;
}



// Multiply: res = ds_mul(a, b) => res = a * b
vec2 mul(vec2 dsa, vec2 dsb)
{
  vec2 dsc;
  float c11, c21, c2, e, t1, t2;
  float a1, a2, b1, b2, cona, conb, split = 8193.;

  cona = times_frc(dsa.x, split);
  conb = times_frc(dsb.x, split);
  a1 = minus_frc(cona, minus_frc(cona, dsa.x));
  b1 = minus_frc(conb, minus_frc(conb, dsb.x));
  a2 = minus_frc(dsa.x, a1);
  b2 = minus_frc(dsb.x, b1);
  c11 = times_frc(dsa.x, dsb.x);
  c21 = plus_frc(times_frc(a2, b2), plus_frc(times_frc(a2, b1), plus_frc(times_frc(a1, b2), minus_frc(times_frc(a1, b1), c11))));
  c2 = plus_frc(times_frc(dsa.x, dsb.y), times_frc(dsa.y, dsb.x));
  t1 = plus_frc(c11, c2);
  e = minus_frc(t1, c11);
  t2 = plus_frc(plus_frc(times_frc(dsa.y, dsb.y), plus_frc(minus_frc(c2, e), minus_frc(c11, minus_frc(t1, e)))), c21);
  dsc.x = plus_frc(t1, t2);
  dsc.y = minus_frc(t2, minus_frc(dsc.x, t1));
  return dsc;
}

// create double-single number from float
vec2 set(float a)
{
  return vec2(a, 0.0);
}

vec2 div(vec2 b, vec2 a)
{
    vec2 xn  = set(1.0 / a.x);
    vec2 yn  = mul(b, xn);
    vec2 ayn = mul(a, yn);

    vec2 diff = sub(b, ayn);
    vec2 prod = mul(xn , diff);

    return add(yn , prod);
}

vec2 ds_sqrt(vec2 a)
{
    vec2 xn    = set(sqrt(a.x));
    vec2 yn    = mul(a, xn);
    vec2 ynsqr = mul(yn, yn);

    vec2 diff =  sub(a, ynsqr);
    vec2 prod  = mul(xn , diff ) / vec2(2.0);

    return add(yn, prod);
}


float get2DMercatorYPositionFraction(vec2 textureCoordinates)
{
    // The width of a tile at level 11, in radians and assuming a single root tile, is
    //   2.0 * czm_pi / pow(2.0, 11.0)
    // We want to just linearly interpolate the 2D position from the texture coordinates
    // when we're at this level or higher.  The constant below is the expression
    // above evaluated and then rounded up at the 4th significant digit.

    const float maxTileWidth = 0.003068;
    float positionFraction = textureCoordinates.y;
    float southLatitude = u_southAndNorthLatitude.x;
    float northLatitude = u_southAndNorthLatitude.y;
    if (northLatitude - southLatitude > maxTileWidth)
    {
        float southMercatorY = u_southMercatorYAndOneOverHeight.x;
        float oneOverMercatorHeight = u_southMercatorYAndOneOverHeight.y;

        float currentLatitude = mix(southLatitude, northLatitude, textureCoordinates.y);
        currentLatitude  = clamp(currentLatitude, -og_webMercatorMaxLatitude, og_webMercatorMaxLatitude);
        positionFraction = og_latitudeToWebMercatorFraction(currentLatitude, southMercatorY, oneOverMercatorHeight);
    }

    return positionFraction;

}

float get2DGeographicYPositionFraction(vec2 textureCoordinates)
{
    return textureCoordinates.y;
}

vec4 getPosition3DMode(vec3 position, float height, vec2 textureCoordinates)
{
    return u_modifiedModelViewProjectionMatrix * vec4(position, 1.0);
}

vec4 getPositionPlanarEarth(vec3 position, float height, vec2 textureCoordinates)
{
    float yPositionFraction = get2DYPositionFraction(textureCoordinates);

    vec4  rtcPosition2D     = vec4(
            height,
            mix(u_tileRectangle.st, u_tileRectangle.pq, vec2(textureCoordinates.x, yPositionFraction)),
            1.0);

    return u_modifiedModelViewProjectionMatrix * rtcPosition2D;
}

vec4 getPosition2DMode(vec3 position, float height, vec2 textureCoordinates)
{
    return getPositionPlanarEarth(position, 0.0, textureCoordinates);
}

vec4 getPositionColumbusViewMode(vec3 position, float height, vec2 textureCoordinates)
{
    return getPositionPlanarEarth(position, height, textureCoordinates);
}

vec4 getPositionMorphingMode(vec3 position, float height, vec2 textureCoordinates)
{
    // We do not do RTC while morphing, so there is potential for jitter.
    // This is unlikely to be noticeable, though.

#ifdef ENABLE_DOUBLE_PRECISION
    vec3 position3DWC       = position + u_center3DHigh;
#else
    vec3 position3DWC       = position + u_center3D;
#endif

    float yPositionFraction = get2DYPositionFraction(textureCoordinates);

    vec4 position2DWC  = vec4(
            height,
            mix(u_tileRectangle.st, u_tileRectangle.pq, vec2(textureCoordinates.x, yPositionFraction)),
            1.0);

    vec4 morphPosition = og_columbusViewMorph(position2DWC, vec4(position3DWC, 1.0), og_morphTime);

    return og_modelViewProjectionMatrix * morphPosition;
}

vec3 GeodeticSurfaceNormal(vec3 geodetic)
{
	float cosLatitude = cos(geodetic.y);

	return vec3(
		cosLatitude * cos(geodetic.x),	 
		cosLatitude * sin(geodetic.x),	 
		sin(geodetic.y));				 
}										 

vec3 GeodeticToCartesian(vec3 globeRadiiSquared, vec3 geodetic)
{
    vec3 n = GeodeticSurfaceNormal(geodetic);
    vec3 k = globeRadiiSquared * n;
    float gamma = sqrt(dot(n, k));

    vec3 rSurface = k / gamma;
	return rSurface + (geodetic.z * n);
}


void GeodeticToCartesianDoublePrecision(vec3 globeRadiiSquared, vec3 geodetic, out vec3 cartHigh, out vec3 cartLow)
{
    vec3 n = GeodeticSurfaceNormal(geodetic);

    vec2 nX = set(n.x);
    vec2 nY = set(n.y);
    vec2 nZ = set(n.z);

    vec2 globeRadiiSquaredX = set(globeRadiiSquared.x);
    vec2 globeRadiiSquaredY = set(globeRadiiSquared.y);
    vec2 globeRadiiSquaredZ = set(globeRadiiSquared.z);

    vec2 kX = mul(globeRadiiSquaredX, nX);
    vec2 kY = mul(globeRadiiSquaredY, nY);
    vec2 kZ = mul(globeRadiiSquaredZ, nZ);

    vec2 nDotK = add(mul(nX, kX), mul(nY, kY));
    nDotK = add(nDotK, mul(nZ, kZ));

	vec2 gamma     = set(sqrt(nDotK.x));
    vec2 rSurfaceX = div(kX, gamma);
    vec2 rSurfaceY = div(kY, gamma);
    vec2 rSurfaceZ = div(kZ, gamma);

    vec2 zNX    = set(geodetic.z * n.x);
    vec2 zNY    = set(geodetic.z * n.y);
    vec2 zNZ    = set(geodetic.z * n.z);

    vec2 cartX = add(rSurfaceX, zNX);
    vec2 cartY = add(rSurfaceY, zNY);
    vec2 cartZ = add(rSurfaceZ, zNZ);

    cartHigh = vec3(
        cartX.x,
        cartY.x,
        cartZ.x);

    cartLow = vec3(
        cartX.y,
        cartY.y,
        cartZ.y);
}

float SampleHeight(vec2 levelPos)																								 
{																																 
	fsHeightFineTextureCoordinates   = (levelPos + u_fineTextureOrigin)             * u_oneOverClipmapSize;
	fsHeightCoarseTextureCoordinates = (levelPos * 0.5 + u_fineLevelOriginInCoarse) * u_oneOverClipmapSize;

	if (u_useBlendRegions)
	{
		vec2 alpha = clamp(
		 (abs(levelPos - u_viewPosInClippedLevel) - u_unblendedRegionSize) * u_oneOverBlendedRegionSize,
		 vec2(0, 0), 
		 vec2(1, 1));

		fsAlpha  = max(alpha.x, alpha.y);																						 
	}
	else
	{
		fsAlpha = 0.0f;
	}

	float fineHeight   = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(og_texture0, fsHeightFineTextureCoordinates)),
		u_maxMinHeights.x,
		u_maxMinHeights.y);

	float coarseHeight = og_denormalizeFloat(
		og_unpackFloatFromVec4(texture(og_texture1, fsHeightCoarseTextureCoordinates)),
		u_maxMinHeights.x,
		u_maxMinHeights.y);

	return mix(fineHeight, coarseHeight, fsAlpha) * u_heightExaggeration;
}



void main()																										 
{																													 
	vec2 levelPos = position + u_patchOriginInClippedLevel;															 

    // Get the height form the height texture.
	fsHeight  = SampleHeight(levelPos);

    // Longitude, Latitude
	vec2 lonLat =
		(levelPos + u_levelOffsetFromWorldOrigin) *
		u_levelScaleFactor *
		u_levelZeroWorldScaleFactor;


#ifdef ENABLE_DOUBLE_PRECISION
    vec3 positionGeocentricHigh;
    vec3 positionGeocentricLow;

    GeodeticToCartesianDoublePrecision(
        u_globeRadiiSquared,
        vec3(lonLat * og_radiansPerDegree, fsHeight),
        positionGeocentricHigh,
        positionGeocentricLow);

    vec3 highDifference   = positionGeocentricHigh - u_center3DHigh;
	vec3 lowDifference    = positionGeocentricLow  - u_center3DLow;


	vec3 centeredPosition  = highDifference + lowDifference;
    vec3 displacedPosition = positionGeocentricHigh + positionGeocentricLow;

#else
    vec3 geodeticPosition = GeodeticToCartesian(
        u_globeRadiiSquared,
        vec3(lonLat * og_radiansPerDegree, fsHeight));

    vec3 centeredPosition       = geodeticPosition - u_center3D;
    vec3 displacedPosition      = geodeticPosition;
#endif


    vec2 textureCoordinates     = position * u_oneOverBlockSize;

    fsPositionToEye             = og_cameraEye - displacedPosition;

	fsPositionEC                = (u_modifiedModelViewMatrix * vec4(centeredPosition, 1.0)).xyz;
	fsImageryTextureCoordinates = (levelPos * u_terrainToImageryResolutionRatio + u_terrainOffsetInImagery) * u_oneOverImagerySize;
    gl_Position                 = getPosition(centeredPosition, fsHeight, textureCoordinates);
}
)";

static const char globe_clipmap_fs[] =
R"(
precision highp float;

in vec3 fsPositionEC;
in vec3 fsPositionToEye;

in float fsAlpha;
in float fsHeight;

in vec2 fsImageryTextureCoordinates;
in vec2 fsHeightFineTextureCoordinates;
in vec2 fsHeightCoarseTextureCoordinates;

uniform sampler2D og_fineNormalTexture;
uniform sampler2D og_coarseNormalTexture;

uniform bool  u_showImagery;
uniform bool  u_shade;
			
uniform vec2 u_maxMinHeights;

out vec4 fragmentColor;

vec3 ComputeNormal()
{
    vec3 fineNormal   = texture(og_fineNormalTexture, fsHeightFineTextureCoordinates).rgb;
    fineNormal        = normalize(fineNormal * 2.0f - vec3(1.0f));

    vec3 coarseNormal = texture(og_coarseNormalTexture, fsHeightCoarseTextureCoordinates).rgb;
    coarseNormal      = normalize(coarseNormal * 2.0f - vec3(1.0f));

	return normalize(mix(fineNormal, coarseNormal, fsAlpha));
}

void main()
{
    vec3 normal                  = ComputeNormal();
    vec3 normalEC                = og_normalMatrix * normal;

    vec3 positionToEye           = normalize(fsPositionToEye);
	vec3 positionToEyeEC		 = normalize(fsPositionEC);
	vec3 positionToLightEC		 = normalize(og_sunDirectionEC + fsPositionEC);

	og_materialInput materialInput;

	materialInput.positionToEyeEC    = positionToEyeEC;
	materialInput.normalEC		     = normalEC;

	vec2 st = vec2(
		0.5, 
		og_normalizeFloat(fsHeight, u_maxMinHeights.x, u_maxMinHeights.y));

	materialInput.st			     = st;

	// Get the material
	og_material material			 = og_getMaterial(materialInput);

    if(abs(dot(normalEC, positionToEyeEC)) < 0.015f)
    {
      fragmentColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        if (u_shade)
        {
            fragmentColor = og_phong(
                positionToEyeEC,
                positionToLightEC,
                material);
            fragmentColor.w = 1.0;
        }
        else
        {
            vec3 rgb_normal = (normal * 0.5f) + 0.5f;
            fragmentColor = vec4(rgb_normal, 1.0);
        }
    }
}

)";
}