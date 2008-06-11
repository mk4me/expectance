/* Copyright (C) Dante Treglia II, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Dante Treglia II, 2000"
 */
#include "spline.h"
using namespace ft;


#define STATE_BEGIN 0
#define STATE_SPLINE 1
#define STATE_POINTS 3
static int state = STATE_BEGIN;





//---------------------------------------------------------------------------
// Contstruct a Spline using a Definition File
//---------------------------------------------------------------------------
Spline::Spline(const char * fileName) {
	// Initialization
	controlCnt = 0;
	controlIndx = 0;
	defualtNrm = vector3(0.0, 1.0, 0.0);
	curveSubD = 0;
	curveCnt = 0;
	curveIndx = 0;
	curveType = 0;
	shortestDistance = 0;

	// Fill Structure
	ParseDefinitionFile(fileName);

	// Build Spline
	Build();
}

/*
//---------------------------------------------------------------------------
// Contstruct a Spline using Control Points
// For optimization reasons, the data is not copied here. You
// must make sure that the data remains allocated.
//---------------------------------------------------------------------------
Spline::Spline(SplineControlData * data, int controlCntIn, int curveSubDIn) {
	// Initialization
	controlIndx = 0;
	curveIndx = 0;
	
	// Fill Structure
	controlData = data;
	controlCnt = controlCntIn;
	curveSubD = curveSubDIn;

	curveType = 0;
	displayControlMode = 1;
	displayCurveMode = 1;
	displayCurveModulo = 1;
	displayOrientMode = 0;
	displayOrientModulo = 1;
	displayOrientLen = 0.25;

	// Build Spline
	Build();
}
*/

//---------------------------------------------------------------------------
// Parse a Spline Definitions File
//---------------------------------------------------------------------------
void Spline::ParseDefinitionFile(const char * fileName) {
	char buffer[256];
	double x, y, z, param;
	FILE * file;

	file = fopen(fileName, "r");
	assert(file != NULL && "Spline:: Couldn't Open Parse File");

	// Get track parameters
	while (1) { 
		// Grag Info
		fgets(buffer, 256, file);

		// EOF
		if (feof(file)) {
			assert(state == STATE_BEGIN && "Spline:: Couldn't Parse File");
//			assert(displayOrientModulo && "Spline:: No Modulo Set!");
//			assert(displayCurveModulo && "Spline:: No Modulo Set!");
			return;
		}

		// State Switching
		switch (state) {
			case STATE_BEGIN:
				if (strstr(buffer, "begin_spline")) state = STATE_SPLINE;
			break;
			case STATE_SPLINE:
				if (sscanf(buffer, " control_count %lf", &param) == 1) {
					controlCnt = (int)param;
				}
				else if (sscanf(buffer, " curve_subdivision %lf", &param) == 1) {
					curveSubD = (int)param;
				}
				else if (sscanf(buffer, " curve_type %lf", &param) == 1) {
					curveType = (int)param;
				}
				else if (sscanf(buffer, " display_control_mode %lf", &param) == 1) {
//					displayControlMode = (int)param;
				}
				else if (sscanf(buffer, " display_curve_mode %lf", &param) == 1) {
//					displayCurveMode = (int)param;
				}
				else if (sscanf(buffer, " display_curve_modulo %lf", &param) == 1) {
//					displayCurveModulo = (int)param;
				}
				else if (sscanf(buffer, " display_orientation_mode %lf", &param) == 1) {
//					displayOrientMode = (int)param;
				}
				else if (sscanf(buffer, " display_orientation_modulo %lf", &param) == 1) {
//					displayOrientModulo = (int)param;
				}
				else if (sscanf(buffer, " display_orientation_len %lf", &param) == 1) {
//					displayOrientLen = (float)param;
				}
				else if (strstr(buffer, "begin_points")) {
					// Allocate Point Space
					assert(curveSubD != 0 && "No curve subdivision!\n");
					assert(controlCnt != 0 && "No controlCnt");

					// Alloc Control Points
					controlData = (SplineControlData *)malloc(controlCnt * sizeof(SplineControlData));
					memset(controlData, 0, controlCnt * sizeof(SplineControlData));

					// Alloc Curve Points
					curveCnt = controlCnt * curveSubD;
					curveData = (SplineCurveData *)malloc(curveCnt * sizeof(SplineCurveData));
					memset(curveData, 0, curveCnt * sizeof(SplineCurveData));
					state = STATE_POINTS;
				}
				else if (strstr(buffer, "end_spline")) state = STATE_BEGIN;
			break;
			case STATE_POINTS:
				if (controlIndx == controlCnt) {
					state = STATE_SPLINE;
				}
				if (sscanf(buffer, " C: %lf %lf %lf", &x, &y, &z) == 3) {
					controlData[controlIndx].pos.x = (float)x;
					controlData[controlIndx].pos.y = (float)y;
					controlData[controlIndx].pos.z = (float)z;
					// Since there is no end indicator to the control point, I will increment here
					controlIndx ++;
				}
				if (strstr(buffer, "P:") && controlIndx >= 2) {
					// Since paramters are placed after the control point is defined, and I incremented
					// above, I need to subtract one from the index. PLUS, since the first control
					// point is not drawn, I need to subtract another one...
					if (strstr(buffer, "no_gravity")) {
						controlData[controlIndx - 2].physicsFlag = CONTROL_PHYSICS_NO_GRAVITY;
					}
					else if (sscanf(buffer, " P: break %lf", &x) == 1) {
						controlData[controlIndx - 2].physicsFlag = CONTROL_PHYSICS_BREAK;
						controlData[controlIndx - 2].physicsValue = (float)x;
					}
				}
				if (strstr(buffer, "N:") && controlIndx >= 2) {
					if (sscanf(buffer, " N: %lf %lf %lf ", &x, &y, &z) == 3) {
						controlData[controlIndx - 2].nrmFlag = CONTROL_NORMAL_GIVEN;
						controlData[controlIndx - 2].nrm.x = (float)x;
						controlData[controlIndx - 2].nrm.y = (float)y;
						controlData[controlIndx - 2].nrm.z = (float)z;
					}
					else if (sscanf(buffer, " N: rotate %lf ", &x) == 1) {
						controlData[controlIndx - 2].nrmFlag = CONTROL_NORMAL_ROT;
						controlData[controlIndx - 2].nrmValue = (float)x;
					}
				}
				else if (strstr(buffer, "end_points")) state = STATE_SPLINE;
			break;
		}
	}
}

//---------------------------------------------------------------------------
// Build a Spline
//---------------------------------------------------------------------------
void Spline::Build(vector3 defualtNrmIn) {
	defualtNrm = defualtNrmIn;
	Build();
}

//---------------------------------------------------------------------------
// Build a Spline
//---------------------------------------------------------------------------
void Spline::Build() {
	float u, u_2, u_3;
	int i, j;
	int index;
	vector3 distVec;
	vector3 origin(0.0, 0.0, 0.0);
	matrix44 tempMtx;

	index = 0;
	// For each control Point (Minus the last three)
	for (i = 0; i < controlCnt - 3; i++) {

		// For each subdivision
		for(j = 0; j < curveSubD; j++) {

			u = (float)j / curveSubD;
			u_2 = u * u;
			u_3 = u_2 * u;

			if (curveType == 0) {
				// Position
				curveData[index].pos.x = B_SPLINE(u, u_2, u_3, 
									controlData[i  ].pos.x,
									controlData[i+1].pos.x,
									controlData[i+2].pos.x,
									controlData[i+3].pos.x);

				curveData[index].pos.y = B_SPLINE(u, u_2, u_3, 
									controlData[i  ].pos.y,
									controlData[i+1].pos.y,
									controlData[i+2].pos.y,
									controlData[i+3].pos.y);

				curveData[index].pos.z = B_SPLINE(u, u_2, u_3, 
									controlData[i  ].pos.z,
									controlData[i+1].pos.z,
									controlData[i+2].pos.z,
									controlData[i+3].pos.z);

				// Tangent
				curveData[index].tan.x = B_TANGENT(u, u_2,
									controlData[i  ].pos.x,
									controlData[i+1].pos.x,
									controlData[i+2].pos.x,
									controlData[i+3].pos.x);

				curveData[index].tan.y = B_TANGENT(u, u_2,
									controlData[i  ].pos.y,
									controlData[i+1].pos.y,
									controlData[i+2].pos.y,
									controlData[i+3].pos.y);

				curveData[index].tan.z = B_TANGENT(u, u_2,
									controlData[i  ].pos.z,
									controlData[i+1].pos.z,
									controlData[i+2].pos.z,
									controlData[i+3].pos.z);
				curveData[index].tan.normalize();
			}
			else {
				curveData[index].pos.x = CATMULL_ROM_SPLINE(u, u_2, u_3, 
									controlData[i  ].pos.x,
									controlData[i+1].pos.x,
									controlData[i+2].pos.x,
									controlData[i+3].pos.x);

				curveData[index].pos.y = CATMULL_ROM_SPLINE(u, u_2, u_3, 
									controlData[i  ].pos.y,
									controlData[i+1].pos.y,
									controlData[i+2].pos.y,
									controlData[i+3].pos.y);

				curveData[index].pos.z = CATMULL_ROM_SPLINE(u, u_2, u_3, 
									controlData[i  ].pos.z,
									controlData[i+1].pos.z,
									controlData[i+2].pos.z,
									controlData[i+3].pos.z);

				// Tangent
				curveData[index].tan.x = CATMULL_ROM_TANGENT(u, u_2,
									controlData[i  ].pos.x,
									controlData[i+1].pos.x,
									controlData[i+2].pos.x,
									controlData[i+3].pos.x);

				curveData[index].tan.y = CATMULL_ROM_TANGENT(u, u_2,
									controlData[i  ].pos.y,
									controlData[i+1].pos.y,
									controlData[i+2].pos.y,
									controlData[i+3].pos.y);

				curveData[index].tan.z = CATMULL_ROM_TANGENT(u, u_2,
									controlData[i  ].pos.z,
									controlData[i+1].pos.z,
									controlData[i+2].pos.z,
									controlData[i+3].pos.z);
				curveData[index].tan.normalize();
			}

			// Normal
			if (j == 0 && i == 0) {
				curveData[0].nrm = defualtNrm;
				curveData[0].sid = CrossProduct(curveData[0].nrm, curveData[0].tan);
			}
			else {
				// Cross two tangents To get a side vector
				curveData[index].sid = CrossProduct(curveData[index - 1].tan, curveData[index].tan);
				curveData[index].sid.normalize();
				// Modify side vector
#if 0
				if (controlData[i].nrmFlag == CONTROL_NORMAL_ROT) {
					MTXRotAxis(tempMtx, &curveData[index].tan, controlData[i].nrmValue);
					MTXMultVec(tempMtx, &curveData[index].sid, &curveData[index].sid);
				}
				if (controlData[i].nrmFlag == CONTROL_NORMAL_GIVEN) {
					MTXRotAxis(tempMtx, &curveData[index].tan, 90);
					MTXMultVec(tempMtx, &controlData[i].nrm, &curveData[index].sid);
					MTXLookAt(tempMtx, &origin,
						&curveData[index].sid,
						&curveData[index].tan);
					MTXMultVec(tempMtx, &curveData[index].sid, &curveData[index].sid);
					curveData[index].sid.z = 0;
					VECNormalize(&curveData[index].sid, &curveData[index].sid);
					MTXInverse(tempMtx, tempMtx);
					MTXMultVec(tempMtx, &curveData[index].sid, &curveData[index].sid);
					VECNormalize(&curveData[index].sid, &curveData[index].sid);
				}
#endif
				// Make sure there are no Zero vectors
				if (curveData[index].sid.isZero()) curveData[index - 1].sid = curveData[index].sid;
				// Check for Vector Flip
				if (DotProduct(curveData[index - 1].sid, curveData[index].sid) < 0) {
					curveData[index].sid *= -1.0;
				}
				// Cross side vector and tangent to get the normal
				curveData[index].nrm = CrossProduct(curveData[index].tan, curveData[index].sid);
				// Normalize
				curveData[index].nrm.normalize();
			}

			// Distance
			if (j == 0 && i == 0) {
				curveData[index].distance = 0;
				shortestDistance = 65536.0;
			}
			else {
				distVec = curveData[index].pos - curveData[index - 1].pos;
				curveData[index].distance = 
					curveData[index - 1].distance + distVec.length();
				if (curveData[index].distance < shortestDistance)
					shortestDistance = curveData[index].distance;
			}
								
			index++;
		}
	}
	
	curveCnt = index;
}

//---------------------------------------------------------------------------
// Compute a spline index at distance
//---------------------------------------------------------------------------
int Spline::GetIndexAtDistance(float distance) {
	int index = 0;

	if (distance < 0.0) return -1;

	// Forward Push
	while (index < curveCnt && distance > curveData[index].distance) {
		index++;
	}

	if (index >= curveCnt) return -1;

	return index;
}

//---------------------------------------------------------------------------
// Compute a spline index at distance
//---------------------------------------------------------------------------
int Spline::GetIndexAtDistanceFromIndex(float distance, int startIndex) {
	int index = startIndex;

	// Forward Push
	while (index < curveCnt && distance > curveData[index].distance) {
		index++;
	}
	// Backward Push
	while (index > 0 && distance <= curveData[index - 1].distance) {
		index--;
	}

	return index;
}


//---------------------------------------------------------------------------
// Helper Functs
//---------------------------------------------------------------------------
int Spline::getSplineIndex(Spline * sP, float * distance, float radius) {
	int index;

	// Get index at distance
	index = sP->GetIndexAtDistance(*distance);

	// Check to see if we have gone beyond the spline (-1)
	if (index  == -1) {
		// Move Last 3 Control Points to First 3
		for (int i = 0; i < 3; i++) {
			sP->controlData[i].pos = sP->controlData[i+1].pos;
		}
		// ADD New Point
		sP->getRandCamLocation(sP->controlData[3].pos, radius);
		//sP->controlData[3].pos;
		// Rebuild Spline
		sP->Build(sP->curveData[sP->curveCnt - 1].nrm);
		// Reset Distance
		*distance = 0.0;
		// Get new index
		index = sP->GetIndexAtDistance(*distance);
		assert(index != -1 && "Bad Index");
	}

	return index;
}
//---------------------------------------------------------------------------
void Spline::getRandCamLocation (vector3 &input, const float radius) {
	static float rot = 0;

	rot -= 0.9F;
	input.x = cosf(rot) * radius;
	input.y = ((float)(rand()%2048))/2048.0F * 50.0F + 50;
	input.z = sinf(rot) * radius;
}

////---------------------------------------------------------------------------
//void Spline::RAND_FUNC_TARGET (vector3 &input) {
//	static float rot1 = 0;
//	static float rot2 = 0;
//	static int dir = 0;
//	static int tick = 0;
//
//	if (tick-- <= 0) {
//		tick = rand() % 10;
//		dir ^= 1;
//	}
//
//	if (dir) {
//		rot1 += 0.9F;
//		rot2 += 0.5F;
//	}
//	else {
//		rot1 -= 0.9F;
//		rot2 += 0.5F;
//	}
//	input.x = cosf(rot1) * 120.0F * cosf(rot2);
//	input.y = ((float)(rand()%2048))/2048.0F * 20.0F + 20;
//	input.z = sinf(rot1) * 120.0F * cosf(rot2);
//}
