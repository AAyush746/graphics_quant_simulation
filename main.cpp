#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Parameters for the qubit state
float theta = M_PI / 4.0f; // Polar angle (0 to PI)
float phi = 0.0f;         // Azimuthal angle (0 to 2PI)
float precession_speed = 0.02f;

// Camera parameters
float cam_angle_x = 20.0f;
float cam_angle_y = 45.0f;
float cam_dist = 5.0f;

void draw_axes() {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.5f, 0.0f, 0.0f);
    glVertex3f(1.5f, 0.0f, 0.0f);
    
    // Y axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -1.5f, 0.0f);
    glVertex3f(0.0f, 1.5f, 0.0f);
    
    // Z axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -1.5f);
    glVertex3f(0.0f, 0.0f, 1.5f);
    glEnd();
}

void draw_bloch_sphere() {
    glColor4f(0.7f, 0.7f, 0.7f, 0.3f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Draw wireframe sphere
    glutWireSphere(1.0, 20, 20);
    
    glDisable(GL_BLEND);
}

void draw_spin_vector() {
    // Calculate Cartesian coordinates from theta and phi
    float x = sin(theta) * cos(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(theta);

    // Draw the vector (arrow)
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(x, y, z);
    glEnd();

    // Draw a small sphere at the tip
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.05, 10, 10);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera transformation
    glTranslatef(0.0f, 0.0f, -cam_dist);
    glRotatef(cam_angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(cam_angle_y, 0.0f, 1.0f, 0.0f);

    draw_axes();
    draw_bloch_sphere();
    draw_spin_vector();

    glutSwapBuffers();
}

void idle() {
    // Simulate Larmor precession (rotation around Z-axis)
    phi += precession_speed;
    if (phi > 2.0 * M_PI) phi -= 2.0 * M_PI;
    
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case 'w': cam_angle_x += 5.0f; break;
        case 's': cam_angle_x -= 5.0f; break;
        case 'a': cam_angle_y -= 5.0f; break;
        case 'd': cam_angle_y += 5.0f; break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Quantum Spin Simulation - Bloch Sphere");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Dark blue background

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    std::cout << "Quantum Spin Simulation started." << std::endl;
    std::cout << "Controls: W/A/S/D to rotate camera, ESC to exit." << std::endl;

    glutMainLoop();
    return 0;
}
