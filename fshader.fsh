struct materialProperty{
    vec3 diffuseColor;
    vec3 ambienceColor;
    vec3 specularColor;
    float shinnes;
};

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_shadowMap;
//uniform highp vec4 u_lightPosition;
uniform materialProperty u_materialProperty;
uniform highp float u_lightPower;
uniform bool u_isUsingDiffuseMap;
uniform bool u_isUsingNormalMap;
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;
varying highp mat3 v_tbnMatrix;
varying highp vec4 v_lightDirection;
varying highp vec4 v_positionLightMatrix;

float SampleShadowMap(sampler2D map, vec2 coords, float compare){
    vec4 v = texture2D(map, coords);
    float value = v.x * 255.0 + (v.y * 255.0 + (v.z *255.0 + v.w) / 255.0) / 255.0;
    return step(compare, value);

}

float CalcShadowAmount(sampler2D map, vec4 initialShadowCoords)
{
    vec3 tmp = v_positionLightMatrix.xyz / v_positionLightMatrix.w;
    tmp = tmp * vec3(0.5) + vec3(0.5);
    return SampleShadowMap(map, tmp.xy, tmp.z * 255.0 - 0.5); //z - удаленность
}

void main(void)
{
    highp float shadowCoef = CalcShadowAmount(u_shadowMap, v_positionLightMatrix);
    vec3 tmp = v_positionLightMatrix.xyz / v_positionLightMatrix.w;
    tmp = tmp * vec3(0.5) + vec3(0.5);
    // ищем коэффициент затенинеия который будет либо 0 либо 1

    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0); // черный цвет без альфа канала
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0); // позиция наблюдателя
    vec4 specColor = vec4(1.0, 1.0, 1.0, 1.0); // цвет блика былый непрозрачный
    vec4 diffMatColor = texture2D(u_diffuseMap, v_texcoord);
    vec3 usingNormal = v_normal;
    if(u_isUsingNormalMap) usingNormal = normalize(texture2D(u_normalMap, v_texcoord).rgb * 2.0 - 1.0);

    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    if(u_isUsingNormalMap)  eyeVect = normalize(v_tbnMatrix * eyeVect);
 //   vec3 lightVect = normalize(v_position.xyz - u_lightPosition.xyz);
    vec3 lightVect = normalize(v_lightDirection.xyz);
    if(u_isUsingNormalMap) lightVect = normalize(v_tbnMatrix * lightVect);
    vec3 reflectLight = normalize(reflect(lightVect, usingNormal));
    float len = length(v_position.xyz - eyePosition.xyz);
    float specularFactor = u_materialProperty.shinnes; //60.0;  // размер пятна блика
    float ambientFactor = 0.1; // светимость самого материала

    if(u_isUsingDiffuseMap == false) diffMatColor = vec4(u_materialProperty.diffuseColor, 1.0);
    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(usingNormal, -lightVect));// / (1.0 + 0.25 * pow(len, 2.0f)); добаляет затухание света при иудалении
    resultColor += diffColor;
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor * vec4(u_materialProperty.ambienceColor,1.0);
    vec4 specularColor = specColor * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);// / (1.0 + 0.25 * pow(len, 2.0f));
    resultColor += specularColor * vec4(u_materialProperty.specularColor,1.0);

    // чтобы не было совсем черных точек изменяем коэффициент затенения
    shadowCoef += 0.2;
    if(shadowCoef > 1.0) shadowCoef = 1.0;


    gl_FragColor = resultColor * shadowCoef; //texture2D(u_texture, v_texcoord);//vec4(1.0, 0.0, 0.0, 1.0);//texture2D(qt_Texture0, qt_TexCoord0);
}
