#version 120

varying vec4 color;
varying vec2 texCoord;
varying vec4 N, V, L;

uniform sampler2D texture;
uniform int use_texture;

vec4 ambient, diffuse, specular;

void main()
{
    if(use_texture == 1) {
        vec4 the_color = texture2D(texture, texCoord);
        vec4 NN = normalize(N);
        vec4 LL = normalize(L);
        vec4 VV = normalize(V);
				vec4 H = normalize(LL + VV);
        ambient = 0.3 * the_color;
        diffuse = max(dot(LL, NN), 0.0) * the_color;
				specular = pow(max(dot(NN, H), 0.0), 50) * the_color * vec4(1.0);
        gl_FragColor = ambient + diffuse + specular;
    }
    else {
        vec4 NN = normalize(N);
        vec4 LL = normalize(L);
        vec4 VV = normalize(V);
				vec4 H = normalize(LL + VV);
        ambient = 0.3 * color;
        diffuse = max(dot(LL, NN), 0.0) * color;
				specular = pow(max(dot(NN, H), 0.0), 50) * color * vec4(1.0);
				gl_FragColor = ambient + diffuse + specular;
    }
}
