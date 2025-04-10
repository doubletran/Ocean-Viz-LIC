// Uniform variables
uniform float Length;             // Length of the convolution line
uniform bool ShowMagnitude;       // Flag for showing magnitude of flow
uniform sampler2D TexUnit;       // Main texture
uniform sampler2D FlowUnit;      // Flow vector texture

float step = 0.05f;               // Step size for movement

void main(void) {

    vec2 v;                      // Flow vector
    int LengthP;                  // Length of the convolution line
    ivec2 res = textureSize(TexUnit, 0); // Texture resolution

    vec3 color;                   // Accumulated color value
    
    vec2 st = gl_TexCoord[0].st;  // Texture coordinates
    vec4 tex = texture(FlowUnit, st); // Sample flow texture
    v = tex.rg;                   // Flow direction from red and green channels

    v *= 1.0 / res;               // Normalize flow direction
    color = vec3(texture2D(TexUnit, st)); // Sample color from the main texture

    // Set number of steps based on flow magnitude or constant length
    st = gl_TexCoord[0].st;
    if (ShowMagnitude) {
        LengthP = int(tex.b *10) ;
    } else {
        LengthP = int(Length);
    }

    // Forward direction LIC
    for (int i = 0; i < LengthP; i++) {
        st += v;
        st = clamp(st, 0.0, 1.0);  // Keep coordinates within bounds
        color += vec3(texture2D(TexUnit, st)); // Accumulate color
    }

    // Reverse direction LIC
    st = gl_TexCoord[0].st;
    LengthP = int(Length);
    for (int i = 0; i < LengthP; i++) {
        st -= v;
        st = clamp(st, 0.0, 1.0);
        color += vec3(texture2D(TexUnit, st)); // Accumulate color
    }

    // Normalize color
    float OneOverNum = 1.0 / (LengthP + LengthP + 1.0);
    color *= OneOverNum;
   // color.z = 0.7;  // Set blue component

    // If no flow, set color to black
    if ((v.x == 0.0) && (v.y == 0.0)) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        gl_FragColor = vec4(color, 1.0); // Output final color
    }

}
