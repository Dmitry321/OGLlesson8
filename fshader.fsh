struct materialProperty{
    vec3 diffuseColor;
    vec3 ambienceColor;
    vec3 specularColor;
    float shinnes;
};

struct lightProperty
{
    vec3 ambienceColor;
    vec3 diffuseColor;
    vec3 specularColor;
    vec4 position;
    vec4 direction;
    float cutoff;
    int type; // Directional = 0 Point = 1 Spot = 2

};

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_shadowMap;
//uniform highp vec4 u_lightPosition;
uniform materialProperty u_materialProperty;
uniform highp float u_lightPower;
uniform bool u_isUsingDiffuseMap;
uniform bool u_isUsingNormalMap;
//uniform highp vec4 u_lightDirection;
uniform lightProperty u_lightProperty[3];
uniform int u_countLights;
uniform int u_indexLightForShadow;

varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;
varying highp mat3 v_tbnMatrix;
//varying highp vec4 v_lightDirection;
varying highp vec4 v_positionLightMatrix;
varying highp mat4 v_viewMatrix;
lightProperty v_lightProperty[3];

float SampleShadowMap(sampler2D map, vec2 coords, float compare){
    vec4 v = texture2D(map, coords);
    float value = v.x * 255.0 + (v.y * 255.0 + (v.z *255.0 + v.w) / 255.0) / 255.0;
    return step(compare, value);
}

// Линейная фильтрация чуть сглаживает углы на тени но оставляет лесенки
float SampleShadowMapLinear(sampler2D map, vec2 coords, float compare, vec2 texelSize){ // просто размер пикселя в текстурных координатах от 0 до 1 должен быть размер
    // Изначально получаем позицию пикселя
    vec2 pixelPos = coords / texelSize + 0.5;  // +0.5 чтобы попасть в центр пикселя
    vec2 fractPart = fract(pixelPos);
    vec2 startTexel = (pixelPos - fractPart) * texelSize;
    // Находим приближение по 4м соседним клеткам
    float blTexel = SampleShadowMap(map, startTexel, compare); // bottom left texel
    float brTexel = SampleShadowMap(map, startTexel + vec2(texelSize.x, 0.0), compare); // bottom right texel
    float tlTexel = SampleShadowMap(map, startTexel + vec2(0.0, texelSize.y), compare); // top left texel
    float trTexel = SampleShadowMap(map, startTexel + texelSize, compare); // top right texel
    // Далее ищем интерополяцию от одной точки до другой с учетом дробной части
    float mixA = mix(blTexel, tlTexel, fractPart.y);
    float mixB = mix(brTexel, trTexel, fractPart.y);

    return mix(mixA, mixB, fractPart.x);
}

// еще один алгоритм чтобы убрать лесенки Point Cloud Filtering
float SampleShadowMapPCF(sampler2D map, vec2 coords, float compare, vec2 texelSize)
{
    float result = 0.0;
    // вместо -1.0 и 1.0 можно брать большие числа чтобы больше размывать границу но это требует большей производительности
    for (float y = -1.0; y < 1.0; y += 1.0)
        for (float x = -1.0; x < 1.0; x += 1.0)
        {
            vec2 offset = vec2(x, y) * texelSize;
            result += SampleShadowMapLinear(map, coords + offset, compare, texelSize);
        }
    return result / 9.0; // та как у нас ровно 9 элементов из цикла 3x3
}

float CalcShadowAmount(sampler2D map, vec4 initialShadowCoords)
{
    vec3 tmp = v_positionLightMatrix.xyz / v_positionLightMatrix.w;
    tmp = tmp * vec3(0.5) + vec3(0.5);
    float offset = 2.0;
    offset *= dot(v_normal, v_lightProperty[u_indexLightForShadow].direction.xyz);
    //return SampleShadowMap(map, tmp.xy, tmp.z * 255.0 - 0.5); //z - удаленность
    //return SampleShadowMapLinear(map, tmp.xy, tmp.z * 255.0 - 0.5, vec2(1.0 / 1024.0));
    return SampleShadowMapPCF(map, tmp.xy, tmp.z * 255.0 + offset, vec2(1.0 / 1024.0));
}

void main(void)
{
    for(int i = 0; i < u_countLights; ++i){
        v_lightProperty[i].ambienceColor = u_lightProperty[i].ambienceColor;
        v_lightProperty[i].diffuseColor = u_lightProperty[i].diffuseColor;
        v_lightProperty[i].specularColor = u_lightProperty[i].specularColor;
        v_lightProperty[i].cutoff = u_lightProperty[i].cutoff;
        v_lightProperty[i].type = u_lightProperty[i].type;
        v_lightProperty[i].direction = v_viewMatrix * u_lightProperty[i].direction;
        v_lightProperty[i].position = v_viewMatrix * u_lightProperty[i].position;
    }

    highp float shadowCoef;
    if(v_lightProperty[u_indexLightForShadow].type == 0) // Directional
        shadowCoef = CalcShadowAmount(u_shadowMap, v_positionLightMatrix);
    else
        shadowCoef = 1.0;
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
    for(int i = 0; i < u_countLights; ++i){
        vec4 resultLightColor = vec4(0.0, 0.0, 0.0, 0.0);;

        vec3 lightVect;
        if(v_lightProperty[i].type == 0) // Directional
            lightVect = normalize(v_lightProperty[i].direction.xyz);
        else{ // Point or Spot
            lightVect = normalize(v_position - v_lightProperty[i].position).xyz;
            if(v_lightProperty[i].type == 2){ //Spot
                float angle = acos(dot(v_lightProperty[i].direction.xyz, lightVect)); //  скалярное произведение единичных векторов - произведение модулей векторов на cos угла между ними
                if(angle > v_lightProperty[i].cutoff)
                    lightVect = vec3(0.0, 0.0, 0.0);
            }
        }
        if(u_isUsingNormalMap) lightVect = normalize(v_tbnMatrix * lightVect);
        vec3 reflectLight = normalize(reflect(lightVect, usingNormal));
        float len = length(v_position.xyz - eyePosition.xyz);
        float specularFactor = u_materialProperty.shinnes; //60.0;  // размер пятна блика
        float ambientFactor = 0.1; // светимость самого материала

        if(u_isUsingDiffuseMap == false) diffMatColor = vec4(u_materialProperty.diffuseColor, 1.0);
        vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(usingNormal, -lightVect));// / (1.0 + 0.25 * pow(len, 2.0f)); добаляет затухание света при иудалении
        resultLightColor += diffColor * vec4(v_lightProperty[i].diffuseColor, 1.0);
        vec4 ambientColor = ambientFactor * diffMatColor;
        resultLightColor += ambientColor * vec4(u_materialProperty.ambienceColor,1.0) * vec4(v_lightProperty[i].ambienceColor, 1.0);
        vec4 specularColor = specColor * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);// / (1.0 + 0.25 * pow(len, 2.0f));
        resultLightColor += specularColor * vec4(u_materialProperty.specularColor,1.0) * vec4(v_lightProperty[i].specularColor, 1.0);

        resultColor += resultLightColor;
    }
    // чтобы не было совсем черных точек изменяем коэффициент затенения
    shadowCoef += 0.15; // чем больше тем светлее
    if(shadowCoef > 1.0) shadowCoef = 1.0;


    gl_FragColor = resultColor * shadowCoef; //texture2D(u_texture, v_texcoord);//vec4(1.0, 0.0, 0.0, 1.0);//texture2D(qt_Texture0, qt_TexCoord0);
}
