#include <GL/glut.h>  
#include <math.h>

//global variables 
double PI = 3.141592654;

float objX = 0.0;
float objY = 0.0;
float objZ = 0.0;

int rotateX = 0;
int rotateY = 0;
int rotateZ = 0;

// camera position
float camX = 0.0;
float camY = 15.0;
float camZ = 30.0;

int sceneX = 0;
int rotation = 0;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

//camara operationc
void keyboardSpecial(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        camY++;
        break;
    case GLUT_KEY_DOWN:
        camY--;
        break;
    case GLUT_KEY_LEFT:
        objX--;
        break;
    case GLUT_KEY_RIGHT:
        objX++;
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
        camZ--;
        break;
    case 'z':
        camZ++;
        break;
    case 'l':
        rotateY--;
        break;
    case 'r':
        rotateY++;
        break;
    }


    glutPostRedisplay();

}

void Timer(int x) {
    rotation++;
    glutPostRedisplay();

    glutTimerFunc(60, Timer, 1);
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

    GLfloat ext = 30.0f;
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
    glEnable(GL_LIGHT1);

    // Set lighting intensity and color
    GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, qaSpecularLight);

    // Set the light position
    GLfloat qaLightPosition[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, qaLightPosition);
}

void treeBodyPart(GLfloat radious1, GLfloat radious2, GLfloat height) {

    GLfloat brown[4] = { 0.25, 0.184, 0.1133 , 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, brown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);

    glBegin(GL_QUAD_STRIP);

    for (size_t i = 0; i < 65; i++)
    {
        glVertex3d(radious2 * sin(i * PI / 32), height, radious2 * cos(i * PI / 32));
        glVertex3d(radious1 * sin(i * PI / 32), 0, radious1 * cos(i * PI / 32));
    }

    glEnd();
}

void drawTree1() {
    GLfloat green[4] = { 0.0, 0.8, 0.04 , 1.0 };

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
    glMatrixMode(GL_MODELVIEW);
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

void drawTree2() {
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

    glPopMatrix();

}

void drawLand() {
    GLfloat blueLand[4] = { 0.5, 0.57, 0.22 , 0.0 };
    GLfloat redLand[4] = { 0.3, 0.375, 0.32 , 0.0 };

    GLfloat blueVertices[][3] = { {-30, 0, -29}, {-30, 0, 30}, {29, 0, 30}, {25, 0, 26}, {21, 0, 20}, {14, 0, 15}, {10, 0, 9}, {5, 0, 4}, {0, 0, 1}, {-5, 0, -3}, {-10, 0, -11} };
    GLfloat redVertices[][3] = { {-29, 0, -30}, {30, 0, -30}, {30, 0, 29}, {25, 0, 23}, {21, 0, 18}, {14, 0, 13}, {10, 0, 8}, {5, 0, 2}, {0, 0, -1}, {-5, 0, -5}, {-10, 0, -13} };

    glMaterialfv(GL_FRONT, GL_AMBIENT, blueLand);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blueLand);

    glBegin(GL_TRIANGLE_FAN);
    for (GLint i = 0; i < (sizeof(blueVertices)/sizeof(GLfloat[3])); i++)
    {
        glVertex3fv(blueVertices[i]);
    }
    glVertex3fv(blueVertices[0]);
    glEnd();


    glMaterialfv(GL_FRONT, GL_AMBIENT, redLand);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, redLand);
    glBegin(GL_TRIANGLE_FAN);
    for (GLint i = 0; i < (sizeof(redVertices)/sizeof(GLfloat[3])); i++)
    {
        glVertex3fv(redVertices[i]);
    }
    glVertex3fv(redVertices[0]);
    glEnd();
}

void drawTreeSet1() {

    GLfloat vertices[][3] = {
        {-20, 0, 15}, {-20, 0, 10}, {-20, 0, 8}, {-20, 0, 4}, {-18, 0, 12 }, {-18, 0, 9}, {-15, 0, 10}, {-15, 0, 6},
        {-5, 0, 20}, {-2, 0, 18}, {0, 0, 19}, {0, 0, 18}, {2, 0, 17}, {2.5, 0, 19}, {4, 0, 18},
        {5, 0, 29}, {7, 0, 28}, {6, 0, 29}, {8, 0, 29},
    };

    for (GLint i = 0; i < (sizeof(vertices) / sizeof(GLfloat[3])); i++)
    {
        glPushMatrix();
        glTranslatef(vertices[i][0], vertices[i][1], vertices[i][2]);
        glScalef(0.2, 0.2, 0.2);
        drawTree1();
        glPopMatrix();

    }

}

void drawScene() {
    drawTreeSet1();
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Setting the modelview matrix to the identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // for movieng and rotating entire scene
    glTranslated(objX, objY, objZ);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glRotatef(rotateZ, 0.0, 0.0, 1.0);

    // grid and axis
    DrawGrid();
    drawAxes();

    //lighting
    setLighting();

    // draw objects here
    drawLand();
    drawScene();

    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Penguin scene");
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
