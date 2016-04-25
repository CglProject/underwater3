uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMarix;

attribute vec4 Position;
attribute vec4 Normal;
attribute vec4 TexCoord;

varying vec4 colorVarying;

void main()
{
	vec4 pos = ModelViewMatrix * Position;

	colorVarying = vec4(vec3(0.5) + Normal.xyz * 0.5, 1.0);
	
	gl_Position = ProjectionMatrix * pos;
}