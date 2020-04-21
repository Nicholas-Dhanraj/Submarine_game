
 varying float NdotL;
 varying vec3 R;
 varying vec3 V;
	

void main() {     
vec3 eyePos = vec3(gl_ModelViewMatrix *gl_Vertex);vec3 
 N =normalize(gl_NormalMatrix*gl_Normal);vec3 
 L =normalize(gl_LightSource[0].position.xyz-eyePos);
 R = normalize(reflect(-L, N));V = normalize(-eyePos);
 NdotL = (dot(L, N) + 1.0) * 0.5;gl_Position   = ftransform();
 gl_FrontColor = vec4(vec3(0.75), 1.0);
 gl_BackColor  = vec4(0.0);
	
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    // Set the position of the current vertex 
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}