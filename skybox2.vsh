attribute vec3 a_position;
varying vec3 vTexCoord;
//uniform mat4 mvpMatrix;
uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_modelMatrix;
void main()
{
    mat4 viewMatrix = u_viewMatrix;
    // информация о сдвигах хранится в последнем столбце матрицы
    viewMatrix[3][0] = 0.0; // x
    viewMatrix[3][1] = 0.0; // y
    viewMatrix[3][2] = 0.0; // z
    mat4 mv_matrix = viewMatrix * u_modelMatrix;
    gl_Position = u_projectionMatrix * mv_matrix * vec4(a_position, 1.0);
    //  v_texcoord = a_texcoord;
    //  gl_Position = mvpMatrix * vec4(a_position, 1.0);
    vTexCoord = a_position;
}
