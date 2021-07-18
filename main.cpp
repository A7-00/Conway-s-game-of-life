/*  hey! this my implementation of Conway's game of life,
    my first code that I ever post to my Github and also my first project with OpenGL.
    I will be more than happy to hear your feedback on all of it; how it's putout on Github,
    how it's implemented, my style of writing code; all of it. please email me at a7Electronics.AS@ieee.org */
// author: A7

#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

int phyWidth = 600;
int phyHeight = 600; // the width and height of the physical window

int logWidth = 50;
int logHeight = 50; //  the logical width and height for OpenGL's grid

double mouseX;
double mouseY; // the mouse variables where the mouse position will be stored

bool start = FALSE; // start flag
// The game will stop showing the opening screen and proceed to the game when this flag goes high

struct arrays{
    bool cell [52][52]; // the cells on a certain generation
    // with an extra blank border of cells to avoid out of index exception
    bool cell_next [52][52]; // the cells of the next generation, they are stored a separate array so that the already
    // changed neighboring cells that preceded to the next generation dont effect the result of cells that didn't
};

arrays cells; // the struct "arrays" was used to make it easier to store older generations to be able to back step
std::vector<arrays> v1; // a container that stores older generations in

void SKeyInput(int key, int x, int y) { //Special key function, an event function provided by OpenGl that tigers on keypress
    //key represents the value of the key that was pressed, x and y are needed for the argument but in my case are ignored

    if(start) { // if the user clicked the start button
        switch (key) {
            case GLUT_KEY_RIGHT: // if the user pressed the right button the cells move up a generation
                v1.push_back(cells); //saving current generation before proceeding
                for (int i = 1; i < 51; ++i) {
                    for (int j = 1; j < 51; ++j) { // the nested for loops iterate all the cells ignoring the extra boarder
                        switch (cells.cell[i - 1][j - 1] + cells.cell[i - 1][j] + cells.cell[i - 1][j + 1]
                                + cells.cell[i][j - 1] + cells.cell[i][j + 1] + cells.cell[i + 1][j - 1]
                                + cells.cell[i + 1][j] + cells.cell[i + 1][j + 1]){ // checking all 8 neighbors of a cell
                            case 4:
                            case 5:
                            case 6:
                            case 7:
                            case 8:
                            case 0:
                            case 1:
                                cells.cell_next[i][j] = FALSE; //if the value is less than 2 or more than 3 the cell dies
                                break;
                            case 2:
                                cells.cell_next[i][j] = cells.cell[i][j]; // if the value is 2 the cell keeps state
                                break;
                            case 3:
                                cells.cell_next[i][j] = TRUE; // if the value is 3 the cell lives
                                break;
                        }
                    }
                }
                break;

            case GLUT_KEY_LEFT: // if the user pressed the left button, the previous generation is loaded from v1
                if (!v1.empty()) {
                    cells = v1.back();
                    v1.pop_back();
                }
                break;

            case GLUT_KEY_HOME: // if the user pressed the home key starts goes false turning the user to the main menu
                start = FALSE;
                // note that this case doesn't break so it also restarts the game

            case GLUT_KEY_UP: // if the user pressed the up key the game is reset and cells, cells_next and v1 clears
                for (int i = 1; i < 51; ++i)
                    for (int j = 1; j < 51; ++j) {
                        cells.cell[i][j] = FALSE;
                        cells.cell_next[i][j] = FALSE;
                    }
                v1.clear();
                v1.push_back(cells);
                break;
        }
        std::copy(std::begin(cells.cell_next), std::end(cells.cell_next), std::begin(cells.cell));
        //finally after the next generation is calculated it's assigned to the previous generation
        glutPostRedisplay();
        // displaying the change on the screen
    }

}

void click (int btn, int state,int x,int y){ //mouse click function, an event function provided by OpenGl
    // that tigers on mouse click, btn represents the button clicked, x and y are the current mouse position on screen
    // state is needed for the argument but in my case can be ignored

    if(btn == GLUT_LEFT){ // if the user clicked the left mouse button
        mouseX = x;
        mouseX = mouseX*logWidth/phyWidth;
        mouseY = phyWidth - y;
        mouseY = mouseY*logWidth/phyWidth;
        //turning the physical x,y value on the screen to a logical value that can fit on the OpenGL grid
        if (start) { // if the user clicked the start button
            cells.cell[int(floor(mouseX))][int(floor(mouseY))] = TRUE;
            //checking where the mouse clicked on the screen and setting that cell to true
            cells.cell_next[int(floor(mouseX))][int(floor(mouseY))] = cells.cell[int(floor(mouseX))][int(floor(mouseY))];
        }
        else if (mouseX > 15 && mouseX < 35 && mouseY > 10 && mouseY < 15){ // if the exit button was clicked
            exit(0);
        }
        else if (mouseX > 15 && mouseX < 35 && mouseY > 20 && mouseY < 25){ // if the start button was clicked
            start = TRUE;
        }
    }
    glutPostRedisplay();
    // displaying the change on the screen
}

void PrintS(char* str, float x, float y, void* font){ //Special function I used to make it easier to display text
    //on the screen, str in the text being displayed, x,y are the position of the cursor on the screen,
    // font represents the fount to write in

    glRasterPos2d(x, y);
    int n = strlen(str);
    for (int i = 0; i < n; i++)
        glutBitmapCharacter(font, str[i]);
    glFlush();

}

void init(float r, float g, float b, float alpha){ // the initialization function for OpenGL

    glClearColor(r, g, b, 1);
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, logWidth, 0.0, logHeight);

}

static void display(){ // the display function for OpenGL

    glClear(GL_COLOR_BUFFER_BIT);
    if (start) { //if the user clicked the start button

        for (int i = 0; i < 50; ++i) {
            glBegin(GL_LINES);
            glColor3f(1, 1, 1);
            glVertex2i(i, 50);
            glVertex2i(i, 0);
            glEnd();
        } // drawing the vertical lines

        for (int i = 0; i < 50; ++i) {
            glBegin(GL_LINES);
            glColor3f(1, 1, 1);
            glVertex2i(50, i);
            glVertex2i(0, i);
            glEnd();
        } // drawing the horizontal lines

        for (int i = 1; i < 52; ++i) {
            for (int j = 1; j < 52; ++j) {
                if (cells.cell[i][j]) {
                    glBegin(GL_QUADS);
                    glColor3f(0, 0.5, 0.7);
                    glVertex2i(i, j);
                    glVertex2i(i + 1, j);
                    glVertex2i(i + 1, j + 1);
                    glVertex2i(i, j + 1);
                    glEnd();
                }
            }
        } //drawing the live cells

    }

    else{ // the main menu

        glBegin(GL_QUADS);
        glColor3f(0, 0, 0);
        glVertex2i(0, 0);
        glVertex2i(0, 50);
        glVertex2i(50, 50);
        glVertex2i(50, 0);
        glEnd(); // drawing the background

        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        glVertex2i(15, 20);
        glVertex2i(15, 25);
        glVertex2i(35, 25);
        glVertex2i(35, 20);
        glEnd(); // drawing the upper button

        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        glVertex2i(15, 10);
        glVertex2i(15, 15);
        glVertex2i(35, 15);
        glVertex2i(35, 10);
        glEnd(); //drawing the lower button

        PrintS("Conway's game of life", 16, 37, GLUT_BITMAP_TIMES_ROMAN_24); // drawing the title
        glColor3f(0, 1, 0);
        PrintS("PLAY", 22, 22, GLUT_BITMAP_TIMES_ROMAN_24); // drawing the word "play" in green
        //the string is implicitly casted to char* so it won't error but will give a warning
        glColor3f(1, 0, 0);
        PrintS("QUIT", 22, 12, GLUT_BITMAP_TIMES_ROMAN_24); // drawing the word "quit" in red
        //the string is implicitly casted to char* so it won't error but will give a warning
    }
    glutSwapBuffers(); // swapping frames

}

int main(int argc, char *argv[]){

    v1.push_back(cells); // storing the initial blank state
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize (phyWidth, phyHeight);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("gameoflife");
    init(0, 0, 0, 1);
    glutDisplayFunc(display);
    glutMouseFunc(click);
    glutSpecialFunc(SKeyInput);
    glutMainLoop();
    return EXIT_SUCCESS;

}
