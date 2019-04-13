#version 430

layout (lines_adjacency) in;
layout (line_strip, max_vertices=65) out;

out vec4 color;

void main(void){
    int i;
    float t=0.0, h=1.0/65.0;

    color=vec4(1,0,1,1);

    vec4 q0, q1, q2, r0, r1, s;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    for(i=1; i<64; i++){
        t +=h;
        q0=mix(gl_in[0].gl_Position, gl_in[1].gl_Position,t);
        q1=mix(gl_in[1].gl_Position, gl_in[2].gl_Position,t);
        q2=mix(gl_in[2].gl_Position, gl_in[3].gl_Position,t);
        r0=mix(q0, q1,t);
        r1=mix(q1, q2,t);
        gl_Position = mix(r0, r1, t);
        EmitVertex();
    }
    gl_Position = gl_in[3].gl_Position;
    EmitVertex();

    EndPrimitive();
}
