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
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec3 Bitangent;
attribute vec4 TexCoord;

varying float atten; 
varying float fogFactor; 
varying vec3 viewVec; 

void main() 
{	
	vec4 posViewSpace = ModelViewMatrix*Position;
	
	gl_Position = ftransform();
	
	vec3 vertexNormal_ViewSpace = mat3(ModelViewMatrix) * Normal;
	vec3 vertexTangent_ViewSpace = mat3(ModelViewMatrix) * Tangent;
	vec3 vertexBitangent_ViewSpace = mat3(ModelViewMatrix) * Bitangent;
	vec3 t = vec3(vertexTangent_ViewSpace.x, vertexBitangent_ViewSpace.x, vertexNormal_ViewSpace.x);
	vec3 b = vec3(vertexTangent_ViewSpace.y, vertexBitangent_ViewSpace.y, vertexNormal_ViewSpace.y);
	vec3 n = vec3(vertexTangent_ViewSpace.z, vertexBitangent_ViewSpace.z, vertexNormal_ViewSpace.z);
	mat3 TBN = mat3(t, b, n);
	
	vec3 vVertex = vec3(ModelViewMatrix * gl_Vertex);
	
	vec3 vVec = -vVertex;
	/*viewVec.x = dot(vVec, t);
	viewVec.y = dot(vVec, b);
	viewVec.z = dot(vVec, n);*/
	
	viewVec = vVec * TBN;
	
	fogDensity = 1;
	
	
	const float LOG2 = 1.442695;
	gl_FogFragCoord = length(vVertex);
	fogFactor = exp2( -fogDensity * 
					   fogDensity * 
					   length(vVertex)  * 
					   length(vVertex)  * 
					   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
}
