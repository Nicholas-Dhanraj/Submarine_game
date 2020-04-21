#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include "QuadMesh.h"

int foo[50];
struct blobby
{
	double blobbyX;
	double blobbyZ;
	double blobbyB;
	double blobbyA;
	bool undone;

}blob[1000];


int currentBlob = 0;
int currentBlobPointer = 0;


//float PHI = 1.61803398874989484820459 * 00000.1; // Golden Ratio
//float PI = 3.14159265358979323846264 * 00000.1; // PI
//float SQ2 = 1.41421356237309504880169 * 10000.0; // Square Root of Two
//void changeY(QuadMesh* quadMesh, int blobx, int blobz, int blobB, int blobA);
void changeY(QuadMesh* quadMesh, double blobx, double blobz, double blobB, double blobA);
void undoY(QuadMesh* quadMesh);
bool terrainCollide(QuadMesh* quadMesh, float subY, float subX, float subZ);
bool terrainCollide(QuadMesh* quadMesh,float subY, float subX,float subZ)
{
	Vector3D* currentVertex;
	
	int currentQuad = 0;



	for (int i = 0; i <= quadMesh->maxMeshSize; i++) {
		for (int j = 0; j <= quadMesh->maxMeshSize; j++) {

			currentVertex = &quadMesh->vertices[currentQuad].position;

			float euc = (float)sqrt(pow((double)subX - currentVertex->x, 2) +
				pow((double)subZ - currentVertex->z, 2));

			float df = sqrt(pow(subX - currentVertex->x, 2) +
				pow(subZ - currentVertex->z, 2) * 1.0);

			if (currentVertex->y > subY && euc < 2.0)
			{
				return true;
			}
			
				

			currentQuad++;
		}
	} 


	return false;
}

void waves(QuadMesh* quadMesh)
{
	Vector3D* currentVertex;

	float y;
	int currentQuad = 0;


	//ComputeNormalsQM(quadMesh);
	for (int i = 0; i <= quadMesh->maxMeshSize; i++) {
		for (int j = 0; j <= quadMesh->maxMeshSize; j++) {
			currentVertex = &quadMesh->vertices[currentQuad].position;

			
				y = (rand() % 9 + 1) *0.02;
				float yy = -sin(i*y*0.02 ) + cos(j*y* 0.02);
				
				float yyyy = sqrt(pow((j/y), 3) + pow((i/y), 3));
				float yyy = sin(yyyy * currentVertex->y * 0.0009) + -cos(yyyy * currentVertex->y * 0.0009); //sin(yyy*0.06)+15
			Set(
				currentVertex,
				currentVertex->x,
				y+15,
				currentVertex->z
			);

			currentQuad++;
		}
	}
	quadMesh->numQuads = (quadMesh->maxMeshSize) * (quadMesh->maxMeshSize);
	currentQuad = 0;

	for (int j = 0; j < quadMesh->maxMeshSize; j++)
	{
		for (int k = 0; k < quadMesh->maxMeshSize; k++)
		{
			// Counterclockwise order
			quadMesh->quads[currentQuad].vertices[0] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k];
			quadMesh->quads[currentQuad].vertices[1] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k + 1];
			quadMesh->quads[currentQuad].vertices[2] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k + 1];
			quadMesh->quads[currentQuad].vertices[3] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k];
			currentQuad++;
		}
	}
	ComputeNormalsQM(quadMesh);

}

void resetTerrain(QuadMesh* quadMesh)
{
	Vector3D* currentVertex;
	float y;
	int currentQuad = 0;


	//ComputeNormalsQM(quadMesh);
	for (int i = 0; i <= quadMesh->maxMeshSize; i++) {
		for (int j = 0; j <= quadMesh->maxMeshSize; j++) {
			currentVertex = &quadMesh->vertices[currentQuad].position;

			if (rand() % 20 + 10 == i || rand() % 15 + -1 == j)
			{
				y = (rand() % 1 + -3) * 0.05;
			}
			else y = (rand() % 2 + -0) * 0.13;
			if (i + 10 == j && j > 20 && i < 50)
			{
				y = (rand() % 1 + -6) * 0.021;
			}

			Set(
				currentVertex,
				currentVertex->x,
				y,
				currentVertex->z
			);

			currentQuad++;
		}
	}
	quadMesh->numQuads = (quadMesh->maxMeshSize) * (quadMesh->maxMeshSize);
	currentQuad = 0;

	for (int j = 0; j < quadMesh->maxMeshSize; j++)
	{
		for (int k = 0; k < quadMesh->maxMeshSize; k++)
		{
			// Counterclockwise order
			quadMesh->quads[currentQuad].vertices[0] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k];
			quadMesh->quads[currentQuad].vertices[1] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k + 1];
			quadMesh->quads[currentQuad].vertices[2] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k + 1];
			quadMesh->quads[currentQuad].vertices[3] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k];
			currentQuad++;
		}
	}
	ComputeNormalsQM(quadMesh);

}
const int minMeshSize = 1;
bool idk = true;
void redoY(QuadMesh* quadMesh)
{
	blob[currentBlobPointer].undone = false;
	currentBlobPointer++;
	double oldX = blob[currentBlobPointer].blobbyX;
	double oldZ = blob[currentBlobPointer].blobbyZ;
	double oldB = blob[currentBlobPointer].blobbyB;
	double oldA = blob[currentBlobPointer].blobbyA;

	if (true)
	{
		Vector3D* currentVertex;
		float eu;
		float y;
		int currentQuad = 0;

		for (int i = 0; i <= quadMesh->maxMeshSize; i++) {
			for (int j = 0; j <= quadMesh->maxMeshSize; j++) {

				currentVertex = &quadMesh->vertices[currentQuad].position;

				eu = (float)sqrt(pow((double)oldX - currentVertex->x, 2) +
					pow((double)oldZ - currentVertex->z, 2));

				y = currentVertex->y + (float)((oldB)* exp(-oldA * (pow(eu, 2))) * 1.0f);


				Set(
					currentVertex,
					currentVertex->x,
					y,
					currentVertex->z
				);

				currentQuad++;
			}
		}

		quadMesh->numQuads = (quadMesh->maxMeshSize) * (quadMesh->maxMeshSize);
		currentQuad = 0;

		for (int j = 0; j < quadMesh->maxMeshSize; j++)
		{
			for (int k = 0; k < quadMesh->maxMeshSize; k++)
			{
				// Counterclockwise order
				quadMesh->quads[currentQuad].vertices[0] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k];
				quadMesh->quads[currentQuad].vertices[1] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k + 1];
				quadMesh->quads[currentQuad].vertices[2] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k + 1];
				quadMesh->quads[currentQuad].vertices[3] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k];
				currentQuad++;
			}
		}
		ComputeNormalsQM(quadMesh);

	}


}
void undoY(QuadMesh* quadMesh)
{
	blob[currentBlobPointer].undone = true;
	currentBlobPointer--;
	double oldX = blob[currentBlobPointer].blobbyX;
	double oldZ = blob[currentBlobPointer].blobbyZ;
	double oldB = blob[currentBlobPointer].blobbyB;
	double oldA = blob[currentBlobPointer].blobbyA;

	if (!blob[currentBlobPointer].undone)
	{
		Vector3D* currentVertex;
		float eu;
		float y;
		int currentQuad = 0;

		for (int i = 0; i <= quadMesh->maxMeshSize; i++) {
			for (int j = 0; j <= quadMesh->maxMeshSize; j++) {

				currentVertex = &quadMesh->vertices[currentQuad].position;

				eu = (float)sqrt(pow((double)oldX - currentVertex->x, 2) +
					pow((double)oldZ - currentVertex->z, 2));

				y = currentVertex->y + (float)((oldB * -1) * exp(-oldA * (pow(eu, 2))) * 1.0f);


				Set(
					currentVertex,
					currentVertex->x,
					y,
					currentVertex->z
				);

				currentQuad++;
			}
		}

		quadMesh->numQuads = (quadMesh->maxMeshSize) * (quadMesh->maxMeshSize);
		currentQuad = 0;

		for (int j = 0; j < quadMesh->maxMeshSize; j++)
		{
			for (int k = 0; k < quadMesh->maxMeshSize; k++)
			{
				// Counterclockwise order
				quadMesh->quads[currentQuad].vertices[0] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k];
				quadMesh->quads[currentQuad].vertices[1] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k + 1];
				quadMesh->quads[currentQuad].vertices[2] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k + 1];
				quadMesh->quads[currentQuad].vertices[3] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k];
				currentQuad++;
			}
		}
		ComputeNormalsQM(quadMesh);

	}


}

void changeY(QuadMesh* quadMesh, double blobx, double blobz, double blobB, double blobA)
{
	Vector3D* currentVertex;
	float eu;
	float y;
	int currentQuad = 0;

	/*if (currentBlob != currentBlobPointer)
	{
		currentBlob = currentBlobPointer;
	}*/
	blob[currentBlob].blobbyX = blobx;
	blob[currentBlob].blobbyZ = blobz;
	blob[currentBlob].blobbyB = blobB;
	blob[currentBlob].blobbyA = blobA;
	blob[currentBlob].undone = false;



	currentBlob++;
	currentBlobPointer = currentBlob;
	for (int i = 0; i <= quadMesh->maxMeshSize; i++) {
		for (int j = 0; j <= quadMesh->maxMeshSize; j++) {

			currentVertex = &quadMesh->vertices[currentQuad].position;

			eu = (float)sqrt(pow((double)blobx - currentVertex->x, 2) +
				pow((double)blobz - currentVertex->z, 2));

			y = currentVertex->y + (float)(blobB * exp(-blobA * (pow(eu, 2))) * 1.0f);


			Set(
				currentVertex,
				currentVertex->x,
				y,
				currentVertex->z
			);

			currentQuad++;
		}
	}

	quadMesh->numQuads = (quadMesh->maxMeshSize) * (quadMesh->maxMeshSize);
	currentQuad = 0;

	for (int j = 0; j < quadMesh->maxMeshSize; j++)
	{
		for (int k = 0; k < quadMesh->maxMeshSize; k++)
		{
			// Counterclockwise order
			quadMesh->quads[currentQuad].vertices[0] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k];
			quadMesh->quads[currentQuad].vertices[1] = &quadMesh->vertices[j * (quadMesh->maxMeshSize + 1) + k + 1];
			quadMesh->quads[currentQuad].vertices[2] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k + 1];
			quadMesh->quads[currentQuad].vertices[3] = &quadMesh->vertices[(j + 1) * (quadMesh->maxMeshSize + 1) + k];
			currentQuad++;
		}
	}
	ComputeNormalsQM(quadMesh);


}
//float getVertexHeight(Vector3D* mesh, int deltay)
//{
//            return mesh->y + deltay;
//}
QuadMesh NewQuadMesh(int maxMeshSize)
{
	QuadMesh qm;        // The new quad mesh to be returned
	qm.numVertices = 0;
	qm.vertices = NULL;
	qm.numQuads = 0;
	qm.quads = NULL;
	qm.numFacesDrawn = 0;

	qm.maxMeshSize = maxMeshSize < minMeshSize ? minMeshSize : maxMeshSize;
	CreateMemoryQM(&qm);

	// Set up default material used for the mesh
	qm.mat_ambient[0] = 0.0;
	qm.mat_ambient[1] = 4.0;
	qm.mat_ambient[2] = 0.0;
	qm.mat_ambient[3] = 1.0;
	qm.mat_specular[0] = 0.0;
	qm.mat_specular[1] = 0.0;
	qm.mat_specular[2] = 0.0;
	qm.mat_specular[3] = 1.0;
	qm.mat_diffuse[0] = 0.75;
	qm.mat_diffuse[1] = 2.5;
	qm.mat_diffuse[2] = 0.0;
	qm.mat_diffuse[3] = 1.0;
	qm.mat_shininess[0] = 2.0;

	return qm;
}

void SetMaterialQM(QuadMesh* qm, Vector3D ambient, Vector3D diffuse, Vector3D specular, double shininess)
{
	qm->mat_ambient[0] = ambient.x;
	qm->mat_ambient[1] = ambient.y;
	qm->mat_ambient[2] = ambient.z;
	qm->mat_ambient[3] = 1.0;
	qm->mat_specular[0] = specular.x;
	qm->mat_specular[1] = specular.y;
	qm->mat_specular[2] = specular.z;
	qm->mat_specular[3] = 1.0;
	qm->mat_diffuse[0] = diffuse.x;
	qm->mat_diffuse[1] = diffuse.y;
	qm->mat_diffuse[2] = diffuse.z;
	qm->mat_diffuse[3] = 1.0;
	qm->mat_shininess[0] = (float)shininess;
}

// Allocate dynamic arrays.
bool CreateMemoryQM(QuadMesh* qm)
{
	const int maxVertices = (qm->maxMeshSize + 1) * (qm->maxMeshSize + 1);
	qm->vertices = malloc(sizeof(MeshVertex) * maxVertices);
	if (qm->vertices == NULL)
	{
		return false;
	}

	const int maxQuads = qm->maxMeshSize * qm->maxMeshSize;
	qm->quads = malloc(sizeof(MeshQuad) * maxQuads);
	if (qm->quads == NULL)
	{
		return false;
	}

	return true;
}


// Fills the array of vertices and the array of quads.
bool InitMeshQM(QuadMesh* qm, int meshSize, Vector3D origin, double meshLength, double meshWidth, Vector3D dir1, Vector3D dir2)
{

	Vector3D o;
	double sf1, sf2;

	Vector3D v1, v2;

	v1.x = dir1.x;
	v1.y = dir1.y;
	v1.z = dir1.z;

	sf1 = meshLength / meshSize;
	ScalarMul(&v1, (float)sf1, &v1);

	v2.x = dir2.x;
	v2.y = dir2.y;
	v2.z = dir2.z;
	sf2 = meshWidth / meshSize;
	ScalarMul(&v2, (float)sf2, &v2);

	Vector3D meshpt;

	// Build Vertices
	qm->numVertices = (meshSize + 1) * (meshSize + 1);
	int currentVertex = 0;

	// Starts at front left corner of mesh
	Set(&o, origin.x, origin.y, origin.z);

	for (int i = 0; i < meshSize + 1; i++)
	{
		for (int j = 0; j < meshSize + 1; j++)
		{
			// compute vertex position along mesh row (along x direction)
			meshpt.x = o.x + j * v1.x;
			//implimentation of perlin noise using simple rand value and offset to add resistance, etc
			//some pits and also a trench

			if (rand() % 20 + 10 == i || rand() % 15 + -1 == j)
			{
				meshpt.y = o.y + j * v1.y + (rand() % 1 + -3) * 0.05;
			}
			else meshpt.y = o.y + j * v1.y + (rand() % 9 + 1) * 0.024;
			if (i + 10 == j && j > 20 && i < 50)
			{
				meshpt.y = o.y + j * v1.y + (rand() % 1 + -7) * 0.03;
			}

			//meshpt.y = o.y + j * v1.y;
			meshpt.z = o.z + j * v1.z;
			Set(&qm->vertices[currentVertex].position, meshpt.x, meshpt.y, meshpt.z);
			currentVertex++;
		}
		// go to next row in mesh (negative z direction)
		Add(&o, &v2, &o);
	}

	// Build Quad Polygons
	qm->numQuads = (meshSize) * (meshSize);
	int currentQuad = 0;

	for (int j = 0; j < meshSize; j++)
	{
		for (int k = 0; k < meshSize; k++)
		{
			// Counterclockwise order
			qm->quads[currentQuad].vertices[0] = &qm->vertices[j * (meshSize + 1) + k];
			qm->quads[currentQuad].vertices[1] = &qm->vertices[j * (meshSize + 1) + k + 1];
			qm->quads[currentQuad].vertices[2] = &qm->vertices[(j + 1) * (meshSize + 1) + k + 1];
			qm->quads[currentQuad].vertices[3] = &qm->vertices[(j + 1) * (meshSize + 1) + k];
			currentQuad++;
		}
	}

	ComputeNormalsQM(qm);

	return true;
}

// Draw the mesh by drawing all quads.
void DrawMeshQM(QuadMesh* qm, int meshSize)
{
	int currentQuad = 0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, qm->mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qm->mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qm->mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, qm->mat_shininess);

	for (int j = 0; j < meshSize; j++)
	{
		for (int k = 0; k < meshSize; k++)
		{
			glBegin(GL_QUADS);

			glNormal3f(qm->quads[currentQuad].vertices[0]->normal.x,
				qm->quads[currentQuad].vertices[0]->normal.y,
				qm->quads[currentQuad].vertices[0]->normal.z);
			glTexCoord2f(0.25, 0.25);
			glVertex3f(qm->quads[currentQuad].vertices[0]->position.x,
				qm->quads[currentQuad].vertices[0]->position.y,
				qm->quads[currentQuad].vertices[0]->position.z);
			
			glNormal3f(qm->quads[currentQuad].vertices[1]->normal.x,
				qm->quads[currentQuad].vertices[1]->normal.y,
				qm->quads[currentQuad].vertices[1]->normal.z);
			glTexCoord2f(0, 0.25);
			glVertex3f(qm->quads[currentQuad].vertices[1]->position.x,
				qm->quads[currentQuad].vertices[1]->position.y,
				qm->quads[currentQuad].vertices[1]->position.z);
			

			glNormal3f(qm->quads[currentQuad].vertices[2]->normal.x,
				qm->quads[currentQuad].vertices[2]->normal.y,
				qm->quads[currentQuad].vertices[2]->normal.z);
			glTexCoord2f(0, 0);
			glVertex3f(qm->quads[currentQuad].vertices[2]->position.x,
				qm->quads[currentQuad].vertices[2]->position.y,
				qm->quads[currentQuad].vertices[2]->position.z);
			

			glNormal3f(qm->quads[currentQuad].vertices[3]->normal.x,
				qm->quads[currentQuad].vertices[3]->normal.y,
				qm->quads[currentQuad].vertices[3]->normal.z);
			glTexCoord2f(0.25, 0);
			glVertex3f(qm->quads[currentQuad].vertices[3]->position.x,
				qm->quads[currentQuad].vertices[3]->position.y,
				qm->quads[currentQuad].vertices[3]->position.z);
			

			glEnd();
			//glDisable(GL_TEXTURE_2D);


			currentQuad++;
		}
	}
}

void DrawMeshQMOcean(QuadMesh* qm, int meshSize)
{
	int currentQuad = 0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, qm->mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qm->mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qm->mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, qm->mat_shininess);

	for (int j = 0; j < meshSize; j++)
	{
		for (int k = 0; k < meshSize; k++)
		{
			glBegin(GL_QUADS);

			glNormal3f(qm->quads[currentQuad].vertices[0]->normal.x,
				qm->quads[currentQuad].vertices[0]->normal.y,
				qm->quads[currentQuad].vertices[0]->normal.z);
			//glTexCoord2f(1, 1);
			glVertex3f(qm->quads[currentQuad].vertices[0]->position.x,
				qm->quads[currentQuad].vertices[0]->position.y,
				qm->quads[currentQuad].vertices[0]->position.z);

			glNormal3f(qm->quads[currentQuad].vertices[1]->normal.x,
				qm->quads[currentQuad].vertices[1]->normal.y,
				qm->quads[currentQuad].vertices[1]->normal.z);
			//glTexCoord2f(0, 1);
			glVertex3f(qm->quads[currentQuad].vertices[1]->position.x,
				qm->quads[currentQuad].vertices[1]->position.y,
				qm->quads[currentQuad].vertices[1]->position.z);


			glNormal3f(qm->quads[currentQuad].vertices[2]->normal.x,
				qm->quads[currentQuad].vertices[2]->normal.y,
				qm->quads[currentQuad].vertices[2]->normal.z);
			//glTexCoord2f(0, 0);
			glVertex3f(qm->quads[currentQuad].vertices[2]->position.x,
				qm->quads[currentQuad].vertices[2]->position.y,
				qm->quads[currentQuad].vertices[2]->position.z);


			glNormal3f(qm->quads[currentQuad].vertices[3]->normal.x,
				qm->quads[currentQuad].vertices[3]->normal.y,
				qm->quads[currentQuad].vertices[3]->normal.z);
			//glTexCoord2f(1, 0);
			glVertex3f(qm->quads[currentQuad].vertices[3]->position.x,
				qm->quads[currentQuad].vertices[3]->position.y,
				qm->quads[currentQuad].vertices[3]->position.z);


			glEnd();
			//glDisable(GL_TEXTURE_2D);


			currentQuad++;
		}
	}
}

// Deallocate dynamic arrays.
void FreeMemoryQM(QuadMesh* qm)
{
	if (qm->vertices != NULL)
		free(qm->vertices);
	qm->vertices = NULL;
	qm->numVertices = 0;

	if (qm->quads != NULL)
		free(qm->quads);
	qm->quads = NULL;
	qm->numQuads = 0;
}

// Use cross-products to compute the normal vector at each vertex
void ComputeNormalsQM(QuadMesh* qm)
{
	int currentQuad = 0;

	for (int j = 0; j < qm->maxMeshSize; j++)
	{
		for (int k = 0; k < qm->maxMeshSize; k++)
		{
			Vector3D n0, n1, n2, n3;
			Vector3D e0, e1, e2, e3;

			for (int i = 0; i < 4; i++)
			{
				LoadZero(&qm->quads[currentQuad].vertices[i]->normal);
			}

			Subtract(&qm->quads[currentQuad].vertices[1]->position, &qm->quads[currentQuad].vertices[0]->position, &e0);
			Subtract(&qm->quads[currentQuad].vertices[2]->position, &qm->quads[currentQuad].vertices[1]->position, &e1);
			Subtract(&qm->quads[currentQuad].vertices[3]->position, &qm->quads[currentQuad].vertices[2]->position, &e2);
			Subtract(&qm->quads[currentQuad].vertices[0]->position, &qm->quads[currentQuad].vertices[3]->position, &e3);
			Normalize(&e0);
			Normalize(&e1);
			Normalize(&e2);
			Normalize(&e3);

			Vector3D w;    // Working vector;

			Negate(&e3, &w);
			CrossProduct(&e0, &w, &n0);
			Normalize(&n0);
			Add(&qm->quads[currentQuad].vertices[0]->normal, &n0, &qm->quads[currentQuad].vertices[0]->normal);

			Negate(&e0, &w);
			CrossProduct(&e1, &w, &n1);
			Normalize(&n1);
			Add(&qm->quads[currentQuad].vertices[1]->normal, &n1, &qm->quads[currentQuad].vertices[1]->normal);

			Negate(&e1, &w);
			CrossProduct(&e2, &w, &n2);
			Normalize(&n2);
			Add(&qm->quads[currentQuad].vertices[2]->normal, &n2, &qm->quads[currentQuad].vertices[2]->normal);

			Negate(&e2, &w);
			CrossProduct(&e3, &w, &n3);
			Normalize(&n3);
			Add(&qm->quads[currentQuad].vertices[3]->normal, &n3, &qm->quads[currentQuad].vertices[3]->normal);

			for (int i = 0; i < 4; i++)
			{
				Normalize(&qm->quads[currentQuad].vertices[i]->normal);
			}

			currentQuad++;
		}
	}
}
