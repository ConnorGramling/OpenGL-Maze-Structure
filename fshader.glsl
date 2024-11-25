#version 120

varying vec4 color;
varying vec2 texCoord;
varying vec4 N, V, L;

uniform sampler2D texture;
uniform int use_texture;

uniform int ambient_on;
uniform int diffuse_on;
uniform int specular_on;

vec4 ambient, diffuse, specular;

void main()
{
    vec4 base_color;

    // Determine base color (from texture or vertex color)
    if (use_texture == 1) {
        base_color = texture2D(texture, texCoord);
    } else {
        base_color = color;
    }

    // Normalize vectors
    vec4 NN = normalize(N);
    vec4 LL = normalize(L);
    vec4 VV = normalize(V);
    vec4 H = normalize(LL + VV);

    // Ambient lighting
    ambient = (ambient_on == 1) ? 0.3 * base_color : vec4(0.0);

    // Diffuse lighting
    diffuse = (diffuse_on == 1) ? max(dot(LL, NN), 0.0) * base_color : vec4(0.0);

    // Specular lighting
    specular = (specular_on == 1) ? pow(max(dot(NN, H), 0.0), 50) * vec4(1.0) : vec4(0.0);

    // Combine components
    gl_FragColor = ambient + diffuse + specular;
}
