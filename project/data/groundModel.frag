
// ground-model Fragment Shader loding
// 
//
//
varying vec4 colorVarying;

varying vec4 texCoordVarying;
uniform sampler2D DiffuseMap;

void main()
{
    // TODO: change output color
	//gl_FragColor = vec4(1,0,1,1);
	
	gl_FragColor = texture2D(DiffuseMap, texCoordVarying.st);
}
