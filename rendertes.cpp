#include "rendertes.h";
void Render::loadLight()
{
    if (!status[0])
        return ;
    glEnable(GL_LIGHTING);    
    for (int i = 0; i != 8; ++i)
    {

        if (status[0] >> i & 1)
        {
            glEnable(0x4000+i); 
            for (int j = 0; j != 10; ++j) 
                glLightfv(0x4000+i, 0x1200+j, light[i][j]);
        }
    }
}


void Render::loadObject()
{
    if (!status[1])
        return ;
    //for (int i = 0; i != 6; ++i)
    //   if (object[2] >> i & 1) 
    //        glEnableClientState(0x8074+i);
    int len = objlist.size();
    glPushMatrix();
    glScalef(shift[0][0], shift[0][1], shift[0][2]);
    if (object[4])
    {
       glTranslatef(shift[1][0], shift[1][1], shift[1][2]);
       glRotatef(shift[2][0], shift[2][1], shift[2][2], shift[2][3]);
    }
    else
    {
       glRotatef(shift[2][0], shift[2][1], shift[2][2], shift[2][3]);
       glTranslatef(shift[1][0], shift[1][1], shift[1][2]);
    }
    for (int j = 1; j != len; ++j)
    {
        glBegin(object[0]+0x0000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        for (int i=objlist[j-1]; i != objlist[j]; ++i)
            glArrayElement(i);
        glEnd();
    }
    glPopMatrix();

    //backup
    //backup
}

void Render::setObjectArray(int i, int size, double array[])
{
    glEnableClientState(0x8074);
    glVertexPointer(size, GL_DOUBLE, 0, array);  
}

void Render::setNormArray(int i, int size, double array[])
{
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, array);
    //glColorPointer(3,GL_DOUBLE, 0, array);
}


void Render::loadView()
{
    if (!status[2])
        return ;
    if (status[2] == 1)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(view[0][0], view[0][1], view[0][2], view[1][0], view[1][1], view[1][2], view[2][0], view[2][1], view[2][2]);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(view[3][0], view[4][0], view[3][1], view[4][1], view[0][2]-view[3][2], view[0][2]-view[4][2]); 
        glMatrixMode(GL_MODELVIEW);
    }
    else if(status[2] == 2)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(view[3][0], view[4][0], view[3][1], view[4][1], view[3][2], -view[4][2]);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();        

    }
}

void Render::loadMaterial()
{
    for (int i = 0; i != 5; ++i)
        glMaterialfv(GL_FRONT, MATER[i], &material[i][0]);
}

void Render::loadFog()
{
    //float fogColor[] = {0.5, 0.5, 0.5, 0.5};
    int set = status[3];

    if (!set)
        return ;

    glEnable(GL_FOG);
    glHint(GL_FOG_HINT, GL_DONT_CARE);

    if (set & 1)
        ;//GL_FOG_COORDINATE_ARRAY

    if (set >> 1 & 1)
        glFogi(GL_FOG_MODE, GL_LINEAR);
    else if (set >> 2 & 1)
        glFogi(GL_FOG_MODE, GL_EXP);
    else
        glFogi(GL_FOG_MODE, GL_EXP2);

    glFogfv(GL_FOG_COLOR, &fog[0][0]);
    glFogf(GL_FOG_DENSITY, fog[1][0]);
    glFogf(GL_FOG_START, fog[1][1]);
    glFogf(GL_FOG_END, fog[1][2]);

}

void Render::unArchive(int w, int h)
{
    glClearColor(0.5, 0.5, 0.5, 0.5);
    glEnable(GL_DEPTH_TEST);
    //glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

