$B_SHADER_VERSION
#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 lightPositionViewSpace_0;
uniform float lightIntensity_0;
uniform float lightAttenuation_0;
uniform float lightRadius_0;
uniform vec3 lightDiffuseColor_0;
uniform vec3 lightSpecularColor_0;
varying vec3 lightVectorTangentSpace_0;
varying float intensityBasedOnDist_0;
uniform vec4 lightPositionViewSpace_1;
uniform float lightIntensity_1;
uniform float lightAttenuation_1;
uniform float lightRadius_1;
uniform vec3 lightDiffuseColor_1;
uniform vec3 lightSpecularColor_1;
varying vec3 lightVectorTangentSpace_1;
varying float intensityBasedOnDist_1;
uniform vec4 lightPositionViewSpace_2;
uniform float lightIntensity_2;
uniform float lightAttenuation_2;
uniform float lightRadius_2;
uniform vec3 lightDiffuseColor_2;
uniform vec3 lightSpecularColor_2;
varying vec3 lightVectorTangentSpace_2;
varying float intensityBasedOnDist_2;
uniform vec4 lightPositionViewSpace_3;
uniform float lightIntensity_3;
uniform float lightAttenuation_3;
uniform float lightRadius_3;
uniform vec3 lightDiffuseColor_3;
uniform vec3 lightSpecularColor_3;
varying vec3 lightVectorTangentSpace_3;
varying float intensityBasedOnDist_3;
varying vec4 texCoordVarying;
varying vec3 surfaceToCameraTangentSpace;
uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform vec3 ambientColor;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;
uniform float transparency;
uniform
#ifdef GL_ES
highp   // offset gets very big, needs to be highp on iOS
#endif
float offset;

varying float atten; 
varying float fogFactor; 
varying vec3 viewVec; 

uniform sampler2D fbo_texture;


void main() {
	
	vec3 lVec = normalize(lightVec);
	vec3 vVec = normalize(viewVec);
	
	vec4 base = texture2D(fbo_texture, texCoordVarying);

		
	gl_FragColor = mix(vec4(0.7,0.7,0.7), base, fogFactor );
	
}