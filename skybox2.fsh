uniform samplerCube u_texture;
varying vec3 vTexCoord;

void main()
{
    gl_FragColor = textureCube(u_texture, vTexCoord);
}
