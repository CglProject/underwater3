
// TODO: create varying variable to receive color values from the vertex shader, don't forget to specify precision
varying vec4 colorVarying;

void main()
{
    // TODO: change output color
	//gl_FragColor = vec4(1,0,1,1);
	
	gl_FragColor = colorVarying;
}
