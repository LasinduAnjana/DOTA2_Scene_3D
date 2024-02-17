#include <GL/glut.h>  
#include <math.h>
#include <SOIL2/SOIL2.h>
#include <stdio.h>

//global variables 
double PI = 3.141592654;

float heroB_X = -44.0;
float heroB_Y = 0.0;
float heroB_Z = 44.0;

float heroR_X = 44.0;
float heroR_Y = 0.0;
float heroR_Z = -44.0;

float camDistance = 1.0;
float isTeamBlue = true;

int rotateX = 0;
int rotateY = 0;
int rotateZ = 0;

int rotation = 0;

int i = 0; // for hero animation
int hero_movement = 0;

// for textures 
int width;
int height;
unsigned char* image;

GLuint tex;
GLuint light_land_tex;
GLuint dark_land_tex;
GLuint leaves_tex;

#define IMAGE_ROWS 64      
#define IMAGE_COLS 64

GLuint textureID;

// colors
GLfloat brown[4] = { 0.25, 0.184, 0.1133 , 1.0 };
GLfloat skin_color[4] = { 0.5, 0.3, 0.22 , 1.0 };
GLfloat green[4] = { 0.0, 0.8, 0.04 , 1.0 };
GLfloat pink[4] = { 1.0, 0.72, 0.77 , 1.0 };
GLfloat blue[4] = { 0.0, 0.0, 1.0 , 1.0 };
GLfloat red[4] = { 1.0, 0.0, 0.0 , 1.0 };

// Texture
GLubyte imageData[IMAGE_ROWS][IMAGE_COLS][3]; // Texture image data

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

//camara operationc
void keyboardSpecial(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        heroR_X++;
        heroR_Z++;
        break;
    case GLUT_KEY_RIGHT:
        heroR_X--;
        heroR_Z--;
        break;
    case GLUT_KEY_UP:
        heroR_X--;
        heroR_Z++;
        break;
    case GLUT_KEY_DOWN:
        heroR_X++;
        heroR_Z--;
        break;
    }

    glutPostRedisplay();
}

// object operations
void keyboard(unsigned char key, int x, int y) {
    //z translate along negative z
    //z translate along negative z

    switch (key) {
    case 'Z':
        camDistance--;
        break;
    case 'z':
        camDistance++;
        break;
    // TODO : remove rotation
    case 'R':
        rotateY--;
        break;
    case 'r':
        rotateY++;
        break;
    // hero object movements
    case 'a':
        heroB_X--;
        heroB_Z--;
        break;
    case 's':
        heroB_Z++;
        heroB_X--;
        break;
    case 'd':
        heroB_X++;
        heroB_Z++;
        break;
    case 'w':
        heroB_Z--;
        heroB_X++;
        break;
    // camera focas change to teams
    case 'p':
        if (isTeamBlue)
        {
            isTeamBlue = false;
        }
        else {
            isTeamBlue = true;
        }
        camDistance =1.0;
        break;
    }

    glutPostRedisplay();

}

void Timer(int x) {
    rotation++;

    // for hero moving animation
    int array[] = { 0, 1, 2, 3, 4, 3, 2, 1, 0, -1, -2, -3, -4, -3, -2, -1 };
    hero_movement = array[i];
    i++;
    if (i == 11) {
        i = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(30, Timer, 1);
}

void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define the Perspective projection frustum 
    // (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
    gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

}

void drawAxes() {
    GLfloat qaRed[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat qaGreen[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat qaBlue[] = { 0.0, 0.0, 1.0, 1.0 };

    glBegin(GL_LINES);

    glLineWidth(1.5);

    glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
    glVertex3f(-30, 0, 0);
    glVertex3f(30, 0, 0);

    glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
    glVertex3f(0, -30, 0);
    glVertex3f(0, 30, 0);

    glMaterialfv(GL_FRONT, GL_AMBIENT, qaBlue);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
    glVertex3f(0, 0, -30);
    glVertex3f(0, 0, 30);

    glEnd();
}

void DrawGrid() {
    GLfloat qawhite[] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat ext = 100.0f;
    GLfloat step = 5.0f;
    GLfloat yGrid = 0;
    GLint line;

    glMaterialfv(GL_FRONT, GL_AMBIENT, qawhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qawhite);

    glBegin(GL_LINES);
    for (line = -ext; line <= ext; line += step) {
        glVertex3f(line, yGrid, ext);
        glVertex3f(line, yGrid, -ext);

        glVertex3f(ext, yGrid, line);
        glVertex3f(-ext, yGrid, line);
    }
    glEnd();
}

void setLighting() {
    // Lighting set up
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set lighting intensity and color
    GLfloat qaAmbientLight[] = { 0.02, 0.02, 0.02, 0.0 };
    GLfloat qaDiffuseLight[] = { 0.1, 0.1, 0.1, 0.0 };
    GLfloat qaSpecularLight[] = { 0.8, 0.8, 0.8, 0.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

    // Set the light position
    GLfloat qaLightPosition[] = { 100.0, 50.0, 100.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}

// Load the imageData array with checkerboad pattern
void loadTextureImageData() {
    int value;
    for (int row = 0; row < IMAGE_ROWS; row++) {
        for (int col = 0; col < IMAGE_COLS; col++) {
            // Each cell is 8x8, value is 0 or 255 (black or white)
            value = (((row & 0x1) == 0) ^ ((col & 0x1) == 0)) * 255;
            imageData[row][col][0] = (GLubyte)value;
            imageData[row][col][1] = (GLubyte)value;
            imageData[row][col][2] = (GLubyte)value;
        }
    }
}

//void loadText() {
//    // Texture
//    loadTextureImageData();   // Load pattern into image data array
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_COLS, IMAGE_ROWS, 0, GL_RGB,
//        GL_UNSIGNED_BYTE, imageData);  // Create texture from image data
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//
//    glEnable(GL_TEXTURE_2D);  // Enable 2D texture 
//}

void loadTextureDataFromImage() {
    image = SOIL_load_image("textures/leaves_texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);

    if (image == NULL) {
        printf("Error : %s", SOIL_last_result());
    }
}

void loadDarkLandTexture() {
    image = SOIL_load_image("textures/dark_land_texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);

    if (image == NULL) {
        printf("Error : %s", SOIL_last_result());
    }

    glGenTextures(1, &dark_land_tex);
    glBindTexture(GL_TEXTURE_2D, dark_land_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_COLS, IMAGE_ROWS, 0, GL_RGB,
        GL_UNSIGNED_BYTE, image);  // Create texture from image data
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void loadLightLandTexture() {
    image = SOIL_load_image("textures/light_land_texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);

    if (image == NULL) {
        printf("Error : %s", SOIL_last_result());
    }

    glGenTextures(1, &light_land_tex);
    glBindTexture(GL_TEXTURE_2D, light_land_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_COLS, IMAGE_ROWS, 0, GL_RGB,
        GL_UNSIGNED_BYTE, image);  // Create texture from image data
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void treeBodyPart(GLfloat radious1, GLfloat radious2, GLfloat height) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, brown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);

    glBegin(GL_QUAD_STRIP);

    for (size_t i = 0; i < 65; i++)
    {
        glVertex3d(radious2 * sin(i * PI / 32), height, radious2 * cos(i * PI / 32));
        glVertex3d(radious1 * sin(i * PI / 32), 0, radious1 * cos(i * PI / 32));
    }

    glEnd();

    glPopMatrix();
}

void drawCylinder(float radius, float height) {
    float x = 0.0f;
    float z = 0.0f;
    float angle = 0.0;
    float stepSize = 0.1;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glBegin(GL_QUAD_STRIP);
    float i = stepSize;
    while (angle < 2 * PI) {
        x = radius * cos(angle);
        z = radius * sin(angle);
        glTexCoord2f(0, i / PI);
        glVertex3f(x, 0, z);
        glTexCoord2f(1, i / PI);
        glVertex3f(x, height, z);
        angle += stepSize;
        i += stepSize;
    }
    glVertex3f(radius, 0, 0);
    glVertex3f(radius, height, 0);
    glEnd();

    glPopMatrix();
}

void drawTreeB1() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    //body
    glPushMatrix();
    treeBodyPart(2, 0.1, 30);
    glPopMatrix();

    // branches
    glPushMatrix();
    glTranslatef(0.0, 22.0, 0.0);
    glRotatef(60, 1.0, 0.0, 0.0);
    treeBodyPart(0.5, 0.2, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 15.0, 0.0);
    glRotatef(65, -1.0, 0.0, 0.0);
    treeBodyPart(0.8, 0.2, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 16.0, 0.0);
    glRotatef(60, 1.0, 0.0, 0.0);
    treeBodyPart(0.6, 0.1, 6);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 19.0, 0.0);
    glRotatef(70, -1.0, 0.0, 0.0);
    treeBodyPart(0.6, 0.1, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 20.0, 0.0);
    glRotatef(60, 0.5, 0.0, -1.0);
    treeBodyPart(0.3, 0.1, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 25.0, 0.0);
    glRotatef(60, -1.0, 0.0, 2.0);
    treeBodyPart(0.31, 0.05, 3.4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 26.0, 0.0);
    glRotatef(60, -1.0, 0.0, -2.0);
    treeBodyPart(0.31, 0.05, 3.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 20.0, 0.0);
    glRotatef(60, 0.0, 0.0, 1.0);
    treeBodyPart(0.5, 0.2, 5);
    glPopMatrix();

    // leafs
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, green);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);

    glPushMatrix();
    glTranslatef(0.0, 20.0, 6.0);
    glRotatef(60, 1.0, 0.0, 0.0);
    glScalef(0.4, 1, 0.5);
    glutSolidSphere(6, 5, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 17.0, -6.0);
    glRotatef(65, -1.0, 0.0, 0.0);
    glScalef(0.4, 1, 0.5);
    glutSolidSphere(6, 5, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-6.0, 23.0, 0.0);
    glRotatef(65, 0.0, 0.0, 1.0);
    glScalef(0.4, 1, 0.5);
    glutSolidSphere(6, 5, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 22.0, -6.0);
    glRotatef(65, -2.0, 0.0, 1.0);
    glScalef(0.4, 1, 0.5);
    glutSolidSphere(6, 5, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 24.0, 6.0);
    glRotatef(65, 1.0, 0.0, 0.0);
    glScalef(0.4, 1, 0.5);
    glutSolidSphere(6, 5, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-4.0, 27.0, 0.0);
    glRotatef(65, 0.0, 0.0, 1.0);
    glScalef(0.4, 1, 0.5);
    glutSolidSphere(4, 5, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.0, 27.0, 0.0);
    glRotatef(65, 0.0, 0.0, -1.0);
    glScalef(0.4, 1, 0.5);
    glutSolidSphere(4, 5, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.0, 21.0, 0.0);
    glRotatef(65, -1.0, 0.0, -1.0);
    glScalef(0.4, 1, 0.5);
    glutSolidSphere(6, 5, 3);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.4, 1, 0.5);
    glTranslatef(0.0, 30.0, 0);
    glutSolidSphere(6, 5, 3);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

}

void drawTreeR1() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    //body
    glPushMatrix();
    treeBodyPart(2, 0.1, 30);
    glPopMatrix();

    // branches
    glPushMatrix();
    glTranslatef(0.0, 22.0, 0.0);
    glRotatef(60, 1.0, 0.0, 0.0);
    treeBodyPart(0.5, 0.2, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 15.0, 0.0);
    glRotatef(65, -1.0, 0.0, 0.0);
    treeBodyPart(0.8, 0.2, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 16.0, 0.0);
    glRotatef(60, 1.0, 0.0, 0.0);
    treeBodyPart(0.6, 0.1, 6);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 19.0, 0.0);
    glRotatef(70, -1.0, 0.0, 0.0);
    treeBodyPart(0.6, 0.1, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 20.0, 0.0);
    glRotatef(60, 0.5, 0.0, -1.0);
    treeBodyPart(0.3, 0.1, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 25.0, 0.0);
    glRotatef(60, -1.0, 0.0, 2.0);
    treeBodyPart(0.31, 0.05, 3.4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 26.0, 0.0);
    glRotatef(60, -1.0, 0.0, -2.0);
    treeBodyPart(0.31, 0.05, 3.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 20.0, 0.0);
    glRotatef(60, 0.0, 0.0, 1.0);
    treeBodyPart(0.5, 0.2, 5);
    glPopMatrix();

    glPopMatrix();
}

void drawRoads() {
    GLfloat yellow[4] = { 0.81, 0.75, 0.2 , 0.0 };

    GLfloat middle_road[][3] = {        
        {-43, 0.2, 43}, {-43, 0.2, 42}, {-30, 0.2, 31}, {-31, 0.2, 30}, {-19, 0.2, 18}, {-18, 0.2, 19}, {-10, 0.2, 11}, {-11, 0.2, 10}, {-1, 0.2, 0}, {1, 0.2, -1},
        {10, 0.2, -11}, {11, 0.2, -10}, {19, 0.2, -20}, {20, 0.2, -19}, {29, 0.2, -30}, {30, 0.2, -29}, {39, 0.2, -40}, {40, 0.2, -39}, {45, 0.2, -44}, {44, 0.2, -45}
    };

    GLfloat reft_road[][3] = { 
        {-41, 0.2,43 }, {-42, 0.2,43 }, {-41, 0.2,36 }, {-42, 0.2,36 }, {-41.4, 0.2,30 }, {-42.5, 0.2,31 }, {-40.7, 0.2,23 }, {-41.7, 0.2,23 },
        {-41, 0.2,14 }, {-42, 0.2,13 }, {-40, 0.2,7 }, {-41, 0.2, 6 }, {-39, 0.2,0 }, {-40, 0.2,-1 }, /*bend from here*/  
        {-35, 0.2,-7 }, {-34, 0.2,-6 }, {-30, 0.2,-10 }, {-29, 0.2,-10 }, {-24, 0.2,-16 }, {-23, 0.2,-18 }, {-22, 0.2,-23 }, {-21, 0.2,-22 }, {-13, 0.2,-30 }, {-12, 0.2,-30 }, 
        {-7, 0.2,-36 }, {-6, 0.2,-36 }, {0, 0.2,-40 }, {1, 0.2,-40 }, /*bend from here*/  
        {5, 0.2,-41 }, {5, 0.2,-40 }, {13, 0.2,-41 }, {12, 0.2,-40 }, {16, 0.2,-41.5 }, {16, 0.2,-40 }, {21, 0.2,-43 }, {21, 0.2,-40 }, {27, 0.2,-42 }, {28, 0.2,-40 }, {33, 0.2,-40 }, 
        {34, 0.2,-41 }, {41, 0.2,-40 }, {41, 0.2,-41 }, {43, 0.2,-40 }, {42, 0.2,-42 }
    };

    GLfloat right_road[][3] = { 
        {41, 0.2,-43 }, {42, 0.2,-43 }, {41, 0.2,-36 }, {42, 0.2,-36 }, {41.4, 0.2,-30 }, {42.5, 0.2,-31 }, {40.7, 0.2,-23 }, {41.7, 0.2,-23 },
        {41, 0.2,-14 }, {42, 0.2,-13 }, {40, 0.2,-7 }, {41, 0.2, -6 }, {39, 0.2,0 }, {40, 0.2,-1 }, /*bend from here*/
        {35, 0.2,7 }, {36, 0.2,6 }, {30, 0.2,10 }, {31, 0.2,10 }, {24, 0.2,17 }, {25, 0.2,18 }, {22, 0.2,22 }, {23, 0.2,22 }, {13, 0.2,30 }, {12, 0.2,31 },
        {5, 0.2,36 }, {6, 0.2,38 }, {0, 0.2,39 }, {-1, 0.2,40 }, /*bend from here*/
        {-5, 0.2,41 }, {-5, 0.2,40 }, {-13, 0.2,41 }, {-12, 0.2,40 }, {-16, 0.2,41.5 }, {-16, 0.2,40 }, {-21, 0.2,43 }, {-21, 0.2,40 }, {-27, 0.2,42 }, {-28, 0.2,40 }, {-33, 0.2,40 },
        {-34, 0.2,41 }, {-41, 0.2,40 }, {-41, 0.2,41 }, {-43, 0.2,40 }, {-42, 0.2,42 }
    };

    glMaterialfv(GL_FRONT, GL_AMBIENT, yellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
    glBegin(GL_QUAD_STRIP);
    for (size_t i = 0; i < sizeof(middle_road) / (sizeof(GLfloat) * 3); i++)
    {
        glVertex3d(middle_road[i][0], middle_road[i][1], middle_road[i][2]);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (size_t i = 0; i < sizeof(reft_road) / (sizeof(GLfloat) * 3); i++)
    {
        glVertex3d(reft_road[i][0], reft_road[i][1], reft_road[i][2]);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (size_t i = 0; i < sizeof(right_road) / (sizeof(GLfloat) * 3); i++)
    {
        glVertex3d(right_road[i][0], right_road[i][1], right_road[i][2]);
    }
    glEnd();
}

void drawLand() {
    GLfloat blueLand[4] = { 0.5, 0.57, 0.22 , 0.0 };
    GLfloat redLand[4] = { 0.3, 0.375, 0.32 , 0.0 };

    GLfloat blueVertices[][3] = { {-54, 0, 54}, {54, 0, 54}, {54, 0, 40}, /* other points  */ {-54, 0, -39} };
    GLfloat redVertices[][3] = { {54, 0, -54}, {-54, 0, -54}, {-54, 0, -40},  /* other points */ {54, 0, 39} };

    //loadText();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glPushMatrix();
    //loadTextures();

    glMaterialfv(GL_FRONT, GL_AMBIENT, blueLand);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blueLand);
    //loadLightLandTexture();
    glBegin(GL_TRIANGLE_FAN);
    for (GLint i = 0; i < (sizeof(blueVertices) / sizeof(GLfloat[3])); i++)
    {
        glTexCoord2f(blueVertices[i][0], blueVertices[i][2] ); 
        glVertex3fv(blueVertices[i]);
    }
    glVertex3fv(blueVertices[0]);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    //loadTextures();
    glMaterialfv(GL_FRONT, GL_AMBIENT, redLand);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, redLand);
    //loadDarkLandTexture();
    glBegin(GL_TRIANGLE_FAN);
    for (GLint i = 0; i < (sizeof(redVertices) / sizeof(GLfloat[3])); i++)
    {
        glTexCoord2f(redVertices[i][0] / 50, redVertices[i][2] / 50);
        glVertex3fv(redVertices[i]);
    }
    glVertex3fv(redVertices[0]);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

void drawRiver() {
    GLfloat riverBlue[4] = { 0.26, 0.92, 0.8 , 0.0 };

    GLfloat middle_road[][3] = {
        {54, 0.1, 41}, {54, 0.1, 36}, {45, 0.1, 36.5}, {47, 0.1, 32}, {41, 0.1, 34}, {43, 0.1, 30}, {38, 0.1, 31}, {35, 0.1, 25}, {30, 0.1, 25}, {28, 0.1, 19.5},
        {20, 0.1, 18}, {21, 0.1, 14}, {15, 0.1, 15}, {12, 0.1, 5}, {6, 0.1, 7}, {4, 0.1, -1}, {-2, 0.1, -0.2}, {-3, 0.1, -5.5}, {-10, 0.1, -4}, {-9, 0.1, -9},
        {-15, 0.1, -8}, {-16, 0.1, -15}, {-22, 0.1, -15}, {-24, 0.1, -20}, {-27, 0.1, -18}, {-30, 0.1, -25}, {-36, 0.1, -25}, {-40, 0.1, -34},  {-44, 0.1, -32},
        {-45, 0.1, -38}, {-54, 0.1, -36}, {-54, 0.1, -41}
    };

    glMaterialfv(GL_FRONT, GL_AMBIENT, riverBlue);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, riverBlue);
    glBegin(GL_QUAD_STRIP);
    for (size_t i = 0; i < sizeof(middle_road) / (sizeof(GLfloat) * 3); i++)
    {
        glVertex3d(middle_road[i][0], middle_road[i][1], middle_road[i][2]);
    }
    glEnd();
}

void drawRoundtree() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glPushMatrix();
    glTranslatef(4.6, 0, 0);
    treeBodyPart(4, 3, 4.2);
    glPopMatrix();

    glPushMatrix();
    glRotatef(5, 0, 1, 0);
    glTranslatef(4.4, 3.7, 0);
    glRotatef(10, -1, 0, 0);
    treeBodyPart(3, 2.5, 4);
    glPopMatrix();

    glPushMatrix();
    glRotatef(10, 0, 1, 0);
    glTranslatef(4.2, 7.4, 0);
    glRotatef(15, -1, 0, 0);
    treeBodyPart(2.5, 2.3, 4);
    glPopMatrix();

    // branch 1
    glPushMatrix();
    glRotatef(24, 0, 1, 0);
    glTranslatef(4, 11.2, 0);
    glRotatef(60, 0, 0, -1);
    treeBodyPart(1.2, 0.2, 13);
    glPopMatrix();
    // branch over


    glPushMatrix();
    glRotatef(24, 0, 1, 0);
    glTranslatef(4, 11.2, 0);
    glRotatef(20, -1, 0, 0);
    treeBodyPart(2.3, 2, 4);
    glPopMatrix();

    // branch 2
    glPushMatrix();
    glRotatef(30, 0, 1, 0);
    glTranslatef(3.8, 14.8, 0);
    glRotatef(60, 1, 0, -1);
    treeBodyPart(1.2, 0.2, 13);
    glPopMatrix();
    // branch over

    // branch 2
    glPushMatrix();
    glRotatef(30, 0, 1, 0);
    glTranslatef(3.8, 14.8, 0);
    glRotatef(60, 1, 0, 1);
    treeBodyPart(1.2, 0.2, 13);
    glPopMatrix();
    // branch over


    glPushMatrix();
    glRotatef(35, 0, 1, 0);
    glTranslatef(3.8, 14.8, 0);
    glRotatef(17, -1, 0, 0);
    treeBodyPart(2, 1.5, 3);
    glPopMatrix();

    // branch 3
    glPushMatrix();
    glRotatef(44, 0, 1, 0);
    glTranslatef(3.6, 17, 0);
    glRotatef(70, -3, 0, -2);
    treeBodyPart(1.2, 0.2, 13);
    glPopMatrix();
    // branch over

    glPushMatrix();
    glRotatef(44, 0, 1, 0);
    glTranslatef(3.6, 17, 0);
    glRotatef(18, -1, 0, 0);
    treeBodyPart(1.5, 1.1, 3.5);
    glPopMatrix();

    // branch 3
    glPushMatrix();
    glRotatef(56, 0, 1, 0);
    glTranslatef(3.4, 19, 0);
    glRotatef(20, 0, 0, -1);
    treeBodyPart(1.2, 0.2, 13);
    glPopMatrix();
    // branch over

    glPushMatrix();
    glRotatef(56, 0, 1, 0);
    glTranslatef(3.4, 19, 0);
    glRotatef(14, -1, 0, 0);
    treeBodyPart(1.1, 0.9, 3);
    glPopMatrix();


    glPushMatrix();
    glRotatef(65, 0, 1, 0);
    glTranslatef(3.2, 22, 0);
    glRotatef(15, -1, 0, 0);
    treeBodyPart(0.9, 0.7, 3);
    glPopMatrix();

    // branch 4
    glPushMatrix();
    glRotatef(77, 0, 1, 0);
    glTranslatef(3, 24, 0);
    glRotatef(35, -1, 0, 1);
    treeBodyPart(0.8, 0.1, 9);
    glPopMatrix();
    // branch over

    glPushMatrix();
    glRotatef(77, 0, 1, 0);
    glTranslatef(3, 24, 0);
    glRotatef(12, -1, 0, 0);
    treeBodyPart(0.7, 0.3, 3);
    glPopMatrix();

    glPushMatrix();
    glRotatef(86, 0, 1, 0);
    glTranslatef(2.9, 26.5, 0);
    glRotatef(50, -1, 0, 0);
    treeBodyPart(0.3, 0.01, 10);
    glPopMatrix();

    glPopMatrix();
}

void drawRoundTreeWithLeaves() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glPushMatrix();
    drawRoundtree();
    glPopMatrix();

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, green);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);

    glPushMatrix();
    glTranslatef(9, 18, 0);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(25, 0, 1, 0);
    glTranslatef(9, 14.5, 0);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(250, 0, 1, 0);
    glTranslatef(2, 18, -3);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(100, 0, 1, 0);
    glTranslatef(9, 19.5, 3);
    glRotatef(30, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(100, 0, 1, 0);
    glTranslatef(5, 27, 5);
    glRotatef(60, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();

    glPushMatrix();
    glRotatef(170, 0, 1, 0);
    glTranslatef(3.5, 30, 3.5);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(1.7, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(200, 0, 1, 0);
    glTranslatef(3.5, 30.7, 2.7);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 0.8, 0.9);
    glutSolidSphere(1.7, 5, 7);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
}

void drawRoundTreeWithPinkLeaves() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glPushMatrix();
    drawRoundtree();
    glPopMatrix();

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, pink);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pink);

    glPushMatrix();
    glTranslatef(9, 18, 0);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(25, 0, 1, 0);
    glTranslatef(9, 14.5, 0);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(250, 0, 1, 0);
    glTranslatef(2, 18, -3);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(100, 0, 1, 0);
    glTranslatef(9, 19.5, 3);
    glRotatef(30, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(100, 0, 1, 0);
    glTranslatef(5, 27, 5);
    glRotatef(60, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(2, 5, 7);
    glPopMatrix();

    glPushMatrix();
    glRotatef(170, 0, 1, 0);
    glTranslatef(3.5, 30, 3.5);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 1, 0.9);
    glutSolidSphere(1.7, 5, 7);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(200, 0, 1, 0);
    glTranslatef(3.5, 30.7, 2.7);
    glRotatef(40, 0, 0, 1);
    glScaled(3, 0.8, 0.9);
    glutSolidSphere(1.7, 5, 7);
    glPopMatrix();
    
    glPopMatrix();
    glPopMatrix();
}

void drawRedHighTower() {
    GLfloat gray[4] = { 0.5, 0.5, 0.5 , 1.0 };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // middle tower
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, red);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);

    glPushMatrix();
    drawCylinder(1.2, 50);
    glPopMatrix();

    glPushMatrix();
    drawCylinder(2.0, 30);
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    drawRoundtree();
    glPopMatrix();


    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    drawRoundtree();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    drawRoundtree();
    glPopMatrix();

    glPushMatrix();
    glRotatef(270, 0, 1, 0);
    drawRoundtree();
    glPopMatrix();
    

    glMaterialfv(GL_FRONT, GL_AMBIENT, gray);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);

    glPushMatrix();
    glScaled(10, 10, 10);
    glPushMatrix();

    glPushMatrix();
    glTranslated(1, 0.5, 1);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(1, 0.5, 1);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(180, 0, 1, 0);
    glTranslated(1, 0.5, 1);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(270, 0, 1, 0);
    glTranslated(1, 0.5, 1);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void drawBlueHighTower() {
    GLfloat gray[4] = { 0.7, 0.7, 0.7 , 1.0 };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // middle tower
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);

    glPushMatrix();
    drawCylinder(1.2, 50);
    glPopMatrix();

    glPushMatrix();
    drawCylinder(2.0, 30);
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    glScaled(0.9, 0.9, 0.8);
    drawRoundTreeWithPinkLeaves();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    drawRoundTreeWithPinkLeaves();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    drawRoundTreeWithLeaves();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(270, 0, 1, 0);
    drawRoundTreeWithLeaves();
    glPopMatrix();

    // rocks

    glMaterialfv(GL_FRONT, GL_AMBIENT, gray);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);

    glPushMatrix();
    glScaled(10, 10, 10);
    glPushMatrix();

    glPushMatrix();
    glTranslated(1, 0.5, 1);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(1, 0.5, 1);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(180, 0, 1, 0);
    glTranslated(1, 0.5, 1);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(270, 0, 1, 0);
    glTranslated(1, 0.5, 1);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void drawSmallTreeSetR() {

    GLfloat vertices1[][3] = {
        {-4, 0, 4}, {-4, 0, 8}, {-4, 0, 8}, {4, 0, 4}, {2, 0, 2 }, {0, 0, 9}, {3, 0, 1}, {4, 0, 6},
        {-5, 0, 2}, {-2, 0, 8}, {1, 0, 1}, {0, 0, 1}, {2, 0, 7}, {2.5, 0, 1}, {4, 0, 8},
        {5, 0, 2}, {7, 0, 2}, {6, 0, 2}, {8, 0, 3},
    };
    GLfloat vertices2[][3] = {
        {-2.9, 0, 7.5}, {-2.8, 0, 6}, {0, 0, 1}, {3, 0, -4}, {2, 0, -2 }, {0, 0, 7}, {6,0,2}
    };
    GLfloat vertices3[][3] = {
        {2.9, 0, 7.5}, {2.8, 0, 1.6}, {0.7, 0, 3}, {7, 0, 6}, {-2, 0, -2 }, {1, 0, 5}
    };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    for (GLint i = 0; i < (sizeof(vertices1) / sizeof(GLfloat[3])); i++)
    {
        glPushMatrix();
        glTranslatef(vertices1[i][0], vertices1[i][1], vertices1[i][2]);
        glScalef(0.1, 0.1, 0.1);
        drawRoundtree();
        glPopMatrix();
    }

    for (GLint i = 0; i < (sizeof(vertices2) / sizeof(GLfloat[3])); i++)
    {
        glPushMatrix();
        glTranslatef(vertices2[i][0], vertices2[i][1], vertices2[i][2]);
        glScalef(0.1, 0.1, 0.1);
        drawTreeR1();
        glPopMatrix();
    }

    for (GLint i = 0; i < (sizeof(vertices3) / sizeof(GLfloat[3])); i++)
    {
        glPushMatrix();
        glTranslatef(vertices3[i][0], vertices3[i][1], vertices3[i][2]);
        glScalef(0.1, 0.1, 0.1);
        drawRoundTreeWithPinkLeaves();
        glPopMatrix();
    }

    glPopMatrix();

}

void drawSmallTreeSetB() {

    GLfloat vertices1[][3] = {
        {-4, 0, 4}, {-4, 0, 8}, {-4, 0, 8}, {4, 0, 4}, {2, 0, 2 }, {0, 0, 9}, {3, 0, 1}, {4, 0, 6},
        {-5, 0, 2}, {-2, 0, 8}, {1, 0, 1}, {0, 0, 1}, {2, 0, 7}, {2.5, 0, 1}, {4, 0, 8},
        {5, 0, 2}, {7, 0, 2}, {6, 0, 2}, {8, 0, 3},
    };
    GLfloat vertices2[][3] = {
        {-2.9, 0, 7.5}, {-2.8, 0, 6}, {0, 0, 1}, {3, 0, -4}, {2, 0, -2 }, {0, 0, 7}, {6,0,2}
    };
    GLfloat vertices3[][3] = {
        {2.9, 0, 7.5}, {2.8, 0, 1.6}, {0.7, 0, 3}, {7, 0, 6}, {-2, 0, -2 }, {1, 0, 5}
    };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    for (GLint i = 0; i < (sizeof(vertices1) / sizeof(GLfloat[3])); i++)
    {
        glPushMatrix();
        glTranslatef(vertices1[i][0], vertices1[i][1], vertices1[i][2]);
        glScalef(0.1, 0.1, 0.1);
        drawRoundTreeWithLeaves();
        glPopMatrix();
    }

    for (GLint i = 0; i < (sizeof(vertices2) / sizeof(GLfloat[3])); i++)
    {
        glPushMatrix();
        glTranslatef(vertices2[i][0], vertices2[i][1], vertices2[i][2]);
        glScalef(0.1, 0.1, 0.1);
        drawRoundTreeWithPinkLeaves();
        glPopMatrix();
    }

    for (GLint i = 0; i < (sizeof(vertices3) / sizeof(GLfloat[3])); i++)
    {
        glPushMatrix();
        glTranslatef(vertices3[i][0], vertices3[i][1], vertices3[i][2]);
        glScalef(0.1, 0.1, 0.1);
        drawTreeB1();
        glPopMatrix();
    }

    glPopMatrix();

}

void drawBoundaryLineTrees() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    for (GLint i = -54; i <= 54; i++)
    {
        glPushMatrix();
        glTranslatef(-52, 0, i);
        glScalef(0.1, 0.15, 0.1);
        if (i % 7 == 0) 
        {
            drawRoundTreeWithPinkLeaves();
        }
        else if (i % 4)
        {
            drawRoundTreeWithLeaves();
        }
        else if (i % 3)
        {
            drawRoundtree();
        }
        else if (i % 2)
        {
            drawTreeR1();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-54, 0, i);
        glScalef(0.1, 0.15, 0.1);
        if (i % 4 == 0)
        {
            drawRoundTreeWithPinkLeaves();
        }
        else if (i % 7)
        {
            drawRoundTreeWithLeaves();
        }
        else if (i % 2)
        {
            drawRoundtree();
        }
        else if (i % 1)
        {
            drawTreeR1();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(54, 0, i);
        glScalef(0.1, 0.15, 0.1);
        if (i % 4 == 0)
        {
            drawRoundTreeWithPinkLeaves();
        }
        else if (i % 7)
        {
            drawRoundTreeWithLeaves();
        }
        else if (i % 2)
        {
            drawRoundtree();
        }
        else if (i % 1)
        {
            drawTreeR1();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(52, 0, i);
        glScalef(0.1, 0.15, 0.1);
        if (i % 5 == 0)
        {
            drawRoundTreeWithPinkLeaves();
        }
        else if (i % 6)
        {
            drawRoundTreeWithLeaves();
        }
        else if (i % 4)
        {
            drawRoundtree();
        }
        else if (i % 2)
        {
            drawTreeR1();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(i, 0, 52);
        glScalef(0.1, 0.15, 0.1);
        if (i % 4 == 0)
        {
            drawRoundTreeWithPinkLeaves();
        }
        else if (i % 7)
        {
            drawRoundTreeWithLeaves();
        }
        else if (i % 2)
        {
            drawRoundtree();
        }
        else if (i % 1)
        {
            drawTreeR1();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(i, 0, 54);
        glScalef(0.1, 0.15, 0.1);
        if (i % 8 == 0)
        {
            drawRoundTreeWithPinkLeaves();
        }
        else if (i % 5)
        {
            drawRoundTreeWithLeaves();
        }
        else if (i % 3)
        {
            drawRoundtree();
        }
        else if (i % 2)
        {
            drawTreeR1();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(i, 0, -52);
        glScalef(0.1, 0.15, 0.1);
        if (i % 4 == 0)
        {
            drawRoundTreeWithPinkLeaves();
        }
        else if (i % 7)
        {
            drawRoundTreeWithLeaves();
        }
        else if (i % 2)
        {
            drawRoundtree();
        }
        else if (i % 1)
        {
            drawTreeR1();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(i, 0, -54);
        glScalef(0.1, 0.15, 0.1);
        if (i % 3 == 0)
        {
            drawRoundTreeWithPinkLeaves();
        }
        else if (i % 5)
        {
            drawRoundTreeWithLeaves();
        }
        else if (i % 2)
        {
            drawRoundtree();
        }
        else if (i % 4)
        {
            drawTreeR1();
        }
        glPopMatrix();
    }


    glPopMatrix();
}

void drawRedTreeSet() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    GLfloat positions[][3] = { {-35, 0, -48}, {-47, 0, -46}, {-23, 0, -46}, {-12, 0, -49}, {6, 0, -51}, {-24, 0, -36}, {-14, 0, -23}, {-3, 0, -20}, {0, 0, -35}, 
        {17, 0, -35}, {7, 0, -25}, {30, 0, -25}, {30, 0, -15}, {23, 0, 0}, {13, 0, -5}, {41, 0, 10}, {46, 0, -6}, {47, 0, -20}, {48, 0, -30} };
    
    for (int i = 0; i < sizeof(positions) / (sizeof(GLfloat) * 3); i++)
    {
        glPushMatrix();
        glTranslated(positions[i][0], positions[i][1], positions[i][2]);
        drawSmallTreeSetR();
        glPopMatrix();
    }

    //glPushMatrix();
    //glTranslated(-30, 0, -42);
    //drawSmallTreeSetR();
    //glPopMatrix();

    //glPushMatrix();
    //glTranslated(-20, 0, -42);
    //drawSmallTreeSetR();
    //glPopMatrix();

    //glPushMatrix();
    //glTranslated(-12, 0, -42);
    //drawSmallTreeSetR();
    //glPopMatrix();

    //glPushMatrix();
    //glTranslated(-8, 0, -47);
    //drawSmallTreeSetR();
    //glPopMatrix();

    //glPushMatrix();
    //glTranslated(0, 0, -48);
    //drawSmallTreeSetR();
    //glPopMatrix();

    //glPushMatrix();
    //glTranslated(-28, 0, -32);
    //drawSmallTreeSetR();
    //glPopMatrix();

    //glPushMatrix();
    //glTranslated(-40, 0, -30);
    //drawSmallTreeSetR();
    //glPopMatrix();

    // end
    //glPushMatrix();
    //glTranslated(-9, 0, -9);
    //drawSmallTreeSetR();
    //glPopMatrix();

  /*  glPushMatrix();
    glTranslated(-5, 0, -18);
    drawSmallTreeSetR();
    glPopMatrix();

    glPushMatrix();
    glTranslated(15, 0, -32);
    drawSmallTreeSetR();
    glPopMatrix();

    glPushMatrix();
    glTranslated(2, 0, -29);
    drawSmallTreeSetR();
    glPopMatrix();

    glPushMatrix();
    glTranslated(3, 0, -12);
    drawSmallTreeSetR();
    glPopMatrix();


    glPushMatrix();
    glTranslated(33, 0, -0);
    drawSmallTreeSetR();
    glPopMatrix();

    glPushMatrix();
    glTranslated(35, 0, -20);
    drawSmallTreeSetR();
    glPopMatrix();

    glPushMatrix();
    glTranslated(23, 0, -6);
    drawSmallTreeSetR();
    glPopMatrix();


    glPushMatrix();
    glTranslated(15, 0, 0);
    drawSmallTreeSetR();
    glPopMatrix();
    glPopMatrix();*/
}

void drawblueTreeSet() {
    GLfloat positions[][3] = { {42, 0, 42}, {30, 0, 42}, {20, 0, 42}, {12, 0, 42}, {8, 0, 47 },{0, 0, 48}, {28, 0, 32}, {40, 0, 30}, {6, 0, 9}, {-3, 0, 12}, {-20, 0, 30},
        {-2, 0, 29}, {-3, 0, 12}, {-33, 0, 0}, {-35, 0, 20}, {-23, 0, 6}, {-15, 0, 0}, {-10, 0, 47}, {-40, 0, -20}, {-50, 0, 0}, {-52, 0, 13}, {-52, 0, 25}, {-10, 0, 20}
};

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


    for (int i = 0; i < sizeof(positions) / (sizeof(GLfloat) * 3); i++)
    {
        glPushMatrix();
        glTranslated(positions[i][0], positions[i][1], positions[i][2]);
        drawSmallTreeSetB();
        glPopMatrix();
    }


    glPopMatrix();
}

void drawLeg(int x) {
    // left leg x = 1: right x = -1
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glPushMatrix();
    glTranslated((x * 1.0), 0, 0);
    glRotated(x * -175, 0, 0, 1);
    glRotated(-15, 1, 0, 0);
    drawCylinder(0.5, 2.5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(x * 1.1, -2.0, -0.7);
    glRotated(x * 180, 0, 0, 1);
    glRotated(20, 1, 0, 0);
    drawCylinder(0.4, 2.5);
    glPopMatrix();

    glPopMatrix();
}

void drawHand(int x) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glPushMatrix();

    glPushMatrix();
    glTranslated(x * 1.3, 0, 0);
    glRotated(x * -150, 0, 0, 1);
    glRotated(15, 1, 0, 0);
    drawCylinder(0.4, 1.9);
    glPopMatrix();

    glPushMatrix();
    glTranslated(x * 2.4, -1.5, 0.5);
    glRotated(x * -160, 0, 0, 1);
    glRotated(-30, 1, 0, 0);
    drawCylinder(0.3, 2.0);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
}

void drawHero(bool x) {
    // blue : x = T, red x = F
    glMaterialfv(GL_FRONT, GL_AMBIENT, brown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // hat
    glPushMatrix();
    glTranslated(0, 8.9, 0.4);
    glRotated(180, 0, 1, 0);
    glRotated(20, 1, 0, 0);
    glScaled(1, 0.4, 1);
    glutSolidCone(1, 2, 5, 2);
    glPopMatrix();

    // head
    glPushMatrix();
    glTranslated(0, 8.2, 0);
    glScaled(1, 1.4, 1);
    glutSolidSphere(0.7, 20, 20);
    glPopMatrix();

    // body
    if (x)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    }
    else
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, red);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    }
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glPushMatrix();
    glTranslated(0, 4, 0);
    glScaled(2, 1, 1);
    drawCylinder(0.8, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 7.1, 0);
    glScaled(1.8, 0.1, 1);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();

    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, brown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
    glMatrixMode(GL_MODELVIEW);
    // legs
    glPushMatrix();
    glTranslated(0.0, 4, 0);
    glRotated(hero_movement * 10, 1, 0, 0);
    drawLeg(-1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.0, 4, 0);
    glRotated(hero_movement * -10, 1, 0, 0);
    drawLeg(1);
    glPopMatrix();

    // hands
    glPushMatrix();
    glTranslated(0, 7, 0);
    glRotated(hero_movement * 10, 1, 0, 0);
    drawHand(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 7, 0);
    glRotated(hero_movement * -10, 1, 0, 0);
    drawHand(-1);
    glPopMatrix();

    glPopMatrix();
}

void drawWatchTower(bool x) {
    GLfloat grayB[4] = { 0.7, 0.7, 0.7 , 1.0 };
    GLfloat grayR[4] = { 0.5, 0.5, 0.5 , 1.0 };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    if (x)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    }else {
        glMaterialfv(GL_FRONT, GL_AMBIENT, red);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    }
    glPushMatrix();
    glTranslated(0, 2, 0);
    glScaled(1, 2, 1);
    glutWireSphere(1.2, 8, 7);
    glPopMatrix();

    if (x)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, grayB);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, grayB);
    }
    else
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, grayR);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, grayR);
    }

    glPushMatrix();
    glTranslated(0, 2, 0);
    glScaled(0.7, 3, 0.6);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.7, 0.5, 0.7);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(0.7, 0.5, 0.7);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(180, 0, 1, 0);
    glTranslated(0.7, 0.5, 0.7);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPushMatrix();
    glRotated(270, 0, 1, 0);
    glTranslated(0.7, 0.5, 0.7);
    glRotated(30, 1, 0, 0);
    glScaled(0.5, 2, 0.4);
    glutSolidSphere(1, 8, 7);
    glPopMatrix();

    glPopMatrix();
}

void drawScene() {
    GLfloat blueTowerPositions[][3] = { {-18, 0, 14}, {-40, 0, -5}, {13, 0, 35}, {-32, 0, 38}, {-38, 0, 32} };
    GLfloat redTowerPositions[][3] = { {18, 0, -14}, {40, 0, 5}, {-13, 0, -35}, {32, 0, -38}, {38, 0, -32} };

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    // land
    glPushMatrix();
    drawLand();
    glPopMatrix();
    
    // River
    glPushMatrix();
    drawRiver();
    glPopMatrix();

    // roads
    glPushMatrix();
    drawRoads();
    glPopMatrix();

    // trees
    glPushMatrix();
    drawblueTreeSet();
    glPopMatrix();

    glPushMatrix();
    drawRedTreeSet();
    glPopMatrix();
    
    // watch towers
    glPushMatrix();
    
    for (int i = 0; i < 5; i++)
    {
        glPushMatrix();
        glTranslated(blueTowerPositions[i][0], blueTowerPositions[i][1], blueTowerPositions[i][2]);
        drawWatchTower(true);
        glPopMatrix();
    }

    for (int i = 0; i < 5; i++)
    {
        glPushMatrix();
        glTranslated(redTowerPositions[i][0], redTowerPositions[i][1], redTowerPositions[i][2]);
        drawWatchTower(false);
        glPopMatrix();
    }
    
    glPopMatrix();


    // high towers
    glPushMatrix();
    glTranslatef(40, 0, -41);
    glScaled(0.15, 0.15, 0.15);
    drawRedHighTower();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-41, 0, 40);
    glScaled(0.15, 0.15, 0.15);
    drawBlueHighTower();
    glPopMatrix();

    // boundary trees
    glPushMatrix();
    drawBoundaryLineTrees();
    glPopMatrix();
    
    glPopMatrix();
}

void camera() {
    if (isTeamBlue) {
        gluLookAt(heroB_X + camDistance + 0.5, heroB_Y + (2 * camDistance) + 4, heroB_Z + (1.5 * camDistance) + 2.0, heroB_X, heroB_Y, heroB_Z, 0.0, 1.0, 0.0);
    } else {
        rotateY + 180;
        gluLookAt(heroR_X + (1.5 * camDistance) - 2.5, heroR_Y + (2 * camDistance)+ 4, heroR_Z +  camDistance - 4.0, heroR_X, heroR_Y, heroR_Z, 0.0, 1.0, 0.0);
    }
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Setting the modelview matrix to the identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera();

    glPushMatrix();

    // for movieng and rotating entire scene
    //glTranslated(objX, objY, objZ);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glRotatef(rotateZ, 0.0, 0.0, 1.0);

    // grid and axis
    //DrawGrid();
    //drawAxes();

    //lighting
    setLighting();

    
    glPushMatrix();
    glRotatef(10 * rotateY, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    // draw objects here
    glPushMatrix();
    drawScene();
    glPopMatrix();
    
    // blue team hero
    glPushMatrix();
    glTranslatef(heroB_X, heroB_Y, heroB_Z);
    glRotatef(-45, 0, 1, 0);
    glScaled(0.3, 0.3, 0.3);
    drawHero(true);
    glPopMatrix();

    // read team hero
    glPushMatrix();
    glTranslatef(heroR_X, heroR_Y, heroR_Z);
    glRotatef(135, 0, 1, 0);
    glScaled(0.3, 0.3, 0.3);
    drawHero(false);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Dota 2 Scene");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // keyboard function activation
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);

    glutTimerFunc(60.0, Timer, 1);

    init();

    glutMainLoop();


    return 0;
}