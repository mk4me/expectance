!!ARBvp1.0

ATTRIB v0 = vertex.position;				# vertex position

PARAM WVP[4] = { state.matrix.mvp };

DP4 result.position.x, WVP[0], v0;			# output screen positon
DP4 result.position.y, WVP[1], v0;
DP4 result.position.z, WVP[2], v0;
DP4 result.position.w, WVP[3], v0;

END