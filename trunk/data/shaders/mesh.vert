!!ARBvp1.0
PARAM constant = { 1, 3, 0, 0 };
TEMP R0, R1, R2, R3, R4, R5;
ADDRESS A0;
ATTRIB texCoord = vertex.attrib[8];
ATTRIB normal = vertex.attrib[2];
ATTRIB index = vertex.attrib[3];
ATTRIB weight = vertex.attrib[1];
ATTRIB position = vertex.attrib[0];
PARAM worldViewProjMatrix[4] = { state.matrix.mvp };
PARAM mvinv[4] = { state.matrix.modelview.invtrans };
PARAM diffuse = state.material.diffuse;
PARAM ambient = state.material.ambient;
PARAM lightDir = state.light[0].position;
PARAM halfDir = state.light[0].half;
PARAM specExp = state.material.shininess;
PARAM ambientCol = state.lightprod[0].ambient;
PARAM diffuseCol = state.lightprod[0].diffuse;
PARAM specularCol = state.lightprod[0].specular;
TEMP eyeNormal, temp, dots, lightcoefs;
PARAM matrix[87] = { program.local[0..86] };

MOV result.texcoord[0].xy, texCoord.xyxx;	

MUL R4, index, constant.y;	

ARL A0.x, R4.y;
DP3 R0.x, matrix[A0.x].xyzx, normal.xyzx;
DP3 R0.y, matrix[A0.x + 1].xyzx, normal.xyzx;
DP3 R0.z, matrix[A0.x + 2].xyzx, normal.xyzx;
MUL R1.yzw, R0.xxyz, weight.y;

ARL A0.x, R4.x;
DP3 R0.x, matrix[A0.x].xyzx, normal.xyzx;
DP3 R0.y, matrix[A0.x + 1].xyzx, normal.xyzx;
DP3 R0.z, matrix[A0.x + 2].xyzx, normal.xyzx;
MAD R1.yzw, R0.xxyz, weight.x, R1.yyzw;

DP3 R0.x, R1.yzwy, R1.yzwy;
RSQ R0.x, R0.x;
MUL R0.xyz, R0.x, R1.yzwy;
DP3 R1.x, lightDir.xyzx, lightDir.xyzx;
RSQ R1.x, R1.x;
MUL R2.xyz, R1.x, lightDir.xyzx;
DP3 R0.x, R0.xyzx, R2.xyzx;
MAX R0.x, R0.x, constant.z;
ADD R0, R0.x, ambientCol;

MUL result.color.front.primary, R0, diffuse;

ARL A0.x, R4.w;
DPH R0.x, position.xyzx, matrix[A0.x];
DPH R0.y, position.xyzx, matrix[A0.x + 1];
DPH R0.z, position.xyzx, matrix[A0.x + 2];

ARL A0.x, R4.z;
DPH R3.x, position.xyzx, matrix[A0.x];
DPH R3.y, position.xyzx, matrix[A0.x + 1];
DPH R3.z, position.xyzx, matrix[A0.x + 2];

ARL A0.x, R4.y;
DPH R1.y, position.xyzx, matrix[A0.x];
DPH R1.z, position.xyzx, matrix[A0.x + 1];
DPH R1.w, position.xyzx, matrix[A0.x + 2];
MUL R2.xyz, R1.yzwy, weight.y;

ARL A0.x, R4.x;
DPH R1.x, position.xyzx, matrix[A0.x];
DPH R1.y, position.xyzx, matrix[A0.x + 1];
DPH R1.z, position.xyzx, matrix[A0.x + 2];

MAD R1.xyz, R1.xyzx, weight.x, R2.xyzx;
MAD R1.xyz, R3.xyzx, weight.z, R1.xyzx;
MAD R0.xyz, R0.xyzx, weight.w, R1.xyzx;

DPH result.position.x, R0.xyzx, worldViewProjMatrix[0];
DPH result.position.y, R0.xyzx, worldViewProjMatrix[1];
DPH result.position.z, R0.xyzx, worldViewProjMatrix[2];
DPH result.position.w, R0.xyzx, worldViewProjMatrix[3];
END