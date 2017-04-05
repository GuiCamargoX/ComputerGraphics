#include <stdio.h>
#include <math.h>

typedef struct{
  float x;
  float y;
  float z;
}  Point;

float DEGS_TO_RAD = M_PI/180.0f;
int numVertices = 0;    // Tallies the number of vertex points added.

//------------------------
//-- Prints a sphere as a "standard sphere" triangular mesh with the specified
//-- number of latitude (nLatitude) and longitude (nLongitude) lines and
//-- writes results to the specified output file (fout).

void printStandardSphere(Point pt, float radius, int nLatitude, int nLongitude, FILE *fout)
{
  int p, s, i, j;
  float x, y, z, out;
  int nPitch = nLongitude + 1;

  float pitchInc = (180. / (float)nPitch) * DEGS_TO_RAD;
  float rotInc   = (360. / (float)nLatitude) * DEGS_TO_RAD;

  //## Chose origem do plano cartesiano:
  pt.x=0;
  pt.y=0;
  pt.z=0;

  //## PRINT VERTICES:
  fprintf(fout,"v %g %g %g\n", pt.x, pt.y+radius, pt.z);    // Top vertex.
  fprintf(fout,"v %g %g %g\n", pt.x, pt.y-radius, pt.z);    // Bottom vertex.
  numVertices = numVertices+2;

  int fVert = numVertices;    // Record the first vertex index for intermediate vertices.

  for(p=1; p<nLongitude+1 ; p++){     // Generate all "intermediate vertices":
    for(s=0; s<nLatitude; s++)
    {
      x = radius * sin((float)p * pitchInc) * cos(s * rotInc);//coordenadas esfericas(equação parametrica)
      y = radius * cos(p * pitchInc);
      z = radius * sin((float)p * pitchInc) * sin(s * rotInc);

      fprintf(fout,"v %g %g %g\n", x+pt.x, y+pt.y, z+pt.z);
      numVertices++;
    }
  }

  //## PRINT TRIANGLES FACES BETWEEN INTERMEDIATE POINTS:
int cont=0;

  for(p=3; p<numVertices- nLatitude+1 ; p++) {

    cont++;

    if(cont==nLatitude){//condição ocorre quando o p faz uma volta completa entorno da mesmo nivel

        fprintf(fout,"f %d %d %d\n", p , p+1-(cont) , p+nLatitude);
        fprintf(fout,"f %d %d %d\n", p+1-(cont) , p+(nLatitude+1 -cont) , p+nLatitude);
         cont=0;
    }
    else{
    fprintf(fout,"f %d %d %d\n", p , p+1 , p+nLatitude);
    fprintf(fout,"f %d %d %d\n", p+1 , p+nLatitude+1 , p+nLatitude);
    }

  }

  //## PRINT TRIANGLE FACES CONNECTING TO TOP AND BOTTOM VERTEX:

  int offLastVerts  = fVert + (nLatitude * (nLongitude-1));
  for(s=0; s<nLatitude; s++)
  {
    j = (s==nLatitude-1) ? -1 : s;
    fprintf(fout,"f %d %d %d\n", fVert-1, (j+2)+fVert,        (s+1)+fVert       );
    fprintf(fout,"f %d %d %d\n", fVert,   (s+1)+offLastVerts, (j+2)+offLastVerts);
  }
}




//------------------------
//-- Entry point. This main() function demonstrates how you can
//-- use "printStandardSphere()", but you probably won't
//-- want/need to copy it in your own code.

int main(int argc, char *argv[])
{
  int nLatitude  = 32;                  // Number horizontal lines.
  int nLongitude = nLatitude / 2;      // Number vertical lines.
  // NOTE: for a good sphere use ~half the number of longitude lines than latitude.
  Point centerPt;            // Position the center of out sphere at (0,0,0).

  /*if (argc < 2) {
    fprintf(stderr, "Must enter: './programname outputfile.obj'\n");
    return (-1);
  }
*/
  FILE *fout = fopen("ty.obj" , "w");
  if (fout == NULL) {
     printf("Couldn't open output file %s.\n", argv[1]);
     return (-1);
  }

  printStandardSphere(centerPt, 1., nLatitude, nLongitude, fout);      // Print sphere with radius 10 into file.
  fclose(fout);
  fprintf(stdout, "  # vertices:   %d\n", numVertices);
  return (0);
}
